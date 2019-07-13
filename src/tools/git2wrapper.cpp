#include "git2wrapper.h"

#include "data/logentry.h"


#include "git2cpp/repo.h"
#include "git2cpp/reference.h"
#include "git2cpp/id_to_str.h"

#include "git2/blame.h"

#include <QByteArray>
#include <QMessageBox>
#include <QProcess>
#include <QString>
#include <QTextStream>

namespace
{
    QString InvaliOid()
    {
        static const QString c_invalidOid = "0000000000000000000000000000000000000000";

        return c_invalidOid;
    }

    git_repository_init_options RepoOptions()
    {
        git_repository_init_options opts;

        return opts;
    }
}



CGit2Wrapper::CGit2Wrapper(const QString& sPath)
    : m_repo(sPath.toUtf8().constData())
{
}


void CGit2Wrapper::SetHead(const QString& sHead)
{
    m_repo.set_head(sHead.toUtf8().constData());

    vBranches b = Branches();
}


QString CGit2Wrapper::HeadRef() const
{
    git::Reference ref = m_repo.head();
    return ref.name();
}


void CGit2Wrapper::Initialize()
{
    m_branches = m_repo.branches(git::branch_type::ALL);
}


CGit2Wrapper::vBranches CGit2Wrapper::Branches() const
{
    vBranches branches;

    //std::vector<git::Reference> vBranches = m_repo.branches(git::branch_type::ALL);

    for(const auto& ref : m_branches)
    {
        if(ref.type() != GIT_REF_SYMBOLIC)
        {
            branches.push_back(std::make_pair(QString::fromLocal8Bit(ref.name()), ref.target()));

            emit Message(QString("%1 at %2").arg(branches.back().first).arg(git::id_to_str(ref.target()).c_str()));
        }
    }

    return branches;
}


quokkagit::vLogEntries CGit2Wrapper::Log(int branch,
                                         const CGit2Wrapper::vBranches& b,
                                         const QString& path) const
{
    quokkagit::vLogEntries entries;

    if (branch >= 0 && branch < static_cast<int>(b.size()))
    {
        git_oid oid = b.at(static_cast<vBranches::size_type>(branch)).second;

        char* p = path.toUtf8().data();
        git_diff_options diffopts = GIT_DIFF_OPTIONS_INIT;
        diffopts.pathspec.strings = &p;
        diffopts.pathspec.count = 1;
        git::Pathspec ps(diffopts.pathspec);

        git::RevWalker walk =  m_repo.rev_walker();
        walk.sort(git::revwalker::sorting::topological);
        walk.push(oid);
        while (auto commit = walk.next())
        {
            if (path.isEmpty())
                entries.push_back(quokkagit::SLogEntry::FromCommit(commit));
            else
            {
                std::size_t parents = commit.parents_num();
                if (parents == 0)
                {
                    int i = commit.tree().pathspec_match(GIT_PATHSPEC_NO_MATCH_ERROR, ps);
                    if (i != 0)
                        continue;
                    else
                        entries.push_back(quokkagit::SLogEntry::FromCommit(commit));
                }
                else if (parents == 1)
                {
                    auto parent = commit.parent(0);
                    auto c_tree = commit.tree();
                    auto p_tree = parent.tree();
                    auto diff = commit.repo().diff(p_tree, c_tree, diffopts);

                    if (diff.deltas_num() > 0)
                        entries.push_back(quokkagit::SLogEntry::FromCommit(commit));
                    else continue;
                }
                else
                {

                }

            }
        }
    }

    return entries;
}


void CGit2Wrapper::DiffFinished(int index)
{
    m_diffs.erase(m_diffs.begin() + index);
}



git::Tree CGit2Wrapper::resolve_to_tree(git::Repository const & repo, const QString& identifier)
{
    git::Object obj = git::revparse_single(repo, identifier.toUtf8().constData());

    QString sMessage = QString("%1 - %2")
                       .arg(git_object_type2string(obj.type()))
                       .arg(QString::fromStdString(git::id_to_str(obj.id())));

    emit Message(sMessage);

    switch (obj.type())
    {
    case GIT_OBJ_TREE:
        return obj.to_tree();
    case GIT_OBJ_COMMIT:
        return obj.to_commit().tree();
    default:
        break;
    }

    throw std::logic_error("Invalid Object");
}


git::Diff CGit2Wrapper::find_diff(git::Repository const & repo, git::Tree & t1, git::Tree & t2)
{
    git_diff_options opts;
    git_diff_init_options(&opts, GIT_DIFF_OPTIONS_VERSION);

    if (t1 && t2)
    {
        return repo.diff(t1, t2, opts);
    }
    else
    {
        if (!t1)
            t1 = resolve_to_tree(repo, "HEAD");

        return repo.diff_to_index(t1, opts);
    }
}


CGit2Wrapper::vDeltas CGit2Wrapper::DiffWithParent(int index, const quokkagit::vLogEntries& entries)
{
    vDeltas deltas;

    if(index > 0 ||
       static_cast<quokkagit::vLogEntries::size_type>(index) < entries.size())
    {
        quokkagit::SLogEntry entry = entries.at(static_cast<quokkagit::vLogEntries::size_type>(index));

        git_oid oid = entry.oid();

        git::Commit commit = m_repo.commit_lookup(oid);

        git::Tree commitTree = commit.tree();

        size_t n = commit.parents_num();
        if(n == 0) return deltas;

        git_oid parent_oid = commit.parent_id(0);

        git::Commit parentCommit = m_repo.commit_lookup(parent_oid);

        git::Tree parentTree = parentCommit.tree(); // resolve_to_tree(m_repo, parentId);

        git_diff_options opts;
        git_diff_init_options(&opts, GIT_DIFF_OPTIONS_VERSION);
        git::Diff diff = m_repo.diff(parentTree, commitTree, opts);
        git_diff_find_options findopts = GIT_DIFF_FIND_OPTIONS_INIT;
        diff.find_similar(findopts);


        CFileLogModel::vFiles files;

        std::vector<git::Tree> trees;

        auto callback = [this, &files, &deltas, &trees](git_diff_delta const & delta, git_diff_hunk const &, git_diff_line const &) {

            QString sNewPath(QString::fromLocal8Bit(delta.new_file.path));
            QString sOldPath(QString::fromLocal8Bit(delta.old_file.path));

            QString status;
            switch (delta.status)
            {
            case GIT_DELTA_UNMODIFIED:
                status = "unmodified"; break;
            case GIT_DELTA_ADDED:
                status = "added"; break;
            case GIT_DELTA_DELETED:
                status = "deleted"; break;
            case GIT_DELTA_MODIFIED:
                status = "modified"; break;
            case GIT_DELTA_RENAMED:
                status = "renamed"; break;
            case GIT_DELTA_COPIED:
                status = "copied"; break;
            case GIT_DELTA_IGNORED:
                status = "ignored"; break;
            case GIT_DELTA_UNTRACKED:
                status = "untracked"; break;
            case GIT_DELTA_TYPECHANGE:
                status = "typechange"; break;
            case GIT_DELTA_UNREADABLE:
                status = "UNREADABLE"; break;
            case GIT_DELTA_CONFLICTED:
                status = "conflict"; break;
            }

            emit Message(QString("Similarity of old: %1 new: %2 is %3").arg(sOldPath).arg(sNewPath).arg(delta.similarity));
            //        emit Message(QString("old id: %1 - new id: %2").arg(git::id_to_str(delta.old_file.id).c_str()).arg(git::id_to_str(delta.new_file.id).c_str()));

            if(GIT_DELTA_RENAMED == delta.status)
                files.push_back(std::make_pair(status, sOldPath + " => " + sNewPath));
            else
                files.push_back(std::make_pair(status, sNewPath));


            deltas.push_back(std::make_pair(delta, sNewPath));
        };

        diff.print(git::diff::format::raw, callback);

        emit NewFiles(files);
    }

    return deltas;
}


void CGit2Wrapper::DiffBlobs(int deltaIndex, const vDeltas& deltas)
{
    if(deltaIndex < 0) return;

    static const QString c_msgTempl("Size: %1");
    git_diff_delta delta = deltas.at(static_cast<size_t>(deltaIndex)).first;
    const QString sPath = deltas.at(static_cast<size_t>(deltaIndex)).second;

    QByteArray dataNew;
    QByteArray dataOld;
    // TODO: Use delta paths
    QString pathNew = sPath;
    QString pathOld = sPath;
    QString hashNew;
    QString hashOld;

    QString oid = git::id_to_str(delta.new_file.id).c_str();
    if(oid != InvaliOid())
    {
        //pathNew = delta.new_file.path;

        git::Blob blobNew = m_repo.blob_lookup(delta.new_file.id);

        hashNew = QString::fromStdString(git::id_to_str(delta.new_file.id, 10));

        dataNew = QByteArray(reinterpret_cast<const char*>(blobNew.content()),
                             static_cast<int>(blobNew.size()));
    }

    oid = git::id_to_str(delta.old_file.id).c_str();
    if(oid != InvaliOid())
    {
        //pathNew = delta.old_file.path;

        git::Blob blobOld = m_repo.blob_lookup(delta.old_file.id);

        hashOld = QString::fromStdString(git::id_to_str(delta.old_file.id, 10));

        dataOld = QByteArray(reinterpret_cast<const char*>(blobOld.content()),
                             static_cast<int>(blobOld.size()));
    }


    m_diffs.push_back(CKdiff3(static_cast<int>(m_diffs.size())));

    m_diffs.back().Open(dataOld, dataNew, pathOld, pathNew, hashOld, hashNew);

    connect(&m_diffs.back(), &CKdiff3::Message,
            this, &CGit2Wrapper::Message);

    connect(&m_diffs.back(), &CKdiff3::ProgrammFinished, this, &CGit2Wrapper::DiffFinished);
}


quokkagit::tvBlameData CGit2Wrapper::BlameFile(const QString& path,
                                               const QString& oid)
{
    quokkagit::tvBlameData vData;

    try
    {
        auto revspec  = m_repo.revparse("HEAD");

        git_oid id = git::str_to_id(oid.toUtf8().constData());

        git_blame_options opts = GIT_BLAME_OPTIONS_INIT;
        opts.newest_commit = id;
        opts.flags |= GIT_BLAME_TRACK_COPIES_SAME_COMMIT_MOVES;
        opts.flags |= GIT_BLAME_TRACK_COPIES_SAME_COMMIT_COPIES;
        opts.flags |= GIT_BLAME_FIRST_PARENT;
        git::Blame blame = m_repo.blame_file(path.toUtf8().constData(), opts);

        QString spec = oid + ":" + path;
        auto fileid = m_repo.revparse(spec.toUtf8().constData());

        auto blob = m_repo.blob_lookup(fileid.single()->id());

        const char* data = static_cast<const char*>(blob.content());
        auto size = blob.size();

        QByteArray ba(data, size);
        QTextStream s(&ba);

        QString sLine;
        std::size_t line = 1;
        while (s.readLineInto(&sLine))
        {
            auto hunk = blame.hunk_byline(line);
            if (nullptr == hunk) break;

            quokkagit::SBlameData d;
            d.oid = git::id_to_str(hunk->orig_commit_id).c_str();
            d.signature = QString("%1 <%2>").arg(hunk->final_signature->name)
                          .arg(hunk->final_signature->email);
            d.linenumber = line;
            d.line = sLine;

            vData.push_back(d);

            ++line;
        }
    }
    catch(std::exception& ex)
    {
        QMessageBox::warning(nullptr, tr("Error"), ex.what());
    }

    return vData;
}

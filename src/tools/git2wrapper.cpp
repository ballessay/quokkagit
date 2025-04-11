#include "git2wrapper.h"

#include "tools/difftool.h"

#include "data/diffentry.h"
#include "data/helpers.h"
#include "data/logentry.h"
#include "data/settings.h"

#include "git2cpp/repo.h"
#include "git2cpp/reference.h"

#include "git2/blame.h"
#include "git2/deprecated.h"

#include <QByteArray>
#include <QMessageBox>
#include <QProcess>
#include <QString>
#include <QTextStream>

#include <cassert>


using namespace quokkagit;


namespace
{
    QString InvaliOid()
    {
        static const QString c_invalidOid = GIT_OID_SHA1_HEXZERO;

        return c_invalidOid;
    }

    git_repository_init_options RepoOptions()
    {
        git_repository_init_options opts;

        return opts;
    }
}


CGit2::CGit2(const quokkagit::SSettings& settings)
    : m_repo(),
      m_settings(settings)
{
}


void CGit2::ChangeRepository(const QString& sPath)
{
    try
    {
        m_repo.reset(new git::Repository(sPath.toUtf8().constData()));
    }
    catch(std::exception& ex)
    {
        QMessageBox::information(nullptr, tr("Failure"),
                                 tr("Failure to open repository: %1")
                                    .arg(ex.what()));
    }
}


QString CGit2::Path() const
{
    return nullptr != m_repo ? m_repo->path() : QString();
}


void CGit2::SetHead(const QString& sHead)
{
    if (nullptr == m_repo) return;

    m_repo->set_head(sHead.toUtf8().constData());
}


QString CGit2::HeadRef() const
{
    return nullptr != m_repo ? m_repo->head().name() : QString();
}


CGit2::Branches CGit2::AllBranches() const
{
    Branches branches;
    if (nullptr == m_repo) return branches;

    for(const auto& ref : m_repo->branches(git::branch_type::ALL))
    {
        if(ref.type() != GIT_REFERENCE_SYMBOLIC)
        {
            const QString hash = helpers::QStringFrom(ref.target());
            const QString name = QString::fromLocal8Bit(ref.name());

            branches[name] = hash;

            emit Message(QString("%1 at %2").arg(name).arg(hash));
        }
    }

    return branches;
}


quokkagit::LogEntries CGit2::Log(const QString& branch,
                                 const CGit2::Branches& b,
                                 const QString& path) const
{
    LogEntries entries;
    if (nullptr == m_repo) return entries;

    std::map<QString, QString> idToNameMap;
    auto Transform = [this, &idToNameMap](const Branches& branches)
    {
        for (const auto& branch : branches)
        {
            idToNameMap[branch.second] = HeadsAt(branches, branch.second);
        }
    };
    Transform(b);

    const auto it = b.find(branch);
    if (it != b.end())
    {
        git_oid oid = helpers::OidFrom(it->second);

        char* p = strdup(path.toUtf8().constData());
        git_strarray a;
        a.strings = &p;
        a.count = 1;

        git_diff_options diffopts = GIT_DIFF_OPTIONS_INIT;
        git_strarray_copy(&diffopts.pathspec, &a);
        git::Pathspec ps(diffopts.pathspec);

        git::RevWalker walk =  m_repo->rev_walker();
        walk.sort(git::revwalker::sorting::topological);
        walk.push(oid);
        SLogEntry e;
        while (auto commit = walk.next())
        {
            e = SLogEntry::FromCommit(commit);
            const auto it = idToNameMap.find(e.sSha);
            if (it != idToNameMap.end())
                e.sSummary.prepend(it->second);

            if (path.isEmpty())
            {
                entries.push_back(e);
            }
            else
            {
                std::size_t parents = commit.parents_num();
                if (parents == 0)
                {
                    int i = commit.tree().pathspec_match(GIT_PATHSPEC_NO_MATCH_ERROR, ps);
                    if (i != 0)
                        continue;
                    else
                    {
                        entries.push_back(e);
                    }
                }
                else if (parents == 1)
                {
                    auto parent = commit.parent(0);
                    auto c_tree = commit.tree();
                    auto p_tree = parent.tree();
                    auto diff = commit.repo().diff(p_tree, c_tree, diffopts);

                    if (diff.deltas_num() > 0)
                    {
                        entries.push_back(e);
                    }
                    else continue;
                }
                else
                {

                }
            }
        }

        git_strarray_free(&diffopts.pathspec);

        free(p);
    }

    return entries;
}


void CGit2::DiffFinished()
{
    QObject* s = sender();
    CDiffTool* d = dynamic_cast<CDiffTool*>(s);
    auto it = m_diffs.find(d);
    if (it != m_diffs.end())
    {
        m_diffs.erase(it);
    }
}


CGit2::vDeltas CGit2::DiffWithParent(int index,
                                     const quokkagit::LogEntries& entries)
{
    vDeltas deltas;
    if (nullptr == m_repo) return deltas;

    if(index > 0 ||
       static_cast<LogEntries::size_type>(index) < entries.size())
    {
        SLogEntry entry = entries.at(static_cast<LogEntries::size_type>(index));

        git_oid oid = entry.oid();

        git::Commit commit = m_repo->commit_lookup(oid);

        git::Tree commitTree = commit.tree();

        size_t n = commit.parents_num();
        if(n == 0) return deltas;

        git_oid parent_oid = commit.parent_id(0);

        git::Commit parentCommit = m_repo->commit_lookup(parent_oid);

        git::Tree parentTree = parentCommit.tree(); // resolve_to_tree(m_repo, parentId);

        git_diff_options opts = GIT_DIFF_OPTIONS_INIT;
        git::Diff diff = m_repo->diff(parentTree, commitTree, opts);
        git_diff_find_options findopts = GIT_DIFF_FIND_OPTIONS_INIT;
        diff.find_similar(findopts);


        CFileLogModel::vFiles files;

        std::vector<git::Tree> trees;

        auto callback = [this, &files, &deltas, &trees](git_diff_delta const & delta,
                                                        git_diff_hunk const &,
                                                        git_diff_line const &) {

            quokkagit::SDelta d;

            d.oldFile.oid = delta.old_file.id;
            d.oldFile.path = QString::fromLocal8Bit(delta.old_file.path);
            d.oldFile.id = helpers::QStringFrom(delta.old_file.id);
            d.newFile.oid = delta.new_file.id;
            d.newFile.path = QString::fromLocal8Bit(delta.new_file.path);
            d.newFile.id = helpers::QStringFrom(delta.new_file.id);

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

            emit Message(QString("Similarity of old: %1 new: %2 is %3")
                            .arg(d.oldFile.path)
                            .arg(d.newFile.path)
                            .arg(delta.similarity));

            if(GIT_DELTA_RENAMED == delta.status)
            {
                d.display = d.oldFile.path + " => " + d.newFile.path;
            }
            else
            {
                d.display = d.newFile.path;
            }

            files.push_back(std::make_pair(status, d.display));
            deltas.push_back(d);
        };

        diff.print(git::diff::format::raw, callback);

        emit NewFiles(files);
    }

    return deltas;
}


void CGit2::DiffBlobs(int deltaIndex, const vDeltas& deltas)
{
    if (nullptr == m_repo) return;
    if(deltaIndex < 0 || deltaIndex >= deltas.size()) return;

    const auto& delta = deltas.at(static_cast<size_t>(deltaIndex));

    SDiffEntry e;

    if(delta.newFile.id != InvaliOid())
    {
        e.newFilename = delta.newFile.path;
        e.newHash = delta.newFile.id.left(10);

        git::Blob blobNew = m_repo->blob_lookup(delta.newFile.oid);
        e.newData = QByteArray(reinterpret_cast<const char*>(blobNew.content()),
                               static_cast<int>(blobNew.size()));
    }

    if(delta.oldFile.id != InvaliOid())
    {
        e.oldFilename = delta.oldFile.path;
        e.oldHash = delta.oldFile.id.left(10);

        git::Blob blobOld = m_repo->blob_lookup(delta.oldFile.oid);
        e.oldData = QByteArray(reinterpret_cast<const char*>(blobOld.content()),
                               static_cast<int>(blobOld.size()));
    }

    CDiffTool* d = new CDiffTool(m_settings.diff, e, this);

    connect(d, &CDiffTool::Message,
            this, &CGit2::Message);

    connect(d, &CDiffTool::ProgrammFinished,
            this, &CGit2::DiffFinished);

    d->Open();

    m_diffs.insert(d);
}


quokkagit::BlameData CGit2::BlameFile(const QString& path,
                                      const QString& oid)
{
    BlameData vData;
    if (nullptr == m_repo) return vData;

    try
    {
        auto revspec  = m_repo->revparse("HEAD");

        git_oid id = helpers::OidFrom(oid);

        git_blame_options opts = GIT_BLAME_OPTIONS_INIT;
        opts.newest_commit = id;
        opts.flags |= GIT_BLAME_TRACK_COPIES_SAME_COMMIT_MOVES;
        opts.flags |= GIT_BLAME_TRACK_COPIES_SAME_COMMIT_COPIES;
        opts.flags |= GIT_BLAME_FIRST_PARENT;
        git::Blame blame = m_repo->blame_file(path.toUtf8().constData(), opts);

        QString spec = oid + ":" + path;
        auto fileid = m_repo->revparse(spec.toUtf8().constData());

        auto blob = m_repo->blob_lookup(fileid.single()->id());

        const char* data = static_cast<const char*>(blob.content());
        auto size = blob.size();

        QByteArray ba(data, size);
        QTextStream s(&ba);

        /*
        QString sLine;
        std::size_t line = 1;
        while (s.readLineInto(&sLine))
        {
            auto hunk = blame.hunk_byline(line);
            if (nullptr == hunk) break;

            SBlameData d;
            d.hash = helpers::QStringFrom(hunk->orig_commit_id);
            d.signature = QString("%1 <%2>").arg(hunk->final_signature->name)
                          .arg(hunk->final_signature->email);
            d.line = line;
            d.data = sLine;
            d.origPath = hunk->orig_path;

            vData.push_back(d);

            ++line;
        }
        */

        const auto hunkCount = blame.hunk_count();
        std::size_t lineCount = 1;
        QString line;
        for (uint32_t i = 0; i < hunkCount; ++i)
        {
            auto hunk = blame.hunk_byindex(i);
            if(nullptr == hunk) break;

            auto lines = hunk->lines_in_hunk;
            while (lines > 0)
            {
                if (s.readLineInto(&line))
                {
                    SBlameData d;
                    d.hash = helpers::QStringFrom(hunk->orig_commit_id);
                    d.signature = QString("%1 <%2>")
                                        .arg(hunk->final_signature->name)
                                        .arg(hunk->final_signature->email);
                    d.line = lineCount++;
                    d.data = line;
                    d.origPath = hunk->orig_path;

                    vData.push_back(d);
                }
                --lines;
            }
        }

        assert(!s.readLineInto(&line) && "Here should be no data left");
    }
    catch(std::exception& ex)
    {
        QMessageBox::warning(nullptr, tr("Error"), ex.what());
    }

    return vData;
}


quokkagit::SLogEntry CGit2::CommitLookup(const QString& hash) const
{
  if (nullptr == m_repo) return SLogEntry();

  git_oid oid = helpers::OidFrom(hash);

  git::Commit commit = m_repo->commit_lookup(oid);

  return SLogEntry::FromCommit(commit);
}


git::Tree CGit2::resolve_to_tree(git::Repository const & repo,
                                 const QString& identifier)
{
    git::Object obj = git::revparse_single(repo, identifier.toUtf8().constData());

    QString sMessage = QString("%1 - %2")
                       .arg(git_object_type2string(obj.type()))
                       .arg(helpers::QStringFrom(obj.id()));

    emit Message(sMessage);

    switch (obj.type())
    {
    case GIT_OBJECT_TREE:
        return obj.to_tree();
    case GIT_OBJECT_COMMIT:
        return obj.to_commit().tree();
    default:
        break;
    }

    throw std::logic_error("Invalid Object");
}


git::Diff CGit2::find_diff(git::Repository const & repo,
                           git::Tree & t1, git::Tree & t2)
{
    git_diff_options opts = GIT_DIFF_OPTIONS_INIT;

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


QString CGit2::HeadsAt(const CGit2::Branches& branches, const QString& id) const
{
    QString text;
    bool firstEntry = true;

    std::map<QString, QString> m;

    auto FormatName = [](QString& text)
    {
        text.remove("refs/heads/");
        text.remove("refs/remotes/");
        //return QString("<p style=\"color: red;\"> %1 </p>").arg(text);
        return text;
    };

    for (const auto& branch : branches)
    {
        if (branch.second == id)
        {
            QString b = branch.first;
            if (firstEntry)
            {
                text = "(" + FormatName(b);
                firstEntry = false;
            }
            else
            {
                text += QString(", ") + FormatName(b);
            }
        }
    }

    if (!text.isEmpty())
        text += ") ";

    return text;
}

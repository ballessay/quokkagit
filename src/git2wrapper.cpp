#include "git2wrapper.h"
//#include <git2.h>
#include "logmodel.h"

#include "git2cpp/repo.h"
#include "git2cpp/reference.h"
#include "git2cpp/id_to_str.h"
#include <QString>
#include <QMessageBox>
#include <QTemporaryFile>
#include <QProcess>


CGit2Wrapper::CGit2Wrapper(const QString& sPath)
  : m_repo(sPath.toUtf8().constData())
{
}

bool CGit2Wrapper::Initialize()
{
  int error = 0;

  /*
  if (m_log.empty())
  {
    git::RevWalker walk =  m_repo.rev_walker();
    walk.sort(git::revwalker::sorting::topological);
    //walk.push(oid);
    walk.push_head();
    while (auto commit = walk.next())
    {
      m_log.push_back(qtgit::SLogEntry::FromCommit(commit));
    }
  }
  */

  return error != 0;
}

void CGit2Wrapper::SetHead(const QString& sHead)
{
  m_repo.set_head(sHead.toUtf8().constData());

  Initialize();

  vBranches b = Branches();
}

QString CGit2Wrapper::HeadRef() const
{
  git::Reference ref = m_repo.head();
  return ref.name();
}

CGit2Wrapper::vBranches CGit2Wrapper::Branches() const
{
  vBranches branches;

  std::vector<git::Reference> vBranches = m_repo.branches(git::branch_type::ALL);

  for(const auto& ref : vBranches)
  {
    if(ref.type() != GIT_REF_SYMBOLIC)
    {
      branches.push_back(std::make_pair(QString::fromLocal8Bit(ref.name()), ref.target()));
      //emit Message(branches.back().first);
    }
  }

  return branches;
}

qtgit::vLogEntries CGit2Wrapper::Log(int branch) const
{
//  qtgit::vLogEntries entries;

  vBranches b = Branches();
//  if (branch >= 0 && branch < b.size())
//  {
//    git_oid oid = b.at(branch).second;

//    git::RevWalker walk =  m_repo.rev_walker();
//    walk.sort(git::revwalker::sorting::topological /*|
//              git::revwalker::sorting::reverse*/);
//    //walk.push(oid);
//    walk.push(oid);
//    while (auto commit = walk.next())
//    {
//      entries.push_back(qtgit::SLogEntry::FromCommit(commit));
//    }
//  }

  return Log(branch, b);
}

qtgit::vLogEntries CGit2Wrapper::Log(int branch, const CGit2Wrapper::vBranches& b) const
{
  qtgit::vLogEntries entries;

  if (branch >= 0 && branch < static_cast<int>(b.size()))
  {
    git_oid oid = b.at(static_cast<vBranches::size_type>(branch)).second;

    git::RevWalker walk =  m_repo.rev_walker();
    walk.sort(git::revwalker::sorting::topological);
    walk.push(oid);
    while (auto commit = walk.next())
    {
      entries.push_back(qtgit::SLogEntry::FromCommit(commit));
    }
  }

  return entries;
}


//void CallBack(git_diff_delta const & delta, git_diff_hunk const &, git_diff_line const &)
//{
//  QString sNew = delta.new_file.path;
//  QString sOld = delta.old_file.path;

//  int i = 0;
//}

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

//void CGit2Wrapper::DiffWithParent(int index)
//{
//  DiffWithParent(index, m_log);
//  if(index > 0 ||
//     static_cast<qtgit::vLogEntries::size_type>(index) < m_log.size())
//  {
//    m_currentEntry = m_log.at(static_cast<qtgit::vLogEntries::size_type>(index));

//    git_oid oid = git::str_to_id(m_currentEntry.sSha.toUtf8().constData());

//    git::Commit commit = m_repo.commit_lookup(oid);

//    //git::Object obj = git::revparse_single(m_repo, entry.sSha.toUtf8().constData());

//    git::Tree tree1 = commit.tree(); //resolve_to_tree(m_repo, entry.sSha);

//    size_t n = commit.parents_num();
//    if(n == 0) return;

//    git_oid parent_oid = commit.parent_id(0);

//    git::Commit parentCommit = m_repo.commit_lookup(parent_oid);

//    //git::Object obj2 = git::revparse_single(m_repo, git::id_to_str(parentCommit.id()).data());

//    //QString parentId(QString::fromStdString(git::id_to_str(parentCommit.id())));
//    git::Tree tree2 = parentCommit.tree(); // resolve_to_tree(m_repo, parentId);

//    git_diff_options opts;
//    git_diff_init_options(&opts, GIT_DIFF_OPTIONS_VERSION);
//    git::Diff diff = m_repo.diff(tree1, tree2, opts);
//    //git::Diff::Stats stats = diff.stats();


//    CFileLogModel::vFiles files;

//    auto callback = [this, &files](git_diff_delta const & delta, git_diff_hunk const &, git_diff_line const &) {

//        QString sPath(QString::fromLocal8Bit(delta.new_file.path));

//        m_files.append(sPath);

//        QString status;
//        switch (delta.status)
//        {
//        case GIT_DELTA_UNMODIFIED:
//          status = "unmodified"; break;
//        case GIT_DELTA_ADDED:
//          status = "added"; break;
//        case GIT_DELTA_DELETED:
//          status = "deleted"; break;
//        case GIT_DELTA_MODIFIED:
//          status = "modified"; break;
//        case GIT_DELTA_RENAMED:
//          status = "renamed"; break;
//        case GIT_DELTA_COPIED:
//          status = "copied"; break;
//        case GIT_DELTA_IGNORED:
//          status = "ignored"; break;
//        case GIT_DELTA_UNTRACKED:
//          status = "untracked"; break;
//        case GIT_DELTA_TYPECHANGE:
//          status = "typechange"; break;
//        case GIT_DELTA_UNREADABLE:
//          status = "UNREADABLE"; break;
//        case GIT_DELTA_CONFLICTED:
//          status = "conflict"; break;
//          break;
//        }

//        files.push_back(std::make_pair(status, sPath));

//        QString sId(QString::fromStdString(git::id_to_str(delta.new_file.id)));

//        m_currentDeltas.push_back(std::make_pair(delta, sPath));

////        git::Object obj = m_repo.revparse_single(sId.toUtf8().constData());

////        QString message = QString("%1 - %2")
////                            .arg(git_object_type2string(obj.type()))
////                            .arg(QString::fromStdString(git::id_to_str(obj.id())));

////        emit Message(message);

//        emit NewFile(m_files);
//    };

//    m_currentDeltas.clear();
//    m_files.clear();

//    diff.print(git::diff::format::name_only, callback);

//    emit NewFiles(files);
//  }
//}

void CGit2Wrapper::DiffWithParent(int index, const qtgit::vLogEntries& entries)
{
  if(index > 0 ||
     static_cast<qtgit::vLogEntries::size_type>(index) < entries.size())
  {
    m_currentEntry = entries.at(static_cast<qtgit::vLogEntries::size_type>(index));

    git_oid oid = m_currentEntry.oid();

    git::Commit commit = m_repo.commit_lookup(oid);

    //git::Object obj = git::revparse_single(m_repo, entry.sSha.toUtf8().constData());

    git::Tree tree1 = commit.tree(); //resolve_to_tree(m_repo, entry.sSha);

    size_t n = commit.parents_num();
    if(n == 0) return;

    git_oid parent_oid = commit.parent_id(0);

    git::Commit parentCommit = m_repo.commit_lookup(parent_oid);

    //git::Object obj2 = git::revparse_single(m_repo, git::id_to_str(parentCommit.id()).data());

    //QString parentId(QString::fromStdString(git::id_to_str(parentCommit.id())));
    git::Tree tree2 = parentCommit.tree(); // resolve_to_tree(m_repo, parentId);

    git_diff_options opts;
    git_diff_init_options(&opts, GIT_DIFF_OPTIONS_VERSION);
    git::Diff diff = m_repo.diff(tree1, tree2, opts);
    //git::Diff::Stats stats = diff.stats();


    CFileLogModel::vFiles files;

    auto callback = [this, &files](git_diff_delta const & delta, git_diff_hunk const &, git_diff_line const &) {

        QString sPath(QString::fromLocal8Bit(delta.new_file.path));

        //m_files.append(sPath);

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
          break;
        }

        files.push_back(std::make_pair(status, sPath));

        QString sId(git::id_to_str(delta.new_file.id).c_str());

        m_currentDeltas.push_back(std::make_pair(delta, sPath));

//        git::Object obj = m_repo.revparse_single(sId.toUtf8().constData());

//        QString message = QString("%1 - %2")
//                            .arg(git_object_type2string(obj.type()))
//                            .arg(QString::fromStdString(git::id_to_str(obj.id())));

//        emit Message(message);

        //emit NewFile(m_files);
    };

    m_currentDeltas.clear();
    //m_files.clear();

    diff.print(git::diff::format::name_only, callback);

    emit NewFiles(files);
  }
}

void CGit2Wrapper::DiffBlobs(int deltaIndex)
{
  if(deltaIndex < 0) return;

  static const QString c_msgTempl("Size: %1");
  git_diff_delta delta = m_currentDeltas.at(static_cast<size_t>(deltaIndex)).first;
  const QString sPath = m_currentDeltas.at(static_cast<size_t>(deltaIndex)).second;

  if (GIT_DELTA_MODIFIED == delta.status ||
      GIT_DELTA_RENAMED == delta.status)
  {
    //git_oid oid = delta
    git::Blob blobNew = m_repo.blob_lookup(delta.new_file.id);

    QString hashNew(QString::fromStdString(git::id_to_str(delta.new_file.id, 10)));

    QByteArray ba(reinterpret_cast<const char*>(blobNew.content()),
                  static_cast<int>(blobNew.size()));

    git::Blob blobOld = m_repo.blob_lookup(delta.old_file.id);

    QString hashOld(QString::fromStdString(git::id_to_str(delta.old_file.id, 10)));

    QByteArray ba2(reinterpret_cast<const char*>(blobOld.content()),
                   static_cast<int>(blobOld.size()));

    CKdiff3 diff(ba, ba2);
    m_diffs.push_back(diff);

    m_diffs.back().Open(sPath, sPath, hashNew, hashOld);

    connect(&m_diffs.back(), &CKdiff3::Message,
            this, &CGit2Wrapper::Message);
  }
}



CKdiff3::CKdiff3(const QByteArray& baOld, const QByteArray baNew)
  : m_process(new QProcess),
    m_old(new QTemporaryFile),
    m_new(new QTemporaryFile)
{
  if(m_old->open())
  {
    m_old->write(baOld);
    m_old->setAutoRemove(true);
    m_old->flush();
  }

  if(m_new->open())
  {
    m_new->write(baNew);
    m_new->setAutoRemove(true);
    m_new->flush();
  }

  connect(m_process.get(), SIGNAL(finished(int)),
          this, SLOT(Finished(int)));
}

CKdiff3::CKdiff3(const CKdiff3& diff)
  : m_process(diff.m_process),
    m_old(diff.m_old),
    m_new(diff.m_new)
{
  connect(m_process.get(), SIGNAL(finished(int)),
          this, SLOT(Finished(int)));
}

void CKdiff3::Open(const QString& sFileNew,
                   const QString& sFileOld,
                   const QString& sHashNew,
                   const QString& sHashOld)
{
//  connect(m_process.get(), &QProcess::finished,
//                   this, &CKdiff3::Finished);
  QStringList args;
  args << "--L1"
       << sFileOld + "@" + sHashOld
       << "--L2"
       << sFileNew + "@" + sHashNew
       << m_old->fileName()
       << m_new->fileName();

  m_process->start("kdiff3", args);
}

void CKdiff3::Finished(int exitCode)
{
  QString msg = QString("ExitCode: %1").arg(exitCode);

  emit Message(msg);
}

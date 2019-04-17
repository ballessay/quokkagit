#ifndef GIT2WRAPPER_H
#define GIT2WRAPPER_H

#include "tools/kdiff3.h"
#include "data/logentry.h"
#include "models/filelogmodel.h"
#include "git2cpp/repo.h"
#include "git2cpp/diff.h"
#include <QStringList>
#include <QObject>
#include <memory>
#include <vector>

class QString;


class CGit2Wrapper : public QObject
{
  Q_OBJECT

public:
  using vReferences = std::vector<git::Reference>;
  using vBranches = std::vector<std::pair<QString, git_oid>>;
  using vDeltas = std::vector<std::pair<git_diff_delta, QString>>;

  CGit2Wrapper(const QString& sPath);

  void SetHead(const QString& sHead);
  QString HeadRef() const;

  void Initialize();

  vBranches Branches() const;

  quokkagit::vLogEntries Log(int branch, const vBranches& b) const;

  vDeltas DiffWithParent(int index, const quokkagit::vLogEntries& entries);

  void DiffBlobs(int deltaIndex, const vDeltas& deltas);

  git_repository* repo() { return m_repo.get(); }

signals:
  void NewFile(const QStringList& list);
  void Message(const QString& message) const;
  void NewFiles(CFileLogModel::vFiles files);

private:
  git::Tree resolve_to_tree(git::Repository const & repo, const QString& identifier);
  git::Diff find_diff(git::Repository const & repo, git::Tree & t1, git::Tree & t2);

private:
  git::Repository m_repo;
  vReferences m_branches;
  std::vector<CKdiff3> m_diffs;
};

#endif // GIT2WRAPPER_H

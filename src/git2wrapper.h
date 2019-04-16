#ifndef GIT2WRAPPER_H
#define GIT2WRAPPER_H

#include "logentry.h"
#include "logmodel.h"
#include "git2cpp/repo.h"
#include "git2cpp/diff.h"
#include <QStringList>
#include <QObject>
#include <memory>
#include <vector>
#include <QTemporaryFile>
#include <QProcess>

class QString;

class CKdiff3 : public QObject
{
  Q_OBJECT

public:
  CKdiff3(const QByteArray& baOld, const QByteArray baNew);
  CKdiff3(const CKdiff3& diff);

signals:
  void Message(QString);

public slots:
  void Open(const QString& sFileOld,
            const QString& sFileNew,
            const QString& sHashOld,
            const QString& sHashNew);
  void Finished(int exitCode);

public:
  std::shared_ptr<QProcess> m_process;
  std::shared_ptr<QTemporaryFile> m_old;
  std::shared_ptr<QTemporaryFile> m_new;
};


class CGit2Wrapper : public QObject
{
  Q_OBJECT

public:
  using vBranches = std::vector<std::pair<QString, git_oid>>;
  using vDeltas = std::vector<std::pair<git_diff_delta, QString>>;

  CGit2Wrapper(const QString& sPath);

  void SetHead(const QString& sHead);
  QString HeadRef() const;

  vBranches Branches() const;

  qtgit::vLogEntries Log(int branch, const vBranches& b) const;

  vDeltas DiffWithParent(int index, const qtgit::vLogEntries& entries);

  void DiffBlobs(int deltaIndex, const vDeltas& deltas);

signals:
  void NewFile(const QStringList& list);
  void Message(const QString& message) const;
  void NewFiles(CFileLogModel::vFiles files);

private:
  git::Tree resolve_to_tree(git::Repository const & repo, const QString& identifier);
  git::Diff find_diff(git::Repository const & repo, git::Tree & t1, git::Tree & t2);

private:
  git::Repository m_repo;
  std::vector<CKdiff3> m_diffs;
};

#endif // GIT2WRAPPER_H

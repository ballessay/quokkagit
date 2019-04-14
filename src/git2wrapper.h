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
  void Open(const QString& sFileNew,
            const QString& sFileOld,
            const QString& sHashNew,
            const QString& sHashOld);
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

  CGit2Wrapper(const QString& sPath);

  bool Initialize();

  void SetHead(const QString& sHead);
  QString HeadRef() const;

  vBranches Branches() const;

  qtgit::vLogEntries Log() const { return m_log; }
  qtgit::vLogEntries Log(int branch) const;
  qtgit::vLogEntries Log(int branch, const vBranches& b) const;
  void DiffWithParent(int index);
  QStringList Files() const { return m_files; }

  void DiffBlobs(int deltaIndex);

signals:
  void NewFile(const QStringList& list);
  void Message(const QString& message) const;
  void NewFiles(CFileLogModel::vFiles files);

private:
  git::Tree resolve_to_tree(git::Repository const & repo, const QString& identifier);
  git::Diff find_diff(git::Repository const & repo, git::Tree & t1, git::Tree & t2);

private:
  git::Repository m_repo;
  qtgit::vLogEntries m_log;
  QStringList m_files;
  qtgit::SLogEntry m_currentEntry;
  std::vector<std::pair<git_diff_delta, QString>> m_currentDeltas;
  std::vector<CKdiff3> m_diffs;
};

#endif // GIT2WRAPPER_H

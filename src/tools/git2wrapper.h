#ifndef GIT2WRAPPER_H
#define GIT2WRAPPER_H

#include "data/blamedata.h"
#include "data/diffentry.h"
#include "data/logentry.h"
#include "models/filelogmodel.h"
#include "git2cpp/repo.h"
#include "git2cpp/diff.h"

#include <QStringList>
#include <QObject>

#include <memory>
#include <set>

namespace quokkagit { struct SSettings; }
class CDiffTool;
class QString;


class CGit2 : public QObject
{
    Q_OBJECT

public:
    using Branches = std::map<QString, QString>;
    using vDeltas = std::vector<quokkagit::SDelta>;

    CGit2(const quokkagit::SSettings& settings);

    void ChangeRepository(const QString& sPath);

    QString Path() const;

    void SetHead(const QString& sHead);
    QString HeadRef() const;

    Branches AllBranches() const;

    quokkagit::LogEntries Log(const QString& branch,
                              const Branches& b,
                              const QString& path = QString()) const;

    vDeltas DiffWithParent(int index, const quokkagit::LogEntries& entries);

    void DiffBlobs(int deltaIndex, const vDeltas& deltas);

    quokkagit::BlameData BlameFile(const QString& sPath, const QString& oid);

    quokkagit::SLogEntry CommitLookup(const QString& hash) const;

signals:
    void NewFile(const QStringList& list);
    void Message(const QString& message) const;
    void NewFiles(CFileLogModel::vFiles files);

protected slots:
    void DiffFinished();

private:
    git::Tree resolve_to_tree(git::Repository const & repo, const QString& identifier);
    git::Diff find_diff(git::Repository const & repo, git::Tree & t1, git::Tree & t2);
    QString HeadsAt(const CGit2::Branches& branches, const QString& id) const;

private:
    std::unique_ptr<git::Repository> m_repo;
    std::set<CDiffTool*> m_diffs;
    const quokkagit::SSettings& m_settings;
};

#endif // GIT2WRAPPER_H

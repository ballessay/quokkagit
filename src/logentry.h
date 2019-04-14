#ifndef LOGENTRY_H
#define LOGENTRY_H

//#include "git2cpp/commit.h"
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <vector>

namespace git { class Commit; }

namespace qtgit
{
  struct SLogEntry
  {
    enum Fields
    {
      Sha,
      Summary,
      Message,
      Commiter,
      CommiterEmail,
      CommitDate,
      Author,
      AuthorEmail,
      AuthorDate,
      // Must be the last entry
      NumberOfFields
    };

    static const char* sha;
    static const char* summary;
    static const char* message;
    static const char* commiter;
    static const char* commiterEmail;
    static const char* commitDate;
    static const char* author;
    static const char* authorEmail;
    static const char* authorDate;

    static SLogEntry FromCommit(const git::Commit& commit);

    QString sSha;
    QString sSummary;
    QString sMessage;
    QString sCommiter;
    QString sCommiterEmail;
    QDateTime qcommitDate;
    QString sAuthor;
    QString sAuthorEmail;
    QDateTime qauthorDate;
  };

  using vLogEntries = std::vector<SLogEntry>;
}

#endif // LOGENTRY_H

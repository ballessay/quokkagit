#include "logentry.h"
#include "git2cpp/commit.h"
#include "git2cpp/id_to_str.h"


namespace
{
  QDateTime FromGitTime(git_time time)
  {
    return QDateTime::fromSecsSinceEpoch(time.time, Qt::LocalTime, time.offset);
  }
}

namespace qtgit
{
  const char* SLogEntry::sha = "Sha";
  const char* SLogEntry::summary = "Summary";
  const char* SLogEntry::message = "Message";
  const char* SLogEntry::commiter = "Commiter";
  const char* SLogEntry::commiterEmail = "Commiter Email";
  const char* SLogEntry::commitDate = "Commit Date";
  const char* SLogEntry::author = "Author";
  const char* SLogEntry::authorEmail = "Author Email";
  const char* SLogEntry::authorDate = "Author Date";

  const char* SLogEntry::c_strings[] = { SLogEntry::sha,
                                         SLogEntry::summary,
                                         SLogEntry::message,
                                         SLogEntry::commiter,
                                         SLogEntry::commiterEmail,
                                         SLogEntry::commitDate,
                                         SLogEntry::author,
                                         SLogEntry::authorEmail,
                                         SLogEntry::authorDate
                                       };

  SLogEntry SLogEntry::FromCommit(const git::Commit& commit)
  {
    SLogEntry entry;

    entry.sSha = QString::fromStdString(git::id_to_str(commit.id()));
    entry.sSummary = commit.summary();
    entry.sMessage = commit.message();

    entry.sAuthor = commit.author()->name;
    entry.sAuthorEmail = commit.author()->email;
    entry.qauthorDate = FromGitTime(commit.author()->when);

    entry.sCommiter = commit.commiter()->name;
    entry.sCommiterEmail = commit.commiter()->email;
    entry.qcommitDate = FromGitTime(commit.commiter()->when);

    return entry;
  }

  git_oid SLogEntry::oid() const
  {
    return git::str_to_id(sSha.toUtf8().constData());
  }
}
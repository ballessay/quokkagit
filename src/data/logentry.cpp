#include "logentry.h"
#include "helpers.h"
#include "git2cpp/commit.h"
#include "git2cpp/id_to_str.h"

#include <QTimeZone>


namespace
{
    QDateTime FromGitTime(git_time time)
    {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        return QDateTime::fromSecsSinceEpoch(time.time, Qt::LocalTime, time.offset);
#else
        return QDateTime::fromSecsSinceEpoch(time.time, QTimeZone::systemTimeZone());
#endif
    }
}


namespace quokkagit
{
    const char SLogEntry::sha[] = "Sha";
    const char SLogEntry::summary[] = "Summary";
    const char SLogEntry::message[] = "Message";
    const char SLogEntry::commiter[] = "Commiter";
    const char SLogEntry::commiterEmail[] = "Commiter Email";
    const char SLogEntry::commitDate[] = "Commit Date";
    const char SLogEntry::author[] = "Author";
    const char SLogEntry::authorEmail[] = "Author Email";
    const char SLogEntry::authorDate[] = "Author Date";

    const char* const SLogEntry::c_strings[NumberOfFields] = {
        SLogEntry::sha,
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

        entry.sSha = helpers::QStringFrom(commit.id());
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
        return helpers::OidFrom(sSha);
    }
}

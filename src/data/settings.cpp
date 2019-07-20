#include "settings.h"
#include <QSettings>
#include <QCoreApplication>


using namespace quokkagit;


namespace
{
    const char* const diff_path = "diff.path";
    const char* const diff_arguments = "diff.arguments";

    const QStringList kdiff3Args = QStringList() << "--L1"
                                                 << "%fo@%io"
                                                 << "--L2"
                                                 << "%fn@%in"
                                                 << "%po"
                                                 << "%pn";

    const char* const settings_hash_display_length = "settings.hashDisplayLength";
    const char* const settings_last_repos = "settings.lastRepos";
    const char* const settings_max_last_repos = "settings.maxLastRepos";
    const char* const settings_font = "settings.font";
}


void SDiffSettings::Load(const QSettings& settings)
{
    path = settings.value(diff_path, "kdiff3").toString();
    arguments = settings.value(diff_arguments, kdiff3Args).toStringList();
}


void SDiffSettings::Save(QSettings& settings) const
{
    settings.setValue(diff_path, path);
    settings.setValue(diff_arguments, arguments);
}


SSettings::SSettings()
    : font(),
      diff()
{
    QSettings::setDefaultFormat(QSettings::IniFormat);
}


SSettings::~SSettings()
{
    Save();
}


void SSettings::Load()
{
    QSettings settings;

    hashDisplayLength = settings.value(settings_hash_display_length, 10).toInt();
    lastRepos = settings.value(settings_last_repos, QStringList()).toStringList();
    maxLastRepos = settings.value(settings_max_last_repos, 10).toInt();
    font = settings.value(settings_font, QFont("Hack", 8)).value<QFont>();

    diff.Load(settings);
}


void SSettings::Save() const
{
    QSettings settings;

    settings.setValue(settings_hash_display_length, hashDisplayLength);
    settings.setValue(settings_last_repos, lastRepos);
    settings.setValue(settings_max_last_repos, maxLastRepos);
    settings.setValue(settings_font, font);

    diff.Save(settings);
}


void SSettings::AddRepoPath(const QString& path)
{
    const auto& it  = std::find(lastRepos.begin(), lastRepos.end(), path);
    if (it != lastRepos.end())
    {
        lastRepos.erase(it);
    }

    if (!path.isEmpty())
    {
        lastRepos.push_front(path);
    }

    if (lastRepos.size() > maxLastRepos)
    {
        lastRepos.pop_back();
    }
}

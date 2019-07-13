#include "settings.h"
#include <QSettings>
#include <QCoreApplication>

using namespace quokkagit;

namespace
{
    const char* const diff_path = "diff.path";
    const char* const diff_arguments = "diff.arguments";

    const char* const settings_font = "settings.font";
}

void SDiffSettings::Load(const QSettings& settings)
{
    path = settings.value(diff_path, "kdiff3").toString();
    arguments = settings.value(diff_arguments).toString();
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

    font = settings.value(settings_font, QFont("Hack", 8)).value<QFont>();

    diff.Load(settings);
}


void SSettings::Save() const
{
    QSettings settings;

    settings.setValue(settings_font, font);

    diff.Save(settings);
}

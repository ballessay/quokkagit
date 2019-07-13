#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QStringList>
#include <QFont>

class QSettings;

namespace quokkagit
{
    struct SDiffSettings
    {
        void Load(const QSettings& settings);
        void Save(QSettings& settings) const;

        QString path;
        QStringList arguments;
    };


    struct SSettings
    {
        SSettings();
        ~SSettings();

        void Load();
        void Save() const;

        QString repoPath;
        QFont font;
        SDiffSettings diff;
    };
}

#endif // SETTINGS_H

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QStringList>
#include <QFont>
#include <vector>

class QSettings;


namespace turtlegit
{
    struct SStyleSettings
    {
        enum Styles
        {
            Default,
            Dark
        };

        void Load(const QSettings& settings);
        void Save(QSettings& settings) const;

        static QStringList StyleNames();

        int id;
    };


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

        void AddRepoPath(const QString& path);
        const QStringList& LastRepos() const { return lastRepos; }

        int hashDisplayLength;
        int maxLastRepos;
        QFont font;
        SStyleSettings style;
        SDiffSettings diff;
        QStringList enabledSearchColumns;
        QStringList enabledLogColumns;

    private:
        QStringList lastRepos;
    };
}

#endif // SETTINGS_H

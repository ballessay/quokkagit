#ifndef DIFFTOOL_H
#define DIFFTOOL_H

#include <QObject>
#include <QProcess>
#include <QString>
#include <QTemporaryFile>
#include <memory>

namespace quokkagit
{
    struct SDiffEntry;
    struct SDiffSettings;
}

class CDiffTool : public QObject
{
    Q_OBJECT

public:
    CDiffTool(const quokkagit::SDiffSettings& settings,
              const quokkagit::SDiffEntry& entry,
              int index);
    CDiffTool(const CDiffTool& diff);
    CDiffTool(const CDiffTool&& diff);
    CDiffTool& operator=(const CDiffTool& other);

signals:
    void Message(QString);
    void ProgrammFinished(int index);

protected slots:
    void Finished(int exitCode);

public slots:
    void Open();

protected:
    virtual QStringList args() const;

    int m_index;
    const quokkagit::SDiffSettings& m_settings;
    const quokkagit::SDiffEntry& m_entry;
    std::shared_ptr<QProcess> m_process;
    std::shared_ptr<QTemporaryFile> m_old;
    std::shared_ptr<QTemporaryFile> m_new;
};


#endif // DIFFTOOL_H

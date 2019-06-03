#ifndef KDIFF3_H
#define KDIFF3_H

#include <QObject>
#include <memory>
#include <QProcess>
#include <QTemporaryFile>


class CKdiff3 : public QObject
{
  Q_OBJECT

public:
  CKdiff3(int index);
  CKdiff3(const CKdiff3& diff);
  CKdiff3(const CKdiff3&& diff);
  CKdiff3& operator=(const CKdiff3& other);

signals:
  void Message(QString);
  void ProgrammFinished(int index);

protected slots:
  void Finished(int exitCode);

public slots:
  void Open(const QByteArray& baOld,
            const QByteArray baNew,
            const QString& sFileOld,
            const QString& sFileNew,
            const QString& sHashOld,
            const QString& sHashNew);

public:
  int m_index;
  std::shared_ptr<QProcess> m_process;
  std::shared_ptr<QTemporaryFile> m_old;
  std::shared_ptr<QTemporaryFile> m_new;
};

#endif // KDIFF3_H

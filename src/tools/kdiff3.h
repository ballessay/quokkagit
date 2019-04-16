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
  CKdiff3(const QByteArray& baOld, const QByteArray baNew);
  CKdiff3(const CKdiff3& diff);
  CKdiff3(const CKdiff3&& diff);

signals:
  void Message(QString);

public slots:
  void Open(const QString& sFileOld,
            const QString& sFileNew,
            const QString& sHashOld,
            const QString& sHashNew);
  void Finished(int exitCode);

public:
  std::shared_ptr<QProcess> m_process;
  std::shared_ptr<QTemporaryFile> m_old;
  std::shared_ptr<QTemporaryFile> m_new;
};

#endif // KDIFF3_H

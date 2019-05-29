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
  CKdiff3(std::size_t index);
  CKdiff3(const CKdiff3& diff);
  CKdiff3(const CKdiff3&& diff);

signals:
  void Message(QString);
  void Finished(std::size_t index, int status);

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
  std::size_t m_index;
  std::shared_ptr<QProcess> m_process;
  std::shared_ptr<QTemporaryFile> m_old;
  std::shared_ptr<QTemporaryFile> m_new;
};

#endif // KDIFF3_H

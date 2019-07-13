#include "kdiff3.h"


CKdiff3::CKdiff3(int index)
    : m_index(index),
      m_process(new QProcess),
      m_old(new QTemporaryFile),
      m_new(new QTemporaryFile)
{
    connect(m_process.get(), SIGNAL(finished(int)),
            this, SLOT(Finished(int)), Qt::UniqueConnection);
}


CKdiff3::CKdiff3(const CKdiff3& diff)
    : m_index(diff.m_index),
      m_process(diff.m_process),
      m_old(diff.m_old),
      m_new(diff.m_new)
{
    connect(m_process.get(), SIGNAL(finished(int)),
            this, SLOT(Finished(int)), Qt::UniqueConnection);
}


CKdiff3::CKdiff3(const CKdiff3&& diff)
    : m_index(std::move(diff.m_index)),
      m_process(std::move(diff.m_process)),
      m_old(std::move(diff.m_old)),
      m_new(std::move(diff.m_new))
{
    connect(m_process.get(), SIGNAL(finished(int)),
            this, SLOT(Finished(int)), Qt::UniqueConnection);

    //  diff.m_process.reset(nullptr);
    //  diff.m_old.reset(nullptr);
    //  diff.m_new.reset(nullptr);
}

CKdiff3& CKdiff3::operator=(const CKdiff3& other)
{
    m_index = other.m_index;
    m_process = other.m_process;
    m_old = other.m_old;
    m_new = other.m_new;

    return *this;
}


void CKdiff3::Finished(int)
{
    emit ProgrammFinished(m_index);
}

void CKdiff3::Open(const QByteArray& baOld,
                   const QByteArray baNew,
                   const QString& sFileOld,
                   const QString& sFileNew,
                   const QString& sHashOld,
                   const QString& sHashNew)
{
    if(m_old->open())
    {
        m_old->write(baOld);
        m_old->setAutoRemove(true);
        m_old->flush();
    }

    if(m_new->open())
    {
        m_new->write(baNew);
        m_new->setAutoRemove(true);
        m_new->flush();
    }

    //  connect(m_process.get(), &QProcess::finished,
    //                   this, &CKdiff3::Finished);
    QStringList args;
    args << "--L1"
         << sFileOld + "@" + sHashOld
         << "--L2"
         << sFileNew + "@" + sHashNew
         << m_old->fileName()
         << m_new->fileName();

    m_process->start("kdiff3", args);
}


//void CKdiff3::Finished(int exitCode)
//{
//  QString msg = QString("ExitCode: %1").arg(exitCode);

//  emit Message(msg);
//}

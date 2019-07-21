#include "difftool.h"


CDiffTool::CDiffTool(const quokkagit::SDiffSettings& settings,
                     const quokkagit::SDiffEntry& entry,
                     int index)
    : m_index(index),
      m_settings(settings),
      m_entry(entry),
      m_process(new QProcess),
      m_old(new QTemporaryFile),
      m_new(new QTemporaryFile)
{
    connect(m_process.get(), SIGNAL(finished(int)),
            this, SLOT(Finished(int)), Qt::UniqueConnection);

    if(m_old->open())
    {
        m_old->write(m_entry.oldData);
        m_old->setAutoRemove(true);
        m_old->flush();
    }

    if(m_new->open())
    {
        m_new->write(m_entry.newData);
        m_new->setAutoRemove(true);
        m_new->flush();
    }
}


CDiffTool::CDiffTool(const CDiffTool& diff)
    : m_index(diff.m_index),
      m_settings(diff.m_settings),
      m_entry(diff.m_entry),
      m_process(diff.m_process),
      m_old(diff.m_old),
      m_new(diff.m_new)
{
    connect(m_process.get(), SIGNAL(finished(int)),
            this, SLOT(Finished(int)), Qt::UniqueConnection);
}


CDiffTool::CDiffTool(const CDiffTool&& diff)
    : m_index(std::move(diff.m_index)),
      m_settings(std::move(diff.m_settings)),
      m_entry(std::move(diff.m_entry)),
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

CDiffTool& CDiffTool::operator=(const CDiffTool& other)
{
    m_index = other.m_index;
    m_settings = other.m_settings;
    m_entry = other.m_entry;
    m_process = other.m_process;
    m_old = other.m_old;
    m_new = other.m_new;

    return *this;
}


void CDiffTool::Finished(int)
{
    emit ProgrammFinished(m_index);
}


void CDiffTool::Open()
{
    m_process->start(m_settings.path, args());
}


QStringList CDiffTool::args() const
{
    auto replacePlaceholders = [this](QString& text)
        {
            text.replace("%fn", m_entry.newFilename);
            text.replace("%fo", m_entry.oldFilename);
            text.replace("%in", m_entry.newHash);
            text.replace("%io", m_entry.oldHash);
            text.replace("%pn", m_new->fileName());
            text.replace("%po", m_old->fileName());
        };

    QStringList args{m_settings.arguments};

    for (auto& arg : args)
        replacePlaceholders(arg);

    return args;
}

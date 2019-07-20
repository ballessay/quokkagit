#include "logmodel.h"
#include "data/settings.h"
#include <QFontMetrics>
#include <cassert>


CLogModel::CLogModel(const quokkagit::LogEntries& log,
                     const quokkagit::SSettings& settings,
                     QObject* pParent)
    : QAbstractTableModel(pParent),
      m_log(log),
      m_settings(settings)
{
}


void CLogModel::SetLog(const quokkagit::LogEntries& log)
{
    beginResetModel();

    m_log.clear();

    m_log = log;

    endResetModel();
}


int CLogModel::rowCount(const QModelIndex&) const
{
    return static_cast<int>(m_log.size());
}


int CLogModel::columnCount(const QModelIndex&) const
{
    int i = quokkagit::SLogEntry::NumberOfFields;
    return i;
}


QVariant CLogModel::data(const QModelIndex& index, int role) const
{
    if (index.isValid())
    {
        int column = index.column();
        int row = index.row();

        if(Qt::DisplayRole == role || Qt::UserRole == role)
        {
            if (row < static_cast<int>(m_log.size()))
            {
                const auto& entry = m_log.at(row);
                switch(column)
                {
                case quokkagit::SLogEntry::Sha:
                    if (Qt::DisplayRole == role)
                    {
                        return entry.sSha.left(m_settings.hashDisplayLength);
                    }
                    else
                    {
                        return entry.sSha;
                    }
                case quokkagit::SLogEntry::Summary:
                {
                    return entry.sSummary;
                }
                case quokkagit::SLogEntry::Message:
                    return entry.sMessage;
                case quokkagit::SLogEntry::Commiter:
                    return entry.sCommiter;
                case quokkagit::SLogEntry::CommiterEmail:
                    return entry.sCommiterEmail;
                case quokkagit::SLogEntry::CommitDate:
                    return entry.qcommitDate.toString(Qt::ISODate);
                case quokkagit::SLogEntry::Author:
                    return entry.sAuthor;
                case quokkagit::SLogEntry::AuthorEmail:
                    return entry.sAuthorEmail;
                case quokkagit::SLogEntry::AuthorDate:
                    return entry.qauthorDate.toString(Qt::ISODate);
                default:
                    assert(false);
                    break;
                }
            }
        }
    }

    return QVariant();
}


QVariant CLogModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(Qt::DisplayRole == role)
    {
        if(Qt::Horizontal == orientation)
        {
            switch(section)
            {
            case quokkagit::SLogEntry::Sha:
                return tr(quokkagit::SLogEntry::sha);
            case quokkagit::SLogEntry::Summary:
                return tr(quokkagit::SLogEntry::summary);
            case quokkagit::SLogEntry::Message:
                return tr(quokkagit::SLogEntry::message);
            case quokkagit::SLogEntry::Commiter:
                return tr(quokkagit::SLogEntry::commiter);
            case quokkagit::SLogEntry::CommiterEmail:
                return tr(quokkagit::SLogEntry::commiterEmail);
            case quokkagit::SLogEntry::CommitDate:
                return tr(quokkagit::SLogEntry::commitDate);
            case quokkagit::SLogEntry::Author:
                return tr(quokkagit::SLogEntry::author);
            case quokkagit::SLogEntry::AuthorEmail:
                return tr(quokkagit::SLogEntry::authorEmail);
            case quokkagit::SLogEntry::AuthorDate:
                return tr(quokkagit::SLogEntry::authorDate);
            default:
                assert(false);
                break;
            }
        }
        else
        {
            return QString::number(m_log.size() - section);
        }
    }

    return QVariant();
}

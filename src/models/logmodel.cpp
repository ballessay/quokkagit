#include "logmodel.h"
#include "data/settings.h"
#include <QFontMetrics>
#include <algorithm>
#include <cassert>

using namespace turtlegit;


CLogModel::CLogModel(const LogEntries& log,
                     const SSettings& settings,
                     QObject* pParent)
    : QAbstractTableModel(pParent),
      m_log(log),
      m_settings(settings)
{
}


void CLogModel::SetLog(const LogEntries& log)
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
    int i = SLogEntry::NumberOfFields;
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
                case SLogEntry::Sha:
                    if (Qt::DisplayRole == role)
                    {
                        return entry.sSha.left(m_settings.hashDisplayLength);
                    }
                    else
                    {
                        return entry.sSha;
                    }
                case SLogEntry::Summary:
                {
                    return entry.sSummary;
                }
                case SLogEntry::Message:
                    return entry.sMessage;
                case SLogEntry::Commiter:
                    return entry.sCommiter;
                case SLogEntry::CommiterEmail:
                    return entry.sCommiterEmail;
                case SLogEntry::CommitDate:
                    return entry.qcommitDate.toString(Qt::ISODate);
                case SLogEntry::Author:
                    return entry.sAuthor;
                case SLogEntry::AuthorEmail:
                    return entry.sAuthorEmail;
                case SLogEntry::AuthorDate:
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
            if(section >= 0 &&
               static_cast<std::size_t>(section) < std::size(SLogEntry::c_strings))
            {
                return tr(SLogEntry::c_strings[section]);
            }
            else
            {
                assert(false && "Unknown header section.");
            }
        }
        else
        {
            return QString::number(m_log.size() - section);
        }
    }

    return QVariant();
}

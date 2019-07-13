#include "filelogmodel.h"
#include <QIcon>


CFileLogModel::CFileLogModel(const vFiles& log, QObject* pParent)
    : QAbstractTableModel(pParent),
      m_log(log)
{
}

void CFileLogModel::SetLog(const vFiles& log)
{
    beginResetModel();

    m_log.clear();

    m_log = log;

    endResetModel();
}

int CFileLogModel::rowCount(const QModelIndex&) const
{
    return static_cast<int>(m_log.size());
}

int CFileLogModel::columnCount(const QModelIndex& parent) const
{
    return NumberOfElements;
}

QVariant CFileLogModel::data(const QModelIndex& index, int role) const
{
    if(Qt::DisplayRole == role || Qt::DecorationRole == role)
    {
        if (index.row() < static_cast<int>(m_log.size()))
        {
            switch(index.column())
            {
            case Filename:
                if (Qt::DecorationRole != role)
                    return m_log.at(index.row()).second;
                else
                {
                    QString status = m_log.at(index.row()).first;
                    return QIcon(":/" + status + ".png");
                }
            case Status:
                return m_log.at(index.row()).first;
            }
        }
    }
    return QVariant();
}

QVariant CFileLogModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(Qt::DisplayRole == role)
    {
        if(Qt::Horizontal == orientation)
        {
            switch (section)
            {
            case Filename:
                return tr("Filename");
            case Status:
                return tr("Status");
            default:
                break;
            }
        }
    }

    return QVariant();
}

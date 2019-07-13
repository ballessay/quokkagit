#include "blamemodel.h"
#include <cassert>

namespace
{
    enum Column
    {
        Sha,
        Signature,
        LineNumber,
        Data,
        // must be last
        Count
    };
}


CBlameModel::CBlameModel(const quokkagit::tvBlameData& data, QObject* pParent)
    : QAbstractTableModel(pParent),
      m_data(data)
{
}

void CBlameModel::SetData(const quokkagit::tvBlameData& data)
{
    beginResetModel();

    m_data.clear();

    m_data = data;

    endResetModel();
}

int CBlameModel::rowCount(const QModelIndex&) const
{
    return static_cast<int>(m_data.size());
}

int CBlameModel::columnCount(const QModelIndex&) const
{
    return Column::Count;
}

QVariant CBlameModel::data(const QModelIndex& index, int role) const
{
    if (Qt::DisplayRole == role && index.isValid())
    {
        int column = index.column();
        int row = index.row();


        if (row < static_cast<int>(m_data.size()))
        {
            const auto& entry = m_data.at(row);
            switch(column)
            {
            case Column::Sha:
                return entry.oid;
            case Column::Signature:
                return entry.signature;
            case Column::LineNumber:
                return QVariant::fromValue(entry.linenumber);
            case Column::Data:
                return entry.line;
            default:
                assert(false);
                break;
            }
        }
    }

    return QVariant();
}

/*
QVariant CBlameModel::headerData(int section, Qt::Orientation orientation, int role) const
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
            return QString::number(m_data.size() - section);
        }
    }

    return QVariant();
}
*/

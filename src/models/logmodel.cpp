#include "logmodel.h"
#include <cassert>
#include <QFontMetrics>

CLogModel::CLogModel(const quokkagit::vLogEntries& log, QObject* pParent)
  : QAbstractTableModel(pParent),
    m_log(log)
{
}

void CLogModel::SetLog(const quokkagit::vLogEntries& log)
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
  if (Qt::DisplayRole == role && index.isValid())
  {
    int column = index.column();
    int row = index.row();


    if (row < static_cast<int>(m_log.size()))
    {
      const auto& entry = m_log.at(row);
      switch(column)
      {
      case quokkagit::SLogEntry::Sha:
        return entry.sSha;
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
        return entry.qcommitDate.toString(Qt::ISODate);;
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

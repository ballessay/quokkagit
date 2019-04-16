#include "logmodel.h"
#include <cassert>
#include <QFontMetrics>
#include <QIcon>

CLogModel::CLogModel(const qtgit::vLogEntries& log, QObject* pParent)
  : QAbstractTableModel(pParent),
    m_log(log)
{
}

void CLogModel::SetLog(const qtgit::vLogEntries& log)
{
  m_log = log;
  beginResetModel();
  endResetModel();
}

int CLogModel::rowCount(const QModelIndex&) const
{
  return static_cast<int>(m_log.size());
}

int CLogModel::columnCount(const QModelIndex&) const
{
  int i = qtgit::SLogEntry::NumberOfFields;
  return i;
}

QVariant CLogModel::data(const QModelIndex& index, int role) const
{
  if (Qt::DisplayRole == role)
  {
    int column = index.column();
    int row = index.row();

    if (row < static_cast<int>(m_log.size()))
    {
      const auto& entry = m_log.at(row);
      switch(column)
      {
      case qtgit::SLogEntry::Sha:
        return entry.sSha.left(10);
      case qtgit::SLogEntry::Summary:
      {
        QFontMetrics metrics(m_font);
        return metrics.elidedText(entry.sSummary, Qt::ElideRight, m_columnWidth - 8);
      }
      case qtgit::SLogEntry::Message:
        return entry.sMessage;
      case qtgit::SLogEntry::Commiter:
        return entry.sCommiter;
      case qtgit::SLogEntry::CommiterEmail:
        return entry.sCommiterEmail;
      case qtgit::SLogEntry::CommitDate:
        return entry.qcommitDate.toString(Qt::ISODate);;
      case qtgit::SLogEntry::Author:
        return entry.sAuthor;
      case qtgit::SLogEntry::AuthorEmail:
        return entry.sAuthorEmail;
      case qtgit::SLogEntry::AuthorDate:
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
      case qtgit::SLogEntry::Sha:
        return tr(qtgit::SLogEntry::sha);
      case qtgit::SLogEntry::Summary:
        return tr(qtgit::SLogEntry::summary);
      case qtgit::SLogEntry::Message:
        return tr(qtgit::SLogEntry::message);
      case qtgit::SLogEntry::Commiter:
        return tr(qtgit::SLogEntry::commiter);
      case qtgit::SLogEntry::CommiterEmail:
        return tr(qtgit::SLogEntry::commiterEmail);
      case qtgit::SLogEntry::CommitDate:
        return tr(qtgit::SLogEntry::commitDate);
      case qtgit::SLogEntry::Author:
        return tr(qtgit::SLogEntry::author);
      case qtgit::SLogEntry::AuthorEmail:
        return tr(qtgit::SLogEntry::authorEmail);
      case qtgit::SLogEntry::AuthorDate:
        return tr(qtgit::SLogEntry::authorDate);
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

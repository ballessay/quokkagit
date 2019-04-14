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
  beginResetModel();
  m_log = log;
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



CFileLogModel::CFileLogModel(const vFiles& log, QObject* pParent)
  : QAbstractTableModel(pParent),
    m_log(log)
{
}

void CFileLogModel::SetLog(const vFiles& log)
{
  //beginResetModel();
  //m_log = log;
  //endResetModel();
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

CLogFilterProxyModel::CLogFilterProxyModel(QObject* parent)
  : QSortFilterProxyModel(parent)
{
}

bool CLogFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
  if(m_filter.isEmpty()) return true;

  for (const auto& action : m_flags)
  {
    if (action->isChecked())
    {
      QModelIndex index = sourceModel()->index(sourceRow, action->data().toInt(), sourceParent);
      if(index.isValid())
         if(index.data().toString().contains(m_filter))
          return true;
    }
  }

  return false;
  /*
  //QModelIndex index = sourceParent.child(sourceRow, qtgit::SLogEntry::Summary);
  QModelIndex index1 = sourceModel()->index(sourceRow, qtgit::SLogEntry::Summary, sourceParent);
  QModelIndex index2 = sourceModel()->index(sourceRow, qtgit::SLogEntry::Message, sourceParent);
  QModelIndex index3 = sourceModel()->index(sourceRow, qtgit::SLogEntry::Author, sourceParent);

  if (!m_filter.isEmpty() && index1.isValid() && index2.isValid())
  {
    QString sSum = index1.data().toString();
    QString sMessage = index2.data().toString();
    QString sAuthor = index3.data().toString();
    return sSum.contains(m_filter) || sMessage.contains(m_filter) || sAuthor.contains(m_filter);
  }
  else
    return true;
  */

  //throw std::logic_error("HOw!?!");
}

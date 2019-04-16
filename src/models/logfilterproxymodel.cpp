#include "logfilterproxymodel.h"

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

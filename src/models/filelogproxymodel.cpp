#include "filelogproxymodel.h"


CFileLogProxyModel::CFileLogProxyModel(QObject* parent)
    : QSortFilterProxyModel(parent)
{
}


void CFileLogProxyModel::SetFilter(const QString& filter)
{
    m_filter = filter;

    invalidateFilter();
}


bool CFileLogProxyModel::filterAcceptsRow(int sourceRow,
                                          const QModelIndex& sourceParent) const
{
    if(m_filter.isEmpty()) return true;

    for (int i = 0; i < sourceModel()->columnCount(); ++i)
    {
        QModelIndex index = sourceModel()->index(sourceRow, i, sourceParent);
        if(index.isValid())
            if(index.data().toString().contains(m_filter, Qt::CaseInsensitive))
                return true;
    }

    return false;
}

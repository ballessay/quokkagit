#include "logfilterproxymodel.h"


CLogFilterProxyModel::CLogFilterProxyModel(QObject* parent)
    : QSortFilterProxyModel(parent)
{
}


void CLogFilterProxyModel::SetFilter(const QString& filter,
                                     const QList<QAction*>& flags)
{
    m_filter = filter;
    m_flags = flags;
    invalidateFilter();
}


bool CLogFilterProxyModel::filterAcceptsRow(int sourceRow,
                                            const QModelIndex& sourceParent) const
{
    if(m_filter.isEmpty()) return true;

    for (const auto& action : m_flags)
    {
        if (action->isChecked())
        {
            QModelIndex index = sourceModel()->index(sourceRow,
                                                     action->data().toInt(),
                                                     sourceParent);
            if(index.isValid())
                if(index.data().toString().contains(m_filter, Qt::CaseInsensitive))
                    return true;
        }
    }

    return false;
}

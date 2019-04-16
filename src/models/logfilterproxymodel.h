#ifndef LOGFILTERPROXYMODEL_H
#define LOGFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QList>
#include <QAction>


class CLogFilterProxyModel : public QSortFilterProxyModel
{
  Q_OBJECT

public:
   CLogFilterProxyModel(QObject* parent = nullptr);

   void SetFilter(const QString& filter, const QList<QAction*>& flags)
   { m_filter = filter; m_flags = flags; invalidateFilter();}

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QString m_filter;
    QList<QAction*> m_flags;
};

#endif // LOGFILTERPROXYMODEL_H

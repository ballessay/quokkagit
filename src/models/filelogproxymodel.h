#ifndef LOGFILEFILTERPROXYMODEL_H
#define LOGFILEFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class CFileLogProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    CFileLogProxyModel(QObject* parent = nullptr);

public slots:
    void SetFilter(const QString& filter);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QString m_filter;
};

#endif // LOGFILEFILTERPROXYMODEL_H

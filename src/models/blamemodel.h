#ifndef BLAMEMODEL_H
#define BLAMEMODEL_H

#include "data/blamedata.h"
#include <QAbstractTableModel>

class CBlameModel : public QAbstractTableModel
{
public:
    CBlameModel(const quokkagit::tvBlameData& data, QObject* pParent = nullptr);

    void SetData(const quokkagit::tvBlameData& Log);
    const quokkagit::tvBlameData& Data() const { return m_data; }

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    //QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    quokkagit::tvBlameData m_data;
};

#endif // BLAMEMODEL_H

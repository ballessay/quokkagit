#ifndef BLAMEMODEL_H
#define BLAMEMODEL_H

#include "data/blamedata.h"
#include <QAbstractTableModel>
#include <QColor>
#include <map>

class QBrush;
class QString;

class CBlameModel : public QAbstractTableModel
{
public:
    enum Column
    {
        Sha,
        Signature,
        LineNumber,
        LineData,
        // must be last
        Count
    };

    explicit CBlameModel(QObject* parent = nullptr);
    CBlameModel(const quokkagit::tvBlameData& data,
                QObject* pParent = nullptr);

    void SetData(const quokkagit::tvBlameData& Log);
    const quokkagit::tvBlameData& Data() const { return m_data; }

    void SetColors(const QColor& fg, const QColor& bg);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    //QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    void CalculateBackgroundColors();

    quokkagit::tvBlameData m_data;
    std::map<QString, QBrush> m_colors;
    QColor m_fg;
    QColor m_bg;
};

#endif // BLAMEMODEL_H

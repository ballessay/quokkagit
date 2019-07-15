#ifndef BLAMEMODEL_H
#define BLAMEMODEL_H

#include "data/blamedata.h"
#include <QAbstractTableModel>
#include <QColor>
#include <map>

class CGit2Wrapper;
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

    explicit CBlameModel(const CGit2Wrapper& git, QObject* parent = nullptr);
    CBlameModel(const CGit2Wrapper& git,
                const quokkagit::BlameData& data,
                QObject* pParent = nullptr);

    void SetData(const quokkagit::BlameData& Log);
    const quokkagit::BlameData& Data() const { return m_data; }

    void SetColors(const QColor& fg, const QColor& bg);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    //QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void SetSelectedHash(const QString& hash);

private:
    void CalculateBackgroundColors();

    quokkagit::BlameData m_data;
    std::map<QString, QBrush> m_colors;
    QColor m_fg;
    QColor m_bg;
    const CGit2Wrapper& m_git;
    QString m_selectedHash;
};

#endif // BLAMEMODEL_H

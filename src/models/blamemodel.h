#ifndef BLAMEMODEL_H
#define BLAMEMODEL_H

#include "data/blamedata.h"
#include <QAbstractTableModel>
#include <QColor>
#include <map>

class CGit2;
namespace turtlegit {
    struct SSettings;
}
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
        OrigPath,
        // must be last
        Count
    };

    explicit CBlameModel(const CGit2& git,
                         const turtlegit::SSettings& settings,
                         QObject* parent = nullptr);

    void SetData(const turtlegit::BlameData& Log);
    const turtlegit::BlameData& Data() const { return m_data; }

    void SetColors(const QColor& fg, const QColor& bg);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void SetSelectedHash(const QString& hash);
    const QString& SelectedHash() const { return m_selectedHash; }

private:
    void CalculateBackgroundColors();

    turtlegit::BlameData m_data;
    std::map<QString, QBrush> m_colors;
    QColor m_fg;
    QColor m_bg;
    const CGit2& m_git;
    QString m_selectedHash;
    const turtlegit::SSettings& m_settings;
};

#endif // BLAMEMODEL_H

#include "blamemodel.h"
#include <QBrush>
#include <QString>
#include <cassert>
#include <set>



CBlameModel::CBlameModel(QObject* parent)
    : QAbstractTableModel(parent),
      m_data()
{

}

CBlameModel::CBlameModel(const quokkagit::BlameData& data, QObject* pParent)
    : QAbstractTableModel(pParent),
      m_data(data)
{
}


void CBlameModel::SetData(const quokkagit::BlameData& data)
{
    beginResetModel();

    m_data.clear();

    m_data = data;

    CalculateBackgroundColors();

    endResetModel();
}


void CBlameModel::SetColors(const QColor& fg, const QColor& bg)
{
    m_fg = QColor(Qt::white);
    m_bg = bg;

    CalculateBackgroundColors();
}


int CBlameModel::rowCount(const QModelIndex&) const
{
    return static_cast<int>(m_data.size());
}


int CBlameModel::columnCount(const QModelIndex&) const
{
    return Column::Count;
}


QVariant CBlameModel::data(const QModelIndex& index, int role) const
{
    if (index.isValid())
    {
        int column = index.column();
        int row = index.row();

        if (row < static_cast<int>(m_data.size()))
        {
            const auto& entry = m_data.at(row);

            if (Qt::DisplayRole == role )
            {
                switch(column)
                {
                case Column::Sha:
                    return entry.hash;
                case Column::Signature:
                    return entry.signature;
                case Column::LineNumber:
                    return QVariant::fromValue(entry.line);
                case Column::LineData:
                    return entry.data;
                default:
                    assert(false);
                    break;
                }
            }
            else if (Qt::ForegroundRole == role)
            {
                const auto& it = m_colors.find(entry.hash);
                return it != m_colors.end() ? it->second : QBrush(m_fg);
            }
//            else if (Qt::BackgroundRole == role)
//            {
//                const auto& it = m_colors.find(entry.oid);
//                return it != m_colors.end() ? it->second : QBrush(m_fg);
//            }
        }
    }

    return QVariant();
}


void CBlameModel::CalculateBackgroundColors()
{
    std::set<QString> shas;

    for (const auto& data : m_data)
        shas.insert(data.hash);

    int noColors = static_cast<int>(shas.size());

    // Source: https://doc.qt.io/archives/qq/qq26-adaptivecoloring.html
    const int HUE_BASE = (m_bg.hue() == -1) ? 90 : m_bg.hue();
    int h, s, v;
    auto it = shas.begin();
    for (int i = 0; i <  noColors; i++) {
        h = int(HUE_BASE + (360.0 / noColors * i)) % 360;
        s = 240;
        v = int(qMax(m_bg.value(), m_fg.value()) * 0.85);

        const int M = 35;
        if ((h < m_bg.hue() + M && h > m_bg.hue() - M)
            || (h < m_fg.hue() + M && h > m_fg.hue() - M))
        {
            h = ((m_bg.hue() + m_fg.hue()) / (i+1)) % 360;
            s = ((m_bg.saturation() + m_fg.saturation() + 2*i)
                 / 2) % 256;
            v = ((m_bg.value() + m_fg.value() + 2*i) / 2)
                % 256;
        }

        m_colors[*it] = QBrush(QColor::fromHsv(h, s, v));

        ++it;
    }
}

#ifndef LOGMODEL_H
#define LOGMODEL_H

#include "data/logentry.h"
#include <QAbstractTableModel>

namespace quokkagit
{
    struct SSettings;
}


class CLogModel : public QAbstractTableModel
{
public:
    CLogModel(const quokkagit::LogEntries& log,
              const quokkagit::SSettings& settings,
              QObject* pParent = nullptr);

    void SetLog(const quokkagit::LogEntries& Log);
    const quokkagit::LogEntries& Log() const { return m_log; }

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    quokkagit::LogEntries m_log;
    const quokkagit::SSettings& m_settings;
};

#endif // LOGMODEL_H

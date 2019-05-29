#ifndef LOGMODEL_H
#define LOGMODEL_H

#include "data/logentry.h"
#include <QAbstractTableModel>


class CLogModel : public QAbstractTableModel
{
public:
  CLogModel(const quokkagit::vLogEntries& Log, QObject* pParent = nullptr);

  void SetLog(const quokkagit::vLogEntries& Log);
  const quokkagit::vLogEntries& Log() const { return m_log; }

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  int columnCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
  quokkagit::vLogEntries m_log;
};

#endif // LOGMODEL_H

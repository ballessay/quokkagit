#ifndef LOGMODEL_H
#define LOGMODEL_H

#include "data/logentry.h"
#include <QAbstractTableModel>
#include <QFont>


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

  void SetFont(const QFont& font) { m_font = font; }
  void SetColumnWidth(int width) { m_columnWidth = width; }

private:
  quokkagit::vLogEntries m_log;
  QFont m_font;
  int m_columnWidth;
  int padding;
};

#endif // LOGMODEL_H

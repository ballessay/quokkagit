#ifndef LOGMODEL_H
#define LOGMODEL_H

#include "logentry.h"
#include <QAbstractTableModel>
#include <QSortFilterProxyModel>
#include <QFont>
#include <QList>
#include <QAction>
#include <vector>

class CLogModel : public QAbstractTableModel
{
public:
  CLogModel(const qtgit::vLogEntries& Log, QObject* pParent = nullptr);

  void SetLog(const qtgit::vLogEntries& Log);
  const qtgit::vLogEntries& Log() const { return m_log; }

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  int columnCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

  void SetFont(const QFont& font) { m_font = font; }
  void SetColumnWidth(int width) { m_columnWidth = width; }

private:
  qtgit::vLogEntries m_log;
  QFont m_font;
  int m_columnWidth;
};


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


class CFileLogModel : public QAbstractTableModel
{
public:
  enum Columns
  {
    Status,
    Filename,
    NumberOfElements
  };

  using vFiles = std::vector<std::pair<QString, QString>>;

  CFileLogModel(const vFiles& log, QObject* pParent = nullptr);

  void SetLog(const vFiles& log);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  int columnCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
  vFiles m_log;
};

#endif // LOGMODEL_H

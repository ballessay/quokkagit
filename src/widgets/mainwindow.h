#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tools/git2wrapper.h"

#include "dialogs/debuglogdialog.h"
#include "models/logmodel.h"

#include <QMainWindow>

class CGit2Wrapper;
class CLogModel;
class CFileLogModel;
class CLogFilterProxyModel;
namespace Ui {
  class CMainWindow;
}


class CMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit CMainWindow(CGit2Wrapper& git, QWidget *parent = nullptr);
  ~CMainWindow();

public slots:
  void LogItemSelected(const QModelIndex& index);
  void LogItemKeyPressed();

  void DiffFile(const QModelIndex& index);
  void DiffCurrentIndex();

  void BlameFile(const QModelIndex& index);

  void ToggleColumn(int id, bool enabled);

  void AddFiles(const CFileLogModel::vFiles& files);

  void AddMessage(QString msg);

  void on_actionOpen_log_triggered();

  void on_branchSelectionToolButton_clicked();
  void on_searchLineEdit_returnPressed();

private:
  Ui::CMainWindow* m_pUi;
  CGit2Wrapper& m_git;
  CLogModel* m_pLogModel;
  CFileLogModel* m_logFileModel;
  CLogFilterProxyModel* m_logProxy;
  CDebugLogDialog m_dbgLogDialog;
  CGit2Wrapper::vDeltas m_deltas;
};

#endif // MAINWINDOW_H

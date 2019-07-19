#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tools/git2wrapper.h"

#include "dialogs/debuglogdialog.h"
#include "models/logmodel.h"
#include "models/filelogmodel.h"

#include <QMainWindow>
#include <memory>

namespace quokkagit { struct SSettings; }
class CGit2;
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
    explicit CMainWindow(CGit2& git,
                         quokkagit::SSettings& settings,
                         QWidget *parent = nullptr);
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

    void OnOpenLogActionTriggered();

    void OnBranchSelectionToolButtonClicked();
    void OnSearchLineEditReturnPressed();

    void OnOpenActionTriggered();
    void OnSettingsActionTriggered();

private:
    std::unique_ptr<Ui::CMainWindow> m_ui;
    CGit2& m_git;
    std::unique_ptr<CLogModel> m_logModel;
    std::unique_ptr<CFileLogModel> m_logFileModel;
    std::unique_ptr<CLogFilterProxyModel> m_logProxy;
    CDebugLogDialog m_dbgLogDialog;
    CGit2::vDeltas m_deltas;
    quokkagit::SSettings& m_settings;
};

#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "data/settings.h"
#include "dialogs/aboutdialog.h"
#include "dialogs/branchselectiondialog.h"
#include "dialogs/blamedialog.h"
#include "dialogs/settingsdialog.h"
#include "menus/logcolumnvisibilitymenu.h"
#include "menus/logcontextmenu.h"
#include "models/logmodel.h"
#include "models/filelogmodel.h"
#include "models/filelogproxymodel.h"
#include "models/logfilterproxymodel.h"
#include "tools/git2wrapper.h"

#include <QHeaderView>
#include <QFileDialog>


namespace
{
    QString FindCurrentBranchName(const CGit2::Branches& branches,
                                   const QString& head)
    {
        const auto it = branches.find(head);
        return it != branches.end() ? it->first : QString();
    }
}


CMainWindow::CMainWindow(CGit2& git,
                         quokkagit::SSettings& settings,
                         QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::CMainWindow),
    m_git(git),
    m_logModel(nullptr),
    m_logFileModel(nullptr),
    m_logProxy(nullptr),
    m_settings(settings)
{
    m_ui->setupUi(this);

    m_settings.AddRepoPath(m_git.Path());

    const auto head = m_git.HeadRef();
    const auto branches = m_git.AllBranches();
    const auto branchName = FindCurrentBranchName(branches, head);

    QString path;
    QStringList args(qApp->arguments());
    if (args.size() > 1)
        path = args.at(1);

    m_ui->branchLabel->setText(head);

    m_logModel.reset(new CLogModel(git.Log(branchName, branches, path), settings, this));

    m_logProxy.reset(new CLogFilterProxyModel(this));
    m_logProxy->setSourceModel(m_logModel.get());

    m_ui->logTableView->setModel(m_logProxy.get());

    m_ui->logTableView->horizontalHeader()->hideSection(quokkagit::SLogEntry::Message);
    m_ui->logTableView->horizontalHeader()->setSectionsMovable(true);
    m_ui->logTableView->horizontalHeader()->setSizePolicy(QSizePolicy::Maximum,
                                                          QSizePolicy::Preferred);

    int columns = m_logModel->columnCount();
    for(int i = 0; i < columns; ++i)
    {
        if(i != quokkagit::SLogEntry::Summary)
        {
            m_ui->logTableView->resizeColumnToContents(i);
        }
        else {
            m_ui->logTableView->setColumnWidth(i, 400);
        }
    }

    connect(m_ui->logTableView, &QTableView::activated,
            this, &CMainWindow::LogItemSelected);

    connect(m_ui->logTableView, &QTableView::clicked,
            this, &CMainWindow::LogItemSelected);

    connect(m_ui->logTableView, &CTableWidget::enterOrReturnPressed,
            this, &CMainWindow::LogItemKeyPressed);

    CLogColumnVisibilityMenu* menu = new CLogColumnVisibilityMenu(m_settings, this);
    m_ui->tableViewToolButton->setMenu(menu);

    connect(m_ui->tableViewToolButton, &QToolButton::clicked,
            m_ui->tableViewToolButton, &QToolButton::showMenu);

    connect(menu, &CLogColumnVisibilityMenu::ToggleColumn,
            this, &CMainWindow::OnToggleLogColumn);

    menu->EmitState();

    CFileLogModel::vFiles files;

    m_logFileModel.reset(new CFileLogModel(files, this));

    m_logFileProxy.reset(new CFileLogProxyModel(this));
    m_logFileProxy->setSourceModel(m_logFileModel.get());

    connect(m_ui->fileSearchLineEdit, &QLineEdit::textChanged,
            m_logFileProxy.get(), &CFileLogProxyModel::SetFilter);

    m_ui->filesTableView->setModel(m_logFileProxy.get());

    m_ui->logTableView->setContextMenuPolicy(Qt::ActionsContextMenu);
    CLogContextMenu* pMenu = new CLogContextMenu(this);
    for (auto action : pMenu->actions())
    {
        m_ui->logTableView->addAction(action);
    }

    m_ui->filesTableView->setContextMenuPolicy(Qt::ActionsContextMenu);
    CLogFilesContextMenu* pFileMenu = new CLogFilesContextMenu(this);
    for (auto& action : pFileMenu->actions())
    {
        auto dispatch = [this, action]() {
            if ("Diff" == action->text())
                DiffCurrentIndex();
            if ("Blame" == action->text())
                BlameFile(m_ui->filesTableView->currentIndex());
        };
        m_ui->filesTableView->addAction(action);

        connect(action, &QAction::triggered, dispatch);
    }

    CreateHistoryMenu();

    CLogSearchContextMenu* searchMenu = new CLogSearchContextMenu(m_settings, this);
    m_ui->searchOptionsToolButton->setMenu(searchMenu);

    connect(m_ui->searchOptionsToolButton, &QToolButton::clicked,
            m_ui->searchOptionsToolButton, &QToolButton::showMenu);

    connect(searchMenu, &CLogSearchContextMenu::ToggleColumn,
            this, &CMainWindow::OnToggleSearchColumn);


    connect(&m_git, &CGit2::NewFiles,
            this, &CMainWindow::AddFiles);

    connect(&m_git, &CGit2::Message,
            this, &CMainWindow::AddMessage);

    connect(m_ui->filesTableView, &QAbstractItemView::doubleClicked,
            this, &CMainWindow::DiffFile);

    connect(m_ui->filesTableView, &CTableWidget::enterOrReturnPressed,
            this, &CMainWindow::DiffCurrentIndex);


    // file menu
    connect(m_ui->actionOpen_repository, &QAction::triggered,
            this, &CMainWindow::OnOpenActionTriggered);

    connect(m_ui->actionSettings, &QAction::triggered,
            this, &CMainWindow::OnSettingsActionTriggered);

    connect(m_ui->actionOpen_log, &QAction::triggered,
            this, &CMainWindow::OnOpenLogActionTriggered);

    // help menu
    connect(m_ui->actionAbout, &QAction::triggered,
            this, &CMainWindow::OnAboutActionTriggered);


    // tool buttons
    connect(m_ui->branchSelectionToolButton, &QToolButton::clicked,
            this, &CMainWindow::OnBranchSelectionToolButtonClicked);

    connect(m_ui->logSearchLineEdit, &QLineEdit::returnPressed,
            this, &CMainWindow::OnSearchLineEditReturnPressed);
}


CMainWindow::~CMainWindow()
{
}


void CMainWindow::LogItemSelected(const QModelIndex& index)
{
    m_logFileModel->SetLog(CFileLogModel::vFiles());

    QModelIndex i = m_logProxy->mapToSource(index);

    int r = i.row();
    QModelIndex msgIndex = m_logModel->index(r, quokkagit::SLogEntry::Message);

    const QString msg = m_logModel->data(msgIndex).toString();

    m_ui->messageTextEdit->setText(msg);

    m_ui->logTableView->setCurrentIndex(m_logModel->index(r, 0));

    m_deltas = m_git.DiffWithParent(r, m_logModel->Log());
}


void CMainWindow::LogItemKeyPressed()
{
    LogItemSelected(m_ui->logTableView->currentIndex());
}


void CMainWindow::DiffFile(const QModelIndex& index)
{
    QModelIndex i = m_logFileProxy->mapToSource(index);

    m_git.DiffBlobs(i.row(), m_deltas);
}


void CMainWindow::DiffCurrentIndex()
{
    DiffFile(m_ui->filesTableView->currentIndex());
}


void CMainWindow::BlameFile(const QModelIndex& index)
{
    QModelIndex sourceIndex = m_logFileProxy->mapToSource(index);

    int r = sourceIndex.row();

    const auto& delta = m_deltas.at(r);

    const QModelIndex i = m_logModel->index(m_ui->logTableView->currentIndex().row(),
                                             quokkagit::SLogEntry::Sha);
    const QString hash = m_logModel->data(i, Qt::UserRole).toString();

    CBlameDialog::SData data = {m_git, hash, delta.newFile.path, m_settings};
    CBlameDialog d(data, this);
    d.exec();
}


void CMainWindow::OnToggleLogColumn(int id, bool enabled)
{
    const QString name{quokkagit::SLogEntry::c_strings[id]};

    if (enabled)
    {
        m_ui->logTableView->horizontalHeader()->showSection(id);

        if (!m_settings.enabledLogColumns.contains(name))
            m_settings.enabledLogColumns.append(name);
    }
    else
    {
        m_ui->logTableView->horizontalHeader()->hideSection(id);

        m_settings.enabledLogColumns.removeAll(name);
    }
}


void CMainWindow::OnToggleSearchColumn(int id, bool enabled)
{
    const QString name{quokkagit::SLogEntry::c_strings[id]};

    if (enabled)
    {
        if (!m_settings.enabledSearchColumns.contains(name))
            m_settings.enabledSearchColumns.append(name);
    }
    else
    {
        m_settings.enabledSearchColumns.removeAll(name);
    }
}


void CMainWindow::AddFiles(const CFileLogModel::vFiles& files)
{
    m_logFileModel->SetLog(files);

    m_ui->filesTableView->resizeColumnToContents(0);
}


void CMainWindow::AddMessage(QString msg)
{
    m_dbgLogDialog.AddMessage(msg);
}


void CMainWindow::OnOpenLogActionTriggered()
{
    m_dbgLogDialog.exec();
}


void CMainWindow::OnBranchSelectionToolButtonClicked()
{
    CGit2::Branches b = m_git.AllBranches();

    CBranchSelectionDialog d(b, this);
    if(d.exec() == QDialog::Accepted)
    {
        const auto branch = d.currentSelection();
        if (branch.isEmpty()) return;

        quokkagit::LogEntries entries = m_git.Log(branch, b);

        m_logModel->SetLog(entries);

        m_ui->branchLabel->setText(branch);
    }
}


void CMainWindow::OnSearchLineEditReturnPressed()
{
    QList<QAction*> actions = m_ui->searchOptionsToolButton->menu()->actions();

    m_settings.enabledSearchColumns.clear();
    for (const auto action : actions)
        m_settings.enabledSearchColumns.append(quokkagit::SLogEntry::c_strings[action->data().toInt()]);

    m_logProxy->SetFilter(m_ui->logSearchLineEdit->text(), actions);
}


void CMainWindow::OnOpenActionTriggered()
{
    const QString path =
            QFileDialog::getExistingDirectory(this,
                                              tr("Select git repository..."),
                                              m_git.Path());
    ChangeRepository(path);
}


void CMainWindow::OnSettingsActionTriggered()
{
    CSettingsDialog d(m_settings, this);
    if (d.exec()  == QDialog::Accepted)
    {
        m_settings = d.currentSettings();

        emit SettingsChanged();

        // must be called after the settings change has propagated
        m_ui->logTableView->resizeColumnsToContents();
        m_ui->logTableView->horizontalHeader()->setStretchLastSection(true);
        m_ui->logTableView->horizontalHeader()->resizeSection(quokkagit::SLogEntry::Summary, 350);
        m_ui->filesTableView->resizeColumnsToContents();
        m_ui->filesTableView->horizontalHeader()->setStretchLastSection(true);
    }
}


void CMainWindow::OnHistoryActionTriggered()
{
    auto action = qobject_cast<QAction*>(sender());
    const auto path = action->text();

    ChangeRepository(path);
}


void CMainWindow::OnAboutActionTriggered()
{
    CAboutDialog d(this);
    d.exec();
}


void CMainWindow::CreateHistoryMenu()
{
    m_ui->menuRecent->clear();

    for (const auto& path : m_settings.LastRepos())
    {
        auto action = m_ui->menuRecent->addAction(path);
        connect(action, &QAction::triggered,
                this, &CMainWindow::OnHistoryActionTriggered);
    }
}


void CMainWindow::ChangeRepository(const QString path)
{
    if (!path.isEmpty())
    {
        m_git.ChangeRepository(path);

        const auto repoPath = m_git.Path();
        m_settings.AddRepoPath(repoPath);

        const auto head = m_git.HeadRef();
        const auto branches = m_git.AllBranches();
        const auto branchName = FindCurrentBranchName(branches, head);
        QString path;
        m_logModel->SetLog(m_git.Log(branchName, branches, path));

//        m_ui->logTableView->resizeColumnsToContents();
//        m_ui->logTableView->horizontalHeader()->setStretchLastSection(true);
//        m_ui->filesTableView->resizeColumnsToContents();
//        m_ui->filesTableView->horizontalHeader()->setStretchLastSection(true);

        m_logFileModel->SetLog(CFileLogModel::vFiles());

        CreateHistoryMenu();
    }
}

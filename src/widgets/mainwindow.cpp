#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "models/logmodel.h"
#include "models/filelogmodel.h"
#include "models/logfilterproxymodel.h"
#include "dialogs/branchselectiondialog.h"
#include "dialogs/blamedialog.h"
#include "menus/logcolumnvisibilitymenu.h"
#include "menus/logcontextmenu.h"
#include "tools/git2wrapper.h"
#include <QHeaderView>


CMainWindow::CMainWindow(CGit2Wrapper& git,
                         quokkagit::SSettings& settings,
                         QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::CMainWindow),
    m_git(git),
    m_pLogModel(nullptr),
    m_logFileModel(nullptr),
    m_logProxy(nullptr),
    m_settings(settings)
{
    m_ui->setupUi(this);

    QString head = m_git.HeadRef();
    CGit2Wrapper::vBranches branches = m_git.Branches();
    int index = 0;
    for (const auto& branch : branches)
    {
        if(branch.first == head)
            break;
        else
            ++index;
    }

    QString path;
    QStringList args(qApp->arguments());
    if (args.size() > 1)
        path = args.at(1);

    m_pLogModel = new CLogModel(git.Log(index, branches, path), this);

    m_ui->branchLabel->setText(m_git.HeadRef());

    m_logProxy = new CLogFilterProxyModel(this);
    m_logProxy->setSourceModel(m_pLogModel);
    m_ui->logTableView->setModel(m_logProxy);

    m_ui->logTableView->horizontalHeader()->hideSection(quokkagit::SLogEntry::Message);
    m_ui->logTableView->horizontalHeader()->setSectionsMovable(true);
    m_ui->logTableView->horizontalHeader()->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

    int columns = m_pLogModel->columnCount();
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

    //  connect(m_pUi->logTableView, &QTableView::entered,
    //          this, &CMainWindow::LogItemSelected);

    CLogColumnVisibilityMenu* menu = new CLogColumnVisibilityMenu(this);
    m_ui->tableViewToolButton->setMenu(menu);

    connect(m_ui->tableViewToolButton, &QToolButton::clicked,
            m_ui->tableViewToolButton, &QToolButton::showMenu);

    connect(menu, &CLogColumnVisibilityMenu::ToggleColumn,
            this, &CMainWindow::ToggleColumn);

    menu->EmitState();

    CFileLogModel::vFiles files;

    m_logFileModel = new CFileLogModel(files, this);

    m_ui->pFilesTableView->setModel(m_logFileModel);

    m_ui->logTableView->setContextMenuPolicy(Qt::ActionsContextMenu);
    CLogContextMenu* pMenu = new CLogContextMenu(this);
    for (auto action : pMenu->actions())
    {
        m_ui->logTableView->addAction(action);
    }

    m_ui->pFilesTableView->setContextMenuPolicy(Qt::ActionsContextMenu);
    CLogFilesContextMenu* pFileMenu = new CLogFilesContextMenu(this);
    for (auto& action : pFileMenu->actions())
    {
        auto dispatch = [this, action]() {
            if ("Diff" == action->text())
                DiffFile(m_ui->pFilesTableView->currentIndex());
            if ("Blame" == action->text())
                BlameFile(m_ui->pFilesTableView->currentIndex());
            else {
                int notYet  = 0;
            }
        };
        m_ui->pFilesTableView->addAction(action);

        connect(action, &QAction::triggered, dispatch);
    }

    CLogSearchContextMenu* searchMenu = new CLogSearchContextMenu(this);
    m_ui->searchOptionsToolButton->setMenu(searchMenu);

    connect(m_ui->searchOptionsToolButton, &QToolButton::clicked,
            m_ui->searchOptionsToolButton, &QToolButton::showMenu);

    connect(&m_git, &CGit2Wrapper::NewFiles,
            this, &CMainWindow::AddFiles);

    connect(&m_git, &CGit2Wrapper::Message,
            this, &CMainWindow::AddMessage);

    connect(m_ui->pFilesTableView, &QAbstractItemView::doubleClicked,
            this, &CMainWindow::DiffFile);

    connect(m_ui->pFilesTableView, &CTableWidget::enterOrReturnPressed,
            this, &CMainWindow::DiffCurrentIndex);
}


CMainWindow::~CMainWindow()
{
}


void CMainWindow::LogItemSelected(const QModelIndex& index)
{
    m_logFileModel->SetLog(CFileLogModel::vFiles());

    QModelIndex i = m_logProxy->mapToSource(index);

    int r = i.row();

    const QString msg = m_pLogModel->data(m_pLogModel->index(r, quokkagit::SLogEntry::Message)).toString();

    m_ui->pMessageTextEdit->setText(msg);

    m_ui->logTableView->setCurrentIndex(m_pLogModel->index(r, 0));

    m_deltas = m_git.DiffWithParent(r, m_pLogModel->Log());
}

void CMainWindow::LogItemKeyPressed()
{
    LogItemSelected(m_ui->logTableView->currentIndex());
}


void CMainWindow::DiffFile(const QModelIndex& index)
{
    m_git.DiffBlobs(index.row(), m_deltas);
}

void CMainWindow::DiffCurrentIndex()
{
    DiffFile(m_ui->pFilesTableView->currentIndex());
}


void CMainWindow::BlameFile(const QModelIndex& index)
{
    int r = index.row();

    git_diff_delta delta;
    QString path;

    std::tie(delta, path) = m_deltas.at(r);

    const QModelIndex i = m_pLogModel->index(m_ui->logTableView->currentIndex().row(),
                                             quokkagit::SLogEntry::Sha);
    const QString sha = m_pLogModel->data(i).toString();

    CBlameDialog d(m_git.BlameFile(path, sha), this);
    if (d.exec())
    {

    }
}


void CMainWindow::ToggleColumn(int id, bool enabled)
{
    if (enabled)
    {
        m_ui->logTableView->horizontalHeader()->showSection(id);
    }
    else
    {
        m_ui->logTableView->horizontalHeader()->hideSection(id);
    }
}


void CMainWindow::AddFiles(const CFileLogModel::vFiles& files)
{
    m_logFileModel->SetLog(files);

    m_ui->pFilesTableView->resizeColumnToContents(0);
}


void CMainWindow::AddMessage(QString msg)
{
    m_dbgLogDialog.AddMessage(msg);
}


void CMainWindow::on_actionOpen_log_triggered()
{
    m_dbgLogDialog.exec();
}


void CMainWindow::on_branchSelectionToolButton_clicked()
{
    CGit2Wrapper::vBranches b = m_git.Branches();

    CBranchSelectionDialog d(b, this);
    if(d.exec() == QDialog::Accepted)
    {
        int index = d.currentSelection();
        if(index >= 0 && index < static_cast<int>(b.size()))
        {
            quokkagit::vLogEntries entries = m_git.Log(d.currentSelection(), b);

            m_pLogModel->SetLog(entries);

            m_ui->branchLabel->setText(b.at(static_cast<quokkagit::vLogEntries::size_type>(index)).first);
        }
    }
}


void CMainWindow::on_searchLineEdit_returnPressed()
{
    QList<QAction*> actions = m_ui->searchOptionsToolButton->menu()->actions();

    m_logProxy->SetFilter(m_ui->searchLineEdit->text(), actions);
}

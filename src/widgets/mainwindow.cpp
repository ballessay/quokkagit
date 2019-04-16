#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "models/logmodel.h"
#include "models/filelogmodel.h"
#include "models/logfilterproxymodel.h"
#include "dialogs/branchselectiondialog.h"
#include "menus/logcolumnvisibilitymenu.h"
#include "menus/logcontextmenu.h"
#include "tools/git2wrapper.h"
#include <QHeaderView>

CMainWindow::CMainWindow(CGit2Wrapper& git, QWidget *parent) :
  QMainWindow(parent),
  m_pUi(new Ui::CMainWindow),
  m_git(git),
  m_pLogModel(nullptr),
  m_logFileModel(nullptr),
  m_logProxy(nullptr)
{
  m_pUi->setupUi(this);

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

  m_pLogModel = new CLogModel(git.Log(index, branches), this);

  m_pUi->branchLabel->setText(m_git.HeadRef());

  m_logProxy = new CLogFilterProxyModel(this);
  m_logProxy->setSourceModel(m_pLogModel);
  m_pUi->logTableView->setModel(m_logProxy);

  m_pLogModel->SetFont(m_pUi->logTableView->font());

  m_pUi->logTableView->horizontalHeader()->hideSection(qtgit::SLogEntry::Message);
  m_pUi->logTableView->horizontalHeader()->setSectionsMovable(true);
  m_pUi->logTableView->horizontalHeader()->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
  m_pLogModel->SetColumnWidth(m_pUi->logTableView->columnWidth(qtgit::SLogEntry::Summary));
  //m_pUi->logTableView->horizontalHeader()->setSectionResizeMode(qtgit::SLogEntry::Summary, QHeaderView::Interactive);

  int columns = m_pLogModel->columnCount();
  for(int i = 0; i < columns; ++i)
  {
    if(i != qtgit::SLogEntry::Summary)
    {
      m_pUi->logTableView->resizeColumnToContents(i);
    }
    else {
      m_pUi->logTableView->setColumnWidth(i, 400);
    }
  }

  connect(m_pUi->logTableView, &QTableView::activated,
          this, &CMainWindow::LogItemSelected2);

  connect(m_pUi->logTableView, &QTableView::clicked,
          this, &CMainWindow::LogItemSelected2);

  connect(m_pUi->logTableView, &QTableView::entered,
          this, &CMainWindow::LogItemSelected2);

  connect(m_pUi->logTableView, &QTableView::pressed,
          this, &CMainWindow::LogItemSelected2);

  CLogColumnVisibilityMenu* menu = new CLogColumnVisibilityMenu(this);
  m_pUi->tableViewToolButton->setMenu(menu);

  connect(m_pUi->tableViewToolButton, &QToolButton::clicked,
          m_pUi->tableViewToolButton, &QToolButton::showMenu);

  connect(menu, &CLogColumnVisibilityMenu::ToggleColumn,
          this, &CMainWindow::ToggleColumn);

  menu->EmitState();

  m_pUi->logTableView->setContextMenuPolicy(Qt::ActionsContextMenu);
  CLogContextMenu* pMenu = new CLogContextMenu(this);
  for (auto action : pMenu->actions())
  {
    m_pUi->logTableView->addAction(action);
  }

  m_pUi->pFilesTableView->setContextMenuPolicy(Qt::ActionsContextMenu);
  CLogFilesContextMenu* pFileMenu = new CLogFilesContextMenu(this);
  for (auto& action : pFileMenu->actions())
  {
    auto dispatch = [this, action]() {
      if ("Diff" == action->text())
        FileSelected(m_pUi->pFilesTableView->currentIndex());
      else {
        int notYet  = 0;
      }
    };
    m_pUi->pFilesTableView->addAction(action);

    connect(action, &QAction::triggered, dispatch);
  }

  CLogSearchContextMenu* searchMenu = new CLogSearchContextMenu(this);
  m_pUi->searchOptionsToolButton->setMenu(searchMenu);

  connect(m_pUi->searchOptionsToolButton, &QToolButton::clicked,
          m_pUi->searchOptionsToolButton, &QToolButton::showMenu);

  connect(&m_git, &CGit2Wrapper::NewFiles,
          this, &CMainWindow::AddFiles);

  connect(&m_git, &CGit2Wrapper::Message,
          this, &CMainWindow::AddMessage);

  connect(m_pUi->pFilesTableView, &QAbstractItemView::doubleClicked,
          this, &CMainWindow::FileSelected);
}

CMainWindow::~CMainWindow()
{
  delete m_pUi;
}

void CMainWindow::LogItemSelected(int index)
{
  const QString msg = m_pLogModel->data(m_pLogModel->index(index, qtgit::SLogEntry::Message)).toString();

  m_pUi->pMessageTextEdit->setText(msg);

  m_pLogModel->SetColumnWidth(m_pUi->logTableView->columnWidth(qtgit::SLogEntry::Summary));

  m_pUi->logTableView->setCurrentIndex(m_pLogModel->index(index, 0));

  m_deltas = m_git.DiffWithParent(index, m_pLogModel->Log());
}

void CMainWindow::LogItemSelected2(const QModelIndex& index)
{
  QModelIndex i = m_logProxy->mapToSource(index);
  //QModelIndex i2 = m_logProxy->mapFromSource(index);

  LogItemSelected(i.row());
}

void CMainWindow::FileSelected(const QModelIndex& index)
{
  //QModelIndex i = m_logProxy->mapToSource(index);

  m_git.DiffBlobs(index.row(), m_deltas);
}

void CMainWindow::ToggleColumn(int id, bool enabled)
{
  if (enabled)
  {
    m_pUi->logTableView->horizontalHeader()->showSection(id);
  }
  else
  {
    m_pUi->logTableView->horizontalHeader()->hideSection(id);
  }
}

void CMainWindow::AddFiles(const CFileLogModel::vFiles& files)
{
  if(m_logFileModel)
  {
    m_logFileModel->deleteLater();
    m_logFileModel = nullptr;
  }

  m_logFileModel = new CFileLogModel(files, this);

  m_pUi->pFilesTableView->setModel(m_logFileModel);

//  else
//  {
//    m_logFileModel->SetLog(files);
//  }
    //

  m_pUi->pFilesTableView->resizeColumnToContents(0);
//  m_pUi->pFilesTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
//  m_pUi->pFilesTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
}

void CMainWindow::AddMessage(QString msg)
{
  //m_pUi->pFilesListWidget->addItem(msg);
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
    if(index >= 0 && index < b.size())
    {
      qtgit::vLogEntries entries = m_git.Log(d.currentSelection(), b);

      if (!m_pLogModel)
      {
          m_pLogModel = new CLogModel(entries, this);

          m_logProxy->setSourceModel(m_pLogModel);

          m_pUi->logTableView->setModel(m_logProxy);
      }
      else {
        m_pLogModel->SetLog(entries);
      }

      m_pUi->branchLabel->setText(b.at(index).first);
    }
  }
}

void CMainWindow::on_searchLineEdit_returnPressed()
{
  QList<QAction*> actions = m_pUi->searchOptionsToolButton->menu()->actions();

  m_logProxy->SetFilter(m_pUi->searchLineEdit->text(), actions);
}

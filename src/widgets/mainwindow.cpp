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

  m_pUi->logTableView->horizontalHeader()->hideSection(quokkagit::SLogEntry::Message);
  m_pUi->logTableView->horizontalHeader()->setSectionsMovable(true);
  m_pUi->logTableView->horizontalHeader()->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
  m_pLogModel->SetColumnWidth(m_pUi->logTableView->columnWidth(quokkagit::SLogEntry::Summary));
  //m_pUi->logTableView->horizontalHeader()->setSectionResizeMode(qtgit::SLogEntry::Summary, QHeaderView::Interactive);

  int columns = m_pLogModel->columnCount();
  for(int i = 0; i < columns; ++i)
  {
    if(i != quokkagit::SLogEntry::Summary)
    {
      m_pUi->logTableView->resizeColumnToContents(i);
    }
    else {
      m_pUi->logTableView->setColumnWidth(i, 400);
    }
  }

//  connect(m_pUi->logTableView, &QTableView::activated,
//          this, &CMainWindow::LogItemSelected2);

  connect(m_pUi->logTableView, &QTableView::clicked,
          this, &CMainWindow::LogItemSelected2);

  connect(m_pUi->logTableView, &QTableView::entered,
          this, &CMainWindow::LogItemSelected2);

  CLogColumnVisibilityMenu* menu = new CLogColumnVisibilityMenu(this);
  m_pUi->tableViewToolButton->setMenu(menu);

  connect(m_pUi->tableViewToolButton, &QToolButton::clicked,
          m_pUi->tableViewToolButton, &QToolButton::showMenu);

  connect(menu, &CLogColumnVisibilityMenu::ToggleColumn,
          this, &CMainWindow::ToggleColumn);

  menu->EmitState();

  CFileLogModel::vFiles files;

  m_logFileModel = new CFileLogModel(files, this);

  m_pUi->pFilesTableView->setModel(m_logFileModel);

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
        DiffFile(m_pUi->pFilesTableView->currentIndex());
      if ("Blame" == action->text())
        BlameFile(m_pUi->pFilesTableView->currentIndex());
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
          this, &CMainWindow::DiffFile);
}


CMainWindow::~CMainWindow()
{
  delete m_pUi;
}


void CMainWindow::LogItemSelected2(const QModelIndex& index)
{
  QModelIndex i = m_logProxy->mapToSource(index);

  int r = i.row();

  const QString msg = m_pLogModel->data(m_pLogModel->index(r, quokkagit::SLogEntry::Message)).toString();

  m_pUi->pMessageTextEdit->setText(msg);

  m_pLogModel->SetColumnWidth(m_pUi->logTableView->columnWidth(quokkagit::SLogEntry::Summary));

  m_pUi->logTableView->setCurrentIndex(m_pLogModel->index(r, 0));

  m_deltas = m_git.DiffWithParent(r, m_pLogModel->Log());
}


void CMainWindow::DiffFile(const QModelIndex& index)
{
  m_git.DiffBlobs(index.row(), m_deltas);
}


void CMainWindow::BlameFile(const QModelIndex& index)
{
  int r = index.row();

  auto [delta, path] = m_deltas[r];

  CBlameDialog d(m_git.repo());
  if (d.exec(path))
  {

  }
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
  m_logFileModel->SetLog(files);

  m_pUi->pFilesTableView->resizeColumnToContents(0);
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
    if(index >= 0 && index < b.size())
    {
      quokkagit::vLogEntries entries = m_git.Log(d.currentSelection(), b);

      if (!m_pLogModel)
      {
          //m_pLogModel = new CLogModel(entries, this);
        m_pLogModel->SetLog(entries);

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

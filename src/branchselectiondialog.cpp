#include "branchselectiondialog.h"
#include "ui_branchselectiondialog.h"

CBranchSelectionDialog::CBranchSelectionDialog(const CGit2Wrapper::vBranches& branches, QWidget* parent) :
  QDialog(parent),
  m_ui(new Ui::CBranchSelectionDialog),
  m_branches(branches)
{
  m_ui->setupUi(this);

  QStringList entries;
  for(const auto& branch : branches)
  {
    entries << branch.first;
  }

  m_ui->listWidget->addItems(entries);
}

CBranchSelectionDialog::~CBranchSelectionDialog()
{
  delete m_ui;
}

int CBranchSelectionDialog::currentSelection() const
{
  return m_ui->listWidget->currentIndex().row();
}

void CBranchSelectionDialog::on_listWidget_doubleClicked(const QModelIndex& index)
{
  m_ui->listWidget->setCurrentRow(index.row());

  accept();
}

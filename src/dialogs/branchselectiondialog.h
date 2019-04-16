#ifndef BRANCHSELECTIONDIALOG_H
#define BRANCHSELECTIONDIALOG_H

#include "tools/git2wrapper.h"
#include <QDialog>

namespace Ui {
  class CBranchSelectionDialog;
}

class CBranchSelectionDialog : public QDialog
{
  Q_OBJECT

public:
  explicit CBranchSelectionDialog(const CGit2Wrapper::vBranches& branches, QWidget* parent = nullptr);
  ~CBranchSelectionDialog();

  int currentSelection() const;

public slots:
  void on_listWidget_doubleClicked(const QModelIndex&);

private:
  Ui::CBranchSelectionDialog* m_ui;
  CGit2Wrapper::vBranches m_branches;
};

#endif // BRANCHSELECTIONDIALOG_H

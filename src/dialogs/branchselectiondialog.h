#ifndef BRANCHSELECTIONDIALOG_H
#define BRANCHSELECTIONDIALOG_H

#include "tools/git2wrapper.h"
#include <QDialog>
#include <memory>

class QTreeWidgetItem;
namespace Ui {
    class CBranchSelectionDialog;
}


class CBranchSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CBranchSelectionDialog(const CGit2::Branches& branches,
                                    QWidget* parent = nullptr);
    ~CBranchSelectionDialog();

    QString currentSelection() const;

public slots:
    void OnTreeWidgetItemDoubleClicked(QTreeWidgetItem*, int);

private:
    std::unique_ptr<Ui::CBranchSelectionDialog> m_ui;
    CGit2::Branches m_branches;
};

#endif // BRANCHSELECTIONDIALOG_H

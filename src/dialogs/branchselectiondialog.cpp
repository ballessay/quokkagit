#include "branchselectiondialog.h"
#include "ui_branchselectiondialog.h"
#include <QTreeWidgetItem>
#include <QList>


namespace
{
    const int column = 0;

    QTreeWidgetItem* FindChildItem(const QString& text,
                                   const QTreeWidgetItem* item)
    {
        for (int i = 0; i < item->childCount(); ++i)
        {
            if (text == item->child(i)->text(column)) return item->child(i);
        }

        return nullptr;
    }


    QTreeWidgetItem* FindItem(const QString& text,
                              const QList<QTreeWidgetItem*> items)
    {
        for (const auto& item : items)
        {
            if (text == item->text(column)) return item;
        }

        return nullptr;
    }


    void BuildTree(QStringList& parts,
                   QList<QTreeWidgetItem*>& items,
                   const QString& branch,
                   QTreeWidgetItem* item = nullptr)
    {
        if (parts.isEmpty())
        {
            if (item != nullptr)
            {
                item->setData(column, Qt::UserRole, branch);
            }
            return;
        }

        if (nullptr == item)
        {
            item = FindItem(parts.first(), items);
            if (nullptr == item)
            {
                item = new QTreeWidgetItem();
                item->setText(column, parts.first());
                item->setExpanded(true);
                items.push_back(item);
            }

            parts.pop_front();
        }


        QTreeWidgetItem* i = FindChildItem(parts.first(), item);
        if (nullptr == i)
        {
            i = new QTreeWidgetItem(item);
            i->setText(column, parts.first());
            i->setExpanded(true);

            item->addChild(i);
        }

        parts.pop_front();
        BuildTree(parts, items, branch, i);
    }
}


CBranchSelectionDialog::CBranchSelectionDialog(const CGit2::Branches& branches,
                                               QWidget* parent) :
    QDialog(parent),
    m_ui(new Ui::CBranchSelectionDialog),
    m_branches(branches)
{
    m_ui->setupUi(this);

    QList<QTreeWidgetItem*> items;
    for(const auto& branch : branches)
    {
        QStringList refParts = branch.first.split("/");

        BuildTree(refParts, items, branch.first);
    }

    m_ui->treeWidget->addTopLevelItems(items);
    m_ui->treeWidget->expandAll();

    connect(m_ui->treeWidget, &QTreeWidget::itemDoubleClicked,
            this, &CBranchSelectionDialog::OnTreeWidgetItemDoubleClicked);
}


CBranchSelectionDialog::~CBranchSelectionDialog()
{
}


QString CBranchSelectionDialog::currentSelection() const
{
    QString branch;

    QTreeWidgetItem* item = m_ui->treeWidget->currentItem();
    QVariant v = item->data(column, Qt::UserRole);
    if (v.isValid())
    {
        const auto i = v.toString();
        branch = i;
    }

    return branch;
}


void CBranchSelectionDialog::OnTreeWidgetItemDoubleClicked(QTreeWidgetItem* item,
                                                           int)
{
    m_ui->treeWidget->setCurrentItem(item);

    accept();
}

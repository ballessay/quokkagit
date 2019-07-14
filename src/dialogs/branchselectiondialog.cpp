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
                   int branchIndex,
                   QTreeWidgetItem* item = nullptr)
    {
        if (parts.isEmpty())
        {
            if (item != nullptr)
            {
                item->setData(column, Qt::UserRole, branchIndex);
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
        BuildTree(parts, items, branchIndex, i);
    }
}



CBranchSelectionDialog::CBranchSelectionDialog(const CGit2Wrapper::vBranches& branches,
                                               QWidget* parent) :
    QDialog(parent),
    m_ui(new Ui::CBranchSelectionDialog),
    m_branches(branches)
{
    m_ui->setupUi(this);

    QList<QTreeWidgetItem*> items;
    int branchIndex = 0;
    for(const auto& branch : branches)
    {
        QStringList refParts = branch.first.split("/");

        BuildTree(refParts, items, branchIndex++);
    }

    m_ui->treeWidget->addTopLevelItems(items);
    m_ui->treeWidget->expandAll();
}


CBranchSelectionDialog::~CBranchSelectionDialog()
{
}


int CBranchSelectionDialog::currentSelection() const
{
    int index = 0;

    QTreeWidgetItem* item = m_ui->treeWidget->currentItem();
    QVariant v = item->data(column, Qt::UserRole);
    if (v.isValid())
    {
        bool ok = false;
        int i = v.toInt(&ok);
        if (ok) index = i;
    }

    return index;
}


void CBranchSelectionDialog::on_treeWidget_itemDoubleClicked(QTreeWidgetItem* item,
                                                             int)
{
    m_ui->treeWidget->setCurrentItem(item);

    accept();
}

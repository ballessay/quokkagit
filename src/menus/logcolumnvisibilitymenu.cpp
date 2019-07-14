#include "logcolumnvisibilitymenu.h"
#include "data/logentry.h"
#include <QAction>


namespace
{
    QAction* CreateAction(QMenu* parent, const QString& /*sName*/, int id, bool bChecked)
    {
        QAction* action = new QAction(quokkagit::SLogEntry::c_strings[id], parent);
        action->setCheckable(true);
        action->setChecked(bChecked);
        action->setData(id);
        parent->addAction(action);

        return action;
    }
}



CLogColumnVisibilityMenu::CLogColumnVisibilityMenu(QWidget* parent)
    : QMenu(parent)
{
    AddAction(tr(quokkagit::SLogEntry::sha), quokkagit::SLogEntry::Sha, true);
    AddAction(tr(quokkagit::SLogEntry::summary), quokkagit::SLogEntry::Summary, true);
    AddAction(tr(quokkagit::SLogEntry::message), quokkagit::SLogEntry::Message, false);
    AddAction(tr(quokkagit::SLogEntry::commiter), quokkagit::SLogEntry::Commiter, false);
    AddAction(tr(quokkagit::SLogEntry::commiterEmail), quokkagit::SLogEntry::CommiterEmail, false);
    AddAction(tr(quokkagit::SLogEntry::commitDate), quokkagit::SLogEntry::CommitDate, true);
    AddAction(tr(quokkagit::SLogEntry::author), quokkagit::SLogEntry::Author, true);
    AddAction(tr(quokkagit::SLogEntry::authorEmail), quokkagit::SLogEntry::AuthorEmail, false);
    AddAction(tr(quokkagit::SLogEntry::authorDate), quokkagit::SLogEntry::AuthorDate, true);
}


CLogColumnVisibilityMenu::~CLogColumnVisibilityMenu()
{

}


void CLogColumnVisibilityMenu::EmitState()
{
    for (const auto action : actions())
    {
        emit ToggleColumn(action->data().toInt(), action->isChecked());
    }
}


void CLogColumnVisibilityMenu::Toggled(bool enabled)
{
    QAction* action = dynamic_cast<QAction*>(sender());
    if(action != nullptr)
    {
        int i = action->data().toInt();
        emit ToggleColumn(i, enabled);
    }
}


void CLogColumnVisibilityMenu::AddAction(const QString& sName, int id, bool enabled)
{
    QAction* action = CreateAction(this, sName, id, enabled);

    connect(action, &QAction::toggled, this, &CLogColumnVisibilityMenu::Toggled);

    emit ToggleColumn(id, enabled);
}

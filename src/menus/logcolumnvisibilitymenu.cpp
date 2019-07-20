#include "logcolumnvisibilitymenu.h"
#include "data/logentry.h"
#include "data/settings.h"
#include <QAction>


CLogColumnVisibilityMenu::CLogColumnVisibilityMenu(const quokkagit::SSettings& settings,
                                                   QWidget* parent)
    : QMenu(parent),
      m_settings(settings)
{
    AddAction(quokkagit::SLogEntry::Sha);
    AddAction(quokkagit::SLogEntry::Summary);
    AddAction(quokkagit::SLogEntry::Message);
    AddAction(quokkagit::SLogEntry::Commiter);
    AddAction(quokkagit::SLogEntry::CommiterEmail);
    AddAction(quokkagit::SLogEntry::CommitDate);
    AddAction(quokkagit::SLogEntry::Author);
    AddAction(quokkagit::SLogEntry::AuthorEmail);
    AddAction(quokkagit::SLogEntry::AuthorDate);
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


void CLogColumnVisibilityMenu::AddAction(int id)
{
    auto CreateAction = [this](int id)
    {
        QAction* action = new QAction(tr(quokkagit::SLogEntry::c_strings[id]), this);
        action->setCheckable(true);
        action->setChecked(IsEnabled(id));
        action->setData(id);

        addAction(action);

        return action;
    };

    QAction* action = CreateAction(id);

    connect(action, &QAction::toggled, this, &CLogColumnVisibilityMenu::Toggled);

    emit ToggleColumn(id, IsEnabled(id));
}


bool CLogColumnVisibilityMenu::IsEnabled(int column) const
{
    if (column < 0 || column >= quokkagit::SLogEntry::NumberOfFields) return false;

    return m_settings.enabledLogColumns.contains(quokkagit::SLogEntry::c_strings[column]);
}

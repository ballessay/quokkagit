#include "logcolumnvisibilitymenu.h"
#include "data/logentry.h"
#include "data/settings.h"
#include <QAction>


CLogColumnVisibilityMenu::CLogColumnVisibilityMenu(const turtlegit::SSettings& settings,
                                                   QWidget* parent)
    : QMenu(parent),
      m_settings(settings)
{
    AddAction(turtlegit::SLogEntry::Sha);
    AddAction(turtlegit::SLogEntry::Summary);
    AddAction(turtlegit::SLogEntry::Message);
    AddAction(turtlegit::SLogEntry::Commiter);
    AddAction(turtlegit::SLogEntry::CommiterEmail);
    AddAction(turtlegit::SLogEntry::CommitDate);
    AddAction(turtlegit::SLogEntry::Author);
    AddAction(turtlegit::SLogEntry::AuthorEmail);
    AddAction(turtlegit::SLogEntry::AuthorDate);
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
        QAction* action = new QAction(tr(turtlegit::SLogEntry::c_strings[id]), this);
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
    if (column < 0 || column >= turtlegit::SLogEntry::NumberOfFields) return false;

    return m_settings.enabledLogColumns.contains(turtlegit::SLogEntry::c_strings[column]);
}

#include "logcontextmenu.h"
#include "data/logentry.h"
#include "data/settings.h"


CLogContextMenu::CLogContextMenu(QWidget* parent)
    : QMenu(parent)
{
    //  QAction* pAction = new QAction("Rebase...", this);
    //  addAction(pAction);

    //  pAction = new QAction("Reset", this);
    //  addAction(pAction);
}


CLogFilesContextMenu::CLogFilesContextMenu(QWidget* parent)
    : QMenu(parent)
{
    QAction* pAction = new QAction("Diff", this);
    addAction(pAction);

    pAction = new QAction("Blame", this);
    addAction(pAction);
}


CLogSearchContextMenu::CLogSearchContextMenu(const turtlegit::SSettings& settings,
                                             QWidget* parent)
    : QMenu(parent),
      m_settings(settings)
{
    auto AddAction = [this](turtlegit::SLogEntry::Fields field) {
        QAction* action = new QAction(turtlegit::SLogEntry::c_strings[field], this);
        action->setCheckable(true);
        action->setChecked(IsChecked(field));
        action->setData(field);
        addAction(action);

        connect(action, &QAction::toggled, this, &CLogSearchContextMenu::Toggled);

        return action;
    };

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


void CLogSearchContextMenu::Toggled(bool enabled)
{
    QAction* action = dynamic_cast<QAction*>(sender());
    if(action != nullptr)
    {
        int i = action->data().toInt();
        emit ToggleColumn(i, enabled);
    }
}


bool CLogSearchContextMenu::IsChecked(int id) const
{
    if (id < 0 || id >= turtlegit::SLogEntry::NumberOfFields) return false;

    const QString name{turtlegit::SLogEntry::c_strings[id]};
    return m_settings.enabledSearchColumns.contains(name);
}

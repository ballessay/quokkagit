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


CLogSearchContextMenu::CLogSearchContextMenu(const quokkagit::SSettings& settings,
                                             QWidget* parent)
    : QMenu(parent),
      m_settings(settings)
{
    auto AddAction = [this](quokkagit::SLogEntry::Fields field) {
        QAction* action = new QAction(quokkagit::SLogEntry::c_strings[field], this);
        action->setCheckable(true);
        action->setChecked(IsChecked(field));
        action->setData(field);
        addAction(action);

        connect(action, &QAction::toggled, this, &CLogSearchContextMenu::Toggled);

        return action;
    };

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
    if (id < 0 || id >= quokkagit::SLogEntry::NumberOfFields) return false;

    const QString name{quokkagit::SLogEntry::c_strings[id]};
    return m_settings.enabledSearchColumns.contains(name);
}

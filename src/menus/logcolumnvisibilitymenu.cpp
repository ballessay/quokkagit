#include "logcolumnvisibilitymenu.h"
#include "logentry.h"
#include <QAction>

namespace
{
  QAction* CreateAction(QMenu* parent, const QString& sName, int id, bool bChecked)
  {
    QAction* action = new QAction(sName, parent);
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
//  QAction* action = new QAction(tr("Sha"), this);
//  action->setCheckable(true);
//  addAction(action);
  AddAction(tr(qtgit::SLogEntry::sha), qtgit::SLogEntry::Sha, true);
  AddAction(tr(qtgit::SLogEntry::summary), qtgit::SLogEntry::Summary, true);
  AddAction(tr(qtgit::SLogEntry::message), qtgit::SLogEntry::Message, false);
  AddAction(tr(qtgit::SLogEntry::commiter), qtgit::SLogEntry::Commiter, false);
  AddAction(tr(qtgit::SLogEntry::commiterEmail), qtgit::SLogEntry::CommiterEmail, false);
  AddAction(tr(qtgit::SLogEntry::commitDate), qtgit::SLogEntry::CommitDate, true);
  AddAction(tr(qtgit::SLogEntry::author), qtgit::SLogEntry::Author, true);
  AddAction(tr(qtgit::SLogEntry::authorEmail), qtgit::SLogEntry::AuthorEmail, false);
  AddAction(tr(qtgit::SLogEntry::authorDate), qtgit::SLogEntry::AuthorDate, true);
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

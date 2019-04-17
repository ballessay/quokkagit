#include "logcontextmenu.h"
#include "data/logentry.h"

CLogContextMenu::CLogContextMenu(QWidget* parent)
  : QMenu(parent)
{
  QAction* pAction = new QAction("Rebase...", this);
  addAction(pAction);

  pAction = new QAction("Reset", this);
  addAction(pAction);
}


CLogFilesContextMenu::CLogFilesContextMenu(QWidget* parent)
  : QMenu(parent)
{
  QAction* pAction = new QAction("Diff", this);
  addAction(pAction);

  pAction = new QAction("Blame", this);
  addAction(pAction);
}

CLogSearchContextMenu::CLogSearchContextMenu(QWidget* parent)
  : QMenu(parent)
{
  auto AddAction = [this](quokkagit::SLogEntry::Fields field, bool checked) {
    QAction* pAction = new QAction(quokkagit::SLogEntry::c_strings[field], this);
    pAction->setCheckable(true);
    pAction->setChecked(checked);
    pAction->setData(field);
    addAction(pAction);
    return pAction;
  };

  AddAction(quokkagit::SLogEntry::Sha, true);
  AddAction(quokkagit::SLogEntry::Summary, true);
  AddAction(quokkagit::SLogEntry::Author, true);
  AddAction(quokkagit::SLogEntry::Message, false);
  AddAction(quokkagit::SLogEntry::CommitDate, true);
}

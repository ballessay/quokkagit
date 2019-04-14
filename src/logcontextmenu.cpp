#include "logcontextmenu.h"
#include "logentry.h"

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
  auto AddAction = [this](const QString& text, qtgit::SLogEntry::Fields field, bool checked) {
    QAction* pAction = new QAction(text, this);
    pAction->setCheckable(true);
    pAction->setChecked(checked);
    pAction->setData(field);
    addAction(pAction);
    return pAction;
  };

  AddAction(qtgit::SLogEntry::sha, qtgit::SLogEntry::Sha, true);
  AddAction(qtgit::SLogEntry::summary, qtgit::SLogEntry::Summary, true);
  AddAction(qtgit::SLogEntry::author, qtgit::SLogEntry::Author, true);
  AddAction(qtgit::SLogEntry::message, qtgit::SLogEntry::Message, false);
  AddAction(qtgit::SLogEntry::commitDate, qtgit::SLogEntry::CommitDate, true);
}

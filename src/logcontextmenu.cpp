#include "logcontextmenu.h"

CLogContextMenu::CLogContextMenu(QWidget* parent)
  : QMenu(parent)
{
  QAction* pAction = new QAction("Rebase...", this);
  addAction(pAction);

  pAction = new QAction("Reset", this);
  addAction(pAction);
}

#ifndef LOGCONTEXTMENU_H
#define LOGCONTEXTMENU_H

#include <QMenu>


class CLogContextMenu : public QMenu
{
public:
  CLogContextMenu(QWidget* parent);
};


class CLogFilesContextMenu : public QMenu
{
public:
  CLogFilesContextMenu(QWidget* parent);
};


class CLogSearchContextMenu : public QMenu
{
public:
  CLogSearchContextMenu(QWidget* parent);
};

#endif // LOGCONTEXTMENU_H

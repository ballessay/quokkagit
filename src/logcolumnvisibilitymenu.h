#ifndef LOGCOLUMNVISIBILITYMENU_H
#define LOGCOLUMNVISIBILITYMENU_H

#include <QMenu>

class CLogColumnVisibilityMenu : public QMenu
{
  Q_OBJECT

public:
  CLogColumnVisibilityMenu(QWidget* parent);
  ~CLogColumnVisibilityMenu();

  void EmitState();

public slots:
  void Toggled(bool enabled);

signals:
  void ToggleColumn(int column, bool bVisible);

private:
  void AddAction(const QString& sName, int id, bool bEnabled);
};

#endif // LOGCOLUMNVISIBILITYMENU_H

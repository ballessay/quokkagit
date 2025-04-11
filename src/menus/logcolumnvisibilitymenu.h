#ifndef LOGCOLUMNVISIBILITYMENU_H
#define LOGCOLUMNVISIBILITYMENU_H

#include <QMenu>

namespace turtlegit { struct SSettings; }


class CLogColumnVisibilityMenu : public QMenu
{
    Q_OBJECT

public:
    CLogColumnVisibilityMenu(const turtlegit::SSettings& settings,
                             QWidget* parent);
    ~CLogColumnVisibilityMenu();

    void EmitState();

public slots:
    void Toggled(bool enabled);

signals:
    void ToggleColumn(int column, bool bVisible);

private:
    void AddAction(int id);
    bool IsEnabled(int column) const;

    const turtlegit::SSettings& m_settings;
};

#endif // LOGCOLUMNVISIBILITYMENU_H

#ifndef LOGCOLUMNVISIBILITYMENU_H
#define LOGCOLUMNVISIBILITYMENU_H

#include <QMenu>

namespace quokkagit { struct SSettings; }

class CLogColumnVisibilityMenu : public QMenu
{
    Q_OBJECT

public:
    CLogColumnVisibilityMenu(const quokkagit::SSettings& settings,
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

    const quokkagit::SSettings& m_settings;
};

#endif // LOGCOLUMNVISIBILITYMENU_H

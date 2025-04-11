#ifndef LOGCONTEXTMENU_H
#define LOGCONTEXTMENU_H

#include <QMenu>

namespace turtlegit { struct SSettings; }


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
    Q_OBJECT

public:
    CLogSearchContextMenu(const turtlegit::SSettings& settings, QWidget* parent);

public slots:
    void Toggled(bool enabled);

signals:
    void ToggleColumn(int column, bool enabled);

private:
    bool IsChecked(int id) const;

    const turtlegit::SSettings& m_settings;
};

#endif // LOGCONTEXTMENU_H

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <memory>

namespace turtlegit { struct SSettings; }
namespace Ui {
    class CSettingsDialog;
}


class CSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CSettingsDialog(const turtlegit::SSettings& settings,
                             QWidget *parent = nullptr);
    ~CSettingsDialog();

    turtlegit::SSettings currentSettings() const;

protected slots:
    void OnFontToolButtonTriggered();

private:
    std::unique_ptr<Ui::CSettingsDialog> m_ui;
    const turtlegit::SSettings& m_settings;
};

#endif // SETTINGSDIALOG_H

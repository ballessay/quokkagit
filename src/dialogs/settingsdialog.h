#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <memory>

namespace quokkagit { struct SSettings; }
namespace Ui {
    class CSettingsDialog;
}

class CSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CSettingsDialog(const quokkagit::SSettings& settings,
                             QWidget *parent = nullptr);
    ~CSettingsDialog();

    quokkagit::SSettings currentSettings() const;

protected slots:
    void OnFontToolButtonTriggered();

private:
    std::unique_ptr<Ui::CSettingsDialog> m_ui;
};

#endif // SETTINGSDIALOG_H

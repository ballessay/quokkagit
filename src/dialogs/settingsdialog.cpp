#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "data/settings.h"
#include <QFontDialog>


CSettingsDialog::CSettingsDialog(const quokkagit::SSettings& settings,
                                 QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::CSettingsDialog)
{
    m_ui->setupUi(this);

    m_ui->fontComboBox->setFont(settings.font);

    m_ui->diffPathLineEdit->setText(settings.diff.path);
    m_ui->diffArgsPlainTextEdit->setPlainText(settings.diff.arguments.join(" "));

    connect(m_ui->fontToolButton, &QToolButton::clicked,
            this, &CSettingsDialog::OnFontToolButtonTriggered);
}


CSettingsDialog::~CSettingsDialog()
{
}


quokkagit::SSettings CSettingsDialog::currentSettings() const
{
    quokkagit::SSettings settings;

    settings.font = m_ui->fontComboBox->font();

    settings.diff.path = m_ui->diffPathLineEdit->text();
    settings.diff.arguments =
            m_ui->diffArgsPlainTextEdit->toPlainText().split(" ",
                                                             QString::SkipEmptyParts);

    return settings;
}


void CSettingsDialog::OnFontToolButtonTriggered()
{
    QFontDialog d(m_ui->fontComboBox->font(), this);
    if (d.exec() == QDialog::Accepted)
    {
        m_ui->fontComboBox->setFont(d.selectedFont());
    }
}

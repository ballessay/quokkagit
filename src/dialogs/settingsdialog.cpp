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

    m_ui->styleComboBox->addItems(quokkagit::SStyleSettings::StyleNames());
    m_ui->styleComboBox->setCurrentIndex(settings.style.id);

    m_ui->fontComboBox->setFont(settings.font);
    m_ui->hashLengthSpinBox->setValue(settings.hashDisplayLength);
    m_ui->maxHistroySpinBox->setValue(settings.maxLastRepos);

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

    settings.style.id = m_ui->styleComboBox->currentIndex();

    settings.font = m_ui->fontComboBox->currentFont();
    const QString sName = settings.font.family();
    settings.hashDisplayLength = m_ui->hashLengthSpinBox->value();
    settings.maxLastRepos = m_ui->maxHistroySpinBox->value();

    settings.diff.path = m_ui->diffPathLineEdit->text();
    settings.diff.arguments =
            m_ui->diffArgsPlainTextEdit->toPlainText().split(" ",
                                                             QString::SkipEmptyParts);

    return settings;
}


void CSettingsDialog::OnFontToolButtonTriggered()
{
    QFontDialog d(m_ui->fontComboBox->font(), this);
    d.setOption(QFontDialog::ScalableFonts, false);
    d.setOption(QFontDialog::NonScalableFonts, false);
    d.setOption(QFontDialog::MonospacedFonts, true);
    d.setOption(QFontDialog::ProportionalFonts, false);

    if (d.exec() == QDialog::Accepted)
    {
        m_ui->fontComboBox->setFont(d.selectedFont());
    }
}

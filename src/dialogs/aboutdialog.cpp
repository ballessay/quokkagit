#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "version.h"
#include <QMessageBox>


CAboutDialog::CAboutDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(std::make_unique<Ui::CAboutDialog>())
{
    m_ui->setupUi(this);

    m_ui->pIconLabel->setPixmap(QPixmap(":/quokkagit-transparent.png"));

    m_ui->pApplicationNameLabel->setText(QString("<b>") +
                                         quokkagit::project_name +
                                         "</b>");

    const QString sInfo = QString("Version: ") + quokkagit::project_version + "\n"
                          "Branch: " + quokkagit::git_branch + "\n"
                          "Hash: " + quokkagit::git_hash;
    m_ui->pInfoLabel->setText(sInfo);

    connect(m_ui->pOkPushButton, &QPushButton::pressed,
            this, &CAboutDialog::OnOkPressed);
    connect(m_ui->pAboutQtPushButton, &QPushButton::pressed,
            this, &CAboutDialog::OnAboutQtPressed);
}


CAboutDialog::~CAboutDialog()
{
}


void CAboutDialog::OnOkPressed()
{
    accept();
}


void CAboutDialog::OnAboutQtPressed()
{
    QMessageBox::aboutQt(this);
}

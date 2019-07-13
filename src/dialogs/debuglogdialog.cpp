#include "debuglogdialog.h"
#include "ui_debuglogdialog.h"

CDebugLogDialog::CDebugLogDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::CDebugLogDialog)
{
    m_ui->setupUi(this);
}

CDebugLogDialog::~CDebugLogDialog()
{
}

void CDebugLogDialog::AddMessage(QString msg)
{
    m_ui->logTextEdit->append(msg);
}

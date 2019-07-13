#include "blamedialog.h"
#include "ui_blamedialog.h"
#include "models/blamemodel.h"

CBlameDialog::CBlameDialog(const quokkagit::tvBlameData& vData,
                           QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::CBlameDialog)
{
    m_ui->setupUi(this);

    CBlameModel* model = new CBlameModel(vData, this);
    model->SetColors(palette().windowText().color(),
                     palette().window().color());
    m_ui->tableView->setModel(model);

    m_ui->tableView->resizeColumnsToContents();
    m_ui->tableView->resizeRowsToContents();
    m_ui->tableView->verticalHeader()->resizeSections(QHeaderView::Fixed);
    m_ui->tableView->verticalHeader()->setDefaultSectionSize(14);
}

CBlameDialog::~CBlameDialog()
{
}

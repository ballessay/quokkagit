#include "blamedialog.h"
#include "ui_blamedialog.h"
#include "models/blamemodel.h"

CBlameDialog::CBlameDialog(const quokkagit::tvBlameData& vData,
                           QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CBlameDialog)
{
    ui->setupUi(this);

    CBlameModel* model = new CBlameModel(vData, this);
    ui->tableView->setModel(model);

    ui->tableView->resizeColumnsToContents();
}

CBlameDialog::~CBlameDialog()
{
}

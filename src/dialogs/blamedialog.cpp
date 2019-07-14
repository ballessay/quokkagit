#include "blamedialog.h"
#include "ui_blamedialog.h"
#include "models/blamemodel.h"
#include "tools/git2wrapper.h"
#include <QAction>


namespace
{
    void ResizeTableView(QTableView* table)
    {
        table->resizeColumnsToContents();
        table->resizeRowsToContents();
        table->verticalHeader()->resizeSections(QHeaderView::Fixed);
        table->verticalHeader()->setDefaultSectionSize(14);
    }
}



CBlameDialog::CBlameDialog(SData& data, QWidget* parent) :
    QDialog(parent),
    m_ui(new Ui::CBlameDialog),
    m_model(new CBlameModel(this)),
    m_data(data)
{
    m_ui->setupUi(this);

    m_model->SetColors(palette().windowText().color(),
                     palette().window().color());
    m_ui->tableView->setModel(m_model.get());

    ResizeTableView(m_ui->tableView);

    QAction* action = new QAction("Blame here...", m_ui->tableView);
    connect(action, &QAction::triggered,
            this, &CBlameDialog::OnBlameHereTriggered);
    m_ui->tableView->addAction(action);
    m_ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
}


CBlameDialog::~CBlameDialog()
{
}


int CBlameDialog::exec()
{
    return exec(m_data.path, m_data.sha);
}


int CBlameDialog::exec(const QString& path, const QString& hash)
{
    quokkagit::tvBlameData data = m_data.git.BlameFile(path, hash);

    m_model->SetData(data);

    ResizeTableView(m_ui->tableView);

    return QDialog::exec();
}


void CBlameDialog::OnBlameHereTriggered()
{
    QModelIndex index = m_ui->tableView->currentIndex();

    QString hash = m_model->data(m_model->index(index.row(), CBlameModel::Sha)).toString();

    CBlameDialog d(m_data, this);
    d.exec(m_data.path, hash);
}

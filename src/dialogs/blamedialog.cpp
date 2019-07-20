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
    m_model(new CBlameModel(data.git, data.settings, this)),
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

    connect(m_ui->tableView, &QTableView::pressed,
            this, &CBlameDialog::OnTableViewPressed);
}


CBlameDialog::~CBlameDialog()
{
}


int CBlameDialog::exec()
{
    return exec(m_data.path, m_data.hash);
}


int CBlameDialog::exec(const QString& path, const QString& hash)
{
    quokkagit::BlameData data = m_data.git.BlameFile(path, hash);

    m_model->SetData(data);

    ResizeTableView(m_ui->tableView);

    return QDialog::exec();
}


void CBlameDialog::OnBlameHereTriggered()
{
    QModelIndex index = m_ui->tableView->currentIndex();

    const QModelIndex in{m_model->index(index.row(), CBlameModel::Sha)};
    const QString hash{m_model->data(in, Qt::UserRole).toString()};

    CBlameDialog d(m_data, this);
    d.exec(m_data.path, hash);
}


void CBlameDialog::OnTableViewPressed(const QModelIndex& index)
{
    const QModelIndex in{m_model->index(index.row(), CBlameModel::Sha)};
    const QString hash{m_model->data(in, Qt::UserRole).toString()};

    m_model->SetSelectedHash(hash);
}

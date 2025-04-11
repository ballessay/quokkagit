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
    m_blameHereAction(nullptr),
    m_data(data),
    m_lastClickedRow(-1)
{
    m_ui->setupUi(this);

    setWindowTitle(tr("Blame: %1").arg(data.path));

    m_model->SetColors(palette().windowText().color(),
                     palette().window().color());
    m_ui->tableView->setModel(m_model.get());

    ResizeTableView(m_ui->tableView);

    m_blameHereAction = new QAction("Blame here...", m_ui->tableView);
    connect(m_blameHereAction, &QAction::triggered,
            this, &CBlameDialog::OnBlameHereTriggered);
    m_ui->tableView->addAction(m_blameHereAction);
    m_ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
    m_ui->tableView->horizontalHeader()->hideSection(CBlameModel::OrigPath);

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
    m_data.path = path;
    m_data.hash = hash;

    setWindowTitle(tr("Blame: %1").arg(m_data.path));

    const turtlegit::BlameData data{m_data.git.BlameFile(path, hash)};

    m_model->SetData(data);

    ResizeTableView(m_ui->tableView);

    return QDialog::exec();
}


void CBlameDialog::OnBlameHereTriggered()
{
    const int row{m_lastClickedRow};

    const QModelIndex hashIndex{m_model->index(row, CBlameModel::Sha)};
    const QString hash{m_model->data(hashIndex, Qt::UserRole).toString()};

    if (hash == m_data.hash) return;

    const QModelIndex pathIndex{m_model->index(row, CBlameModel::OrigPath)};
    const QString path{m_model->data(pathIndex).toString()};

    CBlameDialog d(m_data, this);
    d.exec(path, hash);
}


void CBlameDialog::OnTableViewPressed(const QModelIndex& index)
{
    const QModelIndex in{m_model->index(index.row(), CBlameModel::Sha)};
    const QString hash{m_model->data(in, Qt::UserRole).toString()};

    m_model->SetSelectedHash(hash);

    m_lastClickedRow = in.row();

    m_blameHereAction->setEnabled(hash != m_data.hash);
}

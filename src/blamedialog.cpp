#include "blamedialog.h"
#include "ui_blamedialog.h"

CBlameDialog::CBlameDialog(git_repository* repo, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::CBlameDialog),
  m_repo(repo),
  m_blame(nullptr)
{
  ui->setupUi(this);
}

CBlameDialog::~CBlameDialog()
{
  delete ui;
}

int CBlameDialog::exec(const QString& path)
{
  git_blame_options opts(GIT_BLAME_OPTIONS_INIT);
  if(!git_blame_file(&m_blame, m_repo, path.toLocal8Bit().constData(), &opts))
  {
    uint32_t count  = git_blame_get_hunk_count(m_blame);
    for (uint32_t i = 0; i < count; ++i)
    {
      const git_blame_hunk* hunk = git_blame_get_hunk_byindex(m_blame, i);

    }
  }

  return QDialog::exec();
}

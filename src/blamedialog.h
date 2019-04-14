#ifndef BLAMEDIALOG_H
#define BLAMEDIALOG_H

#include <QDialog>
#include <git2cpp/repo.h>
#include <git2.h>

namespace Ui {
  class CBlameDialog;
}

class CBlameDialog : public QDialog
{
  Q_OBJECT

public:
  explicit CBlameDialog(git_repository* repo, QWidget *parent = nullptr);
  ~CBlameDialog() override;

  int exec(const QString& sPath);

private:
  Ui::CBlameDialog *ui;
  git_repository* m_repo;
  git_blame* m_blame;
};

#endif // BLAMEDIALOG_H

#ifndef BLAMEDIALOG_H
#define BLAMEDIALOG_H

#include "data/blamedata.h"
#include <QDialog>
#include <memory>

namespace Ui {
    class CBlameDialog;
}

class CBlameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CBlameDialog(const quokkagit::tvBlameData& vData,
                          QWidget *parent = nullptr);
    ~CBlameDialog() override;

private:
    std::unique_ptr<Ui::CBlameDialog> ui;
};

#endif // BLAMEDIALOG_H

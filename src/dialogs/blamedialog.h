#ifndef BLAMEDIALOG_H
#define BLAMEDIALOG_H

#include "data/blamedata.h"
#include <QDialog>
#include <memory>

class CBlameModel;
class CGit2Wrapper;
namespace Ui {
    class CBlameDialog;
}

class CBlameDialog : public QDialog
{
    Q_OBJECT

public:
    struct SData
    {
        CGit2Wrapper& git;
        QString hash;
        QString path;
    };

    explicit CBlameDialog(SData& data,
                          QWidget* parent = nullptr);
    ~CBlameDialog() override;

    int exec() override;
    int exec(const QString& path, const QString& hash);

private slots:
    void OnBlameHereTriggered();

private:
    std::unique_ptr<Ui::CBlameDialog> m_ui;
    std::unique_ptr<CBlameModel> m_model;
    SData m_data;
};

#endif // BLAMEDIALOG_H

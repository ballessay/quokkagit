#ifndef BLAMEDIALOG_H
#define BLAMEDIALOG_H

#include "data/blamedata.h"
#include <QDialog>
#include <memory>

class CBlameModel;
class CGit2;
namespace quokkagit {
    struct SSettings;
}
namespace Ui {
    class CBlameDialog;
}

class CBlameDialog : public QDialog
{
    Q_OBJECT

public:
    struct SData
    {
        CGit2& git;
        QString hash;
        QString path;
        quokkagit::SSettings& settings;
    };

    explicit CBlameDialog(SData& data,
                          QWidget* parent = nullptr);
    ~CBlameDialog() override;

    int exec() override;
    int exec(const QString& path, const QString& hash);

private slots:
    void OnBlameHereTriggered();
    void OnTableViewPressed(const QModelIndex&);

private:
    std::unique_ptr<Ui::CBlameDialog> m_ui;
    std::unique_ptr<CBlameModel> m_model;
    QAction* m_blameHereAction;
    SData m_data;
    int m_lastClickedRow;
};

#endif // BLAMEDIALOG_H

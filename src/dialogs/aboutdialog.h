#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <memory>

namespace Ui {
    class CAboutDialog;
}


class CAboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CAboutDialog(QWidget *parent = nullptr);
    ~CAboutDialog();

protected slots:
    void OnOkPressed();
    void OnAboutQtPressed();

private:
    std::unique_ptr<Ui::CAboutDialog> m_ui;
};

#endif // ABOUTDIALOG_H

#ifndef DEBUGLOGDIALOG_H
#define DEBUGLOGDIALOG_H

#include <QDialog>
#include <memory>

namespace Ui {
    class CDebugLogDialog;
}

class CDebugLogDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CDebugLogDialog(QWidget* parent = nullptr);
    ~CDebugLogDialog();

public slots:
    void AddMessage(QString msg);

private:
    std::unique_ptr<Ui::CDebugLogDialog> m_ui;
};

#endif // DEBUGLOGDIALOG_H

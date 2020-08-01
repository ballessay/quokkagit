#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QTableView>

class QKeyEvent;


class CTableWidget : public QTableView
{
    Q_OBJECT

public:
    CTableWidget(QWidget* parent);

signals:
    void enterOrReturnPressed();

protected:
    void keyPressEvent(QKeyEvent* event) override;
};

#endif // TABLEWIDGET_H

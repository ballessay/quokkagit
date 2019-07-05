#include "tablewidget.h"
#include <QKeyEvent>


CTableWidget::CTableWidget(QWidget* parent)
  : QTableView(parent)
{
}


void CTableWidget::keyPressEvent(QKeyEvent* event)
{
  if (event->key() == Qt::Key_Enter ||
      event->key() == Qt::Key_Return)
  {
    emit enterOrReturnPressed();
  }

  QTableView::keyPressEvent(event);
}

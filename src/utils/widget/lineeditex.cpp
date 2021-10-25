#include "lineeditex.h"
#include <QEvent>
LineEditEx::LineEditEx(QWidget *parent):
    QLineEdit (parent)
{

}

LineEditEx::~LineEditEx()
{

}

void LineEditEx::mouseDoubleClickEvent(QMouseEvent *mouseEvent)
{
    if(mouseEvent->button() == Qt::LeftButton){
        mouseEvent->accept();
        emit lineEditDblClicked(this);
    }
}

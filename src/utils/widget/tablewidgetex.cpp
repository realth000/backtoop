#include "tablewidgetex.h"
#include <QKeyEvent>
#include <QDebug>
#include <QScrollBar>

TableWidgetEx::TableWidgetEx(QWidget *parent):
    QTableWidget(parent)
{
    connect(this, &TableWidgetEx::currentCellChanged, this ,[this](int currentRowNumber, int currentColumn, int previousRow, int previousColumn){
            qDebug() << "this->currentRowNumber =" << currentRowNumber;
        Q_UNUSED(currentColumn)
        Q_UNUSED(previousRow)
        Q_UNUSED(previousColumn)
        this->currentRowNumber=currentRowNumber;}
    , Qt::UniqueConnection);

    connect(this, &TableWidgetEx::rowInserted, this, [this](){this->rowCountNumber++;});
    connect(this, &TableWidgetEx::rowRemoved, this, [this](){this->rowCountNumber--;});
//    connect(this, &QTableWidget::cellEntered, this, [this](int row, int column){qDebug() << "cellEntered" << row << column << this->sizeHint();}, Qt::UniqueConnection);
//    connect(this, &QTableWidget::cellActivated, this, [this](int row, int column){qDebug() << "cellActivated" << row << column << this->sizeHint();}, Qt::UniqueConnection);
//    connect(this, &QTableWidget::cellClicked, this, [this](int row, int column){qDebug() << "cellClicked" << row << column << this->sizeHint();}, Qt::UniqueConnection);
}

//TableWidgetItemEx *TableWidgetEx::item(int row, int column) const
//{
//    return dynamic_cast<TableWidgetItemEx *>(this->QTableWidget::item(row, column));
//}

//TableWidgetItemEx *TableWidgetEx::takeItem(int row, int column)
//{
//    return dynamic_cast<TableWidgetItemEx *>(this->QTableWidget::takeItem(row, column));
//}

void TableWidgetEx::switchToRow(int row)
{
    this->selectRow(row);
    this->verticalScrollBar()->setValue(row);
}

void TableWidgetEx::insertRow(int row)
{
    // 调用父类方法
    this->QTableWidget::insertRow(row);
    emit rowInserted(row);
}

void TableWidgetEx::removeRow(int row)
{
    this->QTableWidget::removeRow(row);
    emit rowRemoved(row);
}

void TableWidgetEx::keyPressEvent(QKeyEvent *e)
{
    // 上箭头控制往上选一行
    if(e->modifiers() == Qt::NoModifier && e->key() == Qt::Key_Up){
        if(currentRowNumber >0){
            switchToRow(currentRowNumber-1);
            e->accept();
        }
        else{
            // 选到第一行了或者未选行就跳转到最后一行
            switchToRow(rowCountNumber-1);
            e->accept();
        }
        return;
    }
    // 下箭头控制往下选一行
    if(e->modifiers() == Qt::NoModifier && e->key() == Qt::Key_Down){
        if(this->selectedItems().length()==0 || currentRowNumber == rowCountNumber-1){
            switchToRow(0);
            e->accept();
        }
        else if(currentRowNumber == 0 || currentRowNumber < static_cast<int>(rowCountNumber-1)){
            switchToRow(currentRowNumber+1);
            e->accept();
        }

        else{
            e->ignore();
        }
        return;
    }
    // 其他，忽略
    e->ignore();
}

TableWidgetItemEx::TableWidgetItemEx(const QString &text, int type):
    QTableWidgetItem(text, type)
{

}

QString TableWidgetItemEx::text() const
{
    return this->QTableWidgetItem::text();
}

void TableWidgetItemEx::setText(const QString text)
{
    this->QTableWidgetItem::setText(text);
}

#ifndef TABLEWIDGETEX_H
#define TABLEWIDGETEX_H

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QAbstractButton>
#include <QObject>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QPushButton>

class TableWidgetItemEx :  public QPushButton, public QTableWidgetItem
{
    Q_OBJECT
public:
    explicit TableWidgetItemEx(const QString &text, int type = Type);
    QString text() const;
    void setText(const QString text);

protected:
//    virtual void mouseReleaseEvent(QMouseEvent *e) override;
//    virtual void contextMenuEvent(QContextMenuEvent *e) override;
//    virtual void mouseDoubleClickEvent(QMouseEvent *mouseEvent) override;


private:
//    QTableWidgetItem *w = nullptr;
};

class TableWidgetEx : public QTableWidget
{
    Q_OBJECT
public:
    TableWidgetEx(QWidget *parent);
//    TableWidgetItemEx *item(int row, int column) const;
//    TableWidgetItemEx *takeItem(int row, int column);
    int currentRowNumber = -1;
    int rowCountNumber = 0;

signals:
    void rowInserted(int row);
    void rowRemoved(int row);

public slots:
    void switchToRow(int row);
    void insertRow(int row);
    void removeRow(int row);

protected:
    virtual void keyPressEvent(QKeyEvent *e) override;

};

#endif // TABLEWIDGETEX_H

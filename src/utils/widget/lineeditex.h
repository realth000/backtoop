#ifndef QLINEEDITPRO_H
#define QLINEEDITPRO_H

#include <QLineEdit>
#include <QMouseEvent>

class LineEditEx : public QLineEdit
{
    Q_OBJECT
public:
    LineEditEx(QWidget *parent = nullptr);
    ~LineEditEx();

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *mouseEvent) override;

signals:
    void lineEditDblClicked(LineEditEx *);

};

#endif // QLINEEDITPRO_H

#ifndef MAINUI_H
#define MAINUI_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class mainui; }
QT_END_NAMESPACE

class mainui : public QWidget
{
    Q_OBJECT

public:
    mainui(QWidget *parent = nullptr);
    ~mainui();

private:
    Ui::mainui *ui;
    void initUi();
};
#endif // MAINUI_H

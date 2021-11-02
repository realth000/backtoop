#ifndef INPUTBAKPATHWINDOW_H
#define INPUTBAKPATHWINDOW_H

#include <QDialog>
#include "proxystyle.h"

namespace Ui {
class InputBakPathWindow;
}

class InputBakPathWindow : public QDialog
{
    Q_OBJECT

public:
    explicit InputBakPathWindow(QWidget *parent = nullptr);
    ~InputBakPathWindow();

signals:
    void sendNewPath(QString name, QString srcPath, QString dstPath);

private slots:
    void on_selectSrcPathButton_clicked();
    void on_selectDstPathButton_clicked();
    void on_okButton_clicked();

private:
    Ui::InputBakPathWindow *ui;
    PushButtonStyle *pushButtonStyle = nullptr;

    void initUi();
};

#endif // INPUTBAKPATHWINDOW_H

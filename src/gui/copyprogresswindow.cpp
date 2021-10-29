#include "copyprogresswindow.h"
#include "ui_copyprogresswindow.h"
#include "qssinstaller.h"
#include "commoninclude.h"
#include "iconinstaller.h"

CopyProgressWindow::CopyProgressWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CopyProgressWindow)
{
    ui->setupUi(this);
    initUi();
}

CopyProgressWindow::~CopyProgressWindow()
{
    delete ui;
    delete pushButtonStyle;
}

void CopyProgressWindow::setFileCountTotal(quint64 count)
{
    fileCountTotal = count;
    updateFileCount(0);
}

void CopyProgressWindow::initUi()
{
    pushButtonStyle = new PushButtonStyle();
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet(QssInstaller::QssInstallFromFile(":/stylesheet/stylesheet_copyprogresswindow.css").arg(this->objectName(), "rgb(55,85,100)",
                                 "qlineargradient(x1:0, y1:0, x2:0, y2:1, stop: 0 rgb(45,45,45), stop: 1 rgb(51,51,51));"
                                 "alternate-background-color:rgb(55,55,55)"));

    // Title bar style
    ui->titleBar->setFixedWidth(this->width());
    ui->titleBar->setCloseIcon(TITLEBAR_CLOSEICON);
    ui->titleBar->setTitleText(TITLEBAR_TITLETEXT);
    ui->titleBar->setUseGradient(true);
    ui->titleBar->initUi(TitleBar::NoMaxButton, "rgb(240,255,255)", "rgb(93,94,95)",
                         "qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(18,18,18), stop: 1 rgb(21,21,21))", "rgb(240,255,255)");
    ui->titleBar->setTitleIcon(TITLEBAR_TITLEICON);

    ui->copyLogTextEdit->setReadOnly(true);
    ui->copyStopPushButton->setStyle(pushButtonStyle);
    IconInstaller::installPushButtonIcon(ui->copyStopPushButton, ":/pic/no.png");
}

void CopyProgressWindow::updateFileCount(quint64 alreadyCopiedCount)
{
    ui->copyCountLabel->setText(QString("%1/%2").arg(alreadyCopiedCount).arg(fileCountTotal));
}

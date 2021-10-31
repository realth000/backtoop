﻿#include "copyprogresswindow.h"
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

void CopyProgressWindow::parseCopyResult(QString filePath, CopyResult result)
{
    switch (result) {
    case CopyResult::Success:
        updateSuccessCount(filePath);
        break;
    case CopyResult::Failed:
        updateFailedCount(filePath);
        break;
    case CopyResult::AlreadyExists:
        updateFailedCount(filePath);
        log("文件已存在");
        break;
    case CopyResult::HashCheckFailed:
        updateFailedCount(filePath);
        log("校验失败，复制出错");
        break;
    default:
        break;
    }
}

void CopyProgressWindow::initUi()
{
    pushButtonStyle = new PushButtonStyle();
    this->setWindowModality(Qt::ApplicationModal);
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

void CopyProgressWindow::log(QString msg)
{
    ui->copyLogTextEdit->append(msg);
}

void CopyProgressWindow::updateFileCount(quint64 alreadyCopiedCount)
{
    ui->copyCountLabel->setText(QString("%1/%2").arg(alreadyCopiedCount).arg(fileCountTotal));
    ui->copyProgressBar->setValue(100*alreadyCopiedCount/fileCountTotal);
}

void CopyProgressWindow::updateSuccessCount(QString filePath)
{
    fileCountCopied++;
    updateFileCount(fileCountCopied);
    ui->copyLogTextEdit->append(QString("<font color=\"%2\">Success: %1</font>").arg(filePath, LOGTEXTEDIT_COPY_SUCCESS_COLOR));
}

void CopyProgressWindow::updateFailedCount(QString filePath)
{
    fileCountCopied++;
    updateFileCount(fileCountCopied);
    ui->copyLogTextEdit->append(QString("<font color=\"%2\">Failed: %1</font>").arg(filePath, LOGTEXTEDIT_COPY_FAILED_COLOR));
}

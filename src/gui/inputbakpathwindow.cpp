#include "inputbakpathwindow.h"
#include "ui_inputbakpathwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include "qssinstaller.h"
#include "commoninclude.h"

InputBakPathWindow::InputBakPathWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputBakPathWindow)
{
    ui->setupUi(this);
    initUi();
}

InputBakPathWindow::~InputBakPathWindow()
{
    delete ui;
}

void InputBakPathWindow::initUi()
{
    pushButtonStyle = new PushButtonStyle;
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(this->width(), this->height());
    this->setStyleSheet(QssInstaller::QssInstallFromFile(":/stylesheet/stylesheet_inputbakpathwindow.css").arg(this->objectName(), "rgb(55,85,100)",
                                                                                            "qlineargradient(x1:0, y1:0, x2:0, y2:1, stop: 0 rgb(45,45,45), stop: 1 rgb(51,51,51));"
                                                                                            "alternate-background-color:rgb(55,55,55)"));
    this->setWindowModality(Qt::ApplicationModal);
    // Title bar style
    ui->titleBar->setFixedWidth(this->width());
    ui->titleBar->setCloseIcon(TITLEBAR_CLOSEICON);
    ui->titleBar->setTitleText("添加");
    ui->titleBar->setUseGradient(true);
    ui->titleBar->initUi(TitleBar::NoMaxButton, "rgb(240,255,255)", "rgb(93,94,95)",
                         "qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(18,18,18), stop: 1 rgb(21,21,21))", "rgb(240,255,255)");
    ui->titleBar->setTitleIcon(TITLEBAR_TITLEICON);

    ui->hintLabel->setVisible(false);
}

void InputBakPathWindow::on_selectSrcPathButton_clicked()
{
    QString srcPath = QFileDialog::getExistingDirectory(this, "选择目录", QCoreApplication::applicationDirPath(), QFileDialog::ShowDirsOnly);
    if(srcPath.isEmpty()){
        return;
    }
    ui->srcPathLineEdit->setText(srcPath);
    ui->srcPathLineEdit->setCursorPosition(0);
}


void InputBakPathWindow::on_selectDstPathButton_clicked()
{
    QString dstPath = QFileDialog::getExistingDirectory(this, "选择目录", QCoreApplication::applicationDirPath(), QFileDialog::ShowDirsOnly);
    if(dstPath.isEmpty()){
      return;
    }
    ui->dstPathLineEdit->setText(dstPath);
    ui->dstPathLineEdit->setCursorPosition(0);
}


void InputBakPathWindow::on_okButton_clicked()
{
    ui->hintLabel->setVisible(false);
    if(ui->nameLineEdit->text().isEmpty()){
        ui->hintLabel->setText("名称不能为空");
        ui->hintLabel->setVisible(true);
        return;
    }
    qDebug() << QDir(ui->srcPathLineEdit->text()).exists();
    if(ui->srcPathLineEdit->text().isEmpty() || !QDir(ui->srcPathLineEdit->text()).exists()){
        ui->hintLabel->setText("源路径不存在");
        ui->hintLabel->setVisible(true);
        return;
    }
    if(ui->dstPathLineEdit->text().isEmpty() || !QDir(ui->dstPathLineEdit->text()).exists()){
        ui->hintLabel->setText("备份路径不存在");
        ui->hintLabel->setVisible(true);
        return;
    }
    if(ui->srcPathLineEdit->text() == ui->dstPathLineEdit->text()){
        ui->hintLabel->setText("两个路径不能相同");
        ui->hintLabel->setVisible(true);
        return;
    }
    emit sendNewPath(ui->nameLineEdit->text(), ui->srcPathLineEdit->text(), ui->dstPathLineEdit->text());
    this->close();
}


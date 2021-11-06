#include "messageboxexx.h"
#include "ui_messageboxexx.h"
#include "titlebar.h"
#include "commoninclude.h"
#include "src/utils/widget/titlebar.h"
#include <QScrollBar>
#include "proxystyle.h"
#include "qssinstaller.h"

MessageBoxExX::MessageBoxExX(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageBoxExX)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(this->width(), this->height());

    // 标题栏样式
    ui->titleBar->setCloseIcon(TITLEBAR_CLOSEICON);
    ui->titleBar->setTitleText(TITLEBAR_TITLETEXT);
    ui->titleBar->setUseGradient(true);
    ui->titleBar->initUi(TitleBarMode::NoMinAndMaxButton, "rgb(240,255,255)", "rgb(93,94,95)",
                         "qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(18,18,18), stop: 1 rgb(21,21,21))", "rgb(240,255,255)");
    ui->titleBar->setTitleIcon(TITLEBAR_TITLEICON);

    // 样式
    this->setStyleSheet(QssInstaller::QssInstallFromFile(":/stylesheet/stylesheet_messageboxexx.css").arg(this->objectName()).arg("rgb(55,85,100)")
            .arg("qlineargradient(x1:0, y1:0, x2:0, y2:1, stop: 0 rgb(45,45,45), stop: 1 rgb(51,51,51));"
                 "alternate-background-color:rgb(55,55,55)"));
    ui->infoTE->setReadOnly(true);
    ui->infoTE->setLineWrapColumnOrWidth(ui->infoTE->width() - 30);
    ui->infoTE->setLineWrapMode(QTextEdit::WidgetWidth);
    ui->infoTE->setWordWrapMode(QTextOption::WrapAnywhere);
    hScrollBarStyle = new HorizontalScrollBarStyle;
    vScrollBarStyle = new VerticalScrollBarStyle;
    setHorizontalScrollBarStyle(hScrollBarStyle);
    setVerticalScrollBarStyle(vScrollBarStyle);
}

MessageBoxExX::~MessageBoxExX()
{
    delete ui;
    delete hScrollBarStyle;
    delete vScrollBarStyle;
}

void MessageBoxExX::information(QString titleText, QString text, QString buttonText)
{
    ui->titleBar->setTitleText(titleText);
    QMessageBox tmb;
    tmb.setIcon(QMessageBox::Information);
    QPixmap t = QPixmap(tmb.iconPixmap());
    t.scaled(ui->iconL->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->iconL->setScaledContents(true);
    ui->iconL->setPixmap(t);
    ui->infoTE->setText(text);
//    ui->infoTE->setAlignment(Qt::AlignHCenter);
    ui->button->setText(buttonText);
    ui->button2->setVisible(false);
    connect(ui->button, &QPushButton::clicked, this, &MessageBoxExX::close);
    this->exec();
}

int MessageBoxExX::warning(QString titleText, QString text, QString yesText, QString noText)
{
    this->result = Escape;
    ui->titleBar->setTitleText(titleText);
    QMessageBox tmb;
    tmb.setIcon(QMessageBox::Warning);
    QPixmap t = QPixmap(tmb.iconPixmap());
    t.scaled(ui->iconL->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->iconL->setScaledContents(true);
    ui->iconL->setPixmap(t);
    ui->infoTE->setText(text);
//    ui->infoTE->setAlignment(Qt::AlignHCenter);
    ui->button->setText(yesText);
    connect(ui->button, &QPushButton::clicked, this, &MessageBoxExX::resultToYes);
    if(noText.isEmpty()){
        ui->button2->setEnabled(false);
        ui->button2->setVisible(false);
    }
    else{
        ui->button2->setText(noText);
        connect(ui->button2, &QPushButton::clicked, this, &MessageBoxExX::resultToNo);
    }
    this->exec();
    return result;
}

int MessageBoxExX::question(QString titleText, QString text, QString yesText, QString noText)
{
    this->result = Escape;
    ui->titleBar->setTitleText(titleText);
    QMessageBox tmb;
    tmb.setIcon(QMessageBox::Question);
    QPixmap t = QPixmap(tmb.iconPixmap());
    t.scaled(ui->iconL->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->iconL->setScaledContents(true);
    ui->iconL->setPixmap(t);
    ui->infoTE->setText(text);
    ui->button->setText(yesText);
    ui->button2->setText(noText);
    ui->button2->setVisible(true);
    connect(ui->button, &QPushButton::clicked, this, &MessageBoxExX::resultToYes);
    connect(ui->button2, &QPushButton::clicked, this, &MessageBoxExX::resultToNo);
    this->exec();
    return result;
}

void MessageBoxExX::setVerticalScrollBarStyle(VerticalScrollBarStyle *style)
{
    ui->infoTE->verticalScrollBar()->setStyle(style);
}

void MessageBoxExX::setHorizontalScrollBarStyle(HorizontalScrollBarStyle *style)
{
    ui->infoTE->horizontalScrollBar()->setStyle(style);
}

void MessageBoxExX::resultToYes()
{
    this->result = Yes;
    this->close();
}

void MessageBoxExX::resultToNo()
{
    this->result = No;
    this->close();
}

void MessageBoxExY::information(QString titleText, QString text, QString buttonText)
{
    MessageBoxExX *x = new MessageBoxExX;
    x->information(titleText, text, buttonText);
    delete x;
}

int MessageBoxExY::warning(QString titleText, QString text, QString yesText, QString noText)
{
    MessageBoxExX *x = new MessageBoxExX;
    int ret = x->warning(titleText, text, yesText, noText);
    delete x;
    return ret;
}

int MessageBoxExY::question(QString titleText, QString text, QString yesText, QString noText)
{
    MessageBoxExX *x = new MessageBoxExX;
    int ret = x->question(titleText, text, yesText, noText);
    delete x;
    return ret;
}

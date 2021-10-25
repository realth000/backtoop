#include "titlebar.h"
#include <QEvent>

TitleBar::TitleBar(QWidget *parent):
    QGroupBox(parent)
{
    p = parent;
    icoL = new QLabel(this);
    titleL = new QLabel(this);
    minB = new QPushButton(this);
    maxB = new QPushButton(this);
    closeB = new QPushButton(this);
    minB->setFocusPolicy(Qt::NoFocus);
    maxB->setFocusPolicy(Qt::NoFocus);
    closeB->setFocusPolicy(Qt::NoFocus);
}

TitleBar::~TitleBar()
{
    p = nullptr;
    delete icoL;
    delete titleL;
    delete minB;
    delete maxB;
    delete closeB;
}

void TitleBar::initUi(TitleBar::TitleBarMode mode, QString textColor, QString hover_textColor, QString background, QString hover_backgroundColor)
{
    if(useGradient){
        this->setStyleSheet(QString("QWidget{font:15px;color:%1}"
                                "QGroupBox{border:none;background:%2;}"
                                "QPushButton{background:transparent;border:none;border-radius:0px}"
                                "QPushButton:hover{color:%3;background-color:%4}")
                            .arg(textColor).arg(background).arg(hover_textColor).arg(hover_backgroundColor));
    }else{
        this->setStyleSheet(QString("QWidget{font:15px;color:%1}"
                                "QGroupBox{border:none;background-color:%2;}"
                                "QPushButton{background:transparent;border:none;border-radius:0px}"
                                "QPushButton:hover{color:%3;background-color:%4}")
                            .arg(textColor).arg(background).arg(hover_textColor).arg(hover_backgroundColor));
    }
    const int right = this->width();
    const int size = this->height();
    icoL->setGeometry(0, 0, size, size);
    titleL->setGeometry(size+5,0,700,size);
    closeB->setGeometry(right - size, 0, size, size);
    switch (mode) {
    case TitleBarMode::DefaultStyle:
        minB->setGeometry(right-size-size-size, 0, size, size);
        maxB->setGeometry(right-size-size, 0, size, size);
        connect(minB, &QPushButton::clicked, this,[=]{p->showMinimized();}, Qt::UniqueConnection);
        connect(maxB, &QPushButton::clicked, this,[=]{
            if(isMax){
                p->showNormal();
                isMax = false;
            }else{
                p->showMaximized();
                isMax = true;
            }
        }, Qt::UniqueConnection);
        connect(closeB, &QPushButton::clicked, this,[=]{p->close();}, Qt::UniqueConnection);
        break;
    case TitleBarMode::NoMinButton:
        minB->hide();
        maxB->setGeometry(right-size-size, 0, size, size);
        connect(maxB, &QPushButton::clicked, this,[=]{
            if(isMax){
                p->showNormal();
                isMax = false;
            }else{
                p->showMaximized();
                isMax = true;
            }
        }, Qt::UniqueConnection);
        connect(closeB, &QPushButton::clicked, this,[=]{p->close();}, Qt::UniqueConnection);
        break;
    case TitleBarMode::NoMaxButton:
        minB->setGeometry(right-size-size, 0, size, size);
        maxB->hide();
        connect(minB, &QPushButton::clicked, this,[=]{p->showMinimized();}, Qt::UniqueConnection);
        connect(closeB, &QPushButton::clicked, this,[=]{p->close();}, Qt::UniqueConnection);
        break;
    case TitleBarMode::NoMinAndMaxButton:
        minB->hide();
        maxB->hide();
        connect(closeB, &QPushButton::clicked, this,[=]{p->close();}, Qt::UniqueConnection);
        break;
    }
    if(!hasMinIcon){minB->setText("━");}
    if(!hasMaxIcon){
        maxB->setStyleSheet(QString("QPushButton{font:bold;border:none;border-radius:0px}"
                            "QPushButton:hover{color:rgb(93,94,95);background-color:%1}").arg(hover_backgroundColor));
        maxB->setText("█");}
    if(!hasCloseIcon){
        closeB->setStyleSheet("QPushButton{font:bold;border:none;border-radius:0px}"
                              "QPushButton:hover{background-color:rgb(232,17,35)}");
        closeB->setText("X");//⨯✗✘（✕）✖X❌
    }
    else{
        closeB->setStyleSheet("QPushButton:hover{background-color:rgb(232,17,35)}");
    }
}

void TitleBar::setTitleText(QString title) const
{
    titleL->setText(title);
    titleL->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    p->setWindowTitle(title);
}

void TitleBar::setTitleStyleSheet(QString styleSheet) const
{
    titleL->setStyleSheet(styleSheet);
}

void TitleBar::setMinButtonStyleSheet(QString styleSheet) const
{
    minB->setStyleSheet(styleSheet);
}

void TitleBar::setMaxButtonStyleSheet(QString styleSheet) const
{
    maxB->setStyleSheet(styleSheet);
}

void TitleBar::setCloseButtonStyleSheet(QString styleSheet) const
{
    closeB->setStyleSheet(styleSheet);
}

void TitleBar::setTitleIcon(QString fileName) const
{
    p->setWindowIcon(QIcon(fileName));
    QPixmap ico = QPixmap(fileName);
    icoL->setPixmap(ico);
    icoL->setScaledContents(true);
}

void TitleBar::setMinIcon(QString fileName) const
{
    QPixmap ico = QPixmap(fileName);
    minB->setStyleSheet(QString("background-image:%1;").arg(fileName));

}

void TitleBar::setUseGradient(bool use)
{
    useGradient = use;
}

bool TitleBar::getUseGradient() const
{
    return useGradient;
}

void TitleBar::setCloseIcon(QString closeIconPath)
{
    QPixmap pm = QPixmap(closeIconPath).scaled(QSize(this->height(), this->height()), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QIcon ico;
    ico.addPixmap(pm);
    closeB->setIcon(ico);
    hasCloseIcon = true;
}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        leftButtonPressed = true;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        start = event->globalPosition().toPoint();
#else
        start = event->globalPos();
#endif
    }
}

void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if(leftButtonPressed)
    {

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        parentWidget()->move(parentWidget()->geometry().topLeft() + event->globalPosition().toPoint() - start);
        start = event->globalPosition().toPoint();
#else
        parentWidget()->move(parentWidget()->geometry().topLeft() + event->globalPos() - start);
        start = event->globalPos();
#endif
    }
}

void TitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        leftButtonPressed = false;
    }
}

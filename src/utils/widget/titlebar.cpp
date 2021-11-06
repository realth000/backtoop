#include "titlebar.h"
#include <QEvent>

TitleBar::TitleBar(QWidget *parent):
    QGroupBox(parent)
{
    p = parent;
    icoL = new QLabel(this);
    titleL = new QLabel(this);
    minimumButton = new QPushButton(this);
    maximumButton = new QPushButton(this);
    closeButton = new QPushButton(this);
    minimumButton->setFocusPolicy(Qt::NoFocus);
    maximumButton->setFocusPolicy(Qt::NoFocus);
    closeButton->setFocusPolicy(Qt::NoFocus);
}

TitleBar::~TitleBar()
{
    p = nullptr;
    delete icoL;
    delete titleL;
    delete minimumButton;
    delete maximumButton;
    delete closeButton;
}

void TitleBar::initUi(TitleBarModes mode, QString textColor, QString hover_textColor, QString background, QString hover_backgroundColor)
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
    int pos = 1;
    // config closeButton
    if(mode & NoCloseButton){
        closeButton->hide();
    }
    else{
        closeButton->setGeometry(right - size, 0, size, size);
        if(!hasCloseIcon){
            closeButton->setStyleSheet("QPushButton{font:bold;border:none;border-radius:0px}"
                                       "QPushButton:hover{background-color:rgb(232,17,35)}");
            closeButton->setText("X");//⨯✗✘（✕）✖X❌
        }
        else{
            closeButton->setStyleSheet("QPushButton:hover{background-color:rgb(232,17,35)}");
        }
        connect(closeButton, &QPushButton::clicked, this,[=]{p->close();});
        pos++;

    }

    // config maximumButton
    if(mode & NoMaxButton){
        maximumButton->hide();
    }
    else{
        maximumButton->setGeometry(right - pos*size, 0, size, size);
        if(!hasMaxIcon){
            maximumButton->setStyleSheet(QString("QPushButton{font:bold;border:none;border-radius:0px}"
                                "QPushButton:hover{color:rgb(93,94,95);background-color:%1}").arg(hover_backgroundColor));
            maximumButton->setText("█");
        }
        connect(maximumButton, &QPushButton::clicked, this,[=]{
            if(isMax){
                p->showNormal();
                isMax = false;
            }else{
                p->showMaximized();
                isMax = true;
            }
        });
        pos++;
    }

    // config minimumButton
    if(mode & NoMinButton){
        minimumButton->hide();

    }
    else{
        minimumButton->setGeometry(right - pos*size, 0, size, size);
        if(!hasMinIcon){minimumButton->setText("━");}
        connect(minimumButton, &QPushButton::clicked, this,[=]{p->showMinimized();});
        pos++;
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
    minimumButton->setStyleSheet(styleSheet);
}

void TitleBar::setMaxButtonStyleSheet(QString styleSheet) const
{
    maximumButton->setStyleSheet(styleSheet);
}

void TitleBar::setCloseButtonStyleSheet(QString styleSheet) const
{
    closeButton->setStyleSheet(styleSheet);
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
    minimumButton->setStyleSheet(QString("background-image:%1;").arg(fileName));

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
    closeButton->setIcon(ico);
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

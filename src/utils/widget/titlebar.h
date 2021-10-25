#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QGroupBox>
#include <QPoint>
#include <QMouseEvent>
#include <QLabel>
#include <QPushButton>

class TitleBar : public QGroupBox
{
    Q_OBJECT


public:
    enum TitleBarMode{
        DefaultStyle = 0,
        NoMinButton = 1,
        NoMaxButton = 2,
        NoMinAndMaxButton = 3
    };

    TitleBar(QWidget *parent = nullptr);
    ~TitleBar();
    void initUi(enum TitleBarMode mode = DefaultStyle, QString textColor = "black", QString hover_textColor = "white", QString background = "white", QString hover_backgroundColor = "black");
    void setTitleText(QString title) const;
    void setTitleStyleSheet(QString styleSheet) const;
    void setMinButtonStyleSheet(QString styleSheet) const;
    void setMaxButtonStyleSheet(QString styleSheet) const;
    void setCloseButtonStyleSheet(QString styleSheet) const;
    void setTitleIcon(QString fileName) const;
    void setMinIcon(QString fileName) const;
    void setUseGradient(bool use);
    bool getUseGradient() const;
    void setCloseIcon(QString closeIconPath);


protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    bool leftButtonPressed;
    bool useGradient = false;
    QPoint start;
    QLabel *icoL;
    QLabel *titleL;
    QPushButton *minB;
    bool hasMinIcon = false;
    bool hasMaxIcon = false;
    bool hasCloseIcon = false;
    bool isMax = false;
    QPushButton *maxB;
    QPushButton *closeB;
    QWidget *p;
};

#endif // TITLEBAR_H

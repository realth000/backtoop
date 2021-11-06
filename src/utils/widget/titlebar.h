#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QGroupBox>
#include <QPoint>
#include <QMouseEvent>
#include <QLabel>
#include <QPushButton>

enum TitleBarMode{
    DefaultStyle =      0x00,
    NoMinButton =       0x01,
    NoMaxButton =       0x02,
    NoCloseButton =     0x04,
    NoMinAndMaxButton = 0x03
};

Q_DECLARE_FLAGS(TitleBarModes, TitleBarMode)
Q_DECLARE_OPERATORS_FOR_FLAGS(TitleBarModes)

class TitleBar : public QGroupBox
{
    Q_OBJECT


public:
    TitleBar(QWidget *parent = nullptr);
    ~TitleBar();
    void initUi(TitleBarModes mode = DefaultStyle, QString textColor = "black", QString hover_textColor = "white", QString background = "white", QString hover_backgroundColor = "black");
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
    QPushButton *minimumButton;
    bool hasMinIcon = false;
    bool hasMaxIcon = false;
    bool hasCloseIcon = false;
    bool isMax = false;
    QPushButton *maximumButton;
    QPushButton *closeButton;
    QWidget *p;
};

#endif // TITLEBAR_H

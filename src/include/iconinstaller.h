#ifndef ICONINSTALLER_H
#define ICONINSTALLER_H

#include <QString>
#include <QPushButton>

class IconInstaller
{
public:
    explicit IconInstaller();
    static void installPushButtonIcon(QPushButton *pushButton, QString iconPath);
};

#endif // QSSINSTALLER_H

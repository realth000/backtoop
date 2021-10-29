#include "iconinstaller.h"
#include <QPixmap>
#include <QIcon>

IconInstaller::IconInstaller()
{

}

void IconInstaller::installPushButtonIcon(QPushButton *pushButton, QString iconPath)
{
    if(pushButton == nullptr || iconPath.isEmpty()){
        return;
    }
    QIcon icon;
    const QPixmap pixmap = QPixmap(iconPath);
    if(pixmap.isNull()){
        return;
    }
    icon.addPixmap(pixmap, QIcon::Normal, QIcon::Off);
    pushButton->setIcon(icon);
}

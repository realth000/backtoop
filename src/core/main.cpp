#include "../gui/mainui.h"
#include <QApplication>
#include <QFont>
#include "commoninclude.h"

int main(int argc, char *argv[])
{
    QFont appFont;
    appFont.setFamily("Microsoft YaHei");
    appFont.setStyleStrategy(QFont::PreferAntialias);

    QCoreApplication::setApplicationName(TITLEBAR_TITLETEXT);
    QCoreApplication::setApplicationVersion(ABOUT_VERSION);

    QApplication a(argc, argv);
    a.setFont(appFont);
    mainui w;
    w.show();
    return a.exec();
}

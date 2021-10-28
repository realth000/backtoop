#include "../gui/mainui.h"
#include <QApplication>
#include <QFont>
#include "commoninclude.h"

#ifdef ENABLE_VID
#include "vld.h"
#endif

int main(int argc, char *argv[])
{
    QFont appFont;
    appFont.setFamily("Microsoft YaHei");
    appFont.setStyleStrategy(QFont::PreferAntialias);

    QCoreApplication::setApplicationName(TITLEBAR_TITLETEXT);
    QCoreApplication::setApplicationVersion(ABOUT_VERSION);

    QApplication a(argc, argv);
    a.setFont(appFont);
    MainUi w;
    w.show();
    return a.exec();
}

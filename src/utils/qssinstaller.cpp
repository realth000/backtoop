#include "qssinstaller.h"

QssInstaller::QssInstaller()
{

}

QString QssInstaller::QssInstallFromFile(QString qssFilePath)
{
    QFile qss(qssFilePath);
    qss.open(QIODevice::ReadOnly);
    QTextStream qssStream;
    qssStream.setDevice(&qss);
    QString qssString = qss.readAll();
    qss.close();
    return qssString;
}

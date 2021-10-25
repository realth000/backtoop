#ifndef QSSINSTALLER_H
#define QSSINSTALLER_H
#include <QFile>
#include <QTextStream>
#include <QIODevice>

class QssInstaller
{
public:
    explicit QssInstaller();
    static QString QssInstallFromFile(QString qssFilePath);
};

#endif // QSSINSTALLER_H

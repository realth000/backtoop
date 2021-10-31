#ifndef CALCHASH_H
#define CALCHASH_H

#include <QString>
#include <QCryptographicHash>
#include <QFile>
#include <QIODevice>

class CalcHash
{
public:
    CalcHash();
    static QString calcFileMd5(QString filePath);
};

#endif // CALCHASH_H

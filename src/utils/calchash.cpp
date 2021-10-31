#include "calchash.h"
#include <QDebug>

CalcHash::CalcHash()
{

}

QString CalcHash::calcFileMd5(QString filePath)
{

    QFile file(filePath);
    if(!file.exists() || !file.open(QIODevice::ReadOnly)){
        return QString();
    }
    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData(file.readAll());
    file.close();
    return md5.result().toHex();
}

#ifndef COPYWORKER_H
#define COPYWORKER_H

#include <QObject>
#include "commoninclude.h"

class CopyWorker : public QObject
{
    Q_OBJECT
public:
    explicit CopyWorker(QObject *parent = nullptr);
    void setTask(CopyTaskVector copyTaskVector);

public slots:
    void copyStart();

signals:
    void copyFinished();
    void copyOneFileSucceed(QString filePath = "");
    void copyOneFileFailed(QString filePath = "");

private:
    CopyTaskVector taskVector;

    void copyWork(QString srcPath, QString dstPath);
};

#endif // COPYWORKER_H

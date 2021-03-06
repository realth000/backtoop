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
    void setReplaceFile(bool replaceFile);
    void setCheckFileSum(bool checkFileSum);
    void setResetDir(bool resetDir);

public slots:
    void copyStart();
    void copyTerminate();

signals:
    void copyFinished();
    void copyFileResult(QString filePath, CopyResult result);
    void copyTerminated();
    void copyCurrentFile(QString filePath);

private:
    CopyTaskVector taskVector;
    bool replaceFile = false;
    bool checkFileSum = false;
    bool resetDir = false;
    bool running = true;

    void copyWork(QString srcPath, QString dstPath);
    void copyPostWork(QString srcPath, QString dstPath);
    QString getFileSum(QString filePath) const;
};

#endif // COPYWORKER_H

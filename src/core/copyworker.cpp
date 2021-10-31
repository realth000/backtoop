#include "copyworker.h"
#include <QFileInfoList>
#include <QDir>
#include <QDebug>
#include "calchash.h"

CopyWorker::CopyWorker(QObject *parent) : QObject(parent)
{

}

void CopyWorker::setTask(CopyTaskVector copyTaskVector)
{
    taskVector = copyTaskVector;
}

void CopyWorker::setReplaceFile(bool replaceFile)
{
    this->replaceFile = replaceFile;
}

void CopyWorker::setCheckFileSum(bool checkFileSum)
{
    this->checkFileSum = checkFileSum;
}

void CopyWorker::setResetDir(bool resetDir)
{
    this->resetDir = resetDir;
}

void CopyWorker::copyStart()
{
    if(taskVector.length() < 1){
        emit copyFinished();
        return;
    }

    foreach(auto task, taskVector){
        qDebug() << "receive task:" << task.first << task.second;
        if(resetDir){
            QDir d(task.second);
            d.removeRecursively();
            d.mkdir(d.absolutePath());
        }
        copyWork(task.first, task.second);
    }
    emit copyFinished();
}

void CopyWorker::copyWork(QString srcPath, QString dstPath)
{
    QFileInfo srcInfo(srcPath);
    QFileInfo dstInfo(dstPath);
    QFileInfoList infoList = QDir(srcPath).entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    foreach(QFileInfo info, infoList){
        if(info.isFile()){
            QString tmp = dstInfo.absoluteFilePath() + info.filePath().replace(srcInfo.absoluteFilePath(), "");
            qDebug() << "copy" << info.absoluteFilePath() << tmp << dstInfo.absolutePath();
            if(QFile::copy(info.filePath(), tmp)){
                copyPostWork(info.filePath(), tmp);
                continue;
            }
            else{
                if(QFile(tmp).exists() && replaceFile){
                    // 复制失败，且文件已经存在
                    qDebug() << "remove result" << QFile::remove(tmp) << tmp;
                    if(QFile::copy(info.filePath(), tmp)){
                        copyPostWork(info.filePath(), tmp);
                        continue;
                    }
                }
                emit copyFileResult(srcPath, CopyResult::AlreadyExists);
                continue;
            }
        }
        else{
            // 如果是文件夹，递归拷贝，要注意换一下备份路径到具体下一层。
            QString childPath = dstInfo.absoluteFilePath() + info.filePath().replace(srcInfo.absoluteFilePath(), "");
            QDir(dstPath).mkpath(childPath);
            copyWork(info.filePath(), childPath);
        }
    }
}

void CopyWorker::copyPostWork(QString srcPath, QString dstPath)
{
    if(checkFileSum){
        if(getFileSum(srcPath) != getFileSum(dstPath)){
            emit copyFileResult(srcPath, CopyResult::HashCheckFailed);
            return;
        }
    }
    emit copyFileResult(srcPath, CopyResult::Success);
}

QString CopyWorker::getFileSum(QString filePath) const
{
    return CalcHash::calcFileMd5(filePath);
}

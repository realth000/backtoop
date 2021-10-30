#include "copyworker.h"
#include <QFileInfoList>
#include <QDir>
#include <QDebug>

CopyWorker::CopyWorker(QObject *parent) : QObject(parent)
{

}

void CopyWorker::setTask(CopyTaskVector copyTaskVector)
{
    taskVector = copyTaskVector;
}

void CopyWorker::copyStart()
{
    if(taskVector.length() < 1){
        emit copyFinished();
        return;
    }

    foreach(auto task, taskVector){
        qDebug() << "receive task:" << task.first << task.second;
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
                emit copyOneFileSucceed(srcPath);
            }
            else{
                if(info.exists()){
                    // 复制失败，且文件已经存在
                    emit copyOneFileFailed(srcPath);
                }
                else{
                    emit copyOneFileFailed(srcPath);
                }
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

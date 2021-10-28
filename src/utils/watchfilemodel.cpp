#include "watchfilemodel.h"
#include <QDebug>

WatchFileModel::WatchFileModel()
{

}

// 基本照搬源码，只对文件的大小的单位和类型做了修改。
QVariant WatchFileModel::data(const QModelIndex &index, int role) const
{
    if( !index.isValid() || index.model() != this){
        return QVariant();
    }
    switch (role) {
    case Qt::EditRole:
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0:
            return  QFileSystemModel::data(index, role);
        case 1:
            return modSize(QFileSystemModel::data(index, role));
        case 2:
            return modType(QFileSystemModel::data(index, role));
        case 3:
            return QFileSystemModel::data(index, role);
        default:
            qWarning("data: invalid display value column %d", index.column());
            break;
        }
        break;
    case FilePathRole:
        return QFileSystemModel::data(index, FilePathRole);
    case FileNameRole:
        return QFileSystemModel::data(index, FileNameRole);
    case Qt::DecorationRole:
        if(index.column() == 0){
            return QFileSystemModel::data(index, Qt::DecorationRole);
        }
        break;
    case Qt::TextAlignmentRole:
        if (index.column() == 1)
            return QVariant(Qt::AlignTrailing | Qt::AlignVCenter);
        break;
    case FilePermissions:
        int p = permissions(index);
        return p;
    }
    return QVariant();
}

//QModelIndex watchFileModel::setRootPath(const QString &newPath)
//{
//    if(watcher->addPath(newPath)){
//        connect(watcher, &QFileSystemWatcher::directoryChanged, this, refresh)
//        return QFileSystemModel::setRootPath(newPath);
//    }
//    else{
//        return  QModelIndex();
//    }
//}

// 设置horizontal header。
QVariant WatchFileModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // 只设置行标题，不要列标题。
    if(orientation == Qt::Horizontal){
        if(role == Qt::DisplayRole){
            switch (section) {
            case 0:
                return QVariant("名称");
            case 1:
                return QVariant("大小");
            case 2:
                return QVariant("类型");
            case 3:
                return QVariant("修改日期");
            default:
                return QVariant();
            }
        }
    }
    return QVariant();
}

QVariant WatchFileModel::modSize(QVariant size) const
{
    QString sizeString = size.toString();
    sizeString.replace("字节", "B");
    sizeString.replace("KiB", "KB");
    sizeString.replace("MiB", "MB");
    sizeString.replace("GiB", "GB");
    return QVariant(sizeString);
}

QVariant WatchFileModel::modType(QVariant type) const
{
    QString typeString = type.toString();
    typeString.replace("File Folder", "文件夹");
    typeString.replace("File", "文件");
    return QVariant(typeString);
}

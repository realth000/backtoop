#ifndef WATCHFILEMODEL_H
#define WATCHFILEMODEL_H

#include <QFileSystemModel>
#include <QFileSystemWatcher>

class WatchFileModel : public QFileSystemModel
{
    Q_OBJECT
public:
    WatchFileModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole)const override;
    // 需要watcher的原因是 treeview切换路径时需要加载文件数，每次都从表格加的话如果文件数量多会卡
    // 因此 用额外的QMap<QString int> 存文件数，watcher监视当model不为currentModel时改变文件数
    // 当是current时，由于QFileSystemModel具有实时性，每次文件变更会发射信号directoryLoaded，已经连接上的信号槽会自动计算正确的文件数
    // 所以当model为currentModel时不需要watcher监视。
    // QModelIndex setRootPath(const QString &newPath);
protected:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    QVariant modSize(QVariant size) const;
    QVariant modType(QVariant type) const;
};

#endif // WATCHFILEMODEL_H

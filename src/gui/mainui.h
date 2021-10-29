#ifndef MAINUI_H
#define MAINUI_H

#include <QWidget>
#include <QCheckBox>
#include <QList>
#include <QMap>
#include <QCheckBox>
#include <QTableWidgetItem>
#include <QStandardItemModel>
#include "proxystyle.h"
#include "watchfilemodel.h"

enum ModelMode{
    SRC = 0,
    DST
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainUi; }
QT_END_NAMESPACE

class MainUi : public QWidget
{
    Q_OBJECT

public:
    MainUi(QWidget *parent = nullptr);
    ~MainUi();

public slots:
    void log(QString msg) const;


private:
    Ui::MainUi *ui;
    QList<QCheckBox *> bakPathChbList;
    int bakChbCheckedCount = 0;
    // styles
    CheckBoxStyle *checkBoxStyle = nullptr;
    VerticalScrollBarStyle *vScrollBarStyle = nullptr;
    HorizontalScrollBarStyle *hScrollBarStyle = nullptr;
    PushButtonStyle *pushbuttonStyle = nullptr;

    QString currentSrcPath;
    QString currentDstPath;

    // file model: watching source path and destination path
    WatchFileModel *srcPathWatchModel = nullptr;
    WatchFileModel *dstPathWatchModel = nullptr;
    // 保存各model，用currentSrcPath + currentDesPath做key，下同
    QMap<QString, WatchFileModel *> srcPathWatchModelMap;
    QMap<QString, WatchFileModel *> dstPathWatchModelMap;
    // 保存各model中expanded的QModelIndex，保存展开状态
    QMap<QString, QModelIndexList> srcModelIndexList;
    QMap<QString, QModelIndexList> dstModelIndexList;
    // 保存model中项目个个数
    QMap<QString, int> srcMapCountMap;
    QMap<QString, int> dstMapCountMap;
    // dir view header
    QStandardItemModel *dirViewHeaderModel = nullptr;

    void initUi();
    void initConfig();
    QWidget* addCheckBox();
    void addBackupPath(QString name, QString time, QString srcPath, QString dstPath);

private slots:
    void bakChbStateChanged(int state);
    void getSelectedItemInfo(WatchFileModel *model, QModelIndex modelIndex);
    void getModelInfoFromIndex(ModelMode mode, QModelIndex modelIndex);
    void getSrcModelInfoFromIndex(QModelIndex modelIndex);
    void getDstModelInfoFromIndex(QModelIndex modelIndex);
    void getModelInfoFromString(ModelMode mode, QString path);
    void getSrcModelInfoFromString(QString srcPath);
    void getDstModelInfoFromString(QString dstPath);
    void on_backupPathsTableWidget_itemClicked(QTableWidgetItem *item);
};
#endif // MAINUI_H

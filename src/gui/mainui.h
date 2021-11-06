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
    Src = 0,
    Dst
};

enum CopyContentType{
    Content = 0,
    Folder = 1
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
    void insertBackupPath(QString name, QString srcPath, QString dstPath);

private:
    Ui::MainUi *ui;
    QList<QCheckBox *> bakPathChbList;
    int bakChbCheckedCount = 0; // selected path count
    // styles
    CheckBoxStyle *checkBoxStyle = nullptr;
    VerticalScrollBarStyle *vScrollBarStyle = nullptr;
    HorizontalScrollBarStyle *hScrollBarStyle = nullptr;
    PushButtonStyle *pushButtonStyle = nullptr;
    RadioButtonStyle *radioButtonStyle = nullptr;
    TreeViewStyle *treeViewStyle = nullptr;

    QString currentSrcPath;
    QString currentDstPath;
    QString appPath;
    QString dataFilePath;

    // config
    bool replaceFile = false;
    bool checkFileSum = false;
    bool resetDir = false;
    int copyContentType = CopyContentType::Content;

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
    void initDefaultConfig();
    void initConfig();
    void loadConfig();
    void saveConfig();
    void loadData();
    void saveData();
    void clearData();
    QWidget* addCheckBox();
    void addBackupPath(QString name, QString time, QString srcPath, QString dstPath);
    void deleteSelectedPath();
    void getFileCount(QString path, quint64 &num);

private slots:
    void bakChbStateChanged(int state);
    void getSelectedItemInfo(WatchFileModel *model, QModelIndex modelIndex);
    void getModelInfoFromIndex(ModelMode mode, QModelIndex modelIndex);
    void getSrcModelInfoFromIndex(QModelIndex modelIndex);
    void getDstModelInfoFromIndex(QModelIndex modelIndex);
    void getModelInfoFromString(ModelMode mode, QString path);
    void getSrcModelInfoFromString(QString srcPath);
    void getDstModelInfoFromString(QString dstPath);
    void updateBackupTime();
    void on_backupPathsTableWidget_itemClicked(QTableWidgetItem *item);
    void on_allSelectButton_clicked();
    void on_reverseSelectButton_clicked();
    void on_deletePathsTableButton_clicked();
    void on_startBackupButton_clicked();
    void on_replaceFileCheckBox_stateChanged(int state);
    void on_checkSumCheckBox_clicked();
    void on_resetDirCheckBox_clicked();
    void on_openPathTableJsonButton_clicked();
    void on_savePathTableButton_clicked();
    void on_cpContentRadioButton_clicked();
    void on_cpDirRadioButton_clicked();
    void on_refreshPathTableButton_clicked();
};
#endif // MAINUI_H

#include "mainui.h"
#include "ui_mainui.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDir>
#include <QDebug>
#include <QScrollBar>
#include <QThread>
#include <QSettings>
#include <QMenu>
#include <QAction>
#include <QDesktopServices>
#include "commoninclude.h"
#include "qssinstaller.h"
#include "iconinstaller.h"
#include "copyprogresswindow.h"
#include "core/copyworker.h"
#include "messageboxexx.h"
#include "inputbakpathwindow.h"
#include "core/jsonparser.h"

MainUi::MainUi(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainUi)
{
    ui->setupUi(this);
    initConfig();
    initUi();
    loadData();
}

MainUi::~MainUi()
{
    delete ui;

    // delete style
    delete checkBoxStyle;
    delete vScrollBarStyle;
    delete hScrollBarStyle;
    delete pushButtonStyle;
    delete radioButtonStyle;
    delete treeViewStyle;

    //delete view
    delete dirViewHeaderModel;

    // delete model
    if(srcPathWatchModelMap.count() >= 1){
        qDeleteAll(srcPathWatchModelMap);
    }
    if(dstPathWatchModelMap.count() >= 1){
        qDeleteAll(dstPathWatchModelMap);
    }
}

void MainUi::log(QString msg) const
{
    ui->logLineEdit->setText(msg);
}

void MainUi::insertBackupPath(QString name, QString srcPath, QString dstPath)
{
    qDebug() << "receive new backup path:" << name << srcPath << dstPath;
    addBackupPath(name, "-", srcPath, dstPath);
}


void MainUi::initUi()
{
    // init styles
    checkBoxStyle = new CheckBoxStyle;
    vScrollBarStyle = new VerticalScrollBarStyle;
    hScrollBarStyle = new HorizontalScrollBarStyle;
    pushButtonStyle = new PushButtonStyle;
    radioButtonStyle = new RadioButtonStyle;
    treeViewStyle = new TreeViewStyle();
    ui->srcDirTreeView->setStyle(treeViewStyle);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(this->width(), this->height());
    this->setStyleSheet(QssInstaller::QssInstallFromFile(":/stylesheet/stylesheet.css").arg(this->objectName(), "rgb(55,85,100)",
                                 "qlineargradient(x1:0, y1:0, x2:0, y2:1, stop: 0 rgb(45,45,45), stop: 1 rgb(51,51,51));"
                                 "alternate-background-color:rgb(55,55,55)"));
    // Title bar style
    ui->titleBar->setFixedWidth(this->width());
    ui->titleBar->setCloseIcon(TITLEBAR_CLOSEICON);
    ui->titleBar->setTitleText(TITLEBAR_TITLETEXT);
    ui->titleBar->setUseGradient(true);
    ui->titleBar->initUi(TitleBarMode::NoMaxButton, "rgb(240,255,255)", "rgb(93,94,95)",
                         "qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(18,18,18), stop: 1 rgb(21,21,21))", "rgb(240,255,255)");
    ui->titleBar->setTitleIcon(TITLEBAR_TITLEICON);

    // Paths table widget
    QStringList horizontalHeaders;
    horizontalHeaders << "选择" << "名称"<< "上次备份时间" << "源路径" << "备份路径";
    ui->backupPathsTableWidget->setColumnCount(horizontalHeaders.length());
    ui->backupPathsTableWidget->setColumnWidth(0, 40);
    ui->backupPathsTableWidget->setColumnWidth(1, 220);
    ui->backupPathsTableWidget->setColumnWidth(2, 160);
    ui->backupPathsTableWidget->setColumnWidth(3, 310);
    ui->backupPathsTableWidget->setColumnWidth(4, 310);
    ui->backupPathsTableWidget->setHorizontalHeaderLabels(horizontalHeaders);
    ui->backupPathsTableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->backupPathsTableWidget->verticalHeader()->setVisible(false);
    ui->backupPathsTableWidget->verticalHeader()->setDefaultAlignment(Qt::AlignHCenter);
    ui->backupPathsTableWidget->verticalHeader()->setDefaultAlignment(Qt::AlignVCenter);
    ui->backupPathsTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->backupPathsTableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    ui->backupPathsTableWidget->setSelectionBehavior(QTableWidget::SelectRows);
    ui->backupPathsTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->backupPathsTableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->backupPathsTableWidget->horizontalHeader()->setHighlightSections(false);
    ui->backupPathsTableWidget->horizontalScrollBar()->setStyle(hScrollBarStyle);
    ui->backupPathsTableWidget->verticalScrollBar()->setStyle(vScrollBarStyle);
    ui->backupPathsTableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->backupPathsTableWidget, &QTableWidget::customContextMenuRequested, this, &MainUi::backupPathTableContextMenu);


    // Display log
    ui->logLineEdit->setReadOnly(true);
    ui->logLineEdit->setContextMenuPolicy(Qt::NoContextMenu);
    ui->srcDirHintLineEdit->setReadOnly(true);
    ui->srcDirHintLineEdit->setContextMenuPolicy(Qt::NoContextMenu);
    ui->dstDirHintLineEdit->setReadOnly(true);
    ui->dstDirHintLineEdit->setContextMenuPolicy(Qt::NoContextMenu);

    // init dir model
    ui->srcDirTreeView->verticalScrollBar()->setStyle(vScrollBarStyle);
    ui->srcDirTreeView->horizontalScrollBar()->setStyle(hScrollBarStyle);
    ui->dstDirTreeView->verticalScrollBar()->setStyle(vScrollBarStyle);
    ui->dstDirTreeView->horizontalScrollBar()->setStyle(hScrollBarStyle);
    QStringList dirViewHeaders = {"名称", "大小", "类型", "修改时间"};
    dirViewHeaderModel = new QStandardItemModel();
    dirViewHeaderModel->setHorizontalHeaderLabels(dirViewHeaders);
    ui->srcDirTreeView->header()->setModel(dirViewHeaderModel);
    ui->srcDirTreeView->header()->setDefaultAlignment(Qt::AlignCenter);
    ui->srcDirTreeView->setColumnWidth(0, 270);
    ui->srcDirTreeView->setColumnWidth(1, 70);
    ui->srcDirTreeView->setColumnWidth(2, 60);
    ui->srcDirTreeView->setColumnWidth(3, 110);
    ui->dstDirTreeView->header()->setModel(dirViewHeaderModel);
    ui->dstDirTreeView->header()->setDefaultAlignment(Qt::AlignCenter);
    ui->dstDirTreeView->setColumnWidth(0, 270);
    ui->dstDirTreeView->setColumnWidth(1, 70);
    ui->dstDirTreeView->setColumnWidth(2, 60);
    ui->dstDirTreeView->setColumnWidth(3, 110);
    ui->srcDirTreeView->setWordWrap(false);
    ui->dstDirTreeView->setWordWrap(false);
    ui->srcDirTreeView->setStyle(treeViewStyle);
    ui->dstDirTreeView->setStyle(treeViewStyle);

//    connect(ui->srcDirTreeView, &QTreeView::clicked,   this, &MainUi::getSrcModelInfo, Qt::UniqueConnection);
    connect(ui->srcDirTreeView, &QTreeView::collapsed, this, &MainUi::getSrcModelInfoFromIndex, Qt::UniqueConnection);
    connect(ui->srcDirTreeView, &QTreeView::expanded,  this, &MainUi::getSrcModelInfoFromIndex, Qt::UniqueConnection);
//    connect(ui->dstDirTreeView, &QTreeView::clicked,   this, &MainUi::getDstModelInfo, Qt::UniqueConnection);
    connect(ui->dstDirTreeView, &QTreeView::collapsed, this, &MainUi::getDstModelInfoFromIndex, Qt::UniqueConnection);
    connect(ui->dstDirTreeView, &QTreeView::expanded,  this, &MainUi::getDstModelInfoFromIndex, Qt::UniqueConnection);

    // init pushbutton
    IconInstaller::installPushButtonIcon(ui->openPathTableJsonButton, ":/pic/opentable.png");
    IconInstaller::installPushButtonIcon(ui->refreshPathTableButton, ":/pic/refresh.png");
    IconInstaller::installPushButtonIcon(ui->savePathTableButton, ":/pic/save.png");
    IconInstaller::installPushButtonIcon(ui->allSelectButton, ":/pic/true.png");
    IconInstaller::installPushButtonIcon(ui->reverseSelectButton, ":/pic/false.png");
    IconInstaller::installPushButtonIcon(ui->deletePathsTableButton, ":/pic/delete.png");
    IconInstaller::installPushButtonIcon(ui->startBackupButton, ":/pic/run.png");
    ui->openPathTableJsonButton->setStyle(pushButtonStyle);
    ui->refreshPathTableButton->setStyle(pushButtonStyle);
    ui->savePathTableButton->setStyle(pushButtonStyle);
    ui->allSelectButton->setStyle(pushButtonStyle);
    ui->reverseSelectButton->setStyle(pushButtonStyle);
    ui->deletePathsTableButton->setStyle(pushButtonStyle);
    ui->startBackupButton->setStyle(pushButtonStyle);

    // init checkbox
    ui->replaceFileCheckBox->setStyle(checkBoxStyle);
    ui->checkSumCheckBox->setStyle(checkBoxStyle);
    ui->resetDirCheckBox->setStyle(checkBoxStyle);

    ui->cpContentRadioButton->setStyle(radioButtonStyle);
    ui->cpDirRadioButton->setStyle(radioButtonStyle);

    // apply config
    ui->replaceFileCheckBox->setChecked(replaceFile);
    ui->checkSumCheckBox->setChecked(checkFileSum);
    ui->resetDirCheckBox->setChecked(resetDir);

    switch (copyContentType) {
    case 1:
        ui->cpContentRadioButton->setChecked(true);
        break;
    case 0:
    default:
        ui->cpContentRadioButton->setChecked(true);
        break;
    }

    log("启动");
}

void MainUi::initDefaultConfig()
{
    ui->replaceFileCheckBox->setChecked(false);
    ui->checkSumCheckBox->setChecked(false);
    ui->resetDirCheckBox->setChecked(false);
}

void MainUi::initConfig()
{
    appPath = QDir::toNativeSeparators(QCoreApplication::applicationDirPath());
    dataFilePath = QDir::toNativeSeparators(appPath + "/" +APP_DATA_FILE_NAME);
    loadConfig();
}

void MainUi::loadConfig()
{
    QString configFilePath(QDir::toNativeSeparators(appPath + "/" +APP_CONFIG_FILE_NAME));
    if(!QFileInfo::exists(configFilePath)){
        MessageBoxExY::information("未找到配置文件", "未找到配置文件，加载默认配置");
        initDefaultConfig();
        return;
    }
    QSettings *ini = new QSettings(configFilePath, QSettings::IniFormat);
    replaceFile = ini->value(CONFIG_REPLACEFILE_NAME).toBool();
    checkFileSum = ini->value(CONFIG_CHECKFILESUM_NAME).toBool();
    resetDir = ini->value(CONFIG_RESETDIR_NAME).toBool();
    copyContentType = ini->value(CONFIG_COPYCONTENTTYPE_NAME).toInt();
    delete ini;
}

void MainUi::saveConfig()
{
    QString configFilePath(QDir::toNativeSeparators(appPath + "/" +APP_CONFIG_FILE_NAME));
    QSettings *ini = new QSettings(configFilePath, QSettings::IniFormat);
    ini->setValue(CONFIG_REPLACEFILE_NAME, replaceFile);
    ini->setValue(CONFIG_CHECKFILESUM_NAME, checkFileSum);
    ini->setValue(CONFIG_RESETDIR_NAME, resetDir);
    ini->setValue(CONFIG_COPYCONTENTTYPE_NAME, copyContentType);
    delete ini;
}

void MainUi::loadData()
{
    if(!QFileInfo::exists(dataFilePath)){
        qDebug() << "data file not exists";
        return;
    }
    BackupPathDatas datas = JsonParser::loadBackupPathDataFromFile(dataFilePath);
    foreach(BackupPathData data, datas){
        addBackupPath(data.name, data.lastBackupTime, data.srcPath, data.dstPath);
    }

}

void MainUi::saveData()
{
    BackupPathDatas allData;
    for(int pos = 0; pos < ui->backupPathsTableWidget->rowCount(); pos++){
        BackupPathData data;
        data.id = QString::number(pos);
        data.name = ui->backupPathsTableWidget->item(pos, 1)->text();
        data.lastBackupTime = ui->backupPathsTableWidget->item(pos, 2)->text();
        data.srcPath = ui->backupPathsTableWidget->item(pos, 3)->text();
        data.dstPath = ui->backupPathsTableWidget->item(pos, 4)->text();
        allData.append(data);
    }
    bool saveResult = JsonParser::saveBackupPathDataToFile(dataFilePath, allData);
    saveResult ? log("保存完成") : log("保存失败");
}

void MainUi::clearData()
{
    ui->backupPathsTableWidget->clearContents();
    ui->backupPathsTableWidget->setRowCount(0);
    bakChbCheckedCount = 0;
    if(bakPathChbList.length() >= 1){
        qDeleteAll(bakPathChbList);
        bakPathChbList.clear();
    }
    dstPathWatchModel = nullptr;
    if(srcPathWatchModelMap.size() >= 1){
        qDeleteAll(srcPathWatchModelMap);
        srcPathWatchModelMap.clear();
    }

    if(dstPathWatchModelMap.size() >= 1){
        qDeleteAll(dstPathWatchModelMap);
        dstPathWatchModelMap.clear();
    }
    srcModelIndexList.clear();
    dstModelIndexList.clear();
    srcMapCountMap.clear();
    dstMapCountMap.clear();
    if(dirViewHeaderModel != nullptr){
        delete dirViewHeaderModel;
    }
    dirViewHeaderModel = nullptr;
    srcPathWatchModel = nullptr;
    dstPathWatchModel = nullptr;
}

// add checkbox in tablewidget to select
QWidget* MainUi::addCheckBox()
{
    QCheckBox *checkBox = new QCheckBox(ui->backupPathsTableWidget);
    bakPathChbList.append(checkBox);
    checkBox->setStyle(checkBoxStyle);
    connect(checkBox, &QCheckBox::stateChanged, this, &MainUi::bakChbStateChanged, Qt::UniqueConnection);
    QVBoxLayout *vb = new QVBoxLayout;
    vb->addStretch(1);
    vb->addWidget(checkBox);
    QHBoxLayout *hb = new QHBoxLayout;
    hb->addStretch(1);
    hb->addLayout(vb);
    hb->addStretch(1);
    QWidget *resultQWidget = new QWidget;
    resultQWidget->setLayout(hb);
    return resultQWidget;
}

void MainUi::addBackupPath(QString name, QString time, QString srcPath, QString dstPath)
{
    const int insertPosition = ui->backupPathsTableWidget->rowCount();
    ui->backupPathsTableWidget->insertRow(insertPosition);
    ui->backupPathsTableWidget->setRowHeight(insertPosition, 35);
    ui->backupPathsTableWidget->setCellWidget(insertPosition, 0, addCheckBox());
    ui->backupPathsTableWidget->setItem(insertPosition, 1, new QTableWidgetItem(name));
    ui->backupPathsTableWidget->setItem(insertPosition, 2, new QTableWidgetItem(time));
    ui->backupPathsTableWidget->item(insertPosition,2)->setTextAlignment(Qt::AlignCenter);
    ui->backupPathsTableWidget->setItem(insertPosition, 3, new QTableWidgetItem(srcPath));
    ui->backupPathsTableWidget->setItem(insertPosition, 4, new QTableWidgetItem(dstPath));
    ui->backupPathsTableWidget->item(insertPosition, 3)->setForeground(QDir(srcPath).exists() ? QColor(BACKUPPATH_ITEM_EXIST_TEXT_COLOR) : QColor(BACKUPPATH_ITEM_NOT_EXIST_TEXT_COLOR));
    ui->backupPathsTableWidget->item(insertPosition, 4)->setForeground(QDir(dstPath).exists() ? QColor(BACKUPPATH_ITEM_EXIST_TEXT_COLOR) : QColor(BACKUPPATH_ITEM_NOT_EXIST_TEXT_COLOR));
}

void MainUi::deleteSelectedPath()
{
    if(bakChbCheckedCount <= 0){
        return;
    }
    QString keyString;
    for(int tableRowPos = 0; tableRowPos < ui->backupPathsTableWidget->rowCount();){
        if(bakPathChbList[tableRowPos]->isChecked()){
            keyString = ui->backupPathsTableWidget->item(tableRowPos, 1)->text();
            delete bakPathChbList[tableRowPos];
            bakPathChbList.removeAt(tableRowPos);
            delete srcPathWatchModelMap.value(keyString);
            delete dstPathWatchModelMap.value(keyString);
            srcPathWatchModelMap.remove(keyString);
            dstPathWatchModelMap.remove(keyString);
            srcModelIndexList.remove(keyString);
            dstModelIndexList.remove(keyString);
            srcMapCountMap.remove(keyString);
            dstMapCountMap.remove(keyString);
            ui->backupPathsTableWidget->removeRow(tableRowPos);
            bakChbCheckedCount--;
            if(bakChbCheckedCount <= 0){
                break;
            }
        }
        else{
            tableRowPos++;
        }
    }
    bakChbCheckedCount = 0;
}

void MainUi::deletePathAtRow(int row)
{
    QString keyString = ui->backupPathsTableWidget->item(row, 3)->text() + ui->backupPathsTableWidget->item(row, 4)->text();
    delete bakPathChbList[row];
    bakPathChbList.removeAt(row);
    if(srcPathWatchModelMap.value(keyString) == srcPathWatchModel){
        srcPathWatchModel = nullptr;
    }
    delete srcPathWatchModelMap.value(keyString);
    if(dstPathWatchModelMap.value(keyString) == dstPathWatchModel){
        dstPathWatchModel = nullptr;
    }
    delete dstPathWatchModelMap.value(keyString);
    srcPathWatchModelMap.remove(keyString);
    dstPathWatchModelMap.remove(keyString);
    srcModelIndexList.remove(keyString);
    dstModelIndexList.remove(keyString);
    srcMapCountMap.remove(keyString);
    dstMapCountMap.remove(keyString);
    ui->backupPathsTableWidget->removeRow(row);
}

// 这是计算备份路径文件夹下的文件数目（不包括文件夹）的函数
// 用来传给processUI，能弄个进度条，显示文件数量。
void MainUi::getFileCount(QString path, quint64 &num)
{
    QDir dir(path);
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    foreach ( QFileInfo fileInfo, fileInfoList )
    {
        num++;
        QCoreApplication::processEvents();
        if ( fileInfo.isDir() )
        {
            num--;
            getFileCount(fileInfo.filePath(), num);
        }
    }
}

void MainUi::bakChbStateChanged(int state)
{
    if(state == 0){
        bakChbCheckedCount--;
    }
    else if(state == 2){
        bakChbCheckedCount++;
    }
    if(ui->backupPathsTableWidget->rowCount() == 0){
    // TODO: 全选
//        ui->allSelectButton->setText(tr("全选"));
        return;
    }
    // TODO: 全选 全不选
//    bakChbCheckedCount == bakPathChbList.length() ? ui->allSelectButton->setText(tr("全不选")) : ui->allSelectButton->setText(tr("全选"));

}

// TODO: 展示鼠标单击后选中的项的信息
// getSrcModelInfo是获取当前model信息（项目数等）
void MainUi::getSelectedItemInfo(WatchFileModel *model, QModelIndex modelIndex)
{
    if(model != nullptr){
        QString fileName = model->fileName(modelIndex);
        QString filePath = model->filePath(modelIndex);
        QString fileSuffix = model->fileInfo(modelIndex).suffix();
        qDebug() << "on clicked:" << fileName << filePath << fileSuffix;
    }
}

void MainUi::getModelInfoFromIndex(ModelMode mode, QModelIndex modelIndex)
{
    QString keyString=currentSrcPath + currentDstPath;
    WatchFileModel *infoWatchModel = dynamic_cast<WatchFileModel *>(const_cast<QAbstractItemModel *>(modelIndex.model()));
    if(infoWatchModel == nullptr){
        return;
    }
    if(mode == ModelMode::Src){
        srcModelIndexList[keyString].contains(modelIndex) ? srcModelIndexList[keyString].removeAt(srcModelIndexList[keyString].indexOf(modelIndex))
                                                          : srcModelIndexList[keyString].append(modelIndex);
        getSrcModelInfoFromString(infoWatchModel->rootPath());
    }
    else if(mode == ModelMode::Dst){
        dstModelIndexList[keyString].contains(modelIndex) ? dstModelIndexList[keyString].removeAt(dstModelIndexList[keyString].indexOf(modelIndex))
                                                          : dstModelIndexList[keyString].append(modelIndex);
        getDstModelInfoFromString(infoWatchModel->rootPath());
    }
}

void MainUi::getSrcModelInfoFromIndex(QModelIndex modelIndex)
{
    if(!modelIndex.isValid()){
        return;
    }
    getModelInfoFromIndex(ModelMode::Src, modelIndex);
}

void MainUi::getDstModelInfoFromIndex(QModelIndex modelIndex)
{
    if(!modelIndex.isValid()){
        return;
    }
    getModelInfoFromIndex(ModelMode::Dst, modelIndex);
}

void MainUi::getModelInfoFromString(ModelMode mode, QString path)
{
    if(path.isEmpty()){
        return;
    }
    QString keyString=currentSrcPath + currentDstPath;
    if(mode == ModelMode::Src){
        ui->srcDirTreeView->sortByColumn(0, Qt::AscendingOrder);
        int rowIndex = 0;
        QString displayName = "";
        QModelIndex srcItemDisplayNameIndex = ui->srcDirTreeView->indexAt(QPoint(0, rowIndex*21));
        while(srcItemDisplayNameIndex.isValid()){
            displayName = srcPathWatchModel->data(srcItemDisplayNameIndex, Qt::DisplayRole).toString();
            rowIndex++;
            srcItemDisplayNameIndex = ui->srcDirTreeView->indexAt(
                QPoint(0, rowIndex*21));
        }
        ui->srcDirHintLineEdit->setText("共" + QString::number(rowIndex) + "项");
        srcMapCountMap.insert(keyString, rowIndex);
    }
    else if(mode == ModelMode::Dst){
        ui->dstDirTreeView->sortByColumn(0, Qt::AscendingOrder);
        int rowIndex = 0;
        QString displayName = "";
        QModelIndex dstItemDisplayNameIndex = ui->dstDirTreeView->indexAt(QPoint(0, rowIndex*21));
        while(dstItemDisplayNameIndex.isValid()){
            displayName = dstPathWatchModel->data(dstItemDisplayNameIndex, Qt::DisplayRole).toString();
            rowIndex++;
            dstItemDisplayNameIndex = ui->dstDirTreeView->indexAt(
                QPoint(0, rowIndex*21));
        }
        ui->dstDirHintLineEdit->setText("共" + QString::number(rowIndex) + "项");
        dstMapCountMap.insert(keyString, rowIndex);
    }
}

void MainUi::getSrcModelInfoFromString(QString srcPath)
{
    getModelInfoFromString(ModelMode::Src, srcPath);
}

void MainUi::getDstModelInfoFromString(QString dstPath)
{
    getModelInfoFromString(ModelMode::Dst, dstPath);
}

void MainUi::updateBackupTime()
{
    int pos = 0;
    foreach(const QCheckBox *chb, bakPathChbList){
        if(chb->isChecked()){
            ui->backupPathsTableWidget->item(pos, 2)->setText(QDateTime::currentDateTime().toString(DATA_JSON_TIME_FORMAT));
        }
        pos++;
    }
}

void MainUi::backupPathTableContextMenu(const QPoint &pos)
{
    if(ui->backupPathsTableWidget->itemAt(pos) == nullptr){
        return;
    }
    int currentRow = ui->backupPathsTableWidget->selectedItems()[0]->row();
    QMenu *menu = new QMenu(ui->backupPathsTableWidget);
    connect(menu, &QMenu::aboutToHide, menu, &QMenu::deleteLater);
    QAction *act0 = new QAction("打开源文件夹", menu);
    QAction *act1 = new QAction("打开备份文件夹", menu);
    QAction *act2 = new QAction("删除", menu);
    connect(act0, &QAction::triggered, this, [this, currentRow](){QDesktopServices::openUrl(ui->backupPathsTableWidget->item(currentRow, 3)->text());});
    connect(act1, &QAction::triggered, this, [this, currentRow](){QDesktopServices::openUrl(ui->backupPathsTableWidget->item(currentRow, 4)->text());});
    connect(act2, &QAction::triggered, this, [this, currentRow](){this->deletePathAtRow(currentRow);});
    menu->addAction(act0);
    menu->addAction(act1);
    menu->addSeparator();
    menu->addAction(act2);
    menu->move(cursor().pos());
    menu->show();
}

void MainUi::on_backupPathsTableWidget_itemClicked(QTableWidgetItem *item)
{
    if(item == nullptr){
        return;
    }
    bool isSrcExists = false, isDesExists = false;
    currentSrcPath = ui->backupPathsTableWidget->item(ui->backupPathsTableWidget->row(item), 3)->text();
    if(QDir(currentSrcPath).exists()){
        isSrcExists = true;
    }
    currentDstPath = ui->backupPathsTableWidget->item(ui->backupPathsTableWidget->row(item), 4)->text();
    if(QDir(currentDstPath).exists()){
        isDesExists = true;
    }
    QString keyString = currentSrcPath + currentDstPath;
    if(srcPathWatchModelMap.contains(keyString)){
        if(srcPathWatchModel != nullptr){
            disconnect(srcPathWatchModel, nullptr, this, nullptr);
        }
        if(isSrcExists){
            ui->backupPathsTableWidget->item(ui->backupPathsTableWidget->row(item), 3)->setForeground(QColor(BACKUPPATH_ITEM_EXIST_TEXT_COLOR));
            srcPathWatchModel = srcPathWatchModelMap[keyString];
            srcPathWatchModel->setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
            ui->srcDirTreeView->setModel(srcPathWatchModel);
            ui->srcDirTreeView->setRootIndex(srcPathWatchModel->index(srcPathWatchModel->rootPath()));
            // 展开各项前需要先屏蔽treeview的信号，否则会错误清空expSrcMap。
            ui->srcDirTreeView->blockSignals(true);
            if(srcModelIndexList.contains(keyString)){
                QModelIndexList list = srcModelIndexList[keyString];
                foreach(QModelIndex index, list){
                    ui->srcDirTreeView->expand(index);
                }
            }
            ui->srcDirTreeView->blockSignals(false);
            if(srcMapCountMap.contains(keyString)){
                ui->srcDirHintLineEdit->setText("共" + QString::number(srcMapCountMap[keyString]) + "项");
            }
            ui->backupPathsTableWidget->item(ui->backupPathsTableWidget->row(item), 3)->setForeground(QColor(BACKUPPATH_ITEM_EXIST_TEXT_COLOR));
        }
        else{
            ui->srcDirHintLineEdit->setText("路径不存在");
            ui->backupPathsTableWidget->item(ui->backupPathsTableWidget->row(item), 3)->setForeground(QColor(BACKUPPATH_ITEM_NOT_EXIST_TEXT_COLOR));
        }
    }
    else{
        if(srcPathWatchModel != nullptr){
            disconnect(srcPathWatchModel, nullptr, this, nullptr);
        }
        if(isSrcExists){
            srcPathWatchModel = new WatchFileModel;
            srcPathWatchModel->setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
            connect(srcPathWatchModel, &WatchFileModel::directoryLoaded,this, &MainUi::getSrcModelInfoFromString, Qt::UniqueConnection);
            ui->srcDirTreeView->setModel(srcPathWatchModel);
            srcPathWatchModel->setRootPath(currentSrcPath);
            ui->srcDirTreeView->setRootIndex(srcPathWatchModel->index(currentSrcPath));
            srcPathWatchModelMap.insert(keyString, srcPathWatchModel);
            ui->backupPathsTableWidget->item(ui->backupPathsTableWidget->row(item), 3)->setForeground(QColor(BACKUPPATH_ITEM_EXIST_TEXT_COLOR));
        }
        else{
            ui->srcDirHintLineEdit->setText("路径不存在");
            ui->backupPathsTableWidget->item(ui->backupPathsTableWidget->row(item), 3)->setForeground(QColor(BACKUPPATH_ITEM_NOT_EXIST_TEXT_COLOR));
        }
    }

    if(dstPathWatchModelMap.contains(keyString)){
        if(dstPathWatchModel != nullptr){
            disconnect(dstPathWatchModel, nullptr, this, nullptr);
        }
        if(isDesExists){
            ui->backupPathsTableWidget->item(ui->backupPathsTableWidget->row(item), 4)->setForeground(QColor(BACKUPPATH_ITEM_EXIST_TEXT_COLOR));
            dstPathWatchModel = dstPathWatchModelMap[keyString];
            dstPathWatchModel->setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
            ui->dstDirTreeView->setModel(dstPathWatchModel);
            ui->dstDirTreeView->setRootIndex(dstPathWatchModel->index(dstPathWatchModel->rootPath()));
            ui->dstDirTreeView->blockSignals(true);
            if(dstModelIndexList.contains(keyString)){
                QModelIndexList list = dstModelIndexList[keyString];
                foreach(QModelIndex index, list){
                    ui->dstDirTreeView->expand(index);
                }
            }
            ui->dstDirTreeView->blockSignals(false);
            if(dstMapCountMap.contains(keyString)){
                ui->dstDirHintLineEdit->setText("共" + QString::number(dstMapCountMap[keyString]) + "项");
            }
            ui->backupPathsTableWidget->item(ui->backupPathsTableWidget->row(item), 4)->setForeground(QColor(BACKUPPATH_ITEM_EXIST_TEXT_COLOR));
        }
        else{
            ui->dstDirHintLineEdit->setText("路径不存在");
            ui->backupPathsTableWidget->item(ui->backupPathsTableWidget->row(item), 4)->setForeground(QColor(BACKUPPATH_ITEM_NOT_EXIST_TEXT_COLOR));
        }
    }
    else{
        if(dstPathWatchModel != nullptr){
            disconnect(dstPathWatchModel, nullptr, this, nullptr);
        }
        if(isDesExists){
            dstPathWatchModel = new WatchFileModel;
            dstPathWatchModel->setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
            connect(dstPathWatchModel, &WatchFileModel::directoryLoaded,this, &MainUi::getDstModelInfoFromString, Qt::UniqueConnection);
            ui->dstDirTreeView->setModel(dstPathWatchModel);
            dstPathWatchModel->setRootPath(currentDstPath);
            ui->dstDirTreeView->setRootIndex(dstPathWatchModel->index(currentDstPath));
            dstPathWatchModelMap.insert(keyString, dstPathWatchModel);
            ui->backupPathsTableWidget->item(ui->backupPathsTableWidget->row(item), 4)->setForeground(QColor(BACKUPPATH_ITEM_EXIST_TEXT_COLOR));
        }
        else{
            ui->dstDirHintLineEdit->setText("路径不存在");
            ui->backupPathsTableWidget->item(ui->backupPathsTableWidget->row(item), 4)->setForeground(QColor(BACKUPPATH_ITEM_NOT_EXIST_TEXT_COLOR));
        }
    }
}


void MainUi::on_allSelectButton_clicked()
{
    foreach(QCheckBox *chb, bakPathChbList){
        if(!chb->isChecked()){
            bakChbCheckedCount++;
        }
        chb->setChecked(true);
    }
}


void MainUi::on_reverseSelectButton_clicked()
{
    bool oldState;
    foreach(QCheckBox *chb, bakPathChbList){
        oldState = chb->isChecked();
        oldState == true ? bakChbCheckedCount-- : bakChbCheckedCount++;
        chb->setChecked(!oldState);
    }
}


void MainUi::on_deletePathsTableButton_clicked()
{
    deleteSelectedPath();
}

void MainUi::on_startBackupButton_clicked()
{
    QString srcPath, dstPath;
    quint64 fileCount = 0;
    CopyTaskVector copyTaskVector;
    CopyProgressWindow *copyWindow = new CopyProgressWindow();
    // parent not set, use Qt::WA_DeleteOnClose to recycle itself after close()
    copyWindow->setAttribute(Qt::WA_DeleteOnClose);

    for(int pathPos = 0; pathPos < ui->backupPathsTableWidget->rowCount(); pathPos++){
        if(bakPathChbList[pathPos]->isChecked()){
            srcPath = ui->backupPathsTableWidget->item(pathPos, 3)->text();
            if(copyContentType == CopyContentType::Content){
                dstPath = ui->backupPathsTableWidget->item(pathPos, 4)->text();
                if(!(QDir(srcPath)).exists() || !(QDir(dstPath)).exists()){
                    continue;
                }
            }
            else if(copyContentType == CopyContentType::Folder){
                dstPath = QDir::toNativeSeparators(ui->backupPathsTableWidget->item(pathPos, 4)->text()
                            + "/" + QFileInfo(ui->backupPathsTableWidget->item(pathPos, 3)->text()).fileName());
                if(!(QDir(srcPath)).exists() || !(QFileInfo(dstPath).absoluteDir().exists())){
                    continue;
                }
                QDir(dstPath).mkdir(dstPath);
            }
            else{
                MessageBoxExY::critical("备份失败", "拷贝类型错误");
            }

            getFileCount(srcPath, fileCount);
            copyTaskVector.append(CopyTask(srcPath, dstPath));
        }
    }
    if(copyTaskVector.length() <= 0){
        delete copyWindow;
        return;
    }
    copyWindow->setFileCountTotal(fileCount);
    copyWindow->show();
    QThread *copyThread = new QThread();
    CopyWorker *copyWorker = new CopyWorker();
    copyWorker->moveToThread(copyThread);
    copyWorker->setTask(copyTaskVector);
    copyWorker->setReplaceFile(replaceFile);
    copyWorker->setCheckFileSum(checkFileSum);
    copyWorker->setResetDir(resetDir);
    // lifetime management
    connect(copyThread, &QThread::started, copyWorker, &CopyWorker::copyStart);
    connect(copyWorker, &CopyWorker::copyFinished, copyThread, &QThread::quit);
    connect(copyThread, &QThread::finished, copyWorker, &CopyWorker::deleteLater);
    connect(copyThread, &QThread::finished, copyThread, &QThread::deleteLater);
    connect(copyWorker, &CopyWorker::copyTerminated, copyThread, &QThread::quit);

    // send messages
    connect(copyWorker, &CopyWorker::copyFileResult, copyWindow, &CopyProgressWindow::parseCopyResult);
    connect(copyWorker, &CopyWorker::copyFinished, this, &MainUi::updateBackupTime);
    connect(copyWorker, &CopyWorker::copyTerminated, copyWindow, &CopyProgressWindow::copyResultTerminated);
    connect(copyWorker, &CopyWorker::copyCurrentFile, copyWindow, &CopyProgressWindow::setCurrentFilePath, Qt::BlockingQueuedConnection);
    connect(copyWorker, &CopyWorker::copyFinished, copyWindow, &CopyProgressWindow::copyFinished);

    // quit
    QMetaObject::Connection c = connect(copyWindow, &CopyProgressWindow::terminateCopyWork, copyWorker, &CopyWorker::copyTerminate, Qt::DirectConnection);
    copyWorker->moveToThread(copyThread);
    copyThread->start();
}


void MainUi::on_replaceFileCheckBox_stateChanged(int state)
{
    state != 0 ? replaceFile = true : replaceFile = false;
    saveConfig();
}

void MainUi::on_checkSumCheckBox_clicked()
{
    if(ui->checkSumCheckBox->isChecked()){
        if(MessageBoxExY::Yes == MessageBoxExY::warning("校验文件", "将会在备份之后校验文件哈希值，额外耗费更多磁盘读写及更多时间，是否确定？")){
            checkFileSum = ui->checkSumCheckBox->isChecked();
            saveConfig();
            return;
        }
        ui->checkSumCheckBox->setChecked(!ui->checkSumCheckBox->isChecked());
    }
    checkFileSum = ui->checkSumCheckBox->isChecked();
    saveConfig();
}

void MainUi::on_resetDirCheckBox_clicked()
{
    if(ui->resetDirCheckBox->isChecked()){
        if(MessageBoxExY::Yes == MessageBoxExY::warning("清空文件夹", "将会在备份之前先清空备份文件夹，是否确定？")){
            resetDir = ui->resetDirCheckBox->isChecked();
            saveConfig();
            return;
        }
        ui->resetDirCheckBox->setChecked(!ui->resetDirCheckBox->isChecked());
    }
    resetDir = ui->resetDirCheckBox->isChecked();
    saveConfig();
}

void MainUi::on_openPathTableJsonButton_clicked()
{
    InputBakPathWindow *w = new InputBakPathWindow();
    w->setAttribute(Qt::WA_DeleteOnClose);
    connect(w, &InputBakPathWindow::sendNewPath, this, &MainUi::insertBackupPath);
    w->show();
}


void MainUi::on_savePathTableButton_clicked()
{
//    saveConfig();
    saveData();
}


void MainUi::on_cpContentRadioButton_clicked()
{
    copyContentType = CopyContentType::Content;
    saveConfig();
}


void MainUi::on_cpDirRadioButton_clicked()
{
    copyContentType = CopyContentType::Folder;
    saveConfig();
}


void MainUi::on_refreshPathTableButton_clicked()
{
    clearData();
    loadData();
}


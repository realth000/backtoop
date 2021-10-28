#include "mainui.h"
#include "ui_mainui.h"
#include "commoninclude.h"
#include "qssinstaller.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDir>
#include <QDebug>
#include <QScrollBar>

MainUi::MainUi(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainUi)
{
    ui->setupUi(this);
    initUi();
    initConfig();
}

MainUi::~MainUi()
{
    delete ui;

    // delete style
    delete checkBoxStyle;
    delete vScrollBarStyle;
    delete hScrollBarStyle;

    //delete view
    delete dirViewHeaderModel;

    // delete model
    qDeleteAll(srcPathWatchModelMap);
    qDeleteAll(dstPathWatchModelMap);
}

void MainUi::log(QString msg) const
{
    ui->logLineEdit->setText(msg);
}


void MainUi::initUi()
{
    // init styles
    checkBoxStyle = new CheckBoxStyle;
    vScrollBarStyle = new VerticalScrollBarStyle;
    hScrollBarStyle = new HorizontalScrollBarStyle;

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
    ui->titleBar->initUi(TitleBar::NoMaxButton, "rgb(240,255,255)", "rgb(93,94,95)",
                         "qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(18,18,18), stop: 1 rgb(21,21,21))", "rgb(240,255,255)");
    ui->titleBar->setTitleIcon(TITLEBAR_TITLEICON);

    // Paths table widget
    QStringList horizontalHeaders;
    horizontalHeaders << "选择" << "名称"<< "上次修改时间" << "源路径" << "备份路径";
    ui->backupPathsTableWidget->setColumnCount(horizontalHeaders.length());
    ui->backupPathsTableWidget->setColumnWidth(0, 40);
    ui->backupPathsTableWidget->setColumnWidth(1, 220);
    ui->backupPathsTableWidget->setColumnWidth(2, 130);
    ui->backupPathsTableWidget->setColumnWidth(3, 320);
    ui->backupPathsTableWidget->setColumnWidth(4, 10);
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
    ui->srcDirTreeView->setColumnWidth(0, 270);
    ui->srcDirTreeView->setColumnWidth(1, 70);
    ui->srcDirTreeView->setColumnWidth(2, 60);
    ui->srcDirTreeView->setColumnWidth(3, 110);
    ui->dstDirTreeView->header()->setModel(dirViewHeaderModel);
    ui->dstDirTreeView->setColumnWidth(0, 270);
    ui->dstDirTreeView->setColumnWidth(1, 70);
    ui->dstDirTreeView->setColumnWidth(2, 60);
    ui->dstDirTreeView->setColumnWidth(3, 110);
    ui->srcDirTreeView->setWordWrap(false);
    ui->dstDirTreeView->setWordWrap(false);

//    connect(ui->srcDirTreeView, &QTreeView::clicked,   this, &MainUi::getSrcModelInfo, Qt::UniqueConnection);
    connect(ui->srcDirTreeView, &QTreeView::collapsed, this, &MainUi::getSrcModelInfoFromIndex, Qt::UniqueConnection);
    connect(ui->srcDirTreeView, &QTreeView::expanded,  this, &MainUi::getSrcModelInfoFromIndex, Qt::UniqueConnection);
//    connect(ui->dstDirTreeView, &QTreeView::clicked,   this, &MainUi::getDstModelInfo, Qt::UniqueConnection);
    connect(ui->dstDirTreeView, &QTreeView::collapsed, this, &MainUi::getDstModelInfoFromIndex, Qt::UniqueConnection);
    connect(ui->dstDirTreeView, &QTreeView::expanded,  this, &MainUi::getDstModelInfoFromIndex, Qt::UniqueConnection);



    log("启动");
}

void MainUi::initConfig()
{
    addBackupPath("test", "test Time", "D:/Storage", "C:/QtProjects_build");
    addBackupPath("test", "test Time", "D:/Programming", "D:/Tools");
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
    if(mode == ModelMode::SRC){
        srcModelIndexList[keyString].contains(modelIndex) ? srcModelIndexList[keyString].removeAt(srcModelIndexList[keyString].indexOf(modelIndex))
                                                          : srcModelIndexList[keyString].append(modelIndex);
        getSrcModelInfoFromString(infoWatchModel->rootPath());
    }
    else if(mode == ModelMode::DST){
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
    getModelInfoFromIndex(ModelMode::SRC, modelIndex);
}

void MainUi::getDstModelInfoFromIndex(QModelIndex modelIndex)
{
    if(!modelIndex.isValid()){
        return;
    }
    getModelInfoFromIndex(ModelMode::DST, modelIndex);
}

void MainUi::getModelInfoFromString(ModelMode mode, QString path)
{
    if(path.isEmpty()){
        return;
    }
    QString keyString=currentSrcPath + currentDstPath;
    if(mode == ModelMode::SRC){
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
    else if(mode == ModelMode::DST){
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
    getModelInfoFromString(ModelMode::SRC, srcPath);
}

void MainUi::getDstModelInfoFromString(QString dstPath)
{
    getModelInfoFromString(ModelMode::DST, dstPath);
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
            ui->backupPathsTableWidget->item(ui->backupPathsTableWidget->row(item), 3)->setForeground(Qt::black);
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
            ui->backupPathsTableWidget->item(ui->backupPathsTableWidget->row(item), 3)->setForeground(Qt::black);
        }
        else{
            ui->srcDirHintLineEdit->setText("路径不存在");
            ui->backupPathsTableWidget->item(ui->backupPathsTableWidget->row(item), 3)->setForeground(Qt::red);
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
                qDebug() << "get expand" << list.length();
                foreach(QModelIndex index, list){
                    qDebug() << "exped";
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


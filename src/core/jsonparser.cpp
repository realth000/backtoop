/*
 * data json example:
 *
{
    "backuppath_json_engine_version": "1.0",
    "data": {
        "0": {
            "destination_path": "D:/Storage/3",
            "last_modify_time": "test Time",
            "name": "test",
            "source_path": "C:/QtProjects/0/test"
        },
        "1": {
            "destination_path": "C:/QtProjects_build/backtoop/Desktop_Qt_5_15_2_MSVC2019_64bit-Debug/release",
            "last_modify_time": "test Time",
            "name": "te2",
            "source_path": "C:/QtProjects_build/backtoop/Desktop_Qt_5_15_2_MSVC2019_64bit-Debug/debug"
        }
    },
    "data_count": "2",
    "time": "2021-11-03 08:41:13"
}
 *
 * data json format example:
 *
{
    DATA_JSON_ENGINEVERSION_NAME: BACKUPPATH_JSON_ENGINE_VERSION,
    "data": {
        "0": {
            DATA_JSON_DSTPATH_TIME: data.dstPath
            DATA_JSON_LASTMODTIME_NAME: data.lastModifyTime,
            DATA_JSON_NAME_NAME: data.name,
            DATA_JSON_SRCPATH_TIME: data.srcPath,

        },
        "1": {
            DATA_JSON_DSTPATH_TIME: data.dstPath
            DATA_JSON_LASTMODTIME_NAME: data.lastModifyTime,
            DATA_JSON_NAME_NAME: data.name,
            DATA_JSON_SRCPATH_TIME: data.srcPath,
        }
    },
    DATA_JSON_DATACOUNT_NAME: allData.length(),
    DATA_JSON_TIME_NAME: "yyyy-MM-dd HH:mm:ss"
}
 *
*/
#include "jsonparser.h"
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>

JsonParser::JsonParser()
{

}

bool JsonParser::saveBackupPathDataToFile(QString filePath, BackupPathDatas allData)
{
    if(allData.length() <= 0){
        return false;
    }
    QFile dataFile(filePath);
    if(!dataFile.open(QIODevice::WriteOnly)){
        return false;
    }

    QJsonDocument jsonDoc;
    QJsonObject jsonObj;
    QJsonObject jsonObjData;
    int pos = 0;
    jsonObj.insert(DATA_JSON_TIME_NAME, QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    jsonObj.insert(DATA_JSON_DATACOUNT_NAME, QString::number(allData.length()));
    jsonObj.insert(DATA_JSON_ENGINEVERSION_NAME, BACKUPPATH_JSON_ENGINE_VERSION);
    foreach(BackupPathData data, allData){
        QJsonObject dataObj;
        dataObj.insert(DATA_JSON_NAME_NAME, data.name);
        dataObj.insert(DATA_JSON_LASTMODTIME_NAME, data.lastModifyTime);
        dataObj.insert(DATA_JSON_SRCPATH_TIME, data.srcPath);
        dataObj.insert(DATA_JSON_DSTPATH_TIME, data.dstPath);
        jsonObjData.insert(QString::number(pos), dataObj);
        pos++;
    }
    jsonObj.insert("data", jsonObjData);
    jsonDoc.setObject(jsonObj);
    dataFile.write(jsonDoc.toJson());
    dataFile.close();
    return true;
}

BackupPathDatas JsonParser::loadBackupPathDataFromFile(QString filePath)
{
    BackupPathDatas allData;
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;
    QJsonObject jsonObjData;
    QJsonParseError err;
    QFile dataFile(filePath);
    if(!dataFile.exists()){
        return BackupPathDatas();
    }
    if(!dataFile.open(QIODevice::ReadOnly)){
        return BackupPathDatas();
    }

    QString readedData = dataFile.readAll();
    jsonDoc = QJsonDocument::fromJson(readedData.toUtf8().data(), &err);
    if(jsonDoc.isNull()){
        qDebug() << "json parse error:" << err.errorString();
        return BackupPathDatas();
    }
    jsonObj = jsonDoc.object();
    jsonObjData = jsonObj.value("data").toObject();
    QStringList jsonObjDataKeys = jsonObjData.keys();
    foreach(QString dataKey, jsonObjDataKeys){
        QJsonObject dataObj = jsonObjData.value(dataKey).toObject();
        BackupPathData data;
        data.id = dataKey;
        data.name = dataObj.value(DATA_JSON_NAME_NAME).toString();
        data.lastModifyTime = dataObj.value(DATA_JSON_LASTMODTIME_NAME).toString();
        data.srcPath = dataObj.value(DATA_JSON_SRCPATH_TIME).toString();
        data.dstPath = dataObj.value(DATA_JSON_DSTPATH_TIME).toString();
        allData.append(data);
    }
    dataFile.close();
    return allData;
}

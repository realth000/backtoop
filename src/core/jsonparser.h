#ifndef JSONPARSER_H
#define JSONPARSER_H

#include "commoninclude.h"

class JsonParser
{
public:
    JsonParser();
    static bool saveBackupPathDataToFile(QString filePath, BackupPathDatas allData);
    static BackupPathDatas loadBackupPathDataFromFile(QString filePath);
};

#endif // JSONPARSER_H

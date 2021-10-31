#ifndef COPYPROGRESSWINDOW_H
#define COPYPROGRESSWINDOW_H

#include <QDialog>
#include "proxystyle.h"
#include "commoninclude.h"

namespace Ui {
class CopyProgressWindow;
}

class CopyProgressWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CopyProgressWindow(QWidget *parent = nullptr);
    ~CopyProgressWindow();
    void setFileCountTotal(quint64 count);

public slots:
    void parseCopyResult(QString filePath, CopyResult result);

private:
    Ui::CopyProgressWindow *ui;
    PushButtonStyle *pushButtonStyle = nullptr;
    quint64 fileCountCopied = 0;
    quint64 fileCountTotal;

    void initUi();
    void log(QString msg);
    void updateFileCount(quint64 alreadyCopiedCount);
    void updateSuccessCount(QString filePath = "");
    void updateFailedCount(QString filePath = "");
};

#endif // COPYPROGRESSWINDOW_H

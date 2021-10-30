#ifndef COPYPROGRESSWINDOW_H
#define COPYPROGRESSWINDOW_H

#include <QDialog>
#include "proxystyle.h"

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
    void updateSuccessCount(QString filePath = "");
    void updateFailedCount(QString filePath = "");

private:
    Ui::CopyProgressWindow *ui;
    PushButtonStyle *pushButtonStyle = nullptr;
    quint64 fileCountCopied = 0;
    quint64 fileCountTotal;

    void initUi();
    void updateFileCount(quint64 alreadyCopiedCount);
};

#endif // COPYPROGRESSWINDOW_H

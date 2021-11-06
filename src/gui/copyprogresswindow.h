#ifndef COPYPROGRESSWINDOW_H
#define COPYPROGRESSWINDOW_H

#include <QDialog>
#include <QCloseEvent>
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

signals:
    void terminateCopyWork();

public slots:
    void parseCopyResult(QString filePath, CopyResult result);
    void copyResultTerminated();

protected:
    virtual void closeEvent(QCloseEvent *e) override;

private slots:
    void on_copyStopPushButton_clicked();

private:
    Ui::CopyProgressWindow *ui;
    PushButtonStyle *pushButtonStyle = nullptr;
    HorizontalScrollBarStyle *hScrollBarStyle = nullptr;
    VerticalScrollBarStyle *vScrollBarStyle = nullptr;
    quint64 fileCountCopied = 0;
    quint64 fileCountTotal;

    void initUi();
    void log(QString msg);
    void updateFileCount(quint64 alreadyCopiedCount);
    void updateSuccessCount(QString filePath = "");
    void updateFailedCount(QString filePath = "");
};

#endif // COPYPROGRESSWINDOW_H

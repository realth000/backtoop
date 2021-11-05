#ifndef TREEVIEWEX_H
#define TREEVIEWEX_H

#include <QTreeView>

class TreeViewEx : public QTreeView
{
    Q_OBJECT
public:
    TreeViewEx(QWidget *widget = nullptr);

protected:
    virtual void drawBranches(QPainter *painter, const QRect &rect, const QModelIndex &index) const override;
};

#endif // TREEVIEWEX_H

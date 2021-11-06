#include "treeviewex.h"
#include <QPainter>
#include <QDebug>

TreeViewEx::TreeViewEx(QWidget *widget):
    QTreeView(widget)
{

}

void TreeViewEx::drawBranches(QPainter *painter, const QRect &rect, const QModelIndex &index) const
{
    QTreeView::drawBranches(painter, rect, index);
}

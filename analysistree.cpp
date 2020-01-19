#include "analysistree.h"
#include "dtree.h"

#include <QPainter>

analysisTree::analysisTree(DTree* t, QWidget *parent) : QWidget(parent), tree(t)
{
    setMinimumSize(700, 900);
}

void analysisTree::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //painter.drawLine(QPoint(0, 0), QPoint(100, 100));

    //QPen pen(Qt::green, 5, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin);
    tree->traverDTree(tree->returnRoot(), 0, &painter, frameGeometry().width(), 0, 0);

}

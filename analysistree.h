#ifndef ANALYSISTREE_H
#define ANALYSISTREE_H

#include <QWidget>

class DTree;

class analysisTree : public QWidget
{
    Q_OBJECT
public:
    explicit analysisTree(DTree* t, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event);

private:
    DTree* tree;
signals:

};

#endif // ANALYSISTREE_H

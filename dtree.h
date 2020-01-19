#ifndef DTREE_H
#define DTREE_H

#include "struct.h"
#include "dtnode.h"

class DTree
{// 决策树
private:
    DTNode* root;    // 保存决策树的根结点
public:
    DTree(DTNode* node) : root(node){}
    ~DTree(){ destroyDTree(root);}
    void createDTree(DTNode* &node, vector<vector<cell> > chart, vector<string> label, int dataSum);   // 创建决策树
    vector<vector<cell> > getNewChart(DTNode *node, vector<vector<cell>> chart, int i); // 获取新表格
    void destroyDTree(DTNode* &node);                               // 销毁决策树
    void traverDTree(DTNode* node, int i);                          // 遍历决策树
    DTNode* returnRoot(){ return root;}                             // 返回决策树根结点
};

#endif // DTREE_H

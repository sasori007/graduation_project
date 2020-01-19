#ifndef DTNODE_H
#define DTNODE_H

#include "struct.h"

extern const double judge;      // 判断标准，当前某类别占比大于该值时，可认为已纯
extern const double proportion; // 最小数据比例，当前结点拥有的样本数小于总数据占比时，不再分裂

class DTNode
{// 决策树结点
    friend class DTree;
protected:
    string value;       // 当前结点值
    int childNum;       // 孩子数量，为0时表示叶子结点
    string* childValue; // 孩子结点值
    DTNode** child;     // 孩子指针数组

public:
    DTNode(){}
    DTNode(string v, int cN, string* cV) : value(v), childNum(cN)
    {
        if(cN > 0)
        {
            childValue = new string[cN];
            child = new DTNode *[cN];
            for (int i = 0; i < cN; ++i)
            {
                childValue[i] = "";
                child[i] = nullptr;
            }
        }
    }
    ~DTNode()
    {
        cout << "释放结点" << value << endl;
    }
    feature c4_5(vector<vector<cell> > chart, vector<string> label, int dataSum);      // 获取选择分裂的属性下标
    feature c4_5HashAttr(vector<vector<cell> > chart, vector<string> label, double info_D, double &info_A, double &H, double &Gain, double &IGR, int i);  // 离散属性计算信息增益
    feature c4_5ConAttr(vector<vector<cell> > chart, vector<string> label, double info_D, double &info_A, double &H, double &Gain, double &IGR, int i);   // 连续属性计算信息增益
    void createNode(vector<vector<cell> > chart, vector<string> label, int dataSum);   // 创建树结点
};

#endif // DTNODE_H

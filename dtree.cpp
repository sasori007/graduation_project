#include "dtree.h"

void DTree::createDTree(DTNode *&node, vector<vector<cell> > chart, vector<string> label, int dataSum)
{                                               // 输入表格，输出决策树；递归创建决策树
    node->createNode(chart, label, dataSum);    // 创建根结点node

    for (int i = 0; i < node->childNum; ++i)
    {
        // 获取新表格
        vector<vector<cell> > newChart = getNewChart(node, chart, i);
        // 创建子树
        createDTree(node->child[i], newChart, label, dataSum);
    }
    return;
}

vector<vector<cell> > DTree::getNewChart(DTNode *node, vector<vector<cell> > chart, int i)
{
    vector<vector<cell>> newChart; // 保存新表格
    int subscript;                 // 保存当前结点的分裂特征在表格中的下标
    int attrType = -1;             // 保存当前结点的属性类型，-1为离散型，-2为连续型
    string num = "";               // 保存连续属性的分裂点，可能不存在

    vector<cell> name;
    for (int j = 0; j < chart[0].size(); ++j)
    {// 获取属性的标题
        if (chart[0][j].value == node->value)
        {
            subscript = j;
            attrType = chart[0][j].flag;
        }
        else
            name.push_back(chart[0][j]);
    }
    newChart.push_back(name);

    if (attrType == -2)
    {// 当前分裂特征为连续型属性时，进行特殊处理
        int j = 0;
        for (; j < node->childValue[i].size(); ++j)
            if (node->childValue[i][j] >= '0' && node->childValue[i][j] <= '9')
                break;
        for (; j < node->childValue[i].size(); ++j)
            num += node->childValue[i][j];
    }

    for (int j = 1; j < chart.size(); ++j)
    { // 获取新表格
        vector<cell> aData;
        if (chart[j][subscript].value != node->childValue[i] && attrType == -1)
            // 离散型数据处理
            continue;
        if(attrType == -2)
        {// 连续型数据处理
            if(chart[j][subscript].value > num && i == 0)
                continue;
            if(chart[j][subscript].value < num && i == 1)
                continue;
        }
        for (int k = 0; k < chart[0].size(); ++k)
        {
            if (k == subscript)
                continue;
            cell c;
            c = chart[j][k];
            aData.push_back(c);
        }
        newChart.push_back(aData);
    }
    return newChart;
}

void DTree::destroyDTree(DTNode *&node)
{
    for (int i = 0; i < node->childNum; ++i)
        destroyDTree(node->child[i]);
    delete node;
    return;
}

void DTree::traverDTree(DTNode *node, int num)
{
    cout << node->value << "(";
    for (int i = 0; i < node->childNum; ++i)
        cout << node->childValue[i] << " ";
    cout << ")" << endl;
    ++num;
    for (int i = 0; i < node->childNum; ++i)
    {
        for (int j = 0; j < num; ++j)
            cout << "  ";
        cout << node->childValue[i] << "-->";
        traverDTree(node->child[i], num);
    }
    return;
}


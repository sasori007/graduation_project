#include "dtnode.h"
#include "struct.h"

const double judge = 0.95;
const double proportion = 0.1;

feature DTNode::c4_5(vector<vector<cell> > chart, vector<string> label, int dataSum)
{
    double info_D = 0.0;
    double info_A[chart[0].size()];
    double Gain[chart[0].size()];
    double H[chart[0].size()];
    double IGR[chart[0].size()];
    feature result;
    result.subscript = -1;
    result.value = new string[1];

    // 计算info_D
    int lNum[label.size()]; // 保存类别标签各取值的数量
    int maxI = 0;
    for (int i = 0; i < label.size(); ++i) // 初始化为0
        lNum[i] = 0;
    for (int i = 1; i < chart.size(); ++i) // 根据表格所给数据统计各类别数量
        ++lNum[chart[i][0].flag];
    for (int i = 0; i < label.size(); ++i)
    {
        double d = (lNum[i] * 1.0) / (chart.size() - 1);
        if (d >= judge)
        { // 判断当前数据是否已纯
            result.subscript = 0 - (i + 1);
            return result;
        }
        else if (d == 0)
            // 当前标签不存在于当前数据表格中
            continue;
        if (lNum[i] > lNum[maxI])
            maxI = i;
        info_D -= d * log2(d);
    }

    // 当前样本数过少，不再分裂
    if (chart.size() < dataSum * proportion)
    {
        result.subscript = 0 - (maxI + 1);
        return result;
    }

    for (int i = 0; i < chart[0].size(); ++i)
    { // 当前表格需要分裂，初始化各个值
        info_A[i] = 0.0;
        Gain[i] = 0.0;
        H[i] = 0.0;
        IGR[i] = 0.0;
    }

    // 计算infoA、H、Gain、IGR
    for (int i = 0; i < chart[0].size(); ++i)
    {
        feature f;

        // 根据属性类型进行不同的信息增益计算
        if(chart[0][i].flag == -1)
            f = c4_5HashAttr(chart, label, info_D, info_A[i], H[i],Gain[i], IGR[i], i);
        else
            f = c4_5ConAttr(chart, label, info_D, info_A[i], H[i], Gain[i], IGR[i], i);

        // 获取IGR中的最大值
        if (result.subscript == -1)
        {
            delete[] result.value;
            result = f;
        }
        else if (IGR[i] > IGR[result.subscript])
        {
            delete[] result.value;
            result = f;
        }
    }
    return result;
}

feature DTNode::c4_5HashAttr(vector<vector<cell> > chart, vector<string> label, double info_D, double &info_A, double &H, double &Gain, double &IGR, int i)
{
    vector<string> nowValue;// 保存每个特征拥有的值
    vector<int> nowValueNum;// 保存每个值存在的数量
    feature result;         // 保存结果
    vector<string>::iterator iter;
    string s = "";
    for (int j = 1; j < chart.size(); ++j)
    { // 获取每个特征拥有的值的数量和每个值存在的数量
        s = chart[j][i].value;
        iter = find(nowValue.begin(), nowValue.end(), s);
        if (iter == nowValue.end())
        {
            nowValue.push_back(s);
            nowValueNum.push_back(1);
        }
        else
            ++nowValueNum[iter - nowValue.begin()];
    }

    for (int j = 0; j < nowValueNum.size(); ++j)
    { // 计算H
        double d = (nowValueNum[j] * 1.0) / (chart.size() - 1);
        H -= d * log2(d);
    }

    // 计算info_A
    int nowValueFlagNum[nowValue.size()][label.size()]; // 保存每个值对应flag的数量
    for (int j = 0; j < nowValue.size(); ++j)
        for (int k = 0; k < label.size(); ++k)
            // 初始化
            nowValueFlagNum[j][k] = 0;
    for (int j = 1; j < chart.size(); ++j)
    { // 遍历表格，查找flag值的数量
        iter = find(nowValue.begin(), nowValue.end(), chart[j][i].value);
        ++nowValueFlagNum[iter - nowValue.begin()][chart[j][i].flag];
    }
    for (int j = 0; j < nowValue.size(); ++j)
    { // 利用统计到的信息计算ino_A
        double d = 0;
        double x = (nowValueNum[j] * 1.0) / (chart.size() - 1);
        for (int k = 0; k < label.size(); ++k)
        {
            double y = (nowValueFlagNum[j][k] * 1.0) / nowValueNum[j];
            if (y != 0)
                d -= y * log2(y);
        }
        info_A += x * d;
    }

    // 计算Gain
    Gain = info_D - info_A;

    // 计算IGR
    IGR = Gain / H;

    result.subscript = i;
    result.valueNum = nowValue.size();
    result.value = new string[result.valueNum];
    for (int j = 0; j < nowValue.size(); ++j)
        result.value[j] = nowValue[j];

    return result;
}

feature DTNode::c4_5ConAttr(vector<vector<cell> > chart, vector<string> label, double info_D, double &info_A, double &H, double &Gain, double &IGR, int i)
{
    vector<double> median;  // 保存中位值
    double medianBest = 0;  // 保存最好中位值
    double info_AMin = 1;   // 保存最小info值
    feature result;         // 返回值

    for(int j = 0; j < chart.size(); ++j)
    {// 将表格中的数据转换成double型
        stringstream s(chart[j][i].value);
        double p;

        s >> p;
        median.push_back(p);
    }
    sort(median.begin(), median.end()); // 对数据从小到大排序
    for(int j = 0; j < median.size() - 1; ++j)
        // 计算出相邻值的中位值
        median[j] = (median[j] + median[j + 1]) / 2;
    median.pop_back();

    for(int j = 0; j < median.size(); ++j)
    {// 获取最适宜的分裂点
        double info_ANow = 0;                   // 保存当前中位值计算出来的info值
        int smallLabel[label.size()];           // 保存小于当前中位值的数据的类别
        int bigLabel[label.size()];             // 保存大于当前中位值的数据的类别
        string sMedian = to_string(median[j]);  // 保存当前中位值为string型
        int smallLabelSum = 0;                  // 保存小于中位值的数据数
        int bigLabelSum = 0;                    // 保存大于中位值的数据数
        double smallD = 0;                      // 暂存值
        double bigD = 0;                        // 暂存值

        for(int k = 0; k < label.size(); ++k)
        {// 初始化数组
            smallLabel[k] = 0;
            bigLabel[k] = 0;
        }

        for(int k = 0; k < chart.size(); ++k)
        {// 记录数据的类别
            double p;

            if(sMedian > chart[k][i].value)
            {
                ++smallLabel[chart[k][i].flag];
                ++smallLabelSum;
            }
            else
            {
                ++bigLabel[chart[k][i].flag];
                ++bigLabelSum;
            }
        }

        for(int k = 0; k < label.size(); ++k)
        {// 计算当前中位值info_A
            double smallP = (smallLabel[k] * 1.0) / smallLabelSum;
            double bigP = (bigLabel[k] * 1.0) / bigLabelSum;
            if(smallP != 0)
            {
                smallD -= smallP * log2(smallP);
                info_ANow += (smallLabelSum * 1.0) / (chart.size() - 1) * smallD;
            }
            if(bigP != 0)
            {
                bigD -= bigP * log2(bigP);
                info_ANow += (bigLabelSum * 1.0) / (chart.size() - 1) * bigD;
            }
        }

        if(info_AMin > info_ANow)
        {
            // 计算新的H
            H = 0;
            medianBest = median[j];
            info_AMin = info_ANow;
            double smallH = (smallLabelSum * 1.0) / (chart.size() - 1);
            double bigH = (bigLabelSum * 1.0) / (chart.size() - 1);
            H -= smallH * log2(smallH);
            H -= bigH * log2(bigH);
        }
    }

    // 得到info_A
    info_A = info_AMin;

    // 计算Gain
    Gain = info_D - info_A;

    // 计算IGR
    IGR = Gain / H;

    result.subscript = i;
    result.valueNum = 2;
    result.value = new string[2];
    result.value[0] = "小于" + to_string(medianBest);
    result.value[1] = "大于" + to_string(medianBest);
    return result;
}

void DTNode::createNode(vector<vector<cell> > chart, vector<string> label, int dataSum)
{
    feature f = c4_5(chart, label, dataSum);

    if (f.subscript >= 0)
    { // 非叶子结点
        value = chart[0][f.subscript].value;
        childNum = f.valueNum;
        childValue = new string[childNum];
        child = new DTNode *[childNum];
        for (int i = 0; i < f.valueNum; ++i)
        {
            childValue[i] = f.value[i];
            child[i] = new DTNode;
        }
        delete[] f.value;
    }

    else
    { // 叶子结点
        value = label[0 - (f.subscript + 1)];
        childNum = 0;
        childValue = nullptr;
        child = nullptr;
    }
}

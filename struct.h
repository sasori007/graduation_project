#ifndef STRUCT_H
#define STRUCT_H

#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string.h>
#include <cmath>
#include <sstream>
using namespace std;

struct feature
{// 特征的相关信息
    int subscript;  // 特征在表格中的下标
    int valueNum;   // 特征拥有的值的数量
    string* value;  // 特征拥有的值
};

struct cell
{// 表格单个数据值
    string value;   // 类别标签或属性在表格中的值
    int flag;       // 类别标签的标识符
};



#endif // STRUCT_H

#ifndef NAIVEGOMOKU_UTILS_H
#define NAIVEGOMOKU_UTILS_H

#include <iostream>

using namespace std;

// 测试是否为数字
bool is_digit(char a);

// 测试是否为整数
bool test_integer(string s);

// 输入转化、判断合法性(string->int)
bool transform_position_name(const string &xInput, const string &yInput, int &xPos, int &yPos);

// 判断索引值是否在范围内
bool is_pos_in_range(int pos);

#endif //NAIVEGOMOKU_UTILS_H

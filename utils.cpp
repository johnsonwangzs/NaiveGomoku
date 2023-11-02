#include <iostream>
#include <cstring>
#include "utils.h"


bool is_digit(char a) {
    if (a - '0' == 0 || a - '0' == 1 || a - '0' == 2 || a - '0' == 3 || a - '0' == 4 ||
        a - '0' == 5 || a - '0' == 6 || a - '0' == 7 || a - '0' == 8 || a - '0' == 9)
        return true;
    return false;
}

bool test_integer(string s) {
    for (int i = 0; i < s.length(); i++) {
        if (!is_digit(s[i]))
            return false;
    }
    return true;
}

bool transform_position_name(const string &xInput, const string &yInput, int &xPos, int &yPos) {
    // 判断 y
    if (yInput.length() != 1) {
        cout << "  列号非法！请输入一个字母！" << endl;
        return false;
    }
    if (strcmp(yInput.c_str(), "A") >= 0 && strcmp(yInput.c_str(), "O") <= 0) {
        yPos = (int) yInput.c_str()[0] - 65;
    } else if (strcmp(yInput.c_str(), "a") >= 0 && strcmp(yInput.c_str(), "o") <= 0) {
        yPos = (int) yInput.c_str()[0] - 97;
    } else {
        cout << "  列号非法！输入的字母需在A-O（a-o）之间！" << endl;
        return false;
    }
    // 判断 x
    if (!test_integer(xInput)) {
        cout << "  行号非法！需输入一个1-15之间的整数！" << endl;
        return false;
    }
    if (stoi(xInput) < 1 || stoi(xInput) > 15) {
        cout << "  行号非法！请输入一个数（1-15之间）！" << endl;
        return false;
    }
    xPos = 14 - (stoi(xInput) - 1);  // 注意棋盘行号与程序存储的二维数组索引顺序刚好相反，这里转化为程序需要的索引值
    return true;
}

bool is_pos_in_range(int pos) {
    if (pos >= 0 && pos <= 14) {
        return true;
    }
    return false;
}
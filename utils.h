#ifndef NAIVEGOMOKU_UTILS_H
#define NAIVEGOMOKU_UTILS_H

using namespace std;

bool isDigit(char a);
bool test_integer(string s);

bool isDigit(char a) {
    if (a - '0' == 0 || a - '0' == 1 || a - '0' == 2 || a - '0' == 3 || a - '0' == 4 ||
        a - '0' == 5 || a - '0' == 6 || a - '0' == 7 || a - '0' == 8 || a - '0' == 9)
        return true;
    return false;
}

bool test_integer(string s) {
    for (int i = 0; i < s.length(); i++) {
        if (!isDigit(s[i]))
            return false;
    }
    return true;
}
#endif //NAIVEGOMOKU_UTILS_H

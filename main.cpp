#include <iostream>
#include "scheduler.h"
using namespace std;

int main() {
    cout << "> 开始初始化..." << endl;
    auto *scheduler = new Scheduler();
    delete scheduler;
    cout << "> 现在可以退出；要重新开始游戏，请重启程序！" << endl;
    return 0;
}

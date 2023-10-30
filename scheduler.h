#ifndef NAIVEGOMOKU_SCHEDULER_H
#define NAIVEGOMOKU_SCHEDULER_H

#include <iostream>

using namespace std;

// 对局调度器：比赛流程控制
class Scheduler {
private:
    int gameCnt = 0;  // 对局计数器
    int botWinCnt = 0;  // 电脑赢得对局数
    int humanWinCnt = 0;  // 人类玩家赢得对局数
public:
    Scheduler();  // 构造函数
    ~Scheduler();  // 析构函数

    void schedule_game();  // 比赛调度主循环

    int choose_game_option();  // 玩家选择游戏选项
    void start_new_game();  // 开始新一局比赛
    int decide_first_hand();  // 决定先手
    void show_game_score();  // 展示比赛得分
    void end_game();  // 结束比赛
};

#endif //NAIVEGOMOKU_SCHEDULER_H

#ifndef NAIVEGOMOKU_SCHEDULER_H
#define NAIVEGOMOKU_SCHEDULER_H

#include <iostream>
#include "chessboard.h"
#include "player.h"
#include "judge.h"

using namespace std;

// 对局调度器：比赛流程控制
class Scheduler {
private:
    int gameCnt;  // 对局计数器
    int botWinCnt;  // 电脑赢得对局数
    int humanWinCnt;  // 人类玩家赢得对局数
    int drawCnt;  // 平局数
public:
    Scheduler();  // 构造函数
    ~Scheduler();  // 析构函数

    int nextStepPlayer;  // 记录下一步棋轮到哪一方下（马上要下的）
    int lastStepPlayer;  // 记录上一步是哪一方下的（刚下完的）
    int lastStepXPos;
    int lastStepYPos;

    void schedule_game();  // 比赛调度主循环

    int choose_game_option();  // 玩家选择游戏选项
    void start_new_game();  // 开始新一局比赛

    void decide_first_hand(BotPlayer *botPlayer, HumanPlayer *humanPlayer, Judge *judge);  // 决定先手
    void set_first_chess_piece(BotPlayer *botPlayer, HumanPlayer *humanPlayer,
                               Judge *judge, ChessBoard *chessBoard);  // 先手第一次落子
    void set_human_player_chess_piece(BotPlayer *botPlayer, HumanPlayer *humanPlayer,
                                      Judge *judge, ChessBoard *chessBoard);  // 获取人类玩家输入，并落子

    void show_game_score();  // 展示比赛得分
    void end_game();  // 结束比赛
};

#endif //NAIVEGOMOKU_SCHEDULER_H

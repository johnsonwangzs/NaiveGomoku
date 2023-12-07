#ifndef NAIVEGOMOKU_PLAYER_H
#define NAIVEGOMOKU_PLAYER_H

#include <iostream>
#include "chessboard.h"

using namespace std;

const int MCST_MAX_ITER = 50000;  // 蒙特卡洛搜索树的最大迭代轮数
const int MCST_MAX_SEARCH_DEPTH = 20;  // 蒙特卡洛搜索树的最大搜索层数
const int MCST_MAX_SIMULATION_DEPTH = 10;  // 蒙特卡洛搜索树的最大模拟层数
const int MCST_UCB_C = 2;  // UCB计算中c的取值
const double TERMINAL_STATE_WIN_VALUE = 10;  // 终止状态棋盘，我方胜利，节点被赋予的value
const double TERMINAL_STATE_LOSE_VALUE = -10;  // 终止状态棋盘，我方失败，节点被赋予的value
const double TERMINAL_STATE_DUAL_VALUE = 0.0;  // 终止状态棋盘，平局，节点被赋予的value

class Player {
public:
    int playerTypeCode;  // 0：电脑    1：人类
    int playerChessPieceType;  // 1：先手（黑子）   2：后手（白子）
    string playerName;

    virtual int getPlayerTypeCode();

    int getPlayerChessPieceType();

    void setPlayerChessPieceType(int myHand);
};

// 电脑玩家
class BotPlayer : public Player {
public:
    static int playerTypeCode;
    static const int codeNone = 0;  // 评判结果代码：未获胜且无违规
    static const int codeWin = 1;  // 评判结果代码：获胜
    static const int codeForbid33 = 2;  // 评判结果代码：三三禁手
    static const int codeForbid44 = 3;  // 评判结果代码：四四禁手
    static const int codeForbidOverline = 4;  // 评判结果代码：长连禁手
    constexpr static const int move[8][2] = {-1, 0,
                                             1, 0,
                                             0, -1,
                                             0, 1,
                                             -1, -1,
                                             1, 1,
                                             -1, 1,
                                             1, -1};

    int getPlayerTypeCode() override;

    BotPlayer();

    ~BotPlayer();

    // AI决策
    void decide_next_step(ChessBoard *chessBoard, int &xPos, int &yPos);

    // 找UCB最大的节点
    int find_max_ucb(vector<ChessBoardNode *> v, double parentCntVisited);

    // 扩展节点
    void expand_node(ChessBoardNode *chessBoardNode);

    // 模拟
    double rollout(RolloutNode *rolloutNodeRoot);

    // 随机选择下一个rollout节点
    void choose_next_rollout_node(RolloutNode *rolloutNode, int &x, int &y);

    // 反向传播
    void back_propagation(ChessBoardNode *chessBoardNode, double value);

    // 单向延伸计数器
    void counter(RolloutNode *rolloutNode, int direction, int &a, int &b, int &c);

    /* 检查当前棋盘是否为终点 */
    int is_terminal_board(RolloutNode *rolloutNode);

    /* 检查单独一个方向 */
    void check_single_direction(RolloutNode *rolloutNode,
                                int a1, int b1, int c1, int a2, int b2, int c2,
                                int &liveThree, int &four, int &overline, int &five);
    // 搜索结束后释放内存
    void delete_MCST(ChessBoardNode *chessBoardNode);
};

// 人类玩家
class HumanPlayer : public Player {
public:
    static int playerTypeCode;

    int getPlayerTypeCode() override;

    HumanPlayer();

    ~HumanPlayer();
};

#endif //NAIVEGOMOKU_PLAYER_H

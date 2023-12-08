#ifndef NAIVEGOMOKU_PLAYER_H
#define NAIVEGOMOKU_PLAYER_H

#include <iostream>
#include "chessboard.h"

using namespace std;

const int MCST_MAX_ITER = 200000;  // 蒙特卡洛搜索树的最大迭代轮数
const int MCST_MAX_SEARCH_DEPTH = 20;  // 蒙特卡洛搜索树的最大搜索层数
const double MCST_MAX_SEARCH_TIME = 15.0;  // 蒙特卡罗最大决策时间
const double MCST_UCB_C = 2.0;  // UCB计算中c的取值
const double TERMINAL_STATE_WIN_VALUE = 1.0;  // 终止状态棋盘，我方胜利，节点被赋予的value
const double TERMINAL_STATE_LOSE_VALUE = -0.5;  // 终止状态棋盘，我方失败，节点被赋予的value
const double TERMINAL_STATE_DUAL_VALUE = 0.5;  // 终止状态棋盘，平局，节点被赋予的value
const int MCST_MAX_ROLLOUT_STEP = 2;  // rollout时最多向外扩展几个节点
const int MCST_MAX_EXPAND_STEP = 2;  // expand时最多向外扩展几个节点

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

    // AI决策主函数
    void decide_next_step(ChessBoard *chessBoard, int &xPos, int &yPos, int lastStepXPos, int lastStepYPos);

    // 找UCB最大的节点
    int find_max_ucb(vector<ChessBoardNode *> v, double parentCntVisited);

    // 扩展节点（普通版）：当棋盘状态无风险时使用
    void expand_node(ChessBoardNode *chessBoardNode);

    // 扩展节点（安全版）：当棋盘状态存在风险时使用
    void expand_node_safe(ChessBoardNode *chessBoardNode);

    // 搜索涉及三和四的潜在风险位，扩展为新节点
    void search_and_expand(RolloutNode *rolloutTestNode, ChessBoardNode *chessBoardNode, int s, int t);

    // 模拟
    double rollout(RolloutNode *rolloutNodeRoot);

    // 在高价值位置中，随机选择一个作为继续进行rollout的节点
    void choose_next_rollout_node(RolloutNode *rolloutNode, int &x, int &y);

    // 反向传播
    void back_propagation(ChessBoardNode *chessBoardNode, double value);

    // 单向延伸计数器
    void counter(RolloutNode *rolloutNode, int direction, int &a, int &b, int &c);

    // 检查当前棋盘是否为终点
    int is_terminal_board(RolloutNode *rolloutNode);

    // 检查当前棋盘是否为风险状态
    int is_risky_board(RolloutNode *rolloutNode, int &liveThreeCnt, int &FourCnt);

    // 检查单独一个方向的三、四、五、长连情况
    void check_single_direction(RolloutNode *rolloutNode,
                                int a1, int b1, int c1, int a2, int b2, int c2,
                                int &liveThree, int &four, int &overline, int &five);
    // 搜索结束后释放内存
    void delete_MCST(ChessBoardNode *chessBoardNode);

    // 效益衰减因子
    double decay_factor(int depth);
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

#ifndef NAIVEGOMOKU_JUDGE_H
#define NAIVEGOMOKU_JUDGE_H

#include <iostream>

using namespace std;

// 裁判
class Judge {
private:
    const string judgeName = "wangzs";
public:
    Judge();

    ~Judge();

    int whoIsFirstHand;  // 记录谁是先手（取值为playerTypeCode）
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

    /* 单向延伸计数器，方向：上
     player: 当前考虑落子的玩家
     chessBoard: 当前棋盘
     xPos, yPos: 指定棋盘上的一个位置
     direction: 当前考虑的方向
     a: 从相邻位置开始，最多连续几个同色子
     b: a的连续的中止是否因空位导致
     c: b之后，还有几个连续的同色子 */
    void counter(Player *player, ChessBoard *chessBoard, int xPos, int yPos, int direction,
                 int &a, int &b, int &c);

    /* 根据刚才上一步的落子，检查玩家是否能赢/犯规 */
    int check_last_step(Player *player, ChessBoard *chessBoard, int xPos, int yPos);

    /* 检查单独一个方向 */
    void check_single_direction(Player *player, ChessBoard *chessBoard,
                                int a1, int b1, int c1, int a2, int b2, int c2,
                                int &liveThree, int &four, int &overline, int &five);

    // 判断初次落子位置的有效性
    bool judge_first_action_validity(int xPos, int yPos);

    // 判断所选落子位置的有效性
    bool judge_action_validity(Player *player, ChessBoard *chessBoard, int xPos, int yPos);

    // 宣布赢家
    void claim_winner(Player *player, const string &winMsg);

    // 宣布平局
    void claim_draw();
};

#endif //NAIVEGOMOKU_JUDGE_H

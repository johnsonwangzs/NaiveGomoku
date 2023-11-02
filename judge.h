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

    // 同色棋子计数，方向：阳线-垂直
    int cnt_same_piece_vertical(Player *player, ChessBoard *chessBoard, int xPos, int yPos);

    // 同色棋子计数，方向：阳线-水平
    int cnt_same_piece_horizontal(Player *player, ChessBoard *chessBoard, int xPos, int yPos);

    // 同色棋子计数，方向：阴线-斜向右上
    int cnt_same_piece_upright(Player *player, ChessBoard *chessBoard, int xPos, int yPos);

    // 同色棋子计数，方向：阴线-斜向左上
    int cnt_same_piece_upleft(Player *player, ChessBoard *chessBoard, int xPos, int yPos);

    /* 检查是否达到胜利条件
     对于执黑子的一方：判断从4条线（8个方向）上是否形成恰好五连
     注：对于将棋子落在长连禁手处的情况，本函数不会判其胜利，但也不会指出违反禁手（由其他函数实现）
     对于执白子的一方：判断从4条线（8个方向）上是否形成大于或等于五连 */
    bool check_is_win(Player *player, ChessBoard *chessBoard, int xPos, int yPos);

    // 检查指定位置是否超出棋盘，然后监测该处的棋子（或空位）是否为目标类型的棋子
    bool check_piece_at_pos(ChessBoard *chessBoard, int xPos, int yPos, int targetPieceTypeCode);

    /* 检查是否有长连禁手（针对黑子）
     若违反了长连禁手返回true；否则返回false */
    bool check_forbidden_overline(Player *player, ChessBoard *chessBoard, int xPos, int yPos);

    // 四四禁手检查，方向：阳线-垂直
    int check_four_vertical(ChessBoard *chessBoard, int xPos, int yPos);
    // 四四禁手检查，方向：阳线-水平
    int check_four_horizontal(ChessBoard *chessBoard, int xPos, int yPos);
    // 四四禁手检查，方向：阴线-斜向右上
    int check_four_upright(ChessBoard *chessBoard, int xPos, int yPos);
    // 四四禁手检查，方向：阴线-斜向左上
    int check_four_upleft(ChessBoard *chessBoard, int xPos, int yPos);

    /* 检查是否有四四禁手
     四四禁手是指当黑方一子落下时，同时形成两个或两个以上的“四”且没有形成“五连”时
     注：“四”包括活四、冲四（连冲四、跳冲四即嵌五）
       活四：有两个点可以成五的四（活四只有一种形状）
       冲四：只有一个点可以成五的四
       同方向也可形成两个四，例如 0 * 0 * X * 0 * 0 */
    bool check_forbidden_double_four(ChessBoard *chessBoard, int xPos, int yPos);

    // 三三禁手检查，方向：阳线-垂直
    bool check_open_three_vertical(ChessBoard *chessBoard, int xPos, int yPos);

    // 三三禁手检查，方向：阳线-水平
    bool check_open_three_horizontal(ChessBoard *chessBoard, int xPos, int yPos);

    // 三三禁手检查，方向：阴线-斜向右上
    bool check_open_three_upright(ChessBoard *chessBoard, int xPos, int yPos);

    // 三三禁手检查，方向：阴线-斜向左上
    bool check_open_three_upleft(ChessBoard *chessBoard, int xPos, int yPos);

    /* 检查是否有三三禁手
     “三三禁手”是指当黑方一子落下同时形成两个或两个以上的活三 */
    bool check_forbidden_double_three(ChessBoard *chessBoard, int xPos, int yPos);

    // 判断初次落子位置的有效性
    bool judge_first_action_validity(int xPos, int yPos);

    // 判断所选落子位置的有效性
    bool judge_action_validity(Player *player, ChessBoard *chessBoard, int xPos, int yPos);

    // 宣布赢家
    void claim_winner(Player *player, const string& winMsg);

    // 宣布平局
    void claim_draw();
};

#endif //NAIVEGOMOKU_JUDGE_H

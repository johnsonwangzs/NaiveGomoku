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
     从4条线（8个方向）上进行检查是否形成五连
     注：对于将棋子落在长连禁手处的情况，本函数不会判其胜利，但也不会指出违反禁手（由其他函数实现）*/
    bool check_is_win(Player *player, ChessBoard *chessBoard, int xPos, int yPos);

    /* 检查是否有禁手（针对黑子）
     若违反了长连禁手、三三禁手或四四禁手中的某一个，返回true；否则返回false */
    bool check_forbidden(Player *player, ChessBoard *chessBoard, int xPos, int yPos);

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

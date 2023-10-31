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

    bool check_is_win(Player *player, ChessBoard *chessBoard, int lastStepXPos, int lastStepYPos);  // 检查是否达到胜利条件
    bool continue_game(Player *player, ChessBoard *chessBoard, int lastStepXPos, int lastStepYPos);  // 判断是否能继续进行下一回合
    bool judge_first_action_validity(int xPos, int yPos);  // 判断初次落子位置的有效性
    bool judge_action_validity(Player *player, ChessBoard *chessBoard, int xPos, int yPos);  // 判断所选落子位置的有效性
};

#endif //NAIVEGOMOKU_JUDGE_H

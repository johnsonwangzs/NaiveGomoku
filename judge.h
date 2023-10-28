#ifndef NAIVEGOMOKU_JUDGE_H
#define NAIVEGOMOKU_JUDGE_H

using namespace std;

// 裁判
class Judge {
private:
    const string judgeName = "wangzs";
public:
    Judge();
    ~Judge();

    int whoIsFirstHand;  // 记录谁是先手（取值为playerTypeCode）

    bool continue_game();  // 判断是否能继续进行下一回合
    bool judge_first_action_validity(int xPos, int yPos);  // 判断初次落子位置的有效性
    bool judge_action_validity(Player *player, ChessBoard *chessBoard, int xPos, int yPos);  // 判断所选落子位置的有效性
};

Judge::Judge() {
    cout << "> 正在创建本局裁判..." << endl;
}

Judge::~Judge() {
    cout << "> 正在销毁本局裁判..." << endl;
}

bool Judge::continue_game() {
    return true;
}

bool Judge::judge_action_validity(Player *player, ChessBoard *chessBoard, int xPos, int yPos) {
    return false;
}

bool Judge::judge_first_action_validity(int xPos, int yPos) {
    if (xPos == 7 && yPos == 7)
        return true;
    return false;
}

#endif //NAIVEGOMOKU_JUDGE_H

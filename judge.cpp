#include <iostream>
#include "chessboard.h"
#include "player.h"
#include "judge.h"

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
#include <iostream>
#include <cstring>
#include "utils.h"
#include "chessboard.h"
#include "player.h"
#include "judge.h"
#include "scheduler.h"

using namespace std;

class Scheduler;
class BotPlayer;
class HumanPlayer;
class Judge;
class ChessBoard;
class ChessPiece;
class WhiteChessPiece;
class BlackChessPiece;

int main() {
    cout << "> 开始初始化..." << endl;
    auto *scheduler = new Scheduler();

    delete scheduler;
//    string yInput = "O";
//    cout << strcmp(yInput.c_str(), "A") << endl;
//    cout << strcmp(yInput.c_str(), "O") << endl;
    cout << "> 现在可以退出；要重新开始游戏，请重启程序！" << endl;
    return 0;
}

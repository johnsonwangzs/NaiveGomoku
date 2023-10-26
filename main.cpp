#include <iostream>
#include "chessboard.h"
#include "judge.h"
#include "player.h"
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
    auto *chessboard = new ChessBoard();
    chessboard->show(1);
    delete scheduler;
    delete chessboard;
    return 0;
}

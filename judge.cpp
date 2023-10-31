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

bool Judge::continue_game(Player *player, ChessBoard *chessBoard, int lastStepXPos, int lastStepYPos) {
    if (check_is_win(player, chessBoard, lastStepXPos, lastStepYPos)) {
        return false;  // 如果有一方达成胜利条件，则回合不再继续
    }
    return true;
}

bool Judge::judge_action_validity(Player *player, ChessBoard *chessBoard, int xPos, int yPos) {
    if (chessBoard->chessPieceInBoard[xPos][yPos] == BlankPiece::pieceTypeCode) {
        chessBoard->set_new_chess_piece(player->getPlayerChessPieceType(), xPos, yPos);
        return true;
    }
    return false;
}

bool Judge::judge_first_action_validity(int xPos, int yPos) {
    if (xPos == 7 && yPos == 7)
        return true;
    return false;
}

bool Judge::check_is_win(Player *player, ChessBoard *chessBoard, int lastStepXPos, int lastStepYPos) {
    // 从4条线（8个方向）上进行检查是否有5连同色棋子的情况

    // 竖直方向
    int singleDirectionChessPieceCnt = 1;
    // 向上
    int posOffset = 0;
    while (true) {
        posOffset++;
        if (lastStepXPos - posOffset >= 0 && chessBoard->chessPieceInBoard[lastStepXPos - posOffset][lastStepYPos] ==
                                             player->getPlayerChessPieceType()) {
            singleDirectionChessPieceCnt++;
            continue;
        }
        break;
    }
    // 向下
    posOffset = 0;
    while (true) {
        posOffset++;
        if (lastStepXPos + posOffset <= 14 && chessBoard->chessPieceInBoard[lastStepXPos + posOffset][lastStepYPos] ==
                                              player->getPlayerChessPieceType()) {
            singleDirectionChessPieceCnt++;
            continue;
        }
        break;
    }
    if (singleDirectionChessPieceCnt >= 5) {
        return true;
    }

    // 水平方向
    singleDirectionChessPieceCnt = 1;
    // 向左
    posOffset = 0;
    while (true) {
        posOffset++;
        if (lastStepYPos - posOffset >= 0 && chessBoard->chessPieceInBoard[lastStepXPos][lastStepYPos - posOffset] ==
                                             player->getPlayerChessPieceType()) {
            singleDirectionChessPieceCnt++;
            continue;
        }
        break;
    }
    // 向左
    posOffset = 0;
    while (true) {
        posOffset++;
        if (lastStepYPos + posOffset <= 14 && chessBoard->chessPieceInBoard[lastStepXPos][lastStepYPos + posOffset] ==
                                              player->getPlayerChessPieceType()) {
            singleDirectionChessPieceCnt++;
            continue;
        }
        break;
    }
    if (singleDirectionChessPieceCnt >= 5) {
        return true;
    }

    // 右斜
    singleDirectionChessPieceCnt = 1;
    // 右上
    posOffset = 0;
    while (true) {
        posOffset++;
        if (lastStepXPos - posOffset >= 0 && lastStepYPos + posOffset <= 14 &&
            chessBoard->chessPieceInBoard[lastStepXPos - posOffset][lastStepYPos + posOffset] ==
            player->getPlayerChessPieceType()) {
            singleDirectionChessPieceCnt++;
            continue;
        }
        break;
    }
    // 左下
    posOffset = 0;
    while (true) {
        posOffset++;
        if (lastStepXPos + posOffset <= 14 && lastStepYPos - posOffset >= 0 &&
            chessBoard->chessPieceInBoard[lastStepXPos + posOffset][lastStepYPos - posOffset] ==
            player->getPlayerChessPieceType()) {
            singleDirectionChessPieceCnt++;
            continue;
        }
        break;
    }
    if (singleDirectionChessPieceCnt >= 5) {
        return true;
    }

    // 左斜
    singleDirectionChessPieceCnt = 1;
    // 左上
    posOffset = 0;
    while (true) {
        posOffset++;
        if (lastStepXPos - posOffset >= 0 && lastStepYPos - posOffset >= 0 &&
            chessBoard->chessPieceInBoard[lastStepXPos - posOffset][lastStepYPos - posOffset] ==
            player->getPlayerChessPieceType()) {
            singleDirectionChessPieceCnt++;
            continue;
        }
        break;
    }
    // 右下
    posOffset = 0;
    while (true) {
        posOffset++;
        if (lastStepXPos + posOffset <= 14 && lastStepYPos + posOffset <= 14 &&
            chessBoard->chessPieceInBoard[lastStepXPos + posOffset][lastStepYPos + posOffset] ==
            player->getPlayerChessPieceType()) {
            singleDirectionChessPieceCnt++;
            continue;
        }
        break;
    }
    if (singleDirectionChessPieceCnt >= 5) {
        return true;
    }

    return false;
}

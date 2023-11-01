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

bool Judge::judge_action_validity(Player *player, ChessBoard *chessBoard, int xPos, int yPos) {
    if (chessBoard->chessPieceInBoard[xPos][yPos] == BlankPiece::pieceTypeCode) {  // 空位
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

bool Judge::check_is_win(Player *player, ChessBoard *chessBoard, int xPos, int yPos) {
    // 只有严格等于5连才算赢
    if (cnt_same_piece_vertical(player, chessBoard, xPos, yPos) == 5) {
        return true;
    }
    if (cnt_same_piece_horizontal(player, chessBoard, xPos, yPos) == 5) {
        return true;
    }
    if (cnt_same_piece_upright(player, chessBoard, xPos, yPos) == 5) {
        return true;
    }
    if (cnt_same_piece_upleft(player, chessBoard, xPos, yPos) == 5) {
        return true;
    }
    return false;
}

bool Judge::check_forbidden(Player *player, ChessBoard *chessBoard, int xPos, int yPos) {
    // 长连禁手
    int cntVertical, cntHorizontal, cntUpright, cntUpleft;
    bool existEqualFive = false, existMoreThanFive = false;
    cntVertical = cnt_same_piece_vertical(player, chessBoard, xPos, yPos);
    cntHorizontal = cnt_same_piece_horizontal(player, chessBoard, xPos, yPos);
    cntUpright = cnt_same_piece_upright(player, chessBoard, xPos, yPos);
    cntUpleft = cnt_same_piece_upleft(player, chessBoard, xPos, yPos);
    if (cntVertical == 5 || cntHorizontal == 5 || cntUpright == 5 || cntUpleft == 5) {
        existEqualFive = true;
    }
    if (cntVertical > 5 || cntHorizontal > 5 || cntUpright > 5 || cntUpleft > 5) {
        existMoreThanFive = true;
    }
    if (!existEqualFive && existMoreThanFive) {  // 没有五连，但有长连（形成5个以上黑子）
        return true;
    }

    // 三三禁手

    // 四四禁手
    return false;  // 不存在禁手
}

void Judge::claim_winner(Player *player, const string &winMsg) {
    if (player->getPlayerTypeCode() == 0) {
        cout << "> 本局比赛结果为：电脑赢得了本局比赛！" << endl
             << "  获胜原因：" << winMsg << endl
             << "  电脑记一分！" << endl << endl;
    } else if (player->getPlayerTypeCode() == 1) {
        cout << "> 本局比赛结果为：人类玩家赢得了本局比赛！" << endl
             << "  获胜原因：" << winMsg << endl
             << "  人类玩家记一分！" << endl << endl;
    }
}

void Judge::claim_draw() {
    cout << "> 本局比赛结果为：平局！"
         << "  不计分！" << endl;
}

int Judge::cnt_same_piece_vertical(Player *player, ChessBoard *chessBoard, int xPos, int yPos) {
    // 竖直方向
    int singleDirectionChessPieceCnt = 1;
    // 向上
    int posOffset = 0;
    while (true) {
        posOffset++;
        if (xPos - posOffset >= 0
            && chessBoard->chessPieceInBoard[xPos - posOffset][yPos] == player->getPlayerChessPieceType()) {
            singleDirectionChessPieceCnt++;
            continue;
        }
        break;
    }
    // 向下
    posOffset = 0;
    while (true) {
        posOffset++;
        if (xPos + posOffset <= 14
            && chessBoard->chessPieceInBoard[xPos + posOffset][yPos] == player->getPlayerChessPieceType()) {
            singleDirectionChessPieceCnt++;
            continue;
        }
        break;
    }
    return singleDirectionChessPieceCnt;
}

int Judge::cnt_same_piece_horizontal(Player *player, ChessBoard *chessBoard, int xPos, int yPos) {
    // 水平方向
    int singleDirectionPieceCnt = 1;
    // 向左
    int posOffset = 0;
    while (true) {
        posOffset++;
        if (yPos - posOffset >= 0
            && chessBoard->chessPieceInBoard[xPos][yPos - posOffset] == player->getPlayerChessPieceType()) {
            singleDirectionPieceCnt++;
            continue;
        }
        break;
    }
    // 向左
    posOffset = 0;
    while (true) {
        posOffset++;
        if (yPos + posOffset <= 14
            && chessBoard->chessPieceInBoard[xPos][yPos + posOffset] == player->getPlayerChessPieceType()) {
            singleDirectionPieceCnt++;
            continue;
        }
        break;
    }
    return singleDirectionPieceCnt;
}

int Judge::cnt_same_piece_upright(Player *player, ChessBoard *chessBoard, int xPos, int yPos) {
    // 右斜
    int singleDirectionPieceCnt = 1;
    // 右上
    int posOffset = 0;
    while (true) {
        posOffset++;
        if (xPos - posOffset >= 0 && yPos + posOffset <= 14
            && chessBoard->chessPieceInBoard[xPos - posOffset][yPos + posOffset] == player->getPlayerChessPieceType()) {
            singleDirectionPieceCnt++;
            continue;
        }
        break;
    }
    // 左下
    posOffset = 0;
    while (true) {
        posOffset++;
        if (xPos + posOffset <= 14 && yPos - posOffset >= 0
            && chessBoard->chessPieceInBoard[xPos + posOffset][yPos - posOffset] == player->getPlayerChessPieceType()) {
            singleDirectionPieceCnt++;
            continue;
        }
        break;
    }
    return singleDirectionPieceCnt;
}

int Judge::cnt_same_piece_upleft(Player *player, ChessBoard *chessBoard, int xPos, int yPos) {
    // 左斜
    int singleDirectionChessPieceCnt = 1;
    // 左上
    int posOffset = 0;
    while (true) {
        posOffset++;
        if (xPos - posOffset >= 0 && yPos - posOffset >= 0
            && chessBoard->chessPieceInBoard[xPos - posOffset][yPos - posOffset] == player->getPlayerChessPieceType()) {
            singleDirectionChessPieceCnt++;
            continue;
        }
        break;
    }
    // 右下
    posOffset = 0;
    while (true) {
        posOffset++;
        if (xPos + posOffset <= 14 && yPos + posOffset <= 14
            && chessBoard->chessPieceInBoard[xPos + posOffset][yPos + posOffset] == player->getPlayerChessPieceType()) {
            singleDirectionChessPieceCnt++;
            continue;
        }
        break;
    }
    return singleDirectionChessPieceCnt;
}

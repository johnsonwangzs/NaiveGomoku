#include <iostream>
#include "utils.h"
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

void Judge::claim_winner(Player *player, const string &winMsg) {
    if (player->getPlayerTypeCode() == BotPlayer::playerTypeCode) {
        cout << "> 本局比赛结果为：电脑赢得了本局比赛！" << endl
             << "  获胜原因：" << winMsg << endl
             << "  电脑记一分！" << endl << endl;
    } else if (player->getPlayerTypeCode() == HumanPlayer::playerTypeCode) {
        cout << "> 本局比赛结果为：人类玩家赢得了本局比赛！" << endl
             << "  获胜原因：" << winMsg << endl
             << "  人类玩家记一分！" << endl << endl;
    }
}

void Judge::claim_draw() {
    cout << "> 本局比赛结果为：平局！"
         << "  不计分！" << endl;
}

void Judge::counter(Player *player, ChessBoard *chessBoard, int xPos, int yPos, int direction, int &a, int &b, int &c) {
    int curX = xPos, curY = yPos;
    bool continuesFlag = true;
    // 从相邻位置开始，连续数同色子
    while (continuesFlag) {
        curX += Judge::move[direction][0];
        curY += Judge::move[direction][1];
        if (is_pos_in_range(curX) && is_pos_in_range(curY)) {
            if (chessBoard->chessPieceInBoard[curX][curY] != player->getPlayerChessPieceType()) {
                continuesFlag = false;
            } else {
                a++;
            }
        } else {
            break;
        }
    }
    if (!continuesFlag) {  // 连续的同色子计数是因为遇到异色子或空位而停止的
        if (chessBoard->chessPieceInBoard[curX][curY] == BlankPiece::pieceTypeCode) {  // 遇到空位
            b = 1;
        } else {  // 遇到异色子
            b = 0;
            c = 0;
            return;
        }
    } else {  // 连续的同色子计数是因为遇到边界而停止的
        b = 0;
        c = 0;
        return;
    }
    // 继续往后数连续的同色子
    continuesFlag = true;
    while (continuesFlag) {
        curX += Judge::move[direction][0];
        curY += Judge::move[direction][1];
        if (is_pos_in_range(curX) && is_pos_in_range(curY)) {
            if (chessBoard->chessPieceInBoard[curX][curY] != player->getPlayerChessPieceType()) {
                continuesFlag = false;
            } else {
                c++;
            }
        } else {
            break;
        }
    }
}

int Judge::check_last_step(Player *player, ChessBoard *chessBoard, int xPos, int yPos) {
    int a1 = 0, b1 = 0, c1 = 0, a2 = 0, b2 = 0, c2 = 0;  // 同一条线的两个方向
    int liveThree = 0, four = 0, overline = 0, five = 0;

    // 计数并判断  注意这几个if的顺序不能变
    // 上&下
    counter(player, chessBoard, xPos, yPos, 0, a1, b1, c1);
    counter(player, chessBoard, xPos, yPos, 1, a2, b2, c2);
    check_single_direction(player, chessBoard, a1, b1, c1, a2, b2, c2, liveThree, four, overline, five);
    if (five > 0) {
        return Judge::codeWin;
    }
    if (player->getPlayerChessPieceType() == BlackChessPiece::pieceTypeCode) {
        if (overline > 0) {
            return Judge::codeForbidOverline;
        }
        if (four >= 2) {
            return Judge::codeForbid44;
        }
        if (liveThree >= 2) {
            return Judge::codeForbid33;
        }
    }

    // 左&右
    a1 = 0, b1 = 0, c1 = 0, a2 = 0, b2 = 0, c2 = 0;
    counter(player, chessBoard, xPos, yPos, 2, a1, b1, c1);
    counter(player, chessBoard, xPos, yPos, 3, a2, b2, c2);
    check_single_direction(player, chessBoard, a1, b1, c1, a2, b2, c2, liveThree, four, overline, five);
    if (five > 0) {
        return Judge::codeWin;
    }
    if (player->getPlayerChessPieceType() == BlackChessPiece::pieceTypeCode) {
        if (overline > 0) {
            return Judge::codeForbidOverline;
        }
        if (four >= 2) {
            return Judge::codeForbid44;
        }
        if (liveThree >= 2) {
            return Judge::codeForbid33;
        }
    }

    // 左上&右下
    a1 = 0, b1 = 0, c1 = 0, a2 = 0, b2 = 0, c2 = 0;
    counter(player, chessBoard, xPos, yPos, 4, a1, b1, c1);
    counter(player, chessBoard, xPos, yPos, 5, a2, b2, c2);
    check_single_direction(player, chessBoard, a1, b1, c1, a2, b2, c2, liveThree, four, overline, five);
    if (five > 0) {
        return Judge::codeWin;
    }
    if (player->getPlayerChessPieceType() == BlackChessPiece::pieceTypeCode) {
        if (overline > 0) {
            return Judge::codeForbidOverline;
        }
        if (four >= 2) {
            return Judge::codeForbid44;
        }
        if (liveThree >= 2) {
            return Judge::codeForbid33;
        }
    }

    // 右上&左下
    a1 = 0, b1 = 0, c1 = 0, a2 = 0, b2 = 0, c2 = 0;
    counter(player, chessBoard, xPos, yPos, 6, a1, b1, c1);
    counter(player, chessBoard, xPos, yPos, 7, a2, b2, c2);
    check_single_direction(player, chessBoard, a1, b1, c1, a2, b2, c2, liveThree, four, overline, five);
    if (five > 0) {
        return Judge::codeWin;
    }
    if (player->getPlayerChessPieceType() == BlackChessPiece::pieceTypeCode) {
        if (overline > 0) {
            return Judge::codeForbidOverline;
        }
        if (four >= 2) {
            return Judge::codeForbid44;
        }
        if (liveThree >= 2) {
            return Judge::codeForbid33;
        }
    }

    return Judge::codeNone;
}

void Judge::check_single_direction(Player *player, ChessBoard *chessBoard,
                                   int a1, int b1, int c1, int a2, int b2, int c2,
                                   int &liveThree, int &four, int &overline, int &five) {
    int continuous = a1 + a2 + 1;
    int bc1 = b1 + c1;
    int bc2 = b2 + c2;
    // 达成五连获胜条件
    if (continuous == 5 ||
        (player->getPlayerChessPieceType() == WhiteChessPiece::pieceTypeCode && continuous > 5)) {
        five++;
        return;
    }
    // 黑子潜在禁手特判
    if (player->getPlayerChessPieceType() == BlackChessPiece::pieceTypeCode) {
        // 长连
        if (continuous > 5) {
            overline++;
            return;
        }
        // TODO: 活三
        if (b1 != 0 && b2 != 0 && (continuous + bc1 == 4 || continuous + bc2 == 4)) {
            liveThree++;
        }
        // 四
        if (c1 == 1 && c2 == 1) {  // 特殊情况：同一条线可能同时出现两个不同的四
            if (continuous + bc1 == 5) {
                four++;
            }
            if (continuous + bc2 == 5) {
                four++;
            }
        } else {
            if (continuous + bc1 == 5 || continuous + bc2 == 5) {
                four++;
            }
        }
    }
}

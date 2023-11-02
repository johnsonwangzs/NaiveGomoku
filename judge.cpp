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

bool Judge::check_is_win(Player *player, ChessBoard *chessBoard, int xPos, int yPos) {
    // 对于执黑子的，只有严格等于5连才算赢
    if (player->getPlayerChessPieceType() == BlackChessPiece::pieceTypeCode) {
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
    }
    // 对于执白子的，只要大于等于5连都算赢
    if (player->getPlayerChessPieceType() == WhiteChessPiece::pieceTypeCode) {
        if (cnt_same_piece_vertical(player, chessBoard, xPos, yPos) >= 5) {
            return true;
        }
        if (cnt_same_piece_horizontal(player, chessBoard, xPos, yPos) >= 5) {
            return true;
        }
        if (cnt_same_piece_upright(player, chessBoard, xPos, yPos) >= 5) {
            return true;
        }
        if (cnt_same_piece_upleft(player, chessBoard, xPos, yPos) >= 5) {
            return true;
        }
    }
    return false;
}

bool Judge::check_forbidden_overline(Player *player, ChessBoard *chessBoard, int xPos, int yPos) {
    int cntVertical, cntHorizontal, cntUpright, cntUpleft;  // 四条线上棋子的计数器
    bool existEqualFive = false, existMoreThanFive = false;  // 记录是否有方向出现恰好五连或大于等于五连
    cntVertical = cnt_same_piece_vertical(player, chessBoard, xPos, yPos);
    cntHorizontal = cnt_same_piece_horizontal(player, chessBoard, xPos, yPos);
    cntUpright = cnt_same_piece_upright(player, chessBoard, xPos, yPos);
    cntUpleft = cnt_same_piece_upleft(player, chessBoard, xPos, yPos);
//    cout << cntVertical << " " << cntHorizontal << " " << cntUpright << " " << cntUpleft << endl;
    if (cntVertical == 5 || cntHorizontal == 5 || cntUpright == 5 || cntUpleft == 5) {
        existEqualFive = true;
    }
    if (cntVertical > 5 || cntHorizontal > 5 || cntUpright > 5 || cntUpleft > 5) {
        existMoreThanFive = true;
    }
    if (!existEqualFive && existMoreThanFive) {  // 没有五连，但有长连（形成5个以上黑子）
        return true;
    }
    return false;
}

bool Judge::check_forbidden_double_three(ChessBoard *chessBoard, int xPos, int yPos) {
    int cntOpenThree = 0;  // 记录在多少个方向上出现了活三
    // TODO
    if (check_open_three_vertical(chessBoard, xPos, yPos)) {
        cntOpenThree++;
    }
//    if (check_open_three_horizontal(chessBoard, xPos, yPos)) {
//        cntOpenThree++;
//    }
//    if (check_open_three_upright(chessBoard, xPos, yPos)) {
//        cntOpenThree++;
//    }
//    if (check_open_three_upleft(chessBoard, xPos, yPos)) {
//        cntOpenThree++;
//    }
    if (cntOpenThree >= 2) {  // 存在两个以上的活三
        return true;
    }
    return false;
}

bool Judge::check_forbidden_double_four(ChessBoard *chessBoard, int xPos, int yPos) {
    int cntFour = 0;  // 记录在多少个方向上出现了四（活四、连冲四、跳冲四）
    // TODO
    cntFour += check_four_vertical(chessBoard, xPos, yPos);
    if (cntFour >= 2) {  // 即使在同一条线上，在落子时也可能导致2个四同时出现
        return true;
    }
    cntFour += check_four_horizontal(chessBoard, xPos, yPos);
    if (cntFour >= 2) {
        return true;
    }
    cntFour += check_four_upright(chessBoard, xPos, yPos);
    if (cntFour >= 2) {
        return true;
    }
    cntFour += check_four_upleft(chessBoard, xPos, yPos);
    if (cntFour >= 2) {
        return true;
    }
    return false;
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
    int singleDirectionPieceCnt = 1;
    // 向上
    int posOffset = 0;
    while (true) {
        posOffset++;
        if (xPos - posOffset >= 0
            && chessBoard->chessPieceInBoard[xPos - posOffset][yPos] == player->getPlayerChessPieceType()) {
            singleDirectionPieceCnt++;
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
            singleDirectionPieceCnt++;
            continue;
        }
        break;
    }
    return singleDirectionPieceCnt;
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
    int singleDirectionPieceCnt = 1;
    // 左上
    int posOffset = 0;
    while (true) {
        posOffset++;
        if (xPos - posOffset >= 0 && yPos - posOffset >= 0
            && chessBoard->chessPieceInBoard[xPos - posOffset][yPos - posOffset] == player->getPlayerChessPieceType()) {
            singleDirectionPieceCnt++;
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
            singleDirectionPieceCnt++;
            continue;
        }
        break;
    }
    return singleDirectionPieceCnt;
}

bool Judge::check_open_three_vertical(ChessBoard *chessBoard, int xPos, int yPos) {
    // TODO
    int singleDirectionPieceCnt = 1;
    int posOffset = 0;
//    while (true) {
//        posOffset++;
//        if ()
//    }
    return false;
}

int Judge::check_four_vertical(ChessBoard *chessBoard, int xPos, int yPos) {
    // TODO
    int fourCnt = 0;
    // 检查活四
    if (!check_piece_at_pos(chessBoard, xPos + 2, yPos, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos + 1, yPos, BlankPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos - 1, yPos, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos - 2, yPos, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos - 3, yPos, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos - 4, yPos, BlankPiece::pieceTypeCode) &&
        !check_piece_at_pos(chessBoard, xPos - 5, yPos, BlackChessPiece::pieceTypeCode)) {
        fourCnt++; // 棋型1：^ 0 * * * X 0 ^
    }
    if (!check_piece_at_pos(chessBoard, xPos + 3, yPos, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos + 2, yPos, BlankPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos + 1, yPos, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos - 1, yPos, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos - 2, yPos, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos - 3, yPos, BlankPiece::pieceTypeCode) &&
        !check_piece_at_pos(chessBoard, xPos - 4, yPos, BlackChessPiece::pieceTypeCode)) {
        fourCnt++;  // 棋型2：^ 0 * * X * 0 ^
    }
    if (!check_piece_at_pos(chessBoard, xPos + 4, yPos, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos + 3, yPos, BlankPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos + 2, yPos, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos + 1, yPos, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos - 1, yPos, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos - 2, yPos, BlankPiece::pieceTypeCode) &&
        !check_piece_at_pos(chessBoard, xPos - 3, yPos, BlackChessPiece::pieceTypeCode)) {
        fourCnt++;  // 棋型3：^ 0 * X * * 0 ^
    }
    if (!check_piece_at_pos(chessBoard, xPos + 5, yPos, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos + 4, yPos, BlankPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos + 3, yPos, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos + 2, yPos, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos + 1, yPos, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos - 1, yPos, BlankPiece::pieceTypeCode) &&
        !check_piece_at_pos(chessBoard, xPos - 2, yPos, BlackChessPiece::pieceTypeCode)) {
        fourCnt++;  // 棋型4：^ 0 X * * * 0 ^
    }
    // 检查连冲四

    // 检查跳冲四
    return fourCnt;
}

int Judge::check_four_horizontal(ChessBoard *chessBoard, int xPos, int yPos) {
    // TODO
    int fourCnt = 0;
    // 检查活四
    if (!check_piece_at_pos(chessBoard, xPos, yPos + 2, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos, yPos + 1, BlankPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos, yPos - 1, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos, yPos - 2, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos, yPos - 3, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos, yPos - 4, BlankPiece::pieceTypeCode) &&
        !check_piece_at_pos(chessBoard, xPos, yPos - 5, BlackChessPiece::pieceTypeCode)) {
        fourCnt++; // 棋型1：^ 0 * * * X 0 ^
    }
    if (!check_piece_at_pos(chessBoard, xPos, yPos + 3, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos, yPos + 2, BlankPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos, yPos + 1, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos, yPos - 1, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos, yPos - 2, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos, yPos - 3, BlankPiece::pieceTypeCode) &&
        !check_piece_at_pos(chessBoard, xPos, yPos - 4, BlackChessPiece::pieceTypeCode)) {
        fourCnt++;  // 棋型2：^ 0 * * X * 0 ^
    }
    if (!check_piece_at_pos(chessBoard, xPos, yPos + 4, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos, yPos + 3, BlankPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos, yPos + 2, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos, yPos + 1, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos, yPos - 1, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos, yPos - 2, BlankPiece::pieceTypeCode) &&
        !check_piece_at_pos(chessBoard, xPos, yPos - 3, BlackChessPiece::pieceTypeCode)) {
        fourCnt++;  // 棋型3：^ 0 * X * * 0 ^
    }
    if (!check_piece_at_pos(chessBoard, xPos, yPos + 5, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos, yPos + 4, BlankPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos, yPos + 3, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos, yPos + 2, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos, yPos + 1, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos, yPos - 1, BlankPiece::pieceTypeCode) &&
        !check_piece_at_pos(chessBoard, xPos, yPos - 2, BlackChessPiece::pieceTypeCode)) {
        fourCnt++;  // 棋型4：^ 0 X * * * 0 ^
    }
    // 检查连冲四

    // 检查跳冲四
    return fourCnt;
}

int Judge::check_four_upright(ChessBoard *chessBoard, int xPos, int yPos) {
    // TODO
    int fourCnt = 0;
    // 检查活四
    if (!check_piece_at_pos(chessBoard, xPos - 2, yPos + 2, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos - 1, yPos + 1, BlankPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos + 1, yPos - 1, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos + 2, yPos - 2, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos + 3, yPos - 3, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos + 4, yPos - 4, BlankPiece::pieceTypeCode) &&
        !check_piece_at_pos(chessBoard, xPos + 5, yPos - 5, BlackChessPiece::pieceTypeCode)) {
        fourCnt++; // 棋型1：^ 0 * * * X 0 ^
    }
    if (!check_piece_at_pos(chessBoard, xPos - 3, yPos + 3, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos - 2, yPos + 2, BlankPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos - 1, yPos + 1, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos + 1, yPos - 1, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos + 2, yPos - 2, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos + 3, yPos - 3, BlankPiece::pieceTypeCode) &&
        !check_piece_at_pos(chessBoard, xPos + 4, yPos - 4, BlackChessPiece::pieceTypeCode)) {
        fourCnt++;  // 棋型2：^ 0 * * X * 0 ^
    }
    if (!check_piece_at_pos(chessBoard, xPos - 4, yPos + 4, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos - 3, yPos + 3, BlankPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos - 2, yPos + 2, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos - 1, yPos + 1, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos + 1, yPos - 1, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos + 2, yPos - 2, BlankPiece::pieceTypeCode) &&
        !check_piece_at_pos(chessBoard, xPos + 3, yPos - 3, BlackChessPiece::pieceTypeCode)) {
        fourCnt++;  // 棋型3：^ 0 * X * * 0 ^
    }
    if (!check_piece_at_pos(chessBoard, xPos - 5, yPos + 5, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos - 4, yPos + 4, BlankPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos - 3, yPos + 3, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos - 2, yPos + 2, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos - 1, yPos + 1, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos + 1, yPos - 1, BlankPiece::pieceTypeCode) &&
        !check_piece_at_pos(chessBoard, xPos + 2, yPos - 2, BlackChessPiece::pieceTypeCode)) {
        fourCnt++;  // 棋型4：^ 0 X * * * 0 ^
    }
    // 检查连冲四

    // 检查跳冲四
    return fourCnt;
}

int Judge::check_four_upleft(ChessBoard *chessBoard, int xPos, int yPos) {
    // TODO
    int fourCnt = 0;
    // 检查活四
    if (!check_piece_at_pos(chessBoard, xPos + 2, yPos + 2, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos + 1, yPos + 1, BlankPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos - 1, yPos - 1, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos - 2, yPos - 2, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos - 3, yPos - 3, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos - 4, yPos - 4, BlankPiece::pieceTypeCode) &&
        !check_piece_at_pos(chessBoard, xPos - 5, yPos - 5, BlackChessPiece::pieceTypeCode)) {
        fourCnt++; // 棋型1：^ 0 * * * X 0 ^
    }
    if (!check_piece_at_pos(chessBoard, xPos + 3, yPos + 3, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos + 2, yPos + 2, BlankPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos + 1, yPos + 1, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos - 1, yPos - 1, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos - 2, yPos - 2, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos - 3, yPos - 3, BlankPiece::pieceTypeCode) &&
        !check_piece_at_pos(chessBoard, xPos - 4, yPos - 4, BlackChessPiece::pieceTypeCode)) {
        fourCnt++;  // 棋型2：^ 0 * * X * 0 ^
    }
    if (!check_piece_at_pos(chessBoard, xPos + 4, yPos + 4, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos + 3, yPos + 3, BlankPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos + 2, yPos + 2, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos + 1, yPos + 1, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos - 1, yPos - 1, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos - 2, yPos - 2, BlankPiece::pieceTypeCode) &&
        !check_piece_at_pos(chessBoard, xPos - 3, yPos - 3, BlackChessPiece::pieceTypeCode)) {
        fourCnt++;  // 棋型3：^ 0 * X * * 0 ^
    }
    if (!check_piece_at_pos(chessBoard, xPos + 5, yPos + 5, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos + 4, yPos + 4, BlankPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos + 3, yPos + 3, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos + 2, yPos + 2, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos + 1, yPos + 1, BlackChessPiece::pieceTypeCode) &&
        check_piece_at_pos(chessBoard, xPos - 1, yPos - 1, BlankPiece::pieceTypeCode) &&
        !check_piece_at_pos(chessBoard, xPos - 2, yPos - 2, BlackChessPiece::pieceTypeCode)) {
        fourCnt++;  // 棋型4：^ 0 X * * * 0 ^
    }
    // 检查连冲四

    // 检查跳冲四
    return fourCnt;
}

bool Judge::check_piece_at_pos(ChessBoard *chessBoard, int xPos, int yPos, int targetPieceTypeCode) {
    if (is_pos_in_range(xPos) && is_pos_in_range(yPos)) {  // 在棋盘上（没出边界）
        if (chessBoard->chessPieceInBoard[xPos][yPos] == targetPieceTypeCode) {
            return true;
        }
    }
    return false;
}

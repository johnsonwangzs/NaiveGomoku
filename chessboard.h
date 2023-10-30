#ifndef NAIVEGOMOKU_CHESSBOARD_H
#define NAIVEGOMOKU_CHESSBOARD_H

#include <iostream>
#include <cstring>

using namespace std;

// 棋子
class ChessPiece {
public:
    string pieceTypeName;
    string pieceTypeCode;
};

// 黑色棋子
class BlackChessPiece : public ChessPiece {
public:
    static string pieceTypeName;
    static string figure[2];
    static int pieceTypeCode;
};

// 白色棋子
class WhiteChessPiece : public ChessPiece {
public:
    static string pieceTypeName;
    static string figure[2];
    static int pieceTypeCode;
};

// 空位
class BlankPiece : public ChessPiece {
public:
    static string pieceTypeName;
    static string figure;
    static int pieceTypeCode;
};

// 棋盘
class ChessBoard {
private:
    string columnName[15] = {"A", "B", "C", "D", "E", "F", "G", "H",
                             "I", "J", "K", "L", "M", "N", "O"};
    string rowName[15] = {"15", "14", "13", "12", "11", "10", "9", "8",
                          "7", "6", "5", "4", "3", "2", "1"};
public:
    ChessBoard();

    ~ChessBoard();

    int boardStyle = 0;  // 棋盘样式（小/大，玩家自选）
    int chessPieceInBoard[15][15] = {0};  // 记录棋盘上每个位置是什么旗子

    void choose_board_style();  // 选择棋盘样式
    void print_single_chess_piece(int x, int y);  // 在棋盘上打印单个棋子
    void create_new_chessboard();  // 产生新棋盘
    void show();  // 打印当前棋盘
    void set_new_chess_piece(int chessPieceType, int xPos, int yPos);  // 落子
};

#endif //NAIVEGOMOKU_CHESSBOARD_H

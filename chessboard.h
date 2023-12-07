#ifndef NAIVEGOMOKU_CHESSBOARD_H
#define NAIVEGOMOKU_CHESSBOARD_H

#include <iostream>
#include <cstring>
#include <cfloat>
#include <vector>

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

    int boardStyle;  // 棋盘样式（小/大，玩家自选）
    int chessPieceInBoard[15][15] = {0};  // 记录棋盘上每个位置是什么旗子
    int totalChessPieceCnt;  // 当前棋盘上的棋子总数

    void choose_board_style();  // 选择棋盘样式
    void print_single_chess_piece(int x, int y);  // 在棋盘上打印单个棋子
    void create_new_chessboard();  // 产生新棋盘
    void show();  // 打印当前棋盘
    void set_new_chess_piece(int chessPieceType, int xPos, int yPos);  // 落子
};

// 棋盘节点（蒙特卡洛搜索树）
class ChessBoardNode {
public:
    ChessBoardNode();

    ~ChessBoardNode();

    int chessPieceInBoard[15][15] = {0};  // 记录棋盘上每个位置是什么棋子
    int curPieceXPos;  // 当前棋盘比父节点多的棋子的位置 x坐标
    int curPieceYPos;  // 当前棋盘比父节点多的棋子的位置 y坐标
    int curPieceType;  // 当前棋盘比父节点多的棋子的类型
    bool terminalState;  // 当前节点是否是终止棋盘状态
    int totalChessPieceCnt;  // 当前棋盘上的棋子总数
    double value;  // 节点价值
    double cntVisited;  // 节点被探索次数
    double UCB;  // 节点UCB值
    ChessBoardNode *parentNode;  // 父节点
    vector<ChessBoardNode*> childNode;  // 子节点
};

// 蒙特卡罗搜索树rollout节点
class RolloutNode {
public:
    RolloutNode();
    ~RolloutNode();
    int chessPieceInBoard[15][15] = {0};  // 记录棋盘上每个位置是什么棋子
    int curPieceXPos;  // 当前棋盘比父节点多的棋子的位置 x坐标
    int curPieceYPos;  // 当前棋盘比父节点多的棋子的位置 y坐标
    int curPieceType;  // 当前棋盘比父节点多的棋子的类型
    int totalChessPieceCnt;  // 当前棋盘上的棋子总数
    RolloutNode *parentNode;  // 父节点
    RolloutNode *childNode;  // 子节点
};

#endif //NAIVEGOMOKU_CHESSBOARD_H

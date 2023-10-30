#include <iostream>
#include "chessboard.h"

string BlackChessPiece::pieceTypeName = "黑子";
string BlackChessPiece::figure[2] = {"▪", "●"};
int BlackChessPiece::pieceTypeCode = 1;

string WhiteChessPiece::pieceTypeName = "白子";
string WhiteChessPiece::figure[2] = {"▫", "○"};
int WhiteChessPiece::pieceTypeCode = 2;

string BlankPiece::pieceTypeName = "空位";
string BlankPiece::figure = "·";
int BlankPiece::pieceTypeCode = 0;

ChessBoard::ChessBoard() {
    cout << "> 正在构造棋盘..." << endl;
    create_new_chessboard();
}

ChessBoard::~ChessBoard() {
    cout << "> 正在销毁本局棋盘..." << endl;
}

void ChessBoard::create_new_chessboard() {
    memset(chessPieceInBoard, 0, sizeof(chessPieceInBoard));
    choose_board_style();
    cout << "> 棋盘创建完毕！" << endl;
}

void ChessBoard::choose_board_style() {
    string myStyle;
    while (true) {
        cout << "> 请选择棋盘样式：\n"
                "  0. 小棋盘(recommended)\n"
                "  1. 大棋盘" << endl;
        cin >> myStyle;
        if (strcmp(myStyle.c_str(), "0") == 0 || strcmp(myStyle.c_str(), "1") == 0) {
            boardStyle = stoi(myStyle);
            break;
        } else
            cout << "非法输入！请重新选择（0或1）" << endl;
    }
}

void ChessBoard::print_single_chess_piece(int x, int y) {
    switch (chessPieceInBoard[x][y]) {
        case 1:  // 黑棋：1
            cout << BlackChessPiece::figure[boardStyle];
            break;
        case 2:  // 白棋：2
            cout << WhiteChessPiece::figure[boardStyle];
            break;
        default:  // 空位：0
            cout << BlankPiece::figure;
            break;
    }
}

void ChessBoard::show() {
    cout << endl << "   "
         << BlackChessPiece::pieceTypeName << ": " << BlackChessPiece::figure[boardStyle] << "    "
         << WhiteChessPiece::pieceTypeName << ": " << WhiteChessPiece::figure[boardStyle] << "    "
         << BlankPiece::pieceTypeName << ": " << BlankPiece::figure << endl;
    if (boardStyle == 0) {
        cout << "   ";
        for (int i = 0; i < 15; i++)
            cout << columnName[i] << " ";
        for (int i = 0; i < 15; i++) {
            cout << endl;
            if (i >= 6) cout << " " << rowName[i] << " ";
            else cout << rowName[i] << " ";
            // 打印棋子
            for (int j = 0; j < 15; j++) {
                print_single_chess_piece(i, j);
                cout << " ";
            }
            cout << rowName[i];
        }
        cout << endl << "   ";
        for (int i = 0; i < 15; i++)
            cout << columnName[i] << " ";
        cout << endl;
    }
    if (boardStyle == 1) {
        cout << "   ";
        for (int i = 0; i < 15; i++)
            cout << columnName[i] << "   ";
        for (int i = 0; i < 15; i++) {
            cout << endl;
            if (i >= 6) cout << " ";
            cout << rowName[i] << " ";
            for (int j = 0; j < 15; j++) {
                print_single_chess_piece(i, j);
                if (j != 14)
                    cout << "---";
            }
            cout << " " << rowName[i] << endl << "   ";
            for (int j = 0; j < 15; j++) {
                if (i != 14) {
                    if (j != 14) cout << "|   ";
                    else cout << "|";
                }
            }
        }
        for (int i = 0; i < 15; i++)
            cout << columnName[i] << "   ";
        cout << endl;
    }
    cout << endl;
}

void ChessBoard::set_new_chess_piece(int chessPieceType, int xPos, int yPos) {
    chessPieceInBoard[xPos][yPos] = chessPieceType;
}
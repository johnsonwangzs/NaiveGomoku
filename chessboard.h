#ifndef NAIVEGOMOKU_CHESSBOARD_H
#define NAIVEGOMOKU_CHESSBOARD_H

#include <cstring>

using namespace std;

// 棋子
class ChessPiece {
public:
    string pieceTypeName;
    string pieceTypeCode;
};

// 黑色棋子
class BlackChessPiece : ChessPiece {
public:
    static string pieceTypeName;
    static string figure[2];
    const int pieceTypeCode = 1;
};

string BlackChessPiece::pieceTypeName = "黑子";
string BlackChessPiece::figure[2] = {"▪", "●"};

// 白色棋子
class WhiteChessPiece : ChessPiece {
public:
    static string pieceTypeName;
    static string figure[2];
    const int pieceTypeCode = 2;
};

string WhiteChessPiece::pieceTypeName = "白子";
string WhiteChessPiece::figure[2] = {"▫", "○"};

// 空位
class BlankPiece : ChessPiece {
public:
    static string pieceTypeName;
    static string figure;
    const int pieceTypeCode = 0;
};

string BlankPiece::pieceTypeName = "空位";
string BlankPiece::figure = "·";

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
};

ChessBoard::ChessBoard() {
    cout << "> 正在构造棋盘..." << endl;
    create_new_chessboard();
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
                "  0. 小棋盘(recommend)\n"
                "  1. 大棋盘"  << endl;
        cin >> myStyle;
        if (strcmp(myStyle.c_str(), "0") == 0 || strcmp(myStyle.c_str(), "1") == 0) {
            boardStyle = stoi(myStyle);
            break;
        }
        else
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
    cout << endl << "    "
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

ChessBoard::~ChessBoard() {
    cout << "> 正在销毁本局棋盘..." << endl;
}

#endif //NAIVEGOMOKU_CHESSBOARD_H

#ifndef NAIVEGOMOKU_CHESSBOARD_H
#define NAIVEGOMOKU_CHESSBOARD_H
using namespace std;

// 棋盘
class ChessBoard {
private:
    string columnName[15] = {"A", "B", "C", "D", "E", "F", "G", "H",
                             "I", "J", "K", "L", "M", "N", "O"};
    string rowName[15] = {"15", "14", "13", "12", "11", "10", "9", "8",
                          "7", "6", "5", "4", "3", "2", "1"};
public:
    ChessBoard();

    void show(int board_style);
};

ChessBoard::ChessBoard() {
    cout << "> 已创建棋盘" << endl;
}

// 打印棋盘
void ChessBoard::show(int board_style) {
    if (board_style == 1) {
        cout << "   ";
        for (int i = 0; i < 15; i++)
            cout << columnName[i] << " ";
        for (int i = 0; i < 15; i++) {
            cout << endl;
            if (i >= 6) cout << " " << rowName[i] << " ";
            else cout << rowName[i] << " ";
            for (int j = 0; j < 15; j++) {
                if (j == 3) cout << "▪ ";
                else if (j == 7) cout << "▫ ";
                else cout << "· ";
            }
            cout << rowName[i];
        }
        cout << endl << "   ";
        for (int i = 0; i < 15; i++)
            cout << columnName[i] << " ";
        cout << endl;
    }
    if (board_style == 2) {
        for (int i = 0; i < 15; i++) {
            cout << endl << rowName[i] << " ";
            for (int j = 0; j < 15; j++) {
                if (j != 14) cout << "+---";
                else cout << "+";
            }
            cout << endl << "   ";
            for (int j = 0; j < 15; j++)
                if (i != 14) cout << "|   ";
        }
        for (int i = 0; i < 15; i++)
            cout << columnName[i] << "   ";
        cout << endl;
    }
}

// 棋子
class ChessPiece {

};

// 黑色棋子
class BlackChessPiece : ChessPiece {

};

// 白色棋子
class WhiteChessPiece : ChessPiece {

};

// 空位
class BlankPiece: ChessPiece {

};

#endif //NAIVEGOMOKU_CHESSBOARD_H

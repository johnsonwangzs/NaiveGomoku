#ifndef NAIVEGOMOKU_PLAYER_H
#define NAIVEGOMOKU_PLAYER_H

#include <iostream>

using namespace std;

class Player {
public:
    int playerTypeCode;  // 0：电脑    1：人类
    int playerChessPieceType;  // 1：先手（黑子）   2：后手（白子）
    string playerName;

    virtual int getPlayerTypeCode();

    int getPlayerChessPieceType();

    void setPlayerChessPieceType(int myHand);
};

// 电脑玩家
class BotPlayer : public Player {
public:
    static int playerTypeCode;
    int getPlayerTypeCode() override;
    BotPlayer();

    ~BotPlayer();
};

// 人类玩家
class HumanPlayer : public Player {
public:
    static int playerTypeCode;
    int getPlayerTypeCode() override;
    HumanPlayer();

    ~HumanPlayer();
};

#endif //NAIVEGOMOKU_PLAYER_H

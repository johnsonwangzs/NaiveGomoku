#ifndef NAIVEGOMOKU_PLAYER_H
#define NAIVEGOMOKU_PLAYER_H

#include <iostream>

using namespace std;

class Player {
protected:
    int playerTypeCode;  // 0：电脑    1：人类
    int playerHand;  // 1：先手（黑子）   2：后手（白子）
public:
    string playerName;

    int getPlayerTypeCode();

    int getPlayerHand();

    void setPlayerHand(int myHand);
};

// 电脑玩家
class BotPlayer : public Player {
public:
    BotPlayer();

    ~BotPlayer();
};

// 人类玩家
class HumanPlayer : public Player {
public:
    HumanPlayer();

    ~HumanPlayer();
};

#endif //NAIVEGOMOKU_PLAYER_H

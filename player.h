#ifndef NAIVEGOMOKU_PLAYER_H
#define NAIVEGOMOKU_PLAYER_H

using namespace std;

class Player {
protected:
    int playerTypeCode;  // 0：电脑   1：人类
    int playerHand;  // 1：先手（黑子）   2：后手（白子）
public:
    string playerName;
};

// 电脑玩家
class BotPlayer : public Player {
private:
    int playerTypeCode = 0;  // 代码
public:
    BotPlayer();
    ~BotPlayer();
    int getPlayerTypeCode() {
        return playerTypeCode;
    }
    int getPlayerHand() {
        return playerHand;
    }
    void setPlayerHand(int myHand) {
        playerHand = myHand;
    }
};

// 人类玩家
class HumanPlayer : public Player {
private:
    int playerTypeCode = 1;  // 代码
public:
    HumanPlayer();
    ~HumanPlayer();
    int getPlayerTypeCode() {
        return playerTypeCode;
    }
    int getPlayerHand() {
        return playerHand;
    }
    void setPlayerHand(int myHand) {
        playerHand = myHand;
    }
};

BotPlayer::BotPlayer() {
    playerName = "电脑玩家";
    cout << "> 正在创建电脑玩家..." << endl;
}

BotPlayer::~BotPlayer() {
    cout << "> 正在销毁本局电脑玩家..." << endl;
}

HumanPlayer::HumanPlayer() {
    playerName = "人类玩家";
    cout << "> 正在创建人类玩家..." << endl;
}

HumanPlayer::~HumanPlayer() {
    cout << "> 正在销毁本局人类玩家..." << endl;
}

#endif //NAIVEGOMOKU_PLAYER_H

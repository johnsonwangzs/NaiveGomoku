#include <iostream>
#include "player.h"

int Player::getPlayerTypeCode() {
    return playerTypeCode;
}

int Player::getPlayerHand() {
    return playerHand;
}

void Player::setPlayerHand(int myHand) {
    playerHand = myHand;
}

BotPlayer::BotPlayer() {
    playerName = "电脑玩家";
    playerTypeCode = 0;
    cout << "> 正在创建电脑玩家..." << endl;
}

BotPlayer::~BotPlayer() {
    cout << "> 正在销毁本局电脑玩家..." << endl;
}

HumanPlayer::HumanPlayer() {
    playerName = "人类玩家";
    playerTypeCode = 1;
    cout << "> 正在创建人类玩家..." << endl;
}

HumanPlayer::~HumanPlayer() {
    cout << "> 正在销毁本局人类玩家..." << endl;
}
#include <iostream>
#include "player.h"

int Player::getPlayerTypeCode() {
    return playerTypeCode;
}

int BotPlayer::getPlayerTypeCode() {
    return playerTypeCode;
}

int HumanPlayer::getPlayerTypeCode() {
    return playerTypeCode;
}

int Player::getPlayerChessPieceType() {
    return playerChessPieceType;
}

void Player::setPlayerChessPieceType(int myHand) {
    playerChessPieceType = myHand;
}

int BotPlayer::playerTypeCode = 0;
int HumanPlayer::playerTypeCode = 1;

BotPlayer::BotPlayer() {
    playerName = "电脑玩家";
    cout << "> 正在创建电脑玩家..." << endl;
}

BotPlayer::~BotPlayer() {
    cout << "> 正在销毁本局电脑玩家..." << endl;
}

void BotPlayer::decide_next_step(ChessBoard *chessBoard, int &xPos, int &yPos) {
    return;
}

HumanPlayer::HumanPlayer() {
    playerName = "人类玩家";
    cout << "> 正在创建人类玩家..." << endl;
}

HumanPlayer::~HumanPlayer() {
    cout << "> 正在销毁本局人类玩家..." << endl;
}
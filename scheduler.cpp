#include <cstring>
#include "utils.h"
#include "chessboard.h"
#include "player.h"
#include "judge.h"
#include "scheduler.h"


Scheduler::Scheduler() {
    gameCnt = 0;
    botWinCnt = 0;
    humanWinCnt = 0;
    drawCnt = 0;
    cout << "> 正在创建调度器..." << endl;
    schedule_game();
}

Scheduler::~Scheduler() {
    cout << "\n--------------------------------\n"
            "> 正在关闭调度器..." << endl;
}

void Scheduler::schedule_game() {
    cout << "> 调度器工作中，游戏开始！" << endl;
    while (true) {
        int op = choose_game_option();
        if (op == 1) {
            start_new_game();
        } else if (op == 2) {
            show_game_score();
        } else if (op == 3) {
            end_game();
            break;
        }
    }
}

int Scheduler::choose_game_option() {
    string op;  // 注意此处即使输入为数字，也一定要以字符串方式读入
    while (true) {
        cout << "\n--------------------------------\n"
                "> 从以下选项中选择：\n"
                "  1. 开始新比赛\n"
                "  2. 查看累计分数\n"
                "  3. 结束比赛" << endl;
        cin >> op;
        if (strcmp(op.c_str(), "1") == 0 || strcmp(op.c_str(), "2") == 0 || strcmp(op.c_str(), "3") == 0)
            return stoi(op);
        else
            cout << "非法输入！请重新选择（1、2或3）" << endl;
    }
}

void Scheduler::start_new_game() {
    cout << "> 正在创建新比赛..." << endl;
    gameCnt++;

    // 创建当前对局的棋盘对象
    auto *chessBoard = new ChessBoard();
    cout << "> 初始棋盘：" << endl;
    chessBoard->show();

    // 创建裁判对象
    auto *judge = new Judge();

    // 创建玩家
    auto *botPlayer = new BotPlayer();
    auto *humanPlayer = new HumanPlayer();

    // 对局过程
    cout << "\n> 本局比赛开始！" << endl;

    // 决定两方谁为先手，谁为后手
    decide_first_hand(botPlayer, humanPlayer, judge);

    // 第一次：黑方必须在天元8H落子
    set_first_chess_piece(botPlayer, humanPlayer, judge, chessBoard);
    chessBoard->totalChessPieceCnt++;

    // 连续回合
    while (true) {
        // 检查是否平局
        if (chessBoard->totalChessPieceCnt == 225) {  // 检查是否平局
            drawCnt++;
            judge->claim_draw();
            break;
        }
        // 检查上一步（人类玩家）是否能赢/违规
        if (lastStepPlayer == humanPlayer->getPlayerTypeCode()) {
            int resCode;
            resCode = judge->check_last_step(humanPlayer, chessBoard, lastStepXPos, lastStepYPos);
            if (resCode == Judge::codeWin) {
                humanWinCnt++;
                judge->claim_winner(humanPlayer, "人类玩家率先达成五连！");
                break;
            }
            if (resCode == Judge::codeForbidOverline) {
                botWinCnt++;
                judge->claim_winner(botPlayer, "人类玩家犯规：长连禁手");
                break;
            }
            if (resCode == Judge::codeForbid44) {
                botWinCnt++;
                judge->claim_winner(botPlayer, "人类玩家犯规：四四禁手！");
                break;
            }
            if (resCode == Judge::codeForbid33) {
                botWinCnt++;
                judge->claim_winner(botPlayer, "人类玩家犯规：三三禁手！");
                break;
            }
        }
        // 检查上一步（电脑玩家）是否能赢/违规
        if (lastStepPlayer == botPlayer->getPlayerTypeCode()) {
            int resCode;
            resCode = judge->check_last_step(botPlayer, chessBoard, lastStepXPos, lastStepYPos);
            if (resCode == Judge::codeWin) {
                botWinCnt++;
                judge->claim_winner(botPlayer, "电脑率先达成五连！");
                break;
            }
        }

        // 落子
        if (nextStepPlayer == humanPlayer->getPlayerTypeCode()) {  // 当前轮到人类玩家落子
            set_human_player_chess_piece(humanPlayer, judge, chessBoard);
            chessBoard->totalChessPieceCnt++;
            lastStepPlayer = nextStepPlayer;
            nextStepPlayer = botPlayer->getPlayerTypeCode();
            continue;
        }
        if (nextStepPlayer == botPlayer->getPlayerTypeCode()) {  // 当前轮到电脑落子
            int x = 0, y = 0;
            botPlayer->decide_next_step(chessBoard, x, y, lastStepXPos, lastStepYPos);
            set_bot_player_chess_piece(botPlayer, judge, chessBoard, x, y);
            lastStepPlayer = nextStepPlayer;
            nextStepPlayer = humanPlayer->getPlayerTypeCode();
        }
    }

    show_game_score();

    delete humanPlayer;
    delete botPlayer;
    delete judge;
    delete chessBoard;
    cout << "> 本局比赛结束！" << endl;
}

void Scheduler::decide_first_hand(BotPlayer *botPlayer, HumanPlayer *humanPlayer, Judge *judge) {
    cout << "\n> 现在决定谁为先手..." << endl;
    // 让玩家选择先手或后手
    int firstHand = 0;
    string handInput;
    while (true) {
        cout << "  输入先手（1）/后手（2）：";
        cin >> handInput;
        // 判断输入合法性
        if (!test_integer(handInput) || stoi(handInput) < 1 || stoi(handInput) > 2) {
            cout << "  输入非法！需输入1或2！" << endl;
            continue;
        }
        break;
    }
    if (handInput == "1") {
        firstHand = 1;
    } else {
        firstHand = 0;
    }

    if (firstHand == humanPlayer->getPlayerTypeCode()) {  // 1：人类玩家为先手
        judge->whoIsFirstHand = humanPlayer->getPlayerTypeCode();
        botPlayer->setPlayerChessPieceType(WhiteChessPiece::pieceTypeCode);  // 电脑执白子
        humanPlayer->setPlayerChessPieceType(BlackChessPiece::pieceTypeCode);  // 人类玩家执黑子
        cout << "> 人类玩家为先手，电脑为后手！" << endl;
    } else if (firstHand == botPlayer->getPlayerTypeCode()) { // 0：电脑为先手，执黑子
        judge->whoIsFirstHand = botPlayer->getPlayerTypeCode();
        botPlayer->setPlayerChessPieceType(BlackChessPiece::pieceTypeCode);
        humanPlayer->setPlayerChessPieceType(WhiteChessPiece::pieceTypeCode);
        cout << "> 电脑为先手，人类玩家为后手！" << endl;
    }
}

void
Scheduler::set_first_chess_piece(BotPlayer *botPlayer, HumanPlayer *humanPlayer, Judge *judge, ChessBoard *chessBoard) {
    if (judge->whoIsFirstHand == botPlayer->getPlayerTypeCode()) {  // 电脑为先手
        int x, y;
        if (!transform_position_name("8", "H", x, y)) {
            cout << "> Error：棋盘无法落子！" << endl;
            return;
        }
        if (judge->judge_first_action_validity(x, y)) {
            chessBoard->set_new_chess_piece(botPlayer->getPlayerChessPieceType(), x, y);
            chessBoard->show();
            lastStepPlayer = botPlayer->getPlayerTypeCode();
            nextStepPlayer = humanPlayer->getPlayerTypeCode();
        }
    } else if (judge->whoIsFirstHand == humanPlayer->getPlayerTypeCode()) {  // 人类玩家为先手
        int x, y;
        while (true) {
            string xInput, yInput;
            cout << "> 人类玩家为先手，请输入落子位置（行号（数字）和列号（字母），用空格隔开）：";
            cin >> xInput >> yInput;  // 获取人类玩家键盘输入
            if (transform_position_name(xInput, yInput, x, y)) {
                if (judge->judge_first_action_validity(x, y)) {
                    chessBoard->set_new_chess_piece(humanPlayer->getPlayerChessPieceType(), x, y);
                    chessBoard->show();
                    lastStepPlayer = humanPlayer->getPlayerTypeCode();
                    nextStepPlayer = botPlayer->getPlayerTypeCode();
                    break;
                } else {
                    cout << "  输入位置不合法，先手（黑子）初次落子必须在天元（8H）位置！" << endl;
                }
            }
        }
    }
}

void Scheduler::set_human_player_chess_piece(HumanPlayer *humanPlayer, Judge *judge, ChessBoard *chessBoard) {
    while (true) {
        string xInput, yInput;
        cout << "> 轮到人类玩家，请输入落子位置（行号（数字）和列号（字母），用空格隔开）：";
        cin >> xInput >> yInput;  // 获取人类玩家键盘输入
        int x, y;
        if (transform_position_name(xInput, yInput, x, y)) {
            if (judge->judge_action_validity(humanPlayer, chessBoard, x, y)) {
                chessBoard->set_new_chess_piece(humanPlayer->getPlayerChessPieceType(), x, y);
                cout << "  落子成功！" << endl;
                chessBoard->show();
                lastStepXPos = x;
                lastStepYPos = y;
                break;
            } else {
                cout << "  落子失败！原因：该位置已存在其他棋子！" << endl;
            }
        }
    }
}

void
Scheduler::set_bot_player_chess_piece(BotPlayer *botPlayer, Judge *judge, ChessBoard *chessBoard, int xPos, int yPos) {
    if (!judge->judge_action_validity(botPlayer, chessBoard, xPos, yPos)) {
        cout << "> DESTRUCTIVE BUG：AI落子不合法！" << endl;
        return;
    }
    chessBoard->set_new_chess_piece(botPlayer->getPlayerChessPieceType(), xPos, yPos);
    cout << "  落子成功！" << endl;
    chessBoard->show();
    lastStepXPos = xPos;
    lastStepYPos = yPos;
}

void Scheduler::show_game_score() {
    cout << "> 目前比分为：" << endl
         << "  玩家:电脑 = " << humanWinCnt << ":" << botWinCnt << endl << endl;
}

void Scheduler::end_game() {
    cout << "> 游戏结束！" << endl;
    show_game_score();
}

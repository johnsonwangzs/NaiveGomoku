#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <chrono>
#include "player.h"
#include "utils.h"

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

int rootTotalPieceCnt = 0;

void BotPlayer::decide_next_step(ChessBoard *chessBoard, int &xPos, int &yPos, int lastStepXPos, int lastStepYPos) {
    cout << "\n> 正在计算策略..." << endl;
    auto start = chrono::system_clock::now();
    // 创建根节点
    auto rootChessBoardNode = new ChessBoardNode();
    memcpy(rootChessBoardNode->chessPieceInBoard, chessBoard->chessPieceInBoard,
           sizeof(chessBoard->chessPieceInBoard));
    rootChessBoardNode->curPieceXPos = lastStepXPos;
    rootChessBoardNode->curPieceYPos = lastStepYPos;
    rootChessBoardNode->curPieceType = 3 - playerChessPieceType;  // piece-type: white + black = 3
    rootChessBoardNode->cntVisited = 1.0;
    rootChessBoardNode->UCB = -1;
    rootChessBoardNode->totalChessPieceCnt = chessBoard->totalChessPieceCnt;
    rootTotalPieceCnt = chessBoard->totalChessPieceCnt;
    // 为root节点产生子节点
    int fourCnt = 0, liveThreeCnt = 0;
    auto rolloutTestNode = new RolloutNode();
    rolloutTestNode->curPieceXPos = rootChessBoardNode->curPieceXPos;
    rolloutTestNode->curPieceYPos = rootChessBoardNode->curPieceYPos;
    rolloutTestNode->curPieceType = rootChessBoardNode->curPieceType;
    rolloutTestNode->totalChessPieceCnt = rootChessBoardNode->totalChessPieceCnt;
    memcpy(rolloutTestNode->chessPieceInBoard, rootChessBoardNode->chessPieceInBoard,
           sizeof(rootChessBoardNode->chessPieceInBoard));
    int resCode = is_risky_board(rolloutTestNode, liveThreeCnt, fourCnt);
    delete rolloutTestNode;
    if (liveThreeCnt > 0 || fourCnt > 0) {
        expand_node_safe(rootChessBoardNode);
        if (rootChessBoardNode->childNode.empty()) {
            expand_node(rootChessBoardNode);
        }
    } else {
        expand_node(rootChessBoardNode);  // 扩展子节点
    }
    /* 每个节点维护一个子节点（指针）数组（未访问过节点的初始UCB值为无穷大）
     * 每次选择一个节点作为当前节点并进行一次模拟，模拟后反向传播并更新N和V值
     * 当数组中所有节点都被访问过时，选择其中UCB最大的进行扩展 */
    int iter = 0;
//    while (iter < MCST_MAX_ITER) {
    while (true) {
        if (iter % 10000 == 0) {
            cout << "  正在进行第" << iter << "次迭代..." << endl;
        }
        auto end = chrono::system_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        if ((double(duration.count()) * chrono::microseconds::period::num / chrono::microseconds::period::den) >
            MCST_MAX_SEARCH_TIME || iter > MCST_MAX_ITER) {
            cout << "> 决策完成：总迭代次数" << iter - 1 << endl;
            break;
        }
        int searchDepth = 1;
//        cout << "    搜索深度：" << searchDepth << endl;
        ChessBoardNode *curNode = rootChessBoardNode;
        while (searchDepth < MCST_MAX_SEARCH_DEPTH) {
            // 若curNode不是叶子节点，则将其子节点中UCB值最大的节点作为curNode
            if (!curNode->childNode.empty()) {
                int idx = find_max_ucb(curNode->childNode, curNode->cntVisited);
                curNode = curNode->childNode[idx];
                searchDepth++;
//                cout << "    搜索深度：" << searchDepth << endl;
            } else {  // 若curNode是叶子节点
                // 若已经被探索过（这意味着其兄弟节点也都已经被探索过了；也意味着只有一个节点的子节点全部被探索过后，才可能继续向下扩展）
                if (curNode->cntVisited != 0.0 && !curNode->terminalState) {  // 未探索过的节点不会被扩展
                    fourCnt = 0, liveThreeCnt = 0;
                    auto rolloutTestNode = new RolloutNode();
                    rolloutTestNode->curPieceXPos = curNode->curPieceXPos;
                    rolloutTestNode->curPieceYPos = curNode->curPieceYPos;
                    rolloutTestNode->curPieceType = curNode->curPieceType;
                    rolloutTestNode->totalChessPieceCnt = curNode->totalChessPieceCnt;
                    memcpy(rolloutTestNode->chessPieceInBoard, curNode->chessPieceInBoard,
                           sizeof(curNode->chessPieceInBoard));
                    resCode = is_risky_board(rolloutTestNode, liveThreeCnt, fourCnt);
                    delete rolloutTestNode;
                    if (liveThreeCnt > 0 || fourCnt > 0) {
                        expand_node_safe(curNode);
                        if (curNode->childNode.empty()) {
                            expand_node(curNode);
                        }
                    } else {
                        expand_node(curNode);  // 扩展子节点
                    }
//                    expand_node(curNode);  // 扩展子节点
                    curNode = curNode->childNode[0];
                    searchDepth++;
//                    cout << "    搜索深度：" << searchDepth << endl;
                } else {  // 若还未被探索过，则进行一次模拟
                    auto rolloutNodeRoot = new RolloutNode();
                    rolloutNodeRoot->curPieceXPos = curNode->curPieceXPos;
                    rolloutNodeRoot->curPieceYPos = curNode->curPieceYPos;
                    rolloutNodeRoot->curPieceType = curNode->curPieceType;
                    rolloutNodeRoot->totalChessPieceCnt = curNode->totalChessPieceCnt;
                    memcpy(rolloutNodeRoot->chessPieceInBoard, curNode->chessPieceInBoard,
                           sizeof(curNode->chessPieceInBoard));
                    // 利用bias调整潜在危险状态的评分
                    double bias = 0.0;
                    fourCnt = 0, liveThreeCnt = 0;
                    resCode = is_risky_board(rolloutNodeRoot, liveThreeCnt, fourCnt);
                    if (resCode != codeNone) {
                        curNode->terminalState = true;  // 当前节点为一个终止节点
                    }
                    // 计算rollout得分
                    double value = rollout(rolloutNodeRoot) + bias;
                    // 反向传播rollout得分，更新经过节点的N和V
                    back_propagation(curNode, value);
                    curNode->UCB = -1;
                    // 释放内存
                    RolloutNode *curDelete = rolloutNodeRoot;
                    while (true) {
                        if (curDelete->childNode == nullptr) {
                            delete curDelete;
                            break;
                        }
                        curDelete = curDelete->childNode;
                        delete curDelete->parentNode;
                    }
                    break;
                }
            }
        }
        iter++;
    }
    // 搜索结束后选择根节点的子节点中UCB最大的，作为下一步决策
    int decisionIdx = find_max_ucb(rootChessBoardNode->childNode, rootChessBoardNode->cntVisited);
    xPos = rootChessBoardNode->childNode[decisionIdx]->curPieceXPos;
    yPos = rootChessBoardNode->childNode[decisionIdx]->curPieceYPos;
//    for (int i = 0; i < rootChessBoardNode->childNode.size(); i++) {
//        cout << 15 - rootChessBoardNode->childNode[i]->curPieceXPos << " " <<
//             (char) ('A' + rootChessBoardNode->childNode[i]->curPieceYPos) << " " <<
//             rootChessBoardNode->childNode[i]->UCB << endl;
//    }
    // 释放内存
    delete_MCST(rootChessBoardNode);
}

void BotPlayer::delete_MCST(ChessBoardNode *chessBoardNode) {
    if (!chessBoardNode->childNode.empty()) {
        int childSize = chessBoardNode->childNode.size();
        for (int i = childSize - 1; i >= 0; i--) {
            delete_MCST(chessBoardNode->childNode[i]);
        }
    }
    delete chessBoardNode;
}

double BotPlayer::decay_factor(int depth) {
    double decayFactor = 1.0 / (depth + 1);
//    cout << decayFactor << endl;
    return decayFactor;
}

double BotPlayer::rollout(RolloutNode *rolloutNodeRoot) {
    RolloutNode *curRolloutNode = rolloutNodeRoot;
    while (true) {
//        cout << "      Rollout-depth：" << curRolloutNode->totalChessPieceCnt << endl;
        // 首先判断该节点是否是最终状态，是则返回
        int resCode = is_terminal_board(curRolloutNode);
        int rolloutDepth = curRolloutNode->totalChessPieceCnt - rootTotalPieceCnt;
        if (curRolloutNode->totalChessPieceCnt == 225 && resCode == codeNone) {
            return TERMINAL_STATE_DUAL_VALUE * decay_factor(rolloutDepth);  // 平局
        }
        if (getPlayerChessPieceType() == curRolloutNode->curPieceType) {  // 若这一步是我方（AI）落子
            if (resCode == codeWin) {
                return TERMINAL_STATE_WIN_VALUE * decay_factor(rolloutDepth);  // 获胜
            } else if (getPlayerChessPieceType() == BlackChessPiece::pieceTypeCode && resCode != codeNone) {
                return TERMINAL_STATE_LOSE_VALUE * decay_factor(rolloutDepth);  // 我方执黑子且下了禁手->失败
            }
        } else {  // 若这一步是对方落子
            if (resCode == codeWin) {
                return TERMINAL_STATE_LOSE_VALUE * decay_factor(rolloutDepth);  // 对方获胜，我方失败
            } else if (getPlayerTypeCode() == WhiteChessPiece::pieceTypeCode && resCode != codeNone) {
                return TERMINAL_STATE_WIN_VALUE * decay_factor(rolloutDepth);  // 对方执黑子且下了禁手->我方胜利
            }
        }
        // 随机选择下一步的位置
        int nextX, nextY;
        choose_next_rollout_node(curRolloutNode, nextX, nextY);
        auto newRolloutNode = new RolloutNode();
        newRolloutNode->curPieceXPos = nextX;
        newRolloutNode->curPieceYPos = nextY;
        newRolloutNode->curPieceType = 3 - curRolloutNode->curPieceType;
        newRolloutNode->totalChessPieceCnt = curRolloutNode->totalChessPieceCnt + 1;
        newRolloutNode->parentNode = curRolloutNode;
        memcpy(newRolloutNode->chessPieceInBoard, curRolloutNode->chessPieceInBoard,
               sizeof(curRolloutNode->chessPieceInBoard));
        newRolloutNode->chessPieceInBoard[nextX][nextY] = newRolloutNode->curPieceType;
        curRolloutNode->childNode = newRolloutNode;
        curRolloutNode = newRolloutNode;
    }
}

void BotPlayer::choose_next_rollout_node(RolloutNode *rolloutNode, int &x, int &y) {
    int valuableNodeCnt = 0, cnt = 0;
    int valuableNode[225 - rolloutNode->totalChessPieceCnt][2];
    int candidateNode[225 - rolloutNode->totalChessPieceCnt][2];
    // 扩展周围的节点：如果周围八个位置之一非空，则扩展该位置
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            bool flag = true;
            if (rolloutNode->chessPieceInBoard[i][j] == BlankPiece::pieceTypeCode) {
                for (int k = 0; k < 8; k++) {
                    for (int w = 1; w <= MCST_MAX_ROLLOUT_STEP; w++) {
                        int nx = i + w * move[k][0], ny = j + w * move[k][1];
                        if (is_pos_in_range(nx) && is_pos_in_range(ny) &&
                            rolloutNode->chessPieceInBoard[nx][ny] != BlankPiece::pieceTypeCode) {
                            // 检查扩展该节点是否是有价值的
                            auto rolloutTestNode = new RolloutNode();
                            rolloutTestNode->curPieceType = 3 - rolloutNode->curPieceType;
                            rolloutTestNode->curPieceXPos = i;
                            rolloutTestNode->curPieceYPos = j;
                            rolloutTestNode->totalChessPieceCnt = rolloutNode->totalChessPieceCnt + 1;
                            memcpy(rolloutTestNode->chessPieceInBoard, rolloutNode->chessPieceInBoard,
                                   sizeof(rolloutNode->chessPieceInBoard));
                            rolloutTestNode->chessPieceInBoard[i][j] = rolloutTestNode->curPieceType;

                            // 如果有价值，则扩展该节点
                            int liveThreeCnt = 0, fourCnt = 0;
                            int resCode = is_risky_board(rolloutTestNode, liveThreeCnt, fourCnt);
                            if (resCode == codeWin || fourCnt > 0 || liveThreeCnt > 0) {
                                valuableNode[valuableNodeCnt][0] = i;
                                valuableNode[valuableNodeCnt][1] = j;
                                valuableNodeCnt++;
                            }

                            candidateNode[cnt][0] = i;
                            candidateNode[cnt][1] = j;
                            cnt++;
                            delete rolloutTestNode;
                            flag = false;
                            break;
                        }
                    }
                    if (!flag) {
                        break;
                    }
                }
            }
        }
    }
    default_random_engine e;
    e.seed(time(0));
    // 只有不存在有价值节点时，才扩展平常节点
    // 随机选择一个可能的落子位置
    if (valuableNodeCnt != 0) {
        int idx = e() % valuableNodeCnt;
        x = valuableNode[idx][0];
        y = valuableNode[idx][1];
    } else {
        int idx = e() % cnt;
        x = candidateNode[idx][0];
        y = candidateNode[idx][1];
    }
}

int BotPlayer::is_risky_board(RolloutNode *rolloutNode, int &liveThreeCnt, int &fourCnt) {
    int a1 = 0, b1 = 0, c1 = 0, a2 = 0, b2 = 0, c2 = 0;  // 同一条线的两个方向
    int liveThree = 0, four = 0, overline = 0, five = 0;
    // 计数并判断  注意这几个if的顺序不能变
    // 上&下
    counter(rolloutNode, 0, a1, b1, c1);
    counter(rolloutNode, 1, a2, b2, c2);
    check_single_direction(rolloutNode, a1, b1, c1, a2, b2, c2, liveThree, four, overline, five);
    liveThreeCnt = liveThree;
    fourCnt = four;
    if (five > 0) {
        return codeWin;
    }
    if (rolloutNode->curPieceType == BlackChessPiece::pieceTypeCode) {
        if (overline > 0) {
            return codeForbidOverline;
        }
        if (four >= 2) {
            return codeForbid44;
        }
        if (liveThree >= 2) {
            return codeForbid33;
        }
    }

    // 左&右
    a1 = 0, b1 = 0, c1 = 0, a2 = 0, b2 = 0, c2 = 0;
    counter(rolloutNode, 2, a1, b1, c1);
    counter(rolloutNode, 3, a2, b2, c2);
    check_single_direction(rolloutNode, a1, b1, c1, a2, b2, c2, liveThree, four, overline, five);
    liveThreeCnt = liveThree;
    fourCnt = four;
    if (five > 0) {
        return codeWin;
    }
    if (rolloutNode->curPieceType == BlackChessPiece::pieceTypeCode) {
        if (overline > 0) {
            return codeForbidOverline;
        }
        if (four >= 2) {
            return codeForbid44;
        }
        if (liveThree >= 2) {
            return codeForbid33;
        }
    }

    // 左上&右下
    a1 = 0, b1 = 0, c1 = 0, a2 = 0, b2 = 0, c2 = 0;
    counter(rolloutNode, 4, a1, b1, c1);
    counter(rolloutNode, 5, a2, b2, c2);
    check_single_direction(rolloutNode, a1, b1, c1, a2, b2, c2, liveThree, four, overline, five);
    liveThreeCnt = liveThree;
    fourCnt = four;
    if (five > 0) {
        return codeWin;
    }
    if (rolloutNode->curPieceType == BlackChessPiece::pieceTypeCode) {
        if (overline > 0) {
            return codeForbidOverline;
        }
        if (four >= 2) {
            return codeForbid44;
        }
        if (liveThree >= 2) {
            return codeForbid33;
        }
    }

    // 右上&左下
    a1 = 0, b1 = 0, c1 = 0, a2 = 0, b2 = 0, c2 = 0;
    counter(rolloutNode, 6, a1, b1, c1);
    counter(rolloutNode, 7, a2, b2, c2);
    check_single_direction(rolloutNode, a1, b1, c1, a2, b2, c2, liveThree, four, overline, five);
    liveThreeCnt = liveThree;
    fourCnt = four;
    if (five > 0) {
        return codeWin;
    }
    if (rolloutNode->curPieceType == BlackChessPiece::pieceTypeCode) {
        if (overline > 0) {
            return codeForbidOverline;
        }
        if (four >= 2) {
            return codeForbid44;
        }
        if (liveThree >= 2) {
            return codeForbid33;
        }
    }
    return codeNone;
}

int BotPlayer::is_terminal_board(RolloutNode *rolloutNode) {
    int a1 = 0, b1 = 0, c1 = 0, a2 = 0, b2 = 0, c2 = 0;  // 同一条线的两个方向
    int liveThree = 0, four = 0, overline = 0, five = 0;

    // 计数并判断  注意这几个if的顺序不能变
    // 上&下
    counter(rolloutNode, 0, a1, b1, c1);
    counter(rolloutNode, 1, a2, b2, c2);
    check_single_direction(rolloutNode, a1, b1, c1, a2, b2, c2, liveThree, four, overline, five);
    if (five > 0) {
        return codeWin;
    }
    if (rolloutNode->curPieceType == BlackChessPiece::pieceTypeCode) {
        if (overline > 0) {
            return codeForbidOverline;
        }
        if (four >= 2) {
            return codeForbid44;
        }
        if (liveThree >= 2) {
            return codeForbid33;
        }
    }

    // 左&右
    a1 = 0, b1 = 0, c1 = 0, a2 = 0, b2 = 0, c2 = 0;
    counter(rolloutNode, 2, a1, b1, c1);
    counter(rolloutNode, 3, a2, b2, c2);
    check_single_direction(rolloutNode, a1, b1, c1, a2, b2, c2, liveThree, four, overline, five);
    if (five > 0) {
        return codeWin;
    }
    if (rolloutNode->curPieceType == BlackChessPiece::pieceTypeCode) {
        if (overline > 0) {
            return codeForbidOverline;
        }
        if (four >= 2) {
            return codeForbid44;
        }
        if (liveThree >= 2) {
            return codeForbid33;
        }
    }

    // 左上&右下
    a1 = 0, b1 = 0, c1 = 0, a2 = 0, b2 = 0, c2 = 0;
    counter(rolloutNode, 4, a1, b1, c1);
    counter(rolloutNode, 5, a2, b2, c2);
    check_single_direction(rolloutNode, a1, b1, c1, a2, b2, c2, liveThree, four, overline, five);
    if (five > 0) {
        return codeWin;
    }
    if (rolloutNode->curPieceType == BlackChessPiece::pieceTypeCode) {
        if (overline > 0) {
            return codeForbidOverline;
        }
        if (four >= 2) {
            return codeForbid44;
        }
        if (liveThree >= 2) {
            return codeForbid33;
        }
    }

    // 右上&左下
    a1 = 0, b1 = 0, c1 = 0, a2 = 0, b2 = 0, c2 = 0;
    counter(rolloutNode, 6, a1, b1, c1);
    counter(rolloutNode, 7, a2, b2, c2);
    check_single_direction(rolloutNode, a1, b1, c1, a2, b2, c2, liveThree, four, overline, five);
    if (five > 0) {
        return codeWin;
    }
    if (rolloutNode->curPieceType == BlackChessPiece::pieceTypeCode) {
        if (overline > 0) {
            return codeForbidOverline;
        }
        if (four >= 2) {
            return codeForbid44;
        }
        if (liveThree >= 2) {
            return codeForbid33;
        }
    }

    return codeNone;
}

void BotPlayer::counter(RolloutNode *rolloutNode, int direction, int &a, int &b, int &c) {
    int curX = rolloutNode->curPieceXPos, curY = rolloutNode->curPieceYPos;
    bool continuesFlag = true;
    // 从相邻位置开始，连续数同色子
    while (continuesFlag) {
        curX += move[direction][0];
        curY += move[direction][1];
        if (is_pos_in_range(curX) && is_pos_in_range(curY)) {
            if (rolloutNode->chessPieceInBoard[curX][curY] != rolloutNode->curPieceType) {
                continuesFlag = false;
            } else {
                a++;
            }
        } else {
            break;
        }
    }
    if (!continuesFlag) {  // 连续的同色子计数是因为遇到异色子或空位而停止的
        if (rolloutNode->chessPieceInBoard[curX][curY] == BlankPiece::pieceTypeCode) {  // 遇到空位
            b = 1;
        } else {  // 遇到异色子
            b = 0;
            c = 0;
            return;
        }
    } else {  // 连续的同色子计数是因为遇到边界而停止的
        b = 0;
        c = 0;
        return;
    }
    // 继续往后数连续的同色子
    continuesFlag = true;
    while (continuesFlag) {
        curX += move[direction][0];
        curY += move[direction][1];
        if (is_pos_in_range(curX) && is_pos_in_range(curY)) {
            if (rolloutNode->chessPieceInBoard[curX][curY] != rolloutNode->curPieceType) {
                continuesFlag = false;
            } else {
                c++;
            }
        } else {
            break;
        }
    }
}

void BotPlayer::check_single_direction(RolloutNode *rolloutNode, int a1, int b1, int c1, int a2, int b2, int c2,
                                       int &liveThree, int &four, int &overline, int &five) {
    int continuous = a1 + a2 + 1;
    int bc1 = b1 + c1;
    int bc2 = b2 + c2;
    // 达成五连获胜条件
    if (continuous == 5 ||
        (rolloutNode->curPieceType == WhiteChessPiece::pieceTypeCode && continuous > 5)) {
        five++;
    }
    // 长连
    if (continuous > 5) {
        overline++;
    }
    // TODO: 活三
    if (b1 != 0 && b2 != 0 && (continuous + bc1 == 4 || continuous + bc2 == 4)) {
        liveThree++;
    }
    // 四
    if (c1 == 1 && c2 == 1) {  // 特殊情况：同一条线可能同时出现两个不同的四
        if (continuous + bc1 == 5) {
            four++;
        }
        if (continuous + bc2 == 5) {
            four++;
        }
    } else {
        if (continuous + bc1 == 5 || continuous + bc2 == 5) {
            four++;
        }
    }
}

void BotPlayer::back_propagation(ChessBoardNode *chessBoardNode, double value) {
    chessBoardNode->value = chessBoardNode->value + value;
    chessBoardNode->cntVisited = chessBoardNode->cntVisited + 1.0;
    if (chessBoardNode->parentNode == nullptr) {
        return;
    }
    back_propagation(chessBoardNode->parentNode, value);
}

void BotPlayer::search_and_expand(RolloutNode *rolloutTestNode, ChessBoardNode *chessBoardNode, int s, int t) {
    for (int k = s; k <= t; k++) {
        int nx, ny, cnt = 0;
        while (true) {
            cnt++;
            nx = rolloutTestNode->curPieceXPos + cnt * move[k][0];
            ny = rolloutTestNode->curPieceYPos + cnt * move[k][1];
            if (rolloutTestNode->chessPieceInBoard[nx][ny] == BlankPiece::pieceTypeCode) {
                auto newNode = new ChessBoardNode();
                newNode->curPieceType = 3 - chessBoardNode->curPieceType;
                newNode->curPieceXPos = nx;
                newNode->curPieceYPos = ny;
                newNode->totalChessPieceCnt = chessBoardNode->totalChessPieceCnt + 1;
                newNode->parentNode = chessBoardNode;
                memcpy(newNode->chessPieceInBoard, chessBoardNode->chessPieceInBoard,
                       sizeof(chessBoardNode->chessPieceInBoard));
                newNode->chessPieceInBoard[nx][ny] = newNode->curPieceType;
                chessBoardNode->childNode.push_back(newNode);
                break;
            } else if (rolloutTestNode->chessPieceInBoard[nx][ny] != rolloutTestNode->curPieceType) {
                break;
            }
        }
    }
}

void BotPlayer::expand_node_safe(ChessBoardNode *chessBoardNode) {
    // 安全地扩展节点：有连三必防守
    auto rolloutTestNode = new RolloutNode();
    rolloutTestNode->curPieceXPos = chessBoardNode->curPieceXPos;
    rolloutTestNode->curPieceYPos = chessBoardNode->curPieceYPos;
    rolloutTestNode->curPieceType = chessBoardNode->curPieceType;
    rolloutTestNode->totalChessPieceCnt = chessBoardNode->totalChessPieceCnt;
    memcpy(rolloutTestNode->chessPieceInBoard, chessBoardNode->chessPieceInBoard,
           sizeof(chessBoardNode->chessPieceInBoard));
    int a1 = 0, b1 = 0, c1 = 0, a2 = 0, b2 = 0, c2 = 0;  // 同一条线的两个方向
    int liveThree = 0, four = 0, overline = 0, five = 0;
    // 计数并判断  注意这几个if的顺序不能变
    // 上&下
    counter(rolloutTestNode, 0, a1, b1, c1);
    counter(rolloutTestNode, 1, a2, b2, c2);
    check_single_direction(rolloutTestNode, a1, b1, c1, a2, b2, c2, liveThree, four, overline, five);
    if (liveThree > 0 || four > 0) {
        search_and_expand(rolloutTestNode, chessBoardNode, 0, 1);
    }

    // 左&右
    liveThree = 0, four = 0, overline = 0, five = 0;
    a1 = 0, b1 = 0, c1 = 0, a2 = 0, b2 = 0, c2 = 0;
    counter(rolloutTestNode, 2, a1, b1, c1);
    counter(rolloutTestNode, 3, a2, b2, c2);
    check_single_direction(rolloutTestNode, a1, b1, c1, a2, b2, c2, liveThree, four, overline, five);
    if (liveThree > 0 || four > 0) {
        search_and_expand(rolloutTestNode, chessBoardNode, 2, 3);
    }

    // 左上&右下
    liveThree = 0, four = 0, overline = 0, five = 0;
    a1 = 0, b1 = 0, c1 = 0, a2 = 0, b2 = 0, c2 = 0;
    counter(rolloutTestNode, 4, a1, b1, c1);
    counter(rolloutTestNode, 5, a2, b2, c2);
    check_single_direction(rolloutTestNode, a1, b1, c1, a2, b2, c2, liveThree, four, overline, five);
    if (liveThree > 0 || four > 0) {
        search_and_expand(rolloutTestNode, chessBoardNode, 4, 5);
    }

    // 右上&左下
    liveThree = 0, four = 0, overline = 0, five = 0;
    a1 = 0, b1 = 0, c1 = 0, a2 = 0, b2 = 0, c2 = 0;
    counter(rolloutTestNode, 6, a1, b1, c1);
    counter(rolloutTestNode, 7, a2, b2, c2);
    check_single_direction(rolloutTestNode, a1, b1, c1, a2, b2, c2, liveThree, four, overline, five);
    if (liveThree > 0 || four > 0) {
        search_and_expand(rolloutTestNode, chessBoardNode, 6, 7);
    }
}

void BotPlayer::expand_node(ChessBoardNode *chessBoardNode) {
    int valuableNodeCnt = 0;
    int candidateNode[225 - chessBoardNode->totalChessPieceCnt][2], cnt = 0;
    // 扩展周围的节点：如果周围八个位置之一非空，则扩展该位置
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            bool flag = true;
            if (chessBoardNode->chessPieceInBoard[i][j] == BlankPiece::pieceTypeCode) {
                for (int k = 0; k < 8; k++) {
                    for (int w = 1; w <= MCST_MAX_EXPAND_STEP; w++) {
                        int nx = i + w * move[k][0], ny = j + w * move[k][1];
                        if (is_pos_in_range(nx) && is_pos_in_range(ny) &&
                            chessBoardNode->chessPieceInBoard[nx][ny] != BlankPiece::pieceTypeCode) {
                            // 检查扩展该节点是否是有价值的
                            auto rolloutTestNode = new RolloutNode();
                            rolloutTestNode->curPieceType = 3 - chessBoardNode->curPieceType;
                            rolloutTestNode->curPieceXPos = i;
                            rolloutTestNode->curPieceYPos = j;
                            rolloutTestNode->totalChessPieceCnt = chessBoardNode->totalChessPieceCnt + 1;
                            memcpy(rolloutTestNode->chessPieceInBoard, chessBoardNode->chessPieceInBoard,
                                   sizeof(chessBoardNode->chessPieceInBoard));
                            rolloutTestNode->chessPieceInBoard[i][j] = rolloutTestNode->curPieceType;

                            // 如果有价值，则扩展该节点
                            int liveThreeCnt = 0, fourCnt = 0;
                            int resCode = is_risky_board(rolloutTestNode, liveThreeCnt, fourCnt);
                            if (resCode == codeWin || fourCnt > 0 || liveThreeCnt > 0) {
                                auto newNode = new ChessBoardNode();
                                newNode->curPieceType = 3 - chessBoardNode->curPieceType;
                                newNode->curPieceXPos = i;
                                newNode->curPieceYPos = j;
                                newNode->totalChessPieceCnt = chessBoardNode->totalChessPieceCnt + 1;
                                newNode->parentNode = chessBoardNode;
                                memcpy(newNode->chessPieceInBoard, chessBoardNode->chessPieceInBoard,
                                       sizeof(chessBoardNode->chessPieceInBoard));
                                newNode->chessPieceInBoard[i][j] = newNode->curPieceType;
                                chessBoardNode->childNode.push_back(newNode);
                                valuableNodeCnt++;
                            } else {
                                candidateNode[cnt][0] = i;
                                candidateNode[cnt][1] = j;
                                cnt++;
                            }
                            flag = false;
                            delete rolloutTestNode;
                            break;
                        }
                    }
                    if (!flag) {
                        break;
                    }
                }
            }
        }
    }
    // 只有不存在有价值节点时，才扩展平常节点
    if (valuableNodeCnt == 0) {
        for (int i = 0; i < cnt; i++) {
            auto newNode = new ChessBoardNode();
            newNode->curPieceType = 3 - chessBoardNode->curPieceType;
            newNode->curPieceXPos = candidateNode[i][0];
            newNode->curPieceYPos = candidateNode[i][1];
            newNode->totalChessPieceCnt = chessBoardNode->totalChessPieceCnt + 1;
            newNode->parentNode = chessBoardNode;
            memcpy(newNode->chessPieceInBoard, chessBoardNode->chessPieceInBoard,
                   sizeof(chessBoardNode->chessPieceInBoard));
            newNode->chessPieceInBoard[candidateNode[i][0]][candidateNode[i][1]] = newNode->curPieceType;
            chessBoardNode->childNode.push_back(newNode);
        }
    }
}

int BotPlayer::find_max_ucb(vector<ChessBoardNode *> v, double parentCntVisited) {
    int idx = 0, vSize = v.size();
    double maxUCB = -DBL_MAX;
    for (int i = 0; i < vSize; i++) {
        double curNodeUCB;
        if (v[i]->cntVisited == 0.0) {
            curNodeUCB = DBL_MAX;
        } else {
            curNodeUCB = v[i]->value / v[i]->cntVisited + MCST_UCB_C * sqrt(log(parentCntVisited) / v[i]->cntVisited);
            v[i]->UCB = curNodeUCB;
        }
        if (maxUCB < curNodeUCB) {
            maxUCB = curNodeUCB;
            idx = i;
        }
    }
    return idx;
}

HumanPlayer::HumanPlayer() {
    playerName = "人类玩家";
    cout << "> 正在创建人类玩家..." << endl;
}

HumanPlayer::~HumanPlayer() {
    cout << "> 正在销毁本局人类玩家..." << endl;
}
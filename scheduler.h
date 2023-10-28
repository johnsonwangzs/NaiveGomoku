#ifndef NAIVEGOMOKU_SCHEDULER_H
#define NAIVEGOMOKU_SCHEDULER_H
using namespace std;

// 对局调度器：比赛流程控制
class Scheduler {
private:
    int gameCnt = 0;  // 对局计数器
    int botWinCnt = 0;  // 电脑赢得对局数
    int humanWinCnt = 0;  // 人类玩家赢得对局数
public:
    Scheduler();  // 构造函数
    ~Scheduler();  // 析构函数

    void schedule_game();  // 比赛调度主循环

    int choose_game_option();  // 玩家选择游戏选项
    void start_new_game();  // 开始新一局比赛
    int decide_first_hand();  // 决定先手
    void show_game_score();  // 展示比赛得分
    void end_game();  // 结束比赛
};

// 输入转化、判断合法性(string->int)
bool transform_position_name(const string &xInput, const string &yInput, int &xPos, int &yPos);

Scheduler::Scheduler() {
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
    // 判断先手
    int firstHand = decide_first_hand();
    if (firstHand == 0) {  // 0：人类玩家为先手（黑子）
        judge->whoIsFirstHand = humanPlayer->getPlayerTypeCode();
        botPlayer->setPlayerHand(WhiteChessPiece::pieceTypeCode);
        humanPlayer->setPlayerHand(BlackChessPiece::pieceTypeCode);
        cout << "> 电脑玩家为后手，人类玩家为先手！" << endl;
    } else if (firstHand == 1) { // 1：人类玩家为后手（白子）
        judge->whoIsFirstHand = botPlayer->getPlayerTypeCode();
        botPlayer->setPlayerHand(BlackChessPiece::pieceTypeCode);
        humanPlayer->setPlayerHand(WhiteChessPiece::pieceTypeCode);
        cout << "> 电脑玩家为先手，人类玩家为后手！" << endl;
    }

    // 第一次：黑方必须在天元H8落子
    if (judge->whoIsFirstHand == 0) {  // 电脑为先手
        int x, y;
        if (!transform_position_name("8", "H", x, y)) {
            cout << "> Error：棋盘无法落子！" << endl;
            return;
        }
        if (judge->judge_first_action_validity(x, y)) {
            chessBoard->set_new_chess_piece(botPlayer->getPlayerHand(), x, y);
            chessBoard->show();
        }
    } else {  // 人类玩家为先手
        int x, y;
        while (true) {
            string xInput, yInput;
            cout << "> 人类玩家为先手，请输入落子位置（先输入行号（数字）再输入列号（字母），用空格隔开）：";
            cin >> xInput >> yInput;  // 获取人类玩家键盘输入
            if (transform_position_name(xInput, yInput, x, y)) {
                if (judge->judge_first_action_validity(x, y)) {
                    chessBoard->set_new_chess_piece(humanPlayer->getPlayerHand(), x, y);
                    chessBoard->show();
                    break;
                } else {
                    cout << "  输入位置不合法，先手（黑子）初次落子必须在天元（H8）位置！" << endl;
                }
            }
        }
    }

    // 连续回合
//    while (true) {
//        if (!judge->continue_game()) {  // 检查是否能继续对局
//            break;
//        }
//    }

    delete humanPlayer;
    delete botPlayer;
    delete judge;
    delete chessBoard;
    cout << "> 本局比赛结束！" << endl;
}

int Scheduler::decide_first_hand() {
    // TODO: 先手决定
    cout << "\n> 现在决定谁为先手..." << endl;
    int firstHand = 0;
    return firstHand;
}

void Scheduler::show_game_score() {
    cout << "> 目前比分为：\n玩家:电脑 = " << humanWinCnt << ":" << botWinCnt << endl;
}

void Scheduler::end_game() {
    cout << "> 游戏结束！" << endl;
    show_game_score();
}

bool transform_position_name(const string &xInput, const string &yInput, int &xPos, int &yPos) {
    // 判断 y
    if (yInput.length() != 1) {
        cout << "  列号非法！请输入一个字母！" << endl;
        return false;
    }
    if (strcmp(yInput.c_str(), "A") >= 0 && strcmp(yInput.c_str(), "O") <= 0) {
        yPos = (int) yInput.c_str()[0] - 65;
    } else if (strcmp(yInput.c_str(), "a") >= 0 && strcmp(yInput.c_str(), "o") <= 0) {
        yPos = (int) yInput.c_str()[0] - 97;
    } else {
        cout << "  列号非法！输入的字母需在A-O（a-o）之间！" << endl;
        return false;
    }
    // 判断 x
    if (!test_integer(xInput)) {
        cout << "  行号非法！需输入一个1-15之间的整数！" << endl;
        return false;
    }
    if (stoi(xInput) < 1 || stoi(xInput) > 15) {
        cout << "  行号非法！请输入一个数（1-15之间）！" << endl;
        return false;
    }
    xPos = stoi(xInput) - 1;
    return true;
}

#endif //NAIVEGOMOKU_SCHEDULER_H

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
    void show_game_score();  // 展示比赛得分
    void end_game();  // 结束比赛
};

Scheduler::Scheduler() {
    cout << "> 正在创建调度器..." << endl;
    schedule_game();
}

void Scheduler::schedule_game() {
    cout << "> 调度器工作中，游戏开始！" << endl;
    while (true) {
        int op = choose_game_option();
        if (op == 1){
            start_new_game();
        }
        else if (op == 2) {
            show_game_score();
        }
        else if (op == 3) {
            end_game();
            break;
        }
    }
}

int Scheduler::choose_game_option() {
    string op;  // 注意此处即使输入为数字，也一定要以字符串方式读入
    while (true){
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
    cout << "初始棋盘：" << endl;
    chessBoard->show();

    // 创建裁判对象
    auto *judge = new Judge();

    // 对局过程
    cout << "> 本局比赛开始！" << endl;

    delete judge;
    delete chessBoard;
    cout << "> 本局比赛结束！" << endl;
}

void Scheduler::show_game_score() {
    cout << "> 目前比分为：\n玩家:电脑 = " << humanWinCnt << ":" << botWinCnt << endl;
}

void Scheduler::end_game() {
    cout << "> 游戏结束！" << endl;
    show_game_score();
}

Scheduler::~Scheduler() {
    cout << "\n--------------------------------\n"
            "> 正在关闭调度器..." << endl;
}

#endif //NAIVEGOMOKU_SCHEDULER_H

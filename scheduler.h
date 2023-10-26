#ifndef NAIVEGOMOKU_SCHEDULER_H
#define NAIVEGOMOKU_SCHEDULER_H
using namespace std;

// 对局调度器：比赛流程控制
class Scheduler {
private:
    int game_cnt = 0;  // 对局计数器
public:
    Scheduler();

};

Scheduler::Scheduler() {
    cout << "> 已创建调度器" << endl;
}


#endif //NAIVEGOMOKU_SCHEDULER_H

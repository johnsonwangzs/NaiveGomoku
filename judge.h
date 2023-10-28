#ifndef NAIVEGOMOKU_JUDGE_H
#define NAIVEGOMOKU_JUDGE_H

// 裁判
class Judge {
public:
    Judge();
    ~Judge();
    const string judgeName = "wangzs";

};

Judge::Judge() {
    cout << "> 正在创建本局裁判..." << endl;
}

Judge::~Judge() {
    cout << "> 正在销毁本局裁判..." << endl;
}


#endif //NAIVEGOMOKU_JUDGE_H

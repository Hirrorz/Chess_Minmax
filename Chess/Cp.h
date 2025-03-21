#pragma once
#include"HEAD.h"

#include"Game.h"
namespace Minmax {
    using namespace std;
    struct Chess {
        int C[90] = { 0 };
        int& operator()(int i, int j) {
            return C[i * 9 + j];
        }
        bool operator==(Chess& A) {
            bool f = 1;
            L(i, 0, 9)L(j, 0, 8)f &= (A(i, j) == C[i * 9 + j]);
            return f;
        }
    };
    Chess base, prec;
    int maxstep = 3;
    const int INF = 2e9;
    const int qzw[32] = { 150,3,3,3,3,7,7,15,15,10,10,1,1,1,1,1,-150,-3,-3,-3,-3,-7,-7,-15,-15,-10,-10,-1,-1,-1,-1,-1 };
    const pll limx[16] = { {0,2},{0,2},{0,2},{0,4},{0,4},{0,9},{0,9},{0,9},{0,9},{0,9},{0,9},{0,9},{0,9},{0,9},{0,9},{0,9} };
    const pll limy[16] = { {3,5},{3,5},{3,5},{0,8},{0,8},{0,8},{0,8},{0,8},{0,8},{0,8},{0,8},{0,8},{0,8},{0,8},{0,8},{0,8} };
    pair<int, int> POS[32];
    vector<Chess>mrecord;
    void init() {
        srand(time(0));
        POS[0] = { 0,4 }; POS[1] = { 0,3 }; POS[2] = { 0,5 }; POS[3] = { 0,2 }; POS[4] = { 0,6 };
        POS[5] = { 0,1 }; POS[6] = { 0,7 }; POS[7] = { 0,0 }; POS[8] = { 0,8 }; POS[9] = { 2,1 };
        POS[10] = { 2,7 }; POS[11] = { 3,0 }; POS[12] = { 3,2 }; POS[13] = { 3,4 }; POS[14] = { 3,6 };
        POS[15] = { 3,8 }; POS[16] = { 9,4 }; POS[17] = { 9,5 }; POS[18] = { 9,3 }; POS[19] = { 9,6 };
        POS[20] = { 9,2 }; POS[21] = { 9,7 }; POS[22] = { 9,1 }; POS[23] = { 9,8 }; POS[24] = { 9,0 };
        POS[25] = { 7,7 }; POS[26] = { 7,1 }; POS[31] = { 6,0 }; POS[30] = { 6,2 }; POS[29] = { 6,4 };
        POS[28] = { 6,6 }; POS[27] = { 6,8 };
        for (int i = 0; i < 10; i++)for (int j = 0; j < 9; j++)base(i, j) = -1;
        for (int i = 0; i < 32; i++) base(POS[i].first, POS[i].second) = i;
        //base(6, 0) = base(6, 2) = base(6, 4) = base(6, 6) = base(6, 8) = -1;
        //base(3, 0) = base(3, 2) = base(3, 4) = base(3, 6) = base(3, 8) = -1;
    }
    inline Chess rev(Chess& a) {/*局面翻转*/
        Chess k;
        L(i, 0, 9)L(j, 0, 8)k(i, j) = -1;
        L(i, 0, 9)L(j, 0, 8) {
            if (a(9 - i, 8 - j) > 15)k(i, j) = a(9 - i, 8 - j) - 16;
            else if (a(9 - i, 8 - j) >= 0)k(i, j) = a(9 - i, 8 - j) + 16;
        }
        return k;
    }
    lf calval(Chess& u) {/*计算局面得分*/
        lf cn = 0;
        L(i, 0, 9)L(j, 0, 8)if (u(i, j) >= 0)cn += qzw[u(i, j)];
        return -cn;
    }

    inline Chess Creatdes(int posxi, int posyi, int posx, int posy, Chess p) {/*创建新的局面*/
        int i = p(posxi, posyi);
        p(posx, posy) = i;
        p(posxi, posyi) = -1;
        return rev(p);
    }
    inline int judge(Chess& u, int i, int posx, int posy) {
        if (i == 0 && u(posx, posy) == 16)return 1;
        if (!(limx[i].first <= posx && posx <= limx[i].second && limy[i].first <= posy && posy <= limy[i].second))return 0;
        if (!(u(posx, posy) == -1 || u(posx, posy) > 15))return 0;
        return 1;
    }
    inline vector<Chess> step(Chess& tmp) {
        pair<int, int> poi[32];

        for (int i = 0; i < 32; i++)poi[i] = { -1,-1 };

        L(i, 0, 9)L(j, 0, 8)if (tmp(i, j) >= 0)poi[tmp(i, j)] = { i, j };
        vector<Chess> ans;
        Chess F;
        int nposx, nposy, posx, posy;
        /*npos是目标坐标，pos是起始坐标*/
        auto work = [&](int i)-> void {
            if (judge(tmp, i, nposx, nposy)) {
                F = Creatdes(posx, posy, nposx, nposy, tmp);
                if (sz(mrecord) <= 4 || mrecord[sz(mrecord) - 4] != F)ans.push_back(F);
            }
        };
        for (int i = 0; i <= 15; i++) {
            if (poi[i].first == -1)continue;
            posx = poi[i].first, posy = poi[i].second;
            if (i == 0) {/*如果是帅*/
                nposx = posx - 1, nposy = posy;
                work(i);
                nposx = posx + 1, nposy = posy;
                work(i);
                nposx = posx, nposy = posy + 1;
                work(i);
                nposx = posx, nposy = posy - 1;
                work(i);
                if (posy == poi[16].second) {/*敌方将的纵坐标等于我方帅的纵坐标时*/
                    nposx = poi[16].first, nposy = poi[16].second;
                    int x = 0;
                    for (int j = posx + 1; j <= nposx - 1; j++)if (tmp(j, posy) != -1)x = 1;/*判断之间有没有棋子*/
                    if (x == 0) {/*没有棋子可以飞将*/
                        work(i);
                    }
                }
            }
            else if (i == 1 || i == 2) {/*如果是士*/
                nposx = posx - 1, nposy = posy - 1;
                work(i);
                nposx = posx + 1, nposy = posy + 1;
                work(i);
                nposx = posx - 1, nposy = posy + 1;
                work(i);
                nposx = posx + 1, nposy = posy - 1;
                work(i);
            }
            else if (i == 3 || i == 4) {/*如果是象*/
                if (posx - 1 >= 0 && posy - 1 >= 0 && tmp(posx - 1, posy - 1) == -1/*判断有没有塞象眼*/) {
                    nposx = posx - 2, nposy = posy - 2;
                    work(i);
                }
                if (posx + 1 <= 9 && posy + 1 <= 8 && tmp(posx + 1, posy + 1) == -1) {
                    nposx = posx + 2, nposy = posy + 2;
                    work(i);
                }
                if (posx + 1 <= 9 && posy - 1 >= 0 && tmp(posx + 1, posy - 1) == -1) {
                    nposx = posx + 2, nposy = posy - 2;
                    work(i);
                }
                if (posx - 1 >= 0 && posy + 1 <= 8 && tmp(posx - 1, posy + 1) == -1) {
                    nposx = posx - 2, nposy = posy + 2;
                    work(i);
                }
            }
            else if (i == 5 || i == 6) {/*如果是马*/
                if (posx - 1 >= 0 && tmp(posx - 1, posy) == -1/*判断有没有蹩马脚*/) {
                    nposx = posx - 2, nposy = posy - 1;
                    work(i);
                    nposx = posx - 2, nposy = posy + 1;
                    work(i);
                }
                if (posx + 1 <= 9 && tmp(posx + 1, posy) == -1) {
                    nposx = posx + 2, nposy = posy + 1;
                    work(i);
                    nposx = posx + 2, nposy = posy - 1;
                    work(i);
                }
                if (posy - 1 >= 0 && tmp(posx, posy - 1) == -1) {
                    nposx = posx - 1, nposy = posy - 2;
                    work(i);
                    nposx = posx + 1, nposy = posy - 2;
                    work(i);
                }
                if (posy + 1 <= 8 && tmp(posx, posy + 1) == -1) {
                    nposx = posx - 1, nposy = posy + 2;
                    work(i);
                    nposx = posx + 1, nposy = posy + 2;
                    work(i);
                }
            }
            else if (i == 7 || i == 8) {/*如果是车*/
                for (int j = 1; j <= 9; j++) {
                    nposx = posx + j, nposy = posy;/*纵坐标不变，横坐标循环往前探*/
                    work(i);
                    if (posx + j > 9 || tmp(posx + j, posy) != -1)break;/*遇到障碍物了或者超出棋盘范围，退出循环*/
                }
                for (int j = 1; j <= 9; j++) {
                    nposx = posx - j, nposy = posy;/*纵坐标不变，横坐标循环往后探*/
                    work(i);
                    if (posx - j < 0 || tmp(posx - j, posy) != -1)break;
                }
                for (int j = 1; j <= 8; j++) {
                    nposx = posx, nposy = posy + j;/*横坐标不变，纵坐标循环往前探*/
                    work(i);
                    if (posy + j > 8 || tmp(posx, posy + j) != -1)break;
                }
                for (int j = 1; j <= 8; j++) {
                    nposx = posx, nposy = posy - j;/*横坐标不变，纵坐标循环往后探*/
                    work(i);
                    if (posy - j < 0 || tmp(posx, posy - j) != -1)break;
                }
            }
            else if (i == 9 || i == 10) {/*如果是炮*/
                for (int j = 1; j <= 9; j++) {/*纵坐标不变，横坐标循环往前探*/
                    if (posx + j > 9)break;/*超出棋盘范围，退出循环*/
                    if (tmp(posx + j, posy) == -1) {
                        nposx = posx + j, nposy = posy;
                        work(i);
                    }
                    if (tmp(posx + j, posy) != -1) {/*遇到障碍往前跳一格并找到可以打击的目标*/
                        j++;
                        while (posx + j <= 9 && tmp(posx + j, posy) == -1)j++;
                        nposx = posx + j, nposy = posy;
                        work(i);
                        break;
                    }
                }
                for (int j = 1; j <= 9; j++) {/*纵坐标不变，横坐标循环往后探*/
                    if (posx - j < 0)break;/*超出棋盘范围，退出循环*/
                    if (tmp(posx - j, posy) == -1) {
                        nposx = posx - j, nposy = posy;
                        work(i);
                    }
                    if (tmp(posx - j, posy) != -1) {/*遇到障碍往前跳一格并找到可以打击的目标*/
                        j++;
                        while (posx - j >= 0 && tmp(posx - j, posy) == -1)j++;
                        nposx = posx - j, nposy = posy;
                        work(i);
                        break;
                    }
                }
                for (int j = 1; j <= 8; j++) {
                    if (posy + j > 8)break;
                    if (tmp(posx, posy + j) == -1) {
                        nposx = posx, nposy = posy + j;
                        work(i);
                    }
                    if (tmp(posx, posy + j) != -1) {
                        j++;
                        while (posy + j <= 8 && tmp(posx, posy + j) == -1)j++;
                        nposx = posx, nposy = posy + j;
                        work(i);
                        break;
                    }
                }
                for (int j = 1; j <= 8; j++) {
                    if (posy - j < 0)break;
                    if (tmp(posx, posy - j) == -1) {
                        nposx = posx, nposy = posy - j;
                        work(i);
                    }
                    if (tmp(posx, posy - j) != -1) {
                        j++;
                        while (posy - j >= 0 && tmp(posx, posy - j) == -1)j++;
                        nposx = posx, nposy = posy - j;
                        work(i);
                        break;
                    }
                }
            }
            else {/*如果是兵*/
                nposx = posx + 1, nposy = posy;
                work(i);
                if (posx > 4) {/*如果过河了*/
                    nposx = posx, nposy = posy + 1;
                    work(i);
                    nposx = posx, nposy = posy - 1;
                    work(i);
                }
            }
        }
        return ans;
    }
    int Checkend(Chess& u) {/*判断游戏是否结束*/
        int x = 1;
        for (int i = limx[0].first; i <= limx[0].second; i++)
            for (int j = limy[0].first; j <= limy[0].second; j++)
                if (u(i, j) == 0)x = 0;
        return x;
    }

    void Work(int STEP, Chess& Init, lf(*CAL)(Chess&)) {
        int maxn = -300;
        int nay = 0;
        const int M = 10;
        maxstep = STEP;
        vector<pair<int, int>> P[M];
        vector<int> Roaval[M];
        vector<Chess> Snf[M];
        Chess U[M];
        vector<Chess> Ans;

        int A[M], B[M], to[M], scor[M], tmp, cA[M], cB[M];
        U[0] = Init;
        L(i, 0, M - 1)cA[i] = A[i] = -300, cB[i] = B[i] = 300, to[i] = -1;
        int dep = 0;
        while (dep >= 0) {
            int lun = dep % 2;
            if (dep == maxstep || Checkend(U[dep])) {
                if (lun == 0)cA[dep] = scor[dep];
                else cB[dep] = scor[dep];
                dep--;
                continue;
            }
            auto& snf = Snf[dep];
            auto& u = U[dep];
            auto& roaval = Roaval[dep];
            auto& p = P[dep];
            int& a = A[dep], & b = B[dep], & ca = cA[dep], & cb = cB[dep];
            if (to[dep] == -1) {
                ca = -300, cb = 300;
                snf = step(u);
                int sz = snf.size();
                roaval.resize(sz);
                p.resize(sz);
                for (int i = 0; i < sz; i++) {
                    auto& v = snf[i];
                    roaval[i] = CAL(v);
                }
                for (int i = 0; i < sz; i++)p[i] = { roaval[i],i };
                sort(p.begin(), p.end(), greater<pll>());
                to[dep] = 0;
            }
            else {
                if (lun)tmp = cA[dep + 1];
                else tmp = cB[dep + 1];
                if (dep == 0 && tmp == maxn) {
                    Ans.push_back(U[dep + 1]);
                }
                if (dep == 0 && tmp > maxn) {
                    maxn = tmp;
                    Ans.clear();
                    Ans.push_back(U[dep + 1]);
                }
                if (lun)b = min(tmp, b), cb = min(tmp, cb);
                else a = max(tmp, a), ca = max(tmp, ca);
                if (a > b)to[dep] = 300;
            }
            int sz = snf.size(), i = to[dep];
            if (i < sz) {
                //if (dep == 0)nay++, cout << nay << '\n';
                auto& v = snf[p[i].second];
                U[dep + 1] = v;
                A[dep + 1] = a;
                B[dep + 1] = b;
                if (lun == 0)scor[dep + 1] = p[i].first;
                else scor[dep + 1] = -p[i].first;
                to[dep]++;
                dep++;
            }
            else {
                to[dep] = -1;
                dep--;
            }
        }
        int pos = rand() % Ans.size();
        Chess Fans = Ans[pos];

        mrecord.push_back(Fans);
    }
    Chess now = base;
    
    void updata() {
        int x, y, nx, ny;
        Chess u, v;
        if (sz(mrecord) % 2)u = rev(mrecord.back()), v = now;
        else u = mrecord.back(), v = rev(now);
        L(i, 0, 9)L(j, 0, 8)if (v(i, j) != u(i, j)) {
            if (u(i, j) == -1)x = i, y = j;
            else nx = i, ny = j;
        }
        PaneBoard::updata(9-x, 8-y, 9-nx, 8-ny);
        now = mrecord.back();
    }
    void human() {
        auto Tmp = PaneBoard::opt();
        auto [readx, ready] = Tmp.first;
        auto [px, py] = Tmp.second;

        auto tmp = now;
        int name = tmp(9 - readx, 8 - ready);
        tmp(9 - px, 8 - py) = name;
        tmp(9 - readx, 8 - ready) = -1;
        tmp = rev(tmp);
        //ofstream outFile("C:/Users/ASUS/Desktop/22.txt");
        //L(i, 0, 9)L(j, 0, 8)outFile << tmp(i, j) << "\t\n"[j == 8];
        //outFile << readx << " " << ready << '\n';
        //outFile << px << " " << py << '\n';
        //outFile.close();
        mrecord.push_back(tmp);

        
        now = mrecord.back();
    }
    int cp(bool First) {
        mrecord.clear();

        now = base;
        if (!First)Work(5, now, calval), updata();
        if (Checkend(now))return 0;
        int cnt = 4;
        while (1) {
            human();
            if (Checkend(now))return 1;
            //Sleep(1000);
            if(cnt)Work(5, now, calval), updata(),cnt--;
            else Work(6, now, calval), updata();
            if (Checkend(now))return 0;
            //Sleep(1000);
            if (sz(mrecord) > 200)return -1;
        }
    }

    int pass(bool First) {
        
        PaneBoard::startGame();
        int res = cp(First);
        system("pause");
        cleardevice();

        return 1;
    }
}

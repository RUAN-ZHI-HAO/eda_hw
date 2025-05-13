/****************************************************
 *  picRouting.cpp  (C++17)
 *  NTUST Algorithm Design & Analysis 2025  PA-3
 *  改寫重點：
 *    1.  正確交叉公式 β × (usage-1)
 *    2.  A* 加入 closed set、方向資訊計彎折
 *    3.  使用 unordered_map + 自訂雜湊提高存取
 *    4.  簡易 Rip-up & Reroute 迴圈 + 動態權重
 ***************************************************/
#include <bits/stdc++.h>
using namespace std;

/* ================= 基本結構 ================= */
struct Point {
    int x{}, y{};

    /* 相等比較 */
    bool operator==(const Point& o) const noexcept {
        return x == o.x && y == o.y;
    }
    /* 不相等比較 —— 友元自由函式 */
    friend bool operator!=(const Point& lhs, const Point& rhs) noexcept {
        return !(lhs == rhs);
    }
};

/* 如果你需要讓 Point 當 key，用同樣的雜湊 */
struct PointHash {
    size_t operator()(const Point& p) const noexcept {
        return (static_cast<size_t>(p.x) << 20) ^ p.y;
    }
};


struct Net{
    int id{};
    Point src, dst;
    vector<Point> path;               // 由 dst 逆推到 src
    long long loss = 0;
};

/* ===============  全域參數 =============== */
int W, H;                             // die area
int PROP, CROSS, BEND;                // 損失係數 α β γ
vector<Net> nets;                     // 所有 nets
/*   使用 2D vector<int> 當全域使用量，可 O(1) 取值   */
vector<vector<int>> usage;            // usage[y][x] (#nets)

/* ============== 便捷函式 ============== */
inline bool inside(int x,int y){ return 0<=x&&x<W && 0<=y&&y<H; }

/* 曼哈頓啟發式：α×d  (此處 γ 與 β 難預估，不放入 h) */
inline int hCost(const Point &a, const Point &b){
    return (abs(a.x-b.x)+abs(a.y-b.y))*PROP;
}

/* 計算 (x,y) 目前交叉成本，不含自己 => β×usage */
inline int crossCost(int x,int y){
    return usage[y][x]*CROSS;
}

/* ************* A* 單網路搜尋 ************* */
struct Node{
    Point p;
    int g=0, f=0;
    int dir=-1;          // 0:→ 1:← 2:↓ 3:↑ -1:無
    /* for pq：f 較小優先 */
    bool operator<(const Node& o)const{ return f>o.f; }
};
const int dx[4]={1,-1,0,0}, dy[4]={0,0,1,-1};

bool route_one(Net &net){
    priority_queue<Node> pq;
    unordered_map<Point, Point, PointHash> parent;
    unordered_map<Point, int, PointHash> bestG;
    unordered_set<Point, PointHash> closed;

    Node s{net.src, 0, hCost(net.src,net.dst), -1};
    pq.push(s); bestG[s.p]=0;

    while(!pq.empty()){
        Node cur=pq.top(); pq.pop();
        if(cur.p==net.dst){
            /* 回溯路徑（dst→src）*/
            Point v=net.dst; net.path.clear(); net.path.push_back(v);
            while(v!=net.src){ v=parent[v]; net.path.push_back(v); }
            /* 更新 usage：含 src/dst */
            for(const Point &pt: net.path) usage[pt.y][pt.x]++;
            return true;
        }
        if(closed.count(cur.p)) continue;
        closed.insert(cur.p);

        for(int dir=0; dir<4; ++dir){
            int nx=cur.p.x+dx[dir], ny=cur.p.y+dy[dir];
            if(!inside(nx,ny)) continue;

            int addG = PROP + crossCost(nx,ny);            // α+β×usage
            if(cur.dir!=-1 && cur.dir!=dir) addG += BEND;  // 轉彎加 γ
            int ng = cur.g + addG;
            Point np{nx,ny};
            if(!bestG.count(np) || ng<bestG[np]){
                bestG[np]=ng;
                int nf = ng + hCost(np, net.dst);
                pq.push({np, ng, nf, dir});
                parent[np]=cur.p;
            }
        }
    }
    return false;   /* 找不到路 */
}

/* *********** 計算單網路最終損失 *********** */
long long calc_loss(const Net &net){
    long long propLen = (long long)net.path.size()-1;
    long long crossCnt=0, bendCnt=0;

    /* crossing */
    for(const Point &pt: net.path){
        crossCnt += max(0, usage[pt.y][pt.x]-1);
    }
    /* bending */
    for(size_t i=2;i<net.path.size();++i){
        auto &c=net.path[i-2], &b=net.path[i-1], &a=net.path[i];
        if((c.x==b.x&&b.x==a.x)||(c.y==b.y&&b.y==a.y)) continue;
        ++bendCnt;
    }
    return propLen*PROP + crossCnt*CROSS + bendCnt*BEND;
}

/* ********* Rip-up & Reroute 主流程 ********* */
void solve(){
    /* 依曼哈頓長度由長到短排，先走長線 */
    sort(nets.begin(), nets.end(), [](auto &a,auto &b){
        int da=abs(a.src.x-a.dst.x)+abs(a.src.y-a.dst.y);
        int db=abs(b.src.x-b.dst.x)+abs(b.src.y-b.dst.y);
        return da>db;
    });

    /* 初始 routing */
    for(Net &n: nets) route_one(n);

    /* 簡易 RRR：迴圈檢查最差 10% nets，rip-up 再跑一次 */
    const int ITER=5;
    for(int it=0; it<ITER; ++it){
        /* 計算損失 */
        for(Net &n: nets) n.loss = calc_loss(n);
        long long tot=0; for(auto &n:nets) tot+=n.loss;

        /* 找出前 10% 最差 */
        int k = max<int>(1, nets.size()*0.1);
        nth_element(nets.begin(), nets.begin()+k, nets.end(),
                    [](auto&a,auto&b){return a.loss>b.loss;});

        /* rip-up (移除 usage) */
        for(int i=0;i<k;++i){
            for(const Point &pt: nets[i].path){
                usage[pt.y][pt.x]--;
            }
            nets[i].path.clear(); nets[i].loss=0;
        }
        /* 動態權重：若某格 usage>3，提高 β 權重 (簡化示意) */
        if(it==ITER-2) CROSS*=2;

        /* reroute */
        for(int i=0;i<k;++i) route_one(nets[i]);
    }
}

/* ================ 輸出 ================ */
void dump(const string &outfile){
    ofstream fout(outfile);
    for(const Net &n: nets){
        int seg = (int)n.path.size()-1;
        fout<<n.id<<' '<<seg<<'\n';
        for(int i=seg;i>=0;--i){
            if (i == seg) fout<<n.path[i].x<<' '<<n.path[i].y << ' ';
            else if (i == 0) fout<<n.path[i].x<<' '<<n.path[i].y;
            else fout<<n.path[i].x<<' '<<n.path[i].y << "\n" << n.path[i].x << ' ' << n.path[i].y << ' ';
            if(i) fout<<'\n';
        }
        if(&n!=&nets.back()) fout<<'\n';
    }
}

/* ================ 主程式 ================ */
int main(int argc,char*argv[]){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    if(argc<3){ cerr<<"usage: ./picRouting in out\n"; return 1; }
    ifstream fin(argv[1]); string outFile=argv[2];
    if(!fin){ cerr<<"無法開啟輸入檔\n"; return 1; }

    string tok;
    while(fin>>tok){
        if(tok=="grid"){ fin>>W>>H; }
        else if(tok=="propagation"){ fin>>tok>>PROP; }
        else if(tok=="crossing"){ fin>>tok>>CROSS; }
        else if(tok=="bending"){ fin>>tok>>BEND; }
        else if(tok=="num"){ fin>>tok; int N; fin>>N;
            nets.resize(N);
            for(int i=0;i<N;++i){
                int id,xs,ys,xt,yt;
                fin>>id>>xs>>ys>>xt>>yt;
                nets[i].id=id; nets[i].src={xs,ys}; nets[i].dst={xt,yt};
            }
        }
    }
    usage.assign(H, vector<int>(W,0));

    solve();
    dump(outFile);
    return 0;
}

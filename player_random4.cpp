#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <cstdlib>
#include <ctime>

struct Point {
    int x, y;
    
};
class POINT2{
public:
    void WIN2MAP(POINT2 &MAP){            //建立棋盘与矩阵的映射关系
        MAP.x = (x ) / 1;
        MAP.y = (y) / 1;
    }
    void MAP2WIN(POINT2 &WIN){            //建立矩阵与棋盘的映射关系
        WIN.x = x*1 + 1 / 2;
        WIN.y = y*1 + 1 / 2;
    }
    void INIT(int x, int y){               //插入元素
        this->x = x;
        this->y = y;
    }
    void ADD(int x, int y){                //计算和
        this->x += x;
        this->y += y;
    }
    int MAP2WINX(){
        return  x*1 + 1 / 2;
    }
    int MAP2WINY(){
        return  y*1 + 1 / 2;
    }
    int WIN2MAPX(){
        return (x) / 1;
    }
    int WIN2MAPY(){
        return (y) / 1;
    }
    int x, y;
};

int player;
const int SIZE = 8;
std::array<std::array<int, SIZE>, SIZE> board;
std::vector<Point> next_valid_spots;
const int mappoint[8][8] = {
    
    {90, 2, 60, 40, 40, 60, 2,90},
    { 2, 1, 30, 10, 10, 30, 1, 2},
    {60,30, 50, 30, 30, 50,30,60},
    {40,10, 30, 10, 10, 30,10,40},
    {40,10, 30, 10, 10, 30,10,40},
    {60,30, 50, 30, 30, 50,30,60},
    { 2, 1, 30, 10, 10, 30, 1, 2},
    {90, 2, 60, 40, 40, 60, 2,90}
    
};
const int direction[8][2] = {
    { -1, 0 }, { 1, 0 },
    { 0, -1 }, { 0, 1 },
    { -1, -1},{ 1, -1 },
    { 1, 1 }, { -1, 1 }
};

#define re register
int state[8][8], ansx, ansy;
//black:1 white:-1
int cur[8][8];
inline int alpha_beta(int dep, int player, int alpha, int beta, int cnt){ //0 for me 1 for it

    if(dep > 9 || cnt > 64){ //dep是搜索的层数
        int ret = 0;
        for(int i = 1; i <= 8; ++i)
            for(int j = 1; j <= 8; ++j)
                ret += mappoint[i][j]*state[i][j];
        return ret;
    }
    int temp[8][8], tot = 0;
    memcpy(temp, state, sizeof state);
    if(!player)
        for(re int id = 1; id <= 64; ++id){
            int  x, y, i, j;
            i = (id-1)%8+1, j = (id-1)%8+1;
            if(state[i][j] == 0){
                int flag = 0, t;
                for(re int k = 0; k < 8; ++k){
                    t = 0, x = i+direction[k][0], y = j+direction[k][1];
                    while(state[x][y] == -1)
                        t++, x += direction[k][0], y += direction[k][1];
                    if(t && state[x][y] == 1){
                        while(x != i || y != j){
                            x -= direction[k][0], y -= direction[k][1];
                            state[x][y] = 1;
                        }
                        flag = 1;
                    }
                }
                if(flag){
                    tot++;
                    int v = alpha_beta(dep+1, player^1, alpha, beta, cnt+1);
                    if(v > alpha){
                        if(!dep) ansx = i, ansy = j, memcpy(cur, state, sizeof state);
                        alpha = v;
                    }
                    memcpy(state, temp, sizeof temp);
                    if(beta <= alpha) break;
                }
            }
        }
    if(!tot) player = 1;
    if(player){
        for(re int id = 1; id <= 64; ++id){
            int x, y, tx, ty, i, j;
            i = (id-1>>3)+1, j = (id-1)%8+1;
            if(state[i][j] == 0){
                int flag = 0, t;
                for(re int k = 0; k < 8; ++k){
                    t = 0, x = i+direction[k][0], y = j+direction[k][1];
                    while(state[x][y] == 1)
                        t++, x += direction[k][0], y += direction[k][1];
                    if(t && state[x][y] == -1){
                        while(x != i || y != j){
                            x -= direction[k][0], y -= direction[k][1];
                            state[x][y] = -1;
                        }
                        flag = 1;
                    }
                }
                if(flag){
                    int v = alpha_beta(dep+1, player^1, alpha, beta, cnt+1);
                    if(beta > v) beta = v;
                    memcpy(state, temp, sizeof temp);
                    if(beta <= alpha) break;
                }
            }
        }
    }
    return !player ? alpha : beta;
}

void read_board(std::ifstream& fin) {
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
        }
    }
}

void read_valid_spots(std::ifstream& fin) {
    int n_valid_spots;
    fin >> n_valid_spots;
    int x, y;
    for (int i = 0; i < n_valid_spots; i++) {
        fin >> x >> y;
        next_valid_spots.push_back({x, y});
    }
}
int check(int x, int y, int color){                      //预判当前位置能否下子
    // if (board[x][y])return 0;                         //如果当前位置已经有棋子
    int me = color ? 1 : 2;                              //准备落棋棋子颜色
    POINT2 star;
    int count = 0, flag;                                 //count为该位置可以转换对手棋子个数
    for (int i = 0; i < SIZE; ++i){                      //搜索
        flag = 0;
        star.INIT(x + direction[i][0], y + direction[i][1]);
        while (star.x >= 0 && star.x < SIZE&&star.y >= 0 && star.y < SIZE){
            if (board[star.y][star.x] != me) flag++;
            else{
                count += flag;
                break;
            }
            star.ADD(direction[i][0], direction[i][1]);
        }
    }
    return count;                                          //返回该点转换对方棋子个数
}
void write_valid_spot(std::ofstream& fout) {
    int n_valid_spots = next_valid_spots.size();
    srand(time(NULL));
    // Choose random spot. (Not random uniform here)
    
    int a=-2;
    int x,y;
    for(int i=0;i<n_valid_spots;i++){
        int cou=-1;
        Point p = next_valid_spots[i];
        cou = check(p.x,p.y,1)+mappoint[p.x][p.y];

        if(cou>a){
            a=cou;
            x=p.x;
            y=p.y;
        }
    }
    // Remember to flush the output to ensure the last action is written to file.
    fout << x << " " << y << std::endl;
    fout.flush();
}

int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    read_valid_spots(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}

#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <cstdlib>
#include <ctime>

struct Point {
    int x, y;
};
int player;
const int SIZE = 8;
std::array<std::array<int, SIZE>, SIZE> board;
std::vector<Point> next_valid_spots;
int Judge(int x, int y, int color,int board[SIZE][SIZE]);
int MOVE[8][2] = { { -1, 0 },{ 1, 0 },{ 0, -1 },{ 0, 1 },{ -1, -1 },{ 1, -1 },{ 1, 1 },{ -1, 1 } }; //方位
int MAPPOINT[SIZE][SIZE] = {
    {50, 9, 13, 13, 13, 13, 9,50},
    { 9, 1, 10, 10, 10, 10, 1, 9},
    {13,10, 10, 10, 10, 10,10,13},
    {13,10, 10, 10, 10, 10,10,13},
    {13,10, 10, 10, 10, 10,10,13},
    {13,10, 10, 10, 10, 10,10,13},
    { 9, 1, 10, 10, 10, 10, 1, 9},
    {50, 9, 13, 13, 13, 13, 9,50}
};
class POINT2{
public:
    void WIN2MAP(POINT2 &MAP){            //棋盤和矩陣對應
        MAP.x = (x ) / 1;
        MAP.y = (y) / 1;
    }
    void MAP2WIN(POINT2 &WIN){            //矩陣和棋盤對應
        WIN.x = x*1 + 1 / 2;
        WIN.y = y*1 + 1 / 2;
    }
    void INIT(int x, int y){               //插入元素
        this->x = x;
        this->y = y;
    }
    void ADD(int x, int y){                //和
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
void mappadd(int x, int y, int color,int board[SIZE][SIZE])        //在地圖上加棋子
{
    POINT2 WINDOWS2, board2;
    WINDOWS2.INIT(x, y);
    WINDOWS2.WIN2MAP(board2);
    board[board2.x][board2.y] = color ? 1 : 2;
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

int check(int x, int y, int color){ //判斷能不能下棋
   // if (board[x][y])return 0;                           //如果已經有棋子
    int me = color ? 1 : 2;                               //要下的顏色
    POINT2 star;
    int count = 0, flag;                                  //count可以轉換顏色的個數
    for (int i = 0; i < SIZE; ++i){                       //找
        flag = 0;
        //star.INIT(x + MOVE[i][0], y + MOVE[i][1]);
        while (star.x >= 0 && star.x < SIZE&&star.y >= 0 && star.y < SIZE){
            if (board[star.x][star.y] != me) flag++;
            else{
                count = flag+1;
                break;
            }
            star.ADD(MOVE[i][0], MOVE[i][1]);
        }
    }
    return count;                                          //回傳回傳這個點可以轉換對方棋子的數量
}
void write_valid_spot(std::ofstream& fout) {
    int n_valid_spots = next_valid_spots.size();
    srand(time(NULL));
    // Choose random spot. (Not random uniform here)
    //int index = (rand() % n_valid_spots);
    int a=-2;
    int x,y;
    
    for(int i=0;i<n_valid_spots;i++){
        int cou=-1;
        Point p = next_valid_spots[i];
        if(board[0][0]){
            MAPPOINT[1][0]=30;
            MAPPOINT[0][1]=30;
        }else if(board[7][0]){
            MAPPOINT[6][0]=30;
            MAPPOINT[7][1]=30;
        }else if(board[0][7]){
            MAPPOINT[0][6]=30;
            MAPPOINT[1][7]=30;
        }else if(board[7][7]){
            MAPPOINT[6][7]=30;
            MAPPOINT[7][6]=30;
        }
        cou = check(p.x,p.y,1)+MAPPOINT[p.x][p.y];
        
        if(cou>a){
            a=cou;
            x=p.x;
            y=p.y;
        }
        fout << x << " " << y << std::endl;
        fout.flush();
    }
    
        
    // Remember to flush the output to ensure the last action is written to file.
    //fout << x << " " << y << std::endl;
    //fout.flush();
}

void Change(POINT2 NOW,int board[SIZE][SIZE],bool F){      //下棋後的狀態 F要不要輸出
    int me = board[NOW.x][NOW.y];                          //要下的顏色
    bool flag;
    POINT2 a, b;
    for (int i = 0; i<SIZE; ++i){                          //找
        flag = false;
        a.INIT(NOW.x + MOVE[i][0], NOW.y + MOVE[i][1]);
        while (a.x >= 0 && a.x<SIZE&&a.y >= 0 && a.y<SIZE&&board[a.x][a.y]){
            if (board[a.x][a.y] == -me)flag = true;
            else{
                if (flag){
                    a.ADD(-MOVE[i][0], -MOVE[i][1]);
                    b.INIT(NOW.x + MOVE[i][0], NOW.y + MOVE[i][1]);
                    while (((NOW.x <= b.x && b.x <= a.x) || (a.x <= b.x && b.x <= NOW.x)) && ((NOW.y <= b.y && b.y <= a.y) || (a.y <= b.y && b.y <= NOW.y))){
                        if(F)mappadd(b.MAP2WINX(), b.MAP2WINY(), (me == 1) ? 1 : 2,board);    //換棋子
                        if (!F)mappadd(b.MAP2WINX(), b.MAP2WINY(), (me == 1) ? 1 : 2, board); //如果不輸出 改變地圖
                        b.ADD(MOVE[i][0], MOVE[i][1]);
                    }
                }
                break;
            }
            a.ADD(MOVE[i][0], MOVE[i][1]);
        }
    }
}
#define re register
int state[8][8], ansx, ansy;
//black:1 white:-1
int cur[8][8];
inline int alpha_beta(int dep, int player, int alpha, int beta, int cnt){ //0 for me 1 for it

if(dep > 9 || cnt > 64){ //dep是搜索的层数
    int ret = 0;
    for(int i = 1; i <= 8; ++i)
        for(int j = 1; j <= 8; ++j)
            ret += MAPPOINT[i][j]*state[i][j];
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
                t = 0, x = i+MOVE[k][0], y = j+MOVE[k][1];
                while(state[x][y] == -1)
                    t++, x += MOVE[k][0], y += MOVE[k][1];
                if(t && state[x][y] == 1){
                    while(x != i || y != j){
                        x -= MOVE[k][0], y -= MOVE[k][1];
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
                t = 0, x = i+MOVE[k][0], y = j+MOVE[k][1];
                while(state[x][y] == 1)
                    t++, x += MOVE[k][0], y += MOVE[k][1];
                if(t && state[x][y] == -1){
                    while(x != i || y != j){
                        x -= MOVE[k][0], y -= MOVE[k][1];
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


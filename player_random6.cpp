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
int mapp[SIZE][SIZE];
int expect[SIZE][SIZE];
std::array<std::array<int, SIZE>, SIZE> board;
std::vector<Point> next_valid_spots;
int Judge(int x, int y, int color,int board[SIZE][SIZE]);
int MOVE[8][2] = { { -1, 0 }, { 1, 0 },
                         { 0, -1 }, { 0, 1 },
                         { -1, -1},{ 1, -1 },
                         { 1, 1 }, { -1, 1 }
    
}; //方位
int MAPPOINTCOUNT[8][8] = {
    {200,2,10,10,10,10,2,200},
    {2,1,50,50,50,50,1,2},
    {100,50,10,10,10,10,50,100},
    {100,50,10,10,10,10,50,100},
    {100,50,10,10,10,10,50,100},
    {100,50,10,10,10,10,50,100},
    {2,1,50,50,50,50,1,2},
    {200,2,100,100,100,100,2,200}
};
class POINT2{
public:
    void WIN2MAP(POINT2 &MAP){            //建立棋盘与矩阵的映射关系
        MAP.x = (x) / 1;
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


void mappadd(int x, int y, int color,int board[SIZE][SIZE])        //向地图中添加棋子
{
    POINT2 WINDOWS2, board2;
    WINDOWS2.INIT(x, y);
    WINDOWS2.WIN2MAP(board2);
    board[board2.x][board2.y] = color ? 1 : -1;
}
int Judge(int x, int y, int color,int board[SIZE][SIZE]){                      //预判当前位置能否下子
    if (board[x][y])return 0;                         //如果当前位置已经有棋子
    int me = color ? 1 : -1;                              //准备落棋棋子颜色
    POINT2 star;
    int count = 0, flag;                                 //count为该位置可以转换对手棋子个数
    for (int i = 0; i < SIZE; ++i){                      //搜索
        flag = 0;
        star.INIT(x + MOVE[i][0], y + MOVE[i][1]);
        while (star.x >= 0 && star.x < SIZE&&star.y >= 0 && star.y < SIZE){
            if (board[star.x][star.y] != me) flag++;
            else{
                count += flag;
                break;
            }
            star.ADD(MOVE[i][0], MOVE[i][1]);
        }
    }
    return count;                                          //返回该点转换对方棋子个数
}

void Change(POINT2 NOW,int board[SIZE][SIZE],bool F){      //落子后改变棋盘状态 F为是否输出到屏幕
    int me = board[NOW.x][NOW.y];                          //当前落子棋子颜色
    bool flag;
    POINT2 a, b;
    for (int i = 0; i<SIZE; ++i){                          //搜索
        flag = false;
        a.INIT(NOW.x + MOVE[i][0], NOW.y + MOVE[i][1]);
        while (a.x >= 0 && a.x<SIZE&&a.y >= 0 && a.y<SIZE&&board[a.x][a.y]){
            if (board[a.x][a.y] == -me)flag = true;
            else{
                if (flag){
                    a.ADD(-MOVE[i][0], -MOVE[i][1]);
                    b.INIT(NOW.x + MOVE[i][0], NOW.y + MOVE[i][1]);
                    while (((NOW.x <= b.x && b.x <= a.x) || (a.x <= b.x && b.x <= NOW.x)) && ((NOW.y <= b.y && b.y <= a.y) || (a.y <= b.y && b.y <= NOW.y))){
                        if(F)mappadd(b.MAP2WINX(), b.MAP2WINY(), (me == 1) ? 1 : -1,board);    //改变棋子
                        if (!F)mappadd(b.MAP2WINX(), b.MAP2WINY(), (me == 1) ? 1 : -1, board); //如果不输出到屏幕，改变地图数组
                        b.ADD(MOVE[i][0], MOVE[i][1]);
                    }
                }
                break;
            }
            a.ADD(MOVE[i][0], MOVE[i][1]);
        }
    }
}
void copymap(int one[SIZE][SIZE], int last[SIZE][SIZE])                        //拷贝地图
{
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            one[i][j] = last[i][j];
}
int minimax(int x,int y,int mapnow[SIZE][SIZE],int expectnow[SIZE][SIZE],int depin,int depinmax){ //極大極小搜索

    if (depin >= depinmax)return 0;   //遞歸出口

    int maxx = -10005;                //最大權值
    POINT2 NOW;
    int expectnow2[SIZE][SIZE] , mapnow2[SIZE][SIZE],mapnext[SIZE][SIZE],expectlast[SIZE][SIZE];    //定義臨時數組

    copymap(mapnow2, mapnow);                //復制當前棋盤

    mapnow2[x][y] = player ? 1 : -1;        //模擬在當前棋盤上下棋
    int ME = MAPPOINTCOUNT[x][y] + expectnow[x][y];    //當前棋子權
    NOW.INIT(x,y);

    Change(NOW, mapnow2, false);            //改變棋盤AI結束

    int MAXEXPECT = 0, LINEEXPECT = 0;
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j){
            expectnow2[i][j] = Judge(i, j, !player, mapnow2);//預判對方是否可以走棋
            if (expectnow2[i][j]){
                ++MAXEXPECT;
                if ((i == 0 && j == 0) || (i == 0 && j == SIZE - 1) || (i == SIZE - 1 && j == SIZE - 1) || (i == SIZE - 1 && j == 0))
                    return -1800;    //如果對方有占角的可能
                if ((i < 2 && j < 2) || (i < 2 && SIZE - j - 1 < 2) || (SIZE - 1 - i < 2 && j < 2) || (SIZE - 1 - i < 2 && SIZE - 1 - j < 2))
                    ++LINEEXPECT;
            }
        }
    if (LINEEXPECT * 10 > MAXEXPECT * 7)
        return 1400;                      //如果對方走到壞點狀態較多 剪枝

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (expectnow2[i][j]){        //如果對方可以走棋

                int YOU = MAPPOINTCOUNT[i][j] + expectnow2[i][j];//當前權值
                copymap(mapnext, mapnow2);                       //拷貝地圖
                mapnext[i][j] = (!player) ? 1 : -1;              //模擬對方走棋
                NOW.INIT(i, j);
                Change(NOW, mapnext, false);                     //改變棋盤

                for (int k = 0; k < SIZE; k++)
                    for (int l = 0; l < SIZE; l++)
                        expectlast[k][l] = Judge(k, l, player, mapnext);    //尋找AI可行點

                for (int k = 0; k < SIZE; k++)
                    for (int l = 0; l < SIZE;l++)
                        if (expectlast[k][l]){
                            int nowm = ME - YOU + minimax(k, l, mapnext, expectlast, depin + 1, depinmax);
                            maxx = maxx < nowm ? nowm : maxx;
                        }
            }
    return maxx;
}

void write_valid_spot(std::ofstream& fout) {
    int n_valid_spots = next_valid_spots.size();
    srand(time(NULL));
    // Choose random spot. (Not random uniform here)
    //int index = (rand() % n_valid_spots);
    /*
    int a=-2;
    int x,y;
    for(int i=0;i<n_valid_spots;i++){
        int cou=-1;
        Point p = next_valid_spots[i];
        cou = minimax(p.x,p.y,1)+MAPPOINTCOUNT[p.x][p.y];

        if(cou>a){
            a=cou;
            x=p.x;
            y=p.y;
        }
    }
    */
    
    POINT2 MAX;
    int maxx = -2;
    MAX.INIT(0, 0);
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++){
            if (expect[i][j]){
                if ((i == 0 && j == 0) || (i == 0 && j == SIZE - 1) || (i == SIZE - 1 && j == SIZE - 1) || (i == SIZE - 1 && j == 0)){
                    MAX.INIT(i, j);
                    //return MAX;                                        //如果在角，返回角坐标
                }
                for(int a=0;a<n_valid_spots;a++){
                    Point p = next_valid_spots[a];
                    int k = -1;
                    if(board[0][0]==player){
                               MAPPOINTCOUNT[1][0]=30;
                               MAPPOINTCOUNT[0][1]=30;
                           }else if(board[7][0]==player){
                               MAPPOINTCOUNT[6][0]=30;
                               MAPPOINTCOUNT[7][1]=30;
                           }else if(board[0][7]==player){
                               MAPPOINTCOUNT[0][6]=30;
                               MAPPOINTCOUNT[1][7]=30;
                           }else if(board[7][7]==player){
                               MAPPOINTCOUNT[6][7]=30;
                               MAPPOINTCOUNT[7][6]=30;
                           }
                    k = minimax(i,j,mapp,expect,0,3)+MAPPOINTCOUNT[p.x][p.y];       //递归搜索 搜索三层
                    if (k >= maxx){
                        maxx = k;
                        i=p.x;
                        j=p.y;
                    }
                }
            }
            fout << i << " " << j << std::endl;
            fout.flush();
        }
    
    
    // Remember to flush the output to ensure the last action is written to file.
    //fout << x << " " << y << std::endl;
    //fout.flush();
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



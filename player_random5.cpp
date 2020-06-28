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
const int MOVE[8][2] = { { -1, 0 }, { 1, 0 },
                         { 0, -1 }, { 0, 1 },
                         { -1, -1},{ 1, -1 },
                         { 1, 1 }, { -1, 1 }
    
}; //方位
const int MAPPOINTCOUNT[8][8] = {
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
/*
void mappadd(int x, int y, int color,int board[SIZE][SIZE])        //向地图中添加棋子
{
    POINT2 WINDOWS2, board2;
    WINDOWS2.INIT(x, y);
    WINDOWS2.WIN2MAP(board2);
    board[board2.x][board2.y] = color ? 1 : -1;
}
*/
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
    if (board[x][y])return 0;                         //如果当前位置已经有棋子
    int me = color ? 1 : 2;                              //准备落棋棋子颜色
    POINT2 star;
    int count = 0, flag;                                 //count为该位置可以转换对手棋子个数
    for (int i = 0; i < SIZE; ++i){                      //搜索
        flag = 0;
        star.INIT(x + MOVE[i][0], y + MOVE[i][1]);
        while (star.x >= 0 && star.x < SIZE&&star.y >= 0 && star.y < SIZE){
            if (board[star.y][star.x] != me) flag++;
            else{
                count += flag;
                break;
            }
            star.ADD(MOVE[i][0], MOVE[i][1]);
        }
    }
    return count;                                          //返回该点转换对方棋子个数
}
/*
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
                        if(F)mappadd(b.MAP2WINX(), b.MAP2WINY(), (me == 1) ? 1 : 2,board);    //改变棋子
                        if (!F)mappadd(b.MAP2WINX(), b.MAP2WINY(), (me == 1) ? 1 : 2, board); //如果不输出到屏幕，改变地图数组
                        b.ADD(MOVE[i][0], MOVE[i][1]);
                    }
                }
                break;
            }
            a.ADD(MOVE[i][0], MOVE[i][1]);
        }
    }
}
*/
/*
void copymap(int one[SIZE][SIZE], int last[SIZE][SIZE])                        //拷贝地图
{
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            one[i][j] = last[i][j];
}
*/



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
        cou = check(p.x,p.y,1)+MAPPOINTCOUNT[p.x][p.y];

        if(cou>a){
            a=cou;
            x=p.x;
            y=p.y;
        }
    }
    
    
    fout << x << " " << y << std::endl;
    fout.flush();
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



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
const int weight[10][10] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 65,  -3, 6, 4, 4, 6,  -3, 65, 0,
    0, -3, -29, 3, 1, 1, 3, -29, -3, 0,
    0, 6,   3, 5, 3, 3, 5,   3,  6, 0,
    0, 4,   1, 3, 1, 1, 3,   1,  4, 0,
    0, 4,   1, 3, 1, 1, 3,   1,  4, 0,
    0, 6,   3, 5, 3, 3, 5,   3,  6, 0,
    0, -3, -29, 3, 1, 1, 3, -29, -3, 0,
    0, 65,  -3, 6, 4, 4, 6,  -3, 65, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};//苟的debian黑白棋AI的权值表
const int d[8][2] =
{
    -1, 0, 1, 0, 0, -1, 0, 1,
    -1, -1, -1, 1, 1, -1, 1, 1
};

#define re register
int state[10][10], ansx, ansy;
//black:1 white:-1
int cur[10][10];
inline int alpha_beta(int dep, int player, int alpha, int beta, int cnt) //0 for me 1 for it
{
    if(dep > 9 || cnt > 64) //dep是搜索的层数
    {
        int ret = 0;
        for(re int i = 1; i <= 8; ++i)
            for(re int j = 1; j <= 8; ++j)
                ret += weight[i][j]*state[i][j];
        return ret;
    }
    int temp[10][10], tot = 0;
    memcpy(temp, state, sizeof state);
    if(!player)
        for(re int id = 1, x, y, i, j; id <= 64; ++id)
        {
            i = (id-1>>3)+1, j = (id-1)%8+1;
            if(state[i][j] == 0)
            {
                int flag = 0, t;
                for(re int k = 0; k < 8; ++k)
                {
                    t = 0, x = i+d[k][0], y = j+d[k][1];
                    while(state[x][y] == -1) t++, x += d[k][0], y += d[k][1];
                    if(t && state[x][y] == 1)
                    {
                        do
                        {
                            x -= d[k][0], y -= d[k][1];
                            state[x][y] = 1;
                        } while(x != i || y != j);
                        flag = 1;
                    }
                }
                if(flag)
                {
                    tot++;
                    int v = alpha_beta(dep+1, player^1, alpha, beta, cnt+1);
                    if(v > alpha)
                    {
                        if(!dep) ansx = i, ansy = j, memcpy(cur, state, sizeof state);
                        alpha = v;
                    }
                    memcpy(state, temp, sizeof temp);
                    if(beta <= alpha) break;
                }
            }
        }
    if(!tot) player = 1;
    if(player)
    {
        for(re int id = 1, x, y, tx, ty, i, j; id <= 64; ++id)
        {
            i = (id-1>>3)+1, j = (id-1)%8+1;
            if(state[i][j] == 0)
            {
                int flag = 0, t;
                for(re int k = 0; k < 8; ++k)
                {
                    t = 0, x = i+d[k][0], y = j+d[k][1];
                    while(state[x][y] == 1) t++, x += d[k][0], y += d[k][1];
                    if(t && state[x][y] == -1)
                    {
                        do
                        {
                            x -= d[k][0], y -= d[k][1];
                            state[x][y] = -1;
                        } while(x != i || y != j);
                        flag = 1;
                    }
                }
                if(flag)
                {
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

void write_valid_spot(std::ofstream& fout) {
    int n_valid_spots = next_valid_spots.size();
    srand(time(NULL));
    // Choose random spot. (Not random uniform here)
    
    int index = (rand() % n_valid_spots);
    Point p = next_valid_spots[index];
    // Remember to flush the output to ensure the last action is written to file.
    fout << p.x << " " << p.y << std::endl;
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

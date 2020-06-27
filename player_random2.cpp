#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <cassert>
/*struct Point {
    int x, y;
};
*/
struct Point {
    int x, y;
    Point() : Point(0, 0) {}
    Point(int x, int y) : x(x), y(y) {}
    bool operator==(const Point& rhs) const {
        return x == rhs.x && y == rhs.y;
    }
    bool operator!=(const Point& rhs) const {
        return !operator==(rhs);
    }
    Point operator+(const Point& rhs) const {
        return Point(x + rhs.x, y + rhs.y);
    }
    Point operator-(const Point& rhs) const {
        return Point(x - rhs.x, y - rhs.y);
    }
};
int player;
int cur_player;
const int SIZE = 8;
std::array<std::array<int, SIZE>, SIZE> board;
std::vector<Point> next_valid_spots;

const std::array<Point, 8> directions{{
    Point(-1, -1), Point(-1, 0), Point(-1, 1),
    Point(0, -1), /*{0, 0}, */Point(0, 1),
    Point(1, -1), Point(1, 0), Point(1, 1)
}};
const int MAPPOINT[8][8] = {
    {90,-60,10,10,10,10,-60,90},
    {-60,-80,5,5,5,5,-80,-60},
    {10,5,1,1,1,1,5,10},
    {10,5,1,1,1,1,5,10},
    {10,5,1,1,1,1,5,10},
    {10,5,1,1,1,1,5,10},
    {-60,-80,5,5,5,5,-80,-60},
    {90,-60,10,10,10,10,-60,90}
};
enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

int get_next_player(int player) {
    return 3 - player;
}
bool is_spot_on_board(Point p) {
    return 0 <= p.x && p.x < SIZE && 0 <= p.y && p.y < SIZE;
}
int get_disc(Point p)  {
    return board[p.x][p.y];
}
void set_disc(Point p, int disc) {
    board[p.x][p.y] = disc;
}
bool is_disc_at(Point p, int disc) {
    if (!is_spot_on_board(p))
        return false;
    if (get_disc(p) != disc)
        return false;
    return true;
}

bool is_spot_valid(Point center) {
    if (get_disc(center) != EMPTY)
        return false;
    for (Point dir: directions) {
        // Move along the direction while testing.
        Point p = center + dir;
        if (!is_disc_at(p, get_next_player(cur_player)))
            continue;
        p = p + dir;
        while (is_spot_on_board(p) && get_disc(p) != EMPTY) {
            if (is_disc_at(p, cur_player))
                return true;
            p = p + dir;
        }
    }
    return false;
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

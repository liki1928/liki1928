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

void read_board(std::ifstream& cin) {
    cin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cin >> board[i][j];
        }
    }
}

void read_valid_spots(std::ifstream& cin) {
    int n_valid_spots;
    cin >> n_valid_spots;
    int x, y;
    for (int i = 0; i < n_valid_spots; i++) {
        cin >> x >> y;
        next_valid_spots.push_back({x, y});
    }
}

void write_valid_spot(std::ofstream& fout) {
    srand(time(NULL));
    // May result in invalid actions.
    int x = (rand() % 8);
    int y = (rand() % 8);
    // Remember to flush the output to ensure the last action is written to file.
    fout << x << " " << y << std::endl;
    fout.flush();
}

int main(int, char** argv) {
    std::ifstream cin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(cin);
    read_valid_spots(cin);
    write_valid_spot(fout);
    cin.close();
    fout.close();
    return 0;
}

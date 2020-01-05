#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <stack>
#include <list>
#include <iomanip>

#include "IntCode.hpp"
#include "AStar.hpp"

using namespace std;

class OxygenSystem : public IntCode
{
public:
    OxygenSystem(const char* filename) : IntCode(filename) {
        for (auto& r : grid) r.fill(-1);
        grid[position_y][position_x] = 0;

        movements.push({ movement::north, movement::east, movement::south, movement::west });
    }
    
    enum class movement { north = 1, south = 2, west = 3, east = 4 };
    std::array<const char*, 5> movement_str = { "", "north", "south", "west", "east" };

    enum class status { hit_wall = 0, has_moved = 1, has_moved_and_repair = 2 };

    stack<list<movement>> movements;

    static constexpr int SIZE = 50;

    std::array<std::array<short, SIZE>, SIZE> grid;

    const char* player = "D";

    int position_x = SIZE / 2, position_y = SIZE / 2;
    int oxygen_x = 0, oxygen_y = 0;

    movement last_movement = movement::north;

    void display_grid()
    {
        cout << "x: " << position_x << ", y:" << position_y << "        stack size: " << movements.size() << endl;
        cout << string(SIZE, '-') << endl;

        cout << "    ";
        for (size_t column = 0; column < SIZE; ++column) {
            cout << (!(column % 10) && (column / 10) ? std::to_string(column / 10) : " ");
        }
        cout << "\n    ";
        for (size_t column = 0; column < SIZE; ++column)
            cout << column % 10;
        cout << "\n";

        for (int y = 0; y < SIZE; ++y) {
            cout << setw(2) << y << ": ";
            for (int x = 0; x < SIZE; ++x) {
                if (grid[y][x] == 2) {
                    cout << 'O';
                    oxygen_x = x;
                    oxygen_y = y;
                }
                else if (x == position_x && y == position_y) {
                    cout << player;
                }
                else if (x == SIZE / 2 && y == SIZE / 2) {
                    cout << 'x';
                }
                else {
                    if (grid[y][x] == 0) cout << ' ';
                    else if (grid[y][x] == 1) cout << 'X';
                    else if (grid[y][x] == 2) cout << 'O';
                    else cout << ' ';
                }
            }
            cout << endl;
        }
    }

    bool is_grid_empty(int y, int x) { return grid[y][x] != 1; }
    bool is_grid_visited(int y, int x) { return grid[y][x] == 0; }

    bool is_north_empty() { return is_grid_empty(position_y - 1, position_x); }
    bool is_south_empty() { return is_grid_empty(position_y + 1, position_x); }
    bool is_east_empty() { return is_grid_empty(position_y, position_x + 1); }
    bool is_west_empty() { return is_grid_empty(position_y, position_x - 1); }

    bool is_north_visited() { return is_grid_visited(position_y - 1, position_x); }
    bool is_south_visited() { return is_grid_visited(position_y + 1, position_x); }
    bool is_east_visited() { return is_grid_visited(position_y, position_x + 1); }
    bool is_west_visited() { return is_grid_visited(position_y, position_x - 1); }

    void compute() {
        IntCode::compute();

        for (int y = 0; y < SIZE; ++y)
            for (int x = 0; x < SIZE; ++x)
                if (grid[y][x] == -1) grid[y][x] = 1;
    }

    virtual long long get_input() override {
        if (movements.empty()) return 0;

        if (movements.top().empty()) {
            movements.pop();
        }
        if (movements.empty()) return 0;


        last_movement = movements.top().front();
        movements.top().pop_front();

        return (long long)last_movement;
    }

    virtual void get_output(long long output) override {
        if (output == (int)status::hit_wall) {
            switch (last_movement) {
            case movement::north:
                grid[position_y - 1][position_x] = 1;
                break;
            case movement::south:
                grid[position_y + 1][position_x] = 1;
                break;
            case movement::east:
                grid[position_y][position_x + 1] = 1;
                break;
            case movement::west:
                grid[position_y][position_x - 1] = 1;
                break;
            }
        }
        else {
            list<movement> moves;
            switch (last_movement) {
            case movement::north:
                --position_y;
                if (!is_north_visited()) moves.push_back(movement::north);
                if (!is_east_visited()) moves.push_back(movement::east);
                if (!is_west_visited()) moves.push_back(movement::west);
                moves.push_back(movement::south);
                break;
            case movement::south:
                ++position_y;
                if (!is_south_visited()) moves.push_back(movement::south);
                if (!is_east_visited()) moves.push_back(movement::east);
                if (!is_west_visited()) moves.push_back(movement::west);
                moves.push_back(movement::north);
                break;
            case movement::east:
                ++position_x;
                if (!is_east_visited()) moves.push_back(movement::east);
                if (!is_south_visited()) moves.push_back(movement::south);
                if (!is_north_visited()) moves.push_back(movement::north);
                moves.push_back(movement::west);
                break;
            case movement::west:
                --position_x;
                if (!is_west_visited()) moves.push_back(movement::west);
                if (!is_south_visited()) moves.push_back(movement::south);
                if (!is_north_visited()) moves.push_back(movement::north);
                moves.push_back(movement::east);
                break;
            }
            if (movements.top().size())
                movements.push(moves);

            grid[position_y][position_x] = (output == (int)status::has_moved_and_repair ? 2 : 0);
        }
    }

    // part two
    bool grid_filled()
    {
        for (int y = 0; y < SIZE; ++y)
            for (int x = 0; x < SIZE; ++x)
                if (grid[y][x] == 0) return false;
        return true;
    }

    void fill_grid()
    {
        std::array<std::array<short, SIZE>, SIZE> tmp = grid;

        for (int y = 1; y < SIZE - 1; ++y)
            for (int x = 1; x < SIZE - 1; ++x)
                if (grid[y][x] == 2) {
                    if (is_grid_empty(y - 1, x)) tmp[y - 1][x] = 2;
                    if (is_grid_empty(y + 1, x)) tmp[y + 1][x] = 2;
                    if (is_grid_empty(y, x - 1)) tmp[y][x - 1] = 2;
                    if (is_grid_empty(y, x + 1)) tmp[y][x + 1] = 2;
                }
        grid = tmp;
    }

};

int
main(int argc, char* argv[])
{
    { // part one
        OxygenSystem oxygen_system(argv[1]);
        oxygen_system.compute();
        oxygen_system.display_grid();

        Graph g;
        for (int y = 0; y < oxygen_system.SIZE; ++y) {
            string line;
            for (int x = 0; x < oxygen_system.SIZE; ++x) {
                line += (oxygen_system.grid[y][x] == 1 ? 'X' : '.');
            }
            g.push_back(std::move(line));
        }

        //cout << "------------------------------" << endl;
        //for (auto&& r : g) cout << r << endl;

        Node depart(oxygen_system.position_x, oxygen_system.position_y);
        Node objectif(oxygen_system.oxygen_x, oxygen_system.oxygen_y);
        //cout << "Depart: " << depart << "\nObjectif: " << objectif << endl;

        AStar astar;
        auto node = astar.cheminPlusCourt(g, objectif, depart);

        //cout << "------------------------------" << endl;
        //for (auto&& r : g) cout << r << endl;

        cout //<< node << "\n"
            << "Result: " << node.cout
            << "\n    ----> part one: " << (node.cout == 270 ? "Success" : "FAILED") << endl;
    }

    { // part two
        OxygenSystem oxygen_system(argv[1]);
        oxygen_system.compute();
        oxygen_system.display_grid();
 
        int minutes = 0;
        for (; !oxygen_system.grid_filled(); ++minutes) {
            oxygen_system.fill_grid();
        }
        //oxygen_system.display_grid();

        cout << "Result: " << minutes
            << "\n  ----> part two: " << (minutes == 364 ? "Success" : "FAILED") << endl; 
    }
}

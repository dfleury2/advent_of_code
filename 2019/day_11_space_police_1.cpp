#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>

#include "IntCode.hpp"

using namespace std;

class SpacePolice : public IntCode
{
public:
    SpacePolice(const char* filename) : IntCode(filename) {
        for (auto& r : grid) r.fill(-1);
    }

    virtual long long get_input() override {
        int input = grid[position_y][position_x];
        if (input == -1) input = 0;
        return input;
    }

    virtual void get_output(long long output) {
        outputs.push_back(output);

        if (outputs.size() == 2) {
            grid[position_y][position_x] = outputs[0];

            if (outputs[1] == 0) {
                // turn left
                switch (player_direction)
                {
                case direction::UP: player_direction = direction::LEFT; --position_x; break;
                case direction::LEFT: player_direction = direction::DOWN; ++position_y; break;
                case direction::DOWN: player_direction = direction::RIGHT; ++position_x;  break;
                case direction::RIGHT: player_direction = direction::UP; --position_y; break;
                }
            }
            else {
                // turn right
                switch (player_direction)
                {
                case direction::UP: player_direction = direction::RIGHT; ++position_x; break;
                case direction::LEFT: player_direction = direction::UP; --position_y; break;
                case direction::DOWN: player_direction = direction::LEFT; --position_x;  break;
                case direction::RIGHT: player_direction = direction::DOWN; ++position_y; break;
                }
            }

            outputs.clear();
        }
    }

    static constexpr int SIZE = 100;

    std::array<std::array<short, SIZE>, SIZE> grid;
    enum class direction { UP, DOWN, LEFT, RIGHT };

    const char* player = "^v<>";

    int position_x = SIZE / 2, position_y = SIZE / 2;
    direction player_direction = direction::UP;

    vector<long long> outputs;

    void display_grid()
    {
        cout << string(SIZE, '-') << endl;
        for (int y = 0; y < SIZE; ++y) {
            for (int x = 0; x < SIZE; ++x) {
                if (x == position_x && y == position_y) {
                    cout << (char)player[(int)player_direction];
                }
                else {
                    if (grid[y][x] == 0) cout << '.';
                    else if (grid[y][x] == 1) cout << '#';
                    else cout << ' ';
                }
            }
            cout << endl;
        }
    }
};

int
main(int argc, char* argv[])
{
    {   // Part one
        SpacePolice space_police(argv[1]);

        space_police.compute();
        space_police.display_grid();

        int count = 0;
        for (int y = 0; y < space_police.SIZE; ++y)
            for (int x = 0; x < space_police.SIZE; ++x)
                count += (space_police.grid[y][x] != -1);

        cout << "Result: " << count << endl
            << " ---> part one: " << (count == 1885 ? "Success" : "FAILED") << endl;
    }

    {   // Part two
        SpacePolice space_police(argv[1]);
        space_police.grid[space_police.position_y][space_police.position_x] = 1;

        space_police.compute();
        space_police.display_grid();

        int count = 0;
        for (int y = 0; y < space_police.SIZE; ++y)
            for (int x = 0; x < space_police.SIZE; ++x)
                count += (space_police.grid[y][x] != -1);

        cout << "Result: " << count << endl
            << " ---> part two: " << (count == 249 ? "Success" : "FAILED") << endl;
    }
}


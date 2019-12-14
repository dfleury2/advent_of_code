#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>

using namespace std;

long long relative_base = 0;

enum class parameter_mode { position, immediate, relative };

constexpr int SIZE = 100;

std::array<std::array<short, SIZE>, SIZE> grid;
enum class direction { UP, DOWN, LEFT, RIGHT };

const char* player = "^v<>";


int position_x = SIZE / 2, position_y = SIZE / 2;
direction player_direction = direction::UP;

vector<int> outputs;

void
display_grid()
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


long long
get_op_code(long long value)
{
    std::string s = std::to_string(value);
    while(s.size() < 5) s = "0" + s;
    
    return std::stoi(s.substr(s.size() - 2));
}

parameter_mode
get_mode(long long value, int index)
{
    std::string s = std::to_string(value);
    while(s.size() < 5) s = "0" + s;

    switch(s[2 - index]) {
        case '1': return parameter_mode::immediate;
        case '2': return parameter_mode::relative;
    }

    return parameter_mode::position;
}

long long
get_value(vector<long long>& op_codes, int index, parameter_mode mode)
{
    switch(mode) {
    case parameter_mode::position: return op_codes[op_codes[index]];
    case parameter_mode::relative: return op_codes[op_codes[index] + relative_base];
    }

    return op_codes[index];
}

void
compute(vector<long long>& op_codes)
{
    for(size_t i = 0; i < op_codes.size();) {

        auto op_code = get_op_code(op_codes[i]);
        auto mode_1 = get_mode(op_codes[i], 0);
        auto mode_2 = get_mode(op_codes[i], 1);
        auto mode_3 = get_mode(op_codes[i], 2);

        switch(op_code) {
            case 1: { // +
                op_codes[mode_3 == parameter_mode::immediate ? i + 3 : op_codes[i + 3] + (mode_3 == parameter_mode::relative ? relative_base : 0)] =
                        get_value(op_codes, i + 1, mode_1) +
                        get_value(op_codes, i + 2, mode_2);
                i += 4;
                break;
            }
            case 2: { // *
                op_codes[mode_3 == parameter_mode::immediate ? i + 3 : op_codes[i + 3] + (mode_3 == parameter_mode::relative ? relative_base : 0)] =
                        get_value(op_codes, i + 1, mode_1) *
                        get_value(op_codes, i + 2, mode_2);
                i += 4;
                break;
            }
            case 3: { // cin
                int input = grid[position_y][position_x];
                if (input == -1) input = 0;
                if (mode_1 == parameter_mode::relative) {
                    op_codes.at(op_codes[i + 1] + relative_base) = input;
                } else {
                    op_codes[op_codes[i + 1]] = input;
                }
                i += 2;
                break;
            }
            case 4: { // cout
                outputs.push_back(get_value(op_codes, i + 1, mode_1));
                i += 2;
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
                break;
            }
            case 5: { // jump-if-true
                if (get_value(op_codes, i + 1, mode_1)) {
                    i = get_value(op_codes, i + 2, mode_2);
                } else {
                    i += 3;
                }
                break;
            }
            case 6: { // jump-if-false
                if (!get_value(op_codes, i + 1, mode_1)) {
                    i = get_value(op_codes, i + 2, mode_2);
                } else {
                    i += 3;
                }
                break;
            }
            case 7: { // less than
                op_codes[mode_3 == parameter_mode::immediate ? i + 3 : op_codes[i + 3] + (mode_3 == parameter_mode::relative ? relative_base : 0)] =
                        get_value(op_codes, i + 1, mode_1) < get_value(op_codes, i + 2, mode_2) ? 1 : 0;
                i += 4;
                break;
            }
            case 8: { // equals
                op_codes[mode_3 == parameter_mode::immediate ? i + 3 : op_codes[i + 3] + (mode_3 == parameter_mode::relative ? relative_base : 0)] =
                        get_value(op_codes, i + 1, mode_1) == get_value(op_codes, i + 2, mode_2) ? 1 : 0;
                i += 4;
                break;
            }
            case 9: { // adjust the relative base
                relative_base +=  get_value(op_codes, i + 1, mode_1);
                i += 2;
                break;
            }
            case 99:
                return;
        }
    }
}

int
main(int argc, char* argv[])
{
    for (auto& r : grid) r.fill(-1);

    ifstream file(argv[1]);

    vector<long long> op_codes;
    for(long long op_code = 0;file >> op_code; file.ignore(512, ',')) {
        op_codes.push_back(op_code);
    }
    
    while(op_codes.size() < 100000) op_codes.push_back(0);

    compute(op_codes);

    display_grid();

    int count = 0;
    for (int y = 0; y < SIZE; ++y)
        for (int x = 0; x < SIZE; ++x)
            count += (grid[y][x] != -1);
    cout << count << endl;
}

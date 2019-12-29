#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <stack>
#include <list>

using namespace std;

enum class movement { north = 1, south = 2, west = 3, east = 4 };
const char* movement_str[] = { "", "north", "south", "west", "east"};

enum class status { hit_wall = 0, has_moved = 1, has_moved_and_repair = 2 };

stack<list<movement>> movements;

constexpr int SIZE = 50;

std::array<std::array<short, SIZE>, SIZE> grid;

const char* player = "D";

int position_x = SIZE / 2, position_y = SIZE / 2;

movement last_movement = movement::north;

void
display_grid()
{
    cout << "x: " << position_x << ", y:" << position_y << "        stack size: " << movements.size() << endl;
    cout << string(SIZE, '-') << endl;
    for (int y = 0; y < SIZE; ++y) {
        for (int x = 0; x < SIZE; ++x) {
            if (grid[y][x] == 2) cout << 'O';
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

bool is_north_empty() { return is_grid_empty(position_y-1, position_x); }
bool is_south_empty() { return is_grid_empty(position_y+1, position_x); }
bool is_east_empty() { return is_grid_empty(position_y, position_x+1); }
bool is_west_empty() { return is_grid_empty(position_y, position_x-1); }


bool is_north_visited() { return is_grid_visited(position_y - 1, position_x); }
bool is_south_visited() { return is_grid_visited(position_y + 1, position_x); }
bool is_east_visited() { return is_grid_visited(position_y, position_x + 1); }
bool is_west_visited() { return is_grid_visited(position_y, position_x - 1); }




long long relative_base = 0;

enum class parameter_mode { position, immediate, relative };

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
                if (movements.empty()) return;

                if (movements.top().empty()) {
                    movements.pop();
                }
                if (movements.empty()) return;


                last_movement = movements.top().front();
                movements.top().pop_front();

                //cout << "last_movement: " << movement_str[(int)last_movement] << endl;

                if (mode_1 == parameter_mode::relative) {
                    op_codes.at(op_codes[i + 1] + relative_base) = (int)last_movement;
                } else {
                    op_codes[op_codes[i + 1]] = (int)last_movement;
                }
                i += 2;
                break;
            }
            case 4: { // cout
                int output = get_value(op_codes, i + 1, mode_1);
                //cout << "status: " << output <<  endl;

                if (output == (int)status::hit_wall) {
                    switch(last_movement) {
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
                } else {
                    list<movement> moves;
                    switch(last_movement) {
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

                //if (output == (int)status::has_moved_and_repair) {
                //    display_grid();
                //    return;
                //}

                i += 2;
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

bool
grid_filled()
{
    for (int y = 0; y < SIZE; ++y)
        for (int x = 0; x < SIZE; ++x)
            if (grid[y][x] == 0) return false;
    return true;
}

void
fill_grid()
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

int
main(int argc, char* argv[])
{
    for (auto& r : grid) r.fill(-1);
    grid[position_y][position_x] = 0;

    movements.push({ movement::north, movement::east, movement::south, movement::west });

    ifstream file(argv[1]);

    vector<long long> op_codes;
    for(long long op_code = 0;file >> op_code; file.ignore(512, ',')) {
        op_codes.push_back(op_code);
    }
    
    while(op_codes.size() < 100000) op_codes.push_back(0);

    compute(op_codes);

    for (int y = 0; y < SIZE; ++y)
        for (int x = 0; x < SIZE; ++x)
            if (grid[y][x] == -1) grid[y][x] = 1;


    int minutes = 0;
    for(; !grid_filled(); ++minutes) {
        fill_grid();
        //display_grid();
    }
    cout << "minutes: " << minutes << endl;
}

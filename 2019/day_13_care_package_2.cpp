#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>

using namespace std;

array<array<int, 40>, 25> grid = { 0 };
int score = 0;

int paddle_position = 0;
int ball_position = 0;

vector<long long> outputs;

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
display()
{
    for (size_t y = 0; y < grid.size(); ++y) {
        for (size_t x = 0; x < grid[y].size(); ++x) {
            char c = ' ';
            switch (grid[y][x]) {
            case 1: c = '#'; break;
            case 2: c = 'H'; break;
            case 3: c = '-'; break;
            case 4: c = 'o'; break;
            }
            cout << c;
        }
        cout << "\n";
    }
    cout << "Paddle: " << paddle_position << ", Ball: " << ball_position;
    cout << "\nScore: " << score << endl;
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
                int input = 0;
                if (paddle_position < ball_position) input = 1;
                else if (paddle_position > ball_position) input = -1;

                if (mode_1 == parameter_mode::relative) {
                    op_codes.at(op_codes[i + 1] + relative_base) = input;
                } else {
                    op_codes[op_codes[i + 1]] = input;
                }
                i += 2;
                break;
            }
            case 4: { // cout
                auto output = get_value(op_codes, i + 1, mode_1);
                //cout << output << endl;
                i += 2;

                outputs.push_back(output);
                if (outputs.size() == 3) {
                    auto x = outputs[0];
                    auto y = outputs[1];
                    if (x == -1 && y == 0) score = outputs[2];
                    else {
                        grid[y][x] = outputs[2];
                        if (outputs[2] == 3) paddle_position = x;
                        else if (outputs[2] == 4) ball_position = x;
                    }

                    outputs.clear();
                    //display();
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
    ifstream file(argv[1]);

    vector<long long> op_codes;
    for(long long op_code = 0;file >> op_code; file.ignore(512, ',')) {
        op_codes.push_back(op_code);
    }

    op_codes[0] = 2;
    
    while(op_codes.size() < 100000) op_codes.push_back(0);

    compute(op_codes);

    display();
}

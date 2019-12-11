#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int input = 1;
int output = 0;

enum class parameter_mode { position, immediate };

int
get_op_code(int value)
{
    std::string s = std::to_string(value);
    while(s.size() < 5) s = "0" + s;
    
    return std::stoi(s.substr(s.size() - 2));
}

parameter_mode
get_mode(int value, int index)
{
    std::string s = std::to_string(value);
    while(s.size() < 5) s = "0" + s;

    return (s[2 - index] == '1' ? parameter_mode::immediate : parameter_mode::position);
}

int
get_value(vector<int>& op_codes, int index, parameter_mode mode)
{
    return (mode == parameter_mode::immediate ? op_codes[index] : op_codes[op_codes[index]]);
}

void
compute(vector<int>& op_codes)
{
    for(size_t i = 0; i < op_codes.size();) {

        auto op_code = get_op_code(op_codes[i]);
        auto mode_1 = get_mode(op_codes[i], 0);
        auto mode_2 = get_mode(op_codes[i], 1);
        auto mode_3 = get_mode(op_codes[i], 2);

        switch(op_code) {
            case 1: { // +
                if (mode_3 == parameter_mode::immediate) {
                    op_codes[i + 3] = get_value(op_codes, i + 1, mode_1) +
                            get_value(op_codes, i + 2, mode_2);
                } else {
                    op_codes[op_codes[i + 3]] = get_value(op_codes, i + 1, mode_1) +
                            get_value(op_codes, i + 2, mode_2);
                }
                i += 4;
                break;
            }
            case 2: { // *
                if (mode_3 == parameter_mode::immediate) {
                    op_codes[i + 3] = get_value(op_codes, i + 1, mode_1) *
                            get_value(op_codes, i + 2, mode_2);
                } else {
                    op_codes[op_codes[i + 3]] = get_value(op_codes, i + 1, mode_1) *
                            get_value(op_codes, i + 2, mode_2);
                }
                i += 4;
                break;
            }
            case 3: // cin
                op_codes[op_codes[i + 1]] = input;
                i += 2;
                break;
            case 4: // cout
                output = op_codes[op_codes[i + 1]];
                cout << output << endl;
                i += 2;
                break;
            case 99:
                return;
        }
    }
}

int main(int argc, char* argv[])
{
    const char* filename = argv[1];
    ifstream file(filename);

    vector<int> op_codes;
    for(int op_code = 0;file >> op_code; file.ignore(512, ',')) {
        op_codes.push_back(op_code);
    }
    
    compute(op_codes);
}

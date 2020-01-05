#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>

// ---------------------------   IntCode   -----------------------------------
class IntCode {
public:
    using OpCodes = std::vector<long long>;
    OpCodes op_codes;
    long long output = 0;

    IntCode(const char* filename) {
        std::ifstream file(filename);

        for (long long op_code = 0; file >> op_code; file.ignore(512, ',')) {
            op_codes.push_back(op_code);
        }
    }

    virtual ~IntCode() = default;

private:
    std::unordered_map<long long, long long> memory;
    long long relative_base = 0;

    enum class parameter_mode { position, immediate, relative };

    long long get_op_code(long long value) const
    {
        std::string s = std::to_string(value);
        while (s.size() < 5) s = "0" + s;

        return std::stoi(s.substr(s.size() - 2));
    }

    parameter_mode get_mode(long long value, int index) const
    {
        std::string s = std::to_string(value);
        while (s.size() < 5) s = "0" + s;

        switch (s[2 - index]) {
        case '1': return parameter_mode::immediate;
        case '2': return parameter_mode::relative;
        }

        return parameter_mode::position;
    }

    long long& get_value(long long index, parameter_mode mode = parameter_mode::immediate)
    {
        switch (mode) {
        case parameter_mode::position: index = get_value(index); break;
        case parameter_mode::relative: index = get_value(index) + relative_base; break;
        }

        return (index < op_codes.size() ? op_codes[index] : memory[index]);
    }

public:
    virtual long long get_input() = 0;
    virtual void get_output(long long output) {}

    void compute()
    {
        for (long long i = 0; i < (long long)op_codes.size();) {
            auto value = get_value(i);
            
            auto op_code = get_op_code(value);
            auto mode_1 = get_mode(value, 0);
            auto mode_2 = get_mode(value, 1);
            auto mode_3 = get_mode(value, 2);

            switch (op_code) {
            case 1: { // +
                get_value(i + 3, mode_3) = get_value(i + 1, mode_1) + get_value(i + 2, mode_2);
                i += 4;
                break;
            }
            case 2: { // *
                get_value(i + 3, mode_3) = get_value(i + 1, mode_1) * get_value(i + 2, mode_2);
                i += 4;
                break;
            }
            case 3: { // cin
                get_value(i + 1, mode_1) = get_input();
                i += 2;
                break;
            }
            case 4: { // cout
                output = get_value(i + 1, mode_1);
                get_output(output);
                i += 2;
                break;
            }
            case 5: { // jump-if-true
                if (get_value(i + 1, mode_1)) {
                    i = get_value(i + 2, mode_2);
                }
                else {
                    i += 3;
                }
                break;
            }
            case 6: { // jump-if-false
                if (!get_value(i + 1, mode_1)) {
                    i = get_value(i + 2, mode_2);
                }
                else {
                    i += 3;
                }
                break;
            }
            case 7: { // less than
                get_value(i + 3, mode_3) = (get_value(i + 1, mode_1) < get_value(i + 2, mode_2) ? 1 : 0);
                i += 4;
                break;
            }
            case 8: { // equals
                get_value(i + 3, mode_3) =(get_value(i + 1, mode_1) == get_value(i + 2, mode_2) ? 1 : 0);
                i += 4;
                break;
            }
            case 9: { // adjust the relative base
                relative_base += get_value(i + 1, mode_1);
                i += 2;
                break;
            }
            case 99:
                return;
            }
        }
    }
};

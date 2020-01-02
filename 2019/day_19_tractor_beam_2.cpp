#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include <iomanip>
#include <cstring>

#include "IntCode.hpp"

using namespace std;

class BeamDetector : public IntCode
{
public:
    list<int> inputs;

    virtual long long get_input() override {
        if (inputs.empty()) throw std::runtime_error("Inputs is empty");
        int input = inputs.front();
        inputs.pop_front();
        return input;
    }

    virtual void get_output(long long value) override {
        output = value;
    }

    int output = -1;
};


int
main(int argc, char* argv[])
{
    BeamDetector int_code;

    ifstream file(argv[1]);
    for (long long op_code = 0; file >> op_code; file.ignore(512, ',')) {
        int_code.op_codes.push_back(op_code);
    }

    int position_x = 1500, position_y = 1500;

    list<int> check_positions;
    const char* msg = "";

    bool check_mode = false;
    int check_count = 0;

    for (int y = 2060; y < 2200; ++y) {
        for (int x = 1650; x < 1950; ++x) {
            BeamDetector tmp;
            tmp.op_codes = IntCode::OpCodes(5000000, 0);
            memset(&tmp.op_codes[0], 0, tmp.op_codes.size() * sizeof(tmp.op_codes[0]));
            memcpy(&tmp.op_codes[0], &int_code.op_codes[0], int_code.op_codes.size() * sizeof(tmp.op_codes[0]));

            tmp.inputs.push_back(x);
            tmp.inputs.push_back(y);

            tmp.compute();

            cout << (tmp.output ? "#" : ".");
        }
        cout << endl;
    }

//    for (;;) {
//        BeamDetector tmp;
//        tmp.op_codes = IntCode::OpCodes(5000000, 0);
//        memset(&tmp.op_codes[0], 0, tmp.op_codes.size() * sizeof(tmp.op_codes[0]));
//        memcpy(&tmp.op_codes[0], &int_code.op_codes[0], int_code.op_codes.size() * sizeof(tmp.op_codes[0]));
//
//        if (!check_mode) {
//            // just looking for the
//            msg = "find...";
//            tmp.inputs.push_back(position_x);
//            tmp.inputs.push_back(position_y);
//        }
//        else {
//            // check left/top and left/bottom for the ship
//            tmp.inputs.push_back(check_positions.front());
//            check_positions.pop_front();
//
//            tmp.inputs.push_back(check_positions.front());
//            check_positions.pop_front();
//        }
//
//        tmp.compute();
//
//        cout << msg << " : " << position_x << ", " << position_y << " = " << (tmp.output ? "*" : " ")
//            << "    " << check_positions.size() << "   " << check_count
//            << endl;
//
//        if (!check_mode) {
//            if (tmp.output == 1) {
//                check_mode = true;
//                check_count = 0;
//
//                msg = "check left/top and left/bottom";
//                check_positions.push_back(position_x - 100);
//                check_positions.push_back(position_y);
//
//                check_positions.push_back(position_x - 100);
//                check_positions.push_back(position_y + 100);
//
//                position_x++;
//            }
//            else {
//                position_y++;
//            }
//        }
//        else {  // check mode
//            if (tmp.output == 1) {
//                check_count += 1;
//
//                if (check_count == 2) {
//                    cout << "wahou: " << position_x << ", " << position_y << endl;
//                    cout << (position_x - 101) * 10000 + position_y << endl;
//                    break;
//                }
//            }
//            else {
//                check_mode = false;
//                check_positions.clear();
//                check_count = 0;
//            }
//        }
//    }
}

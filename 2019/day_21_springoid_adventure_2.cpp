#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include <iomanip>
#include <cstring>

#include "IntCode.hpp"

using namespace std;

class SpringoidAdventure : public IntCode
{
public:
    list<int> inputs;

    virtual long long get_input() override {
        const char* instruction =
            "NOT C T   \n"
            "NOT B J   \n"
            "OR T J    \n"
            "NOT A T   \n"
            "OR T J    \n"
            "OR E T    \n"
            "OR H T    \n"
            "AND D T   \n"
            "AND T J   \n"
            "RUN       \n";

        static int position = 0;

        int input = instruction[position++];
        cout << (char)input;
        return input;
    }

    virtual void get_output(long long value) override {
        if (value < 127) {
            cout << (char)value;
        }
        output = value;
    }
    int output = 0;
};


int
main(int argc, char* argv[])
{
    SpringoidAdventure int_code;

    ifstream file(argv[1]);
    for (long long op_code = 0; file >> op_code; file.ignore(512, ',')) {
        int_code.op_codes.push_back(op_code);
    }

    SpringoidAdventure tmp;
    tmp.op_codes = IntCode::OpCodes(5000000, 0);
    memset(&tmp.op_codes[0], 0, tmp.op_codes.size() * sizeof(tmp.op_codes[0]));
    memcpy(&tmp.op_codes[0], &int_code.op_codes[0], int_code.op_codes.size() * sizeof(tmp.op_codes[0]));

    tmp.compute();
    cout << endl << tmp.output << endl;
}

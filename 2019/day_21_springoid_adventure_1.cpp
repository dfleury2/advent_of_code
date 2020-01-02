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
                "NOT D J   \n"
                "NOT J J   \n"
                "AND T J   \n"
                "NOT A T   \n"
                "OR  T J   \n"
                "WALK      \n";

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

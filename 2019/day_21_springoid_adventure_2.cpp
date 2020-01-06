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
    using IntCode::IntCode;

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
    SpringoidAdventure int_code(argv[1]);
    int_code.compute();
    cout << endl << int_code.output << endl;
}

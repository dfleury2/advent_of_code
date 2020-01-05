#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <algorithm>
#include <string>
#include <list>
#include <thread>

#include "IntCode.hpp"

using namespace std;

class CategorySix : public IntCode
{
public:
    int id = 0;

    virtual long long get_input() override {
        int input = id;
        string msg = "input: " + std::to_string(input) + "\n";
        cout << msg;
        cout.flush();
        return input;
    }

    virtual void get_output(long long value) override {
        string msg = "    output: " + std::to_string(value) + "\n";
        cout << msg;
        cout.flush();
        output = value;
    }
    int output = 0;
};


int
main(int argc, char* argv[])
{
    CategorySix int_code(argv[1]);

    ifstream file(argv[1]);
    for (long long op_code = 0; file >> op_code; file.ignore(512, ',')) {
        int_code.op_codes.push_back(op_code);
    }

    CategorySix tmp;
    tmp.op_codes = IntCode::OpCodes(2500000, 0);
    memset(&tmp.op_codes[0], 0, tmp.op_codes.size() * sizeof(tmp.op_codes[0]));
    memcpy(&tmp.op_codes[0], &int_code.op_codes[0], int_code.op_codes.size() * sizeof(tmp.op_codes[0]));

    vector<CategorySix> codes;
    for (int i = 0; i < 50; ++i) {
        auto copy = tmp;
        tmp.id = i;
        codes.push_back(std::move(copy));
    }
}

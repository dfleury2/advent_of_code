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
    using IntCode::IntCode;

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

    vector<CategorySix> codes;
    for (int i = 0; i < 50; ++i) {
        auto copy = int_code;
        copy.id = i;
        codes.push_back(std::move(copy));
    }
}

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>

#include "IntCode.hpp"

using namespace std;

class CarePackage : public IntCode
{
public:
    using IntCode::IntCode;

    array<array<int, 40>, 25> grid = { 0 };
    int tile_count = 0;
    int score = 0;
    int paddle_position = 0;
    int ball_position = 0;
    vector<long long> outputs;
    
    virtual long long get_input() override {
        int input = 0;
        if (paddle_position < ball_position) input = 1;
        else if (paddle_position > ball_position) input = -1;

        return input;
    }
    
    virtual void get_output(long long output) override {
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
        }
    }

    void display() {
        for (size_t y = 0; y < grid.size(); ++y) {
            for (size_t x = 0; x < grid[y].size(); ++x) {
                char c = ' ';
                switch (grid[y][x]) {
                case 1: c = '#'; break;
                case 2: c = 'H'; ++tile_count; break;
                case 3: c = '-'; break;
                case 4: c = 'o'; break;
                }
                cout << c;
            }
            cout << endl;
        }
        cout << "Paddle: " << paddle_position << ", Ball: " << ball_position;
        cout << "\nScore: " << score << endl;
    }
};

int
main(int argc, char* argv[])
{
    { // part one
        CarePackage care_package(argv[1]);
        care_package.compute();
        care_package.display();
        
        cout << "Result: " << care_package.tile_count
            << "\n   ----> " << (care_package.tile_count == 253 ? "Success" : "FAILED") << endl;
    }

    { // part two
        CarePackage care_package(argv[1]);
        care_package.op_codes[0] = 2;
        care_package.compute();
        care_package.display();

        cout << "Result: " << care_package.score
            << "\n   ----> " << (care_package.score == 12263 ? "Success" : "FAILED") << endl;
    }
}

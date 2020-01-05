#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "IntCode.hpp"

using namespace std;

class SensorBoost : public IntCode
{
public:
    long long input = 0;
    using IntCode::IntCode;
    virtual long long get_input() override { return input; }
};

int
main(int argc, char* argv[])
{
    {
        SensorBoost sensor_boost(argv[1]);
        sensor_boost.input = 1;
        sensor_boost.compute();
        cout << sensor_boost.output
            << " ---> part one: " << (sensor_boost.output == 2494485073 ? "Success" : "FAILED") << endl;
    }

    {
        SensorBoost sensor_boost(argv[1]);
        sensor_boost.input = 2;
        sensor_boost.compute();
        cout << sensor_boost.output
            << " ---> part two: " << (sensor_boost.output == 44997 ? "Success" : "FAILED") << endl;
    }
}

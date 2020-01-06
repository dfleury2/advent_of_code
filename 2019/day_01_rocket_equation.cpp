#include <istream>
#include <fstream>

#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

using namespace std;

const char* filename = nullptr;

// --- part one ---
struct FuelComputer {
    virtual ~FuelComputer() = default;

    int compute_fuel(istream&& is) {
        int total_fuel = 0;
        for(int mass = 0;is >> mass;)
            total_fuel += fuel(mass);
        return total_fuel;
    }

    virtual int fuel(int mass) {
        return mass / 3 - 2;
    }
};

// --- part two ---
struct FuelComputer2 : FuelComputer {
    int fuel(int v) override {
        int f = v /3 - 2;
        return (f <= 0 ? 0 : f + fuel(f));
    }
};

// --------------------------------------------------------------------------
TEST_CASE("Part one examples") {
    FuelComputer fc;
    CHECK(fc.compute_fuel(istringstream("12")) == 2);
    CHECK(fc.compute_fuel(istringstream("14")) == 2);
    CHECK(fc.compute_fuel(istringstream("1969")) == 654);
    CHECK(fc.compute_fuel(istringstream("100756")) == 33583);
}

TEST_CASE("Part one") {
    FuelComputer fc;
    CHECK(fc.compute_fuel(ifstream(filename)) == 3160932);
}

// --------------------------------------------------------------------------
TEST_CASE("Part two examples") {
    FuelComputer2 fc;
    CHECK(fc.compute_fuel(istringstream("12")) == 2);
    CHECK(fc.compute_fuel(istringstream("1969")) == 966);
    CHECK(fc.compute_fuel(istringstream("100756")) == 50346);
}

TEST_CASE("Part two") {
    FuelComputer2 fc;
    CHECK(fc.compute_fuel(ifstream(filename)) == 4738549);
}

// --------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    doctest::Context context(argc, argv);

    filename = argv[1]; // only to deal with final input filename

    int result = context.run();
    if (context.shouldExit())
        return result;

    { // part one
        FuelComputer fc;
        auto total_fuel = fc.compute_fuel(ifstream(filename));
        cout << "Day 01 - part one: " << total_fuel
            << "\n ---> " << (total_fuel == 3160932 ? "Success" : "FAILED") << endl;
    }

    { // part two
        FuelComputer2 fc;
        auto total_fuel = fc.compute_fuel(ifstream(filename));
        cout << "Day 01 - part two: " << total_fuel
            << "\n ---> " << (total_fuel == 4738549 ? "Success" : "FAILED") << endl;
    }

   return result;
}

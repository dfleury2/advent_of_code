#include <istream>

#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

using namespace std;

const char* filename = nullptr;

// --- part one ---
int compute_fuel(istream&& is) {
    int total_fuel = 0;
    for(int mass = 0;is >> mass;)
        total_fuel += mass / 3 - 2;
    return total_fuel;
}

// --- part two ---
int fuel(int v) {
    int f = v /3 - 2;
    if (f <= 0) return 0;
    return f + fuel(f);
}

int compute_fuel2(istream&& is) {
    int total_fuel = 0;
    for(int mass = 0;is >> mass;)
        total_fuel += fuel(mass);
    return total_fuel;
}

// --------------------------------------------------------------------------
TEST_CASE("Part one examples") {
    CHECK(compute_fuel(istringstream("12")) == 2);
    CHECK(compute_fuel(istringstream("14")) == 2);
    CHECK(compute_fuel(istringstream("1969")) == 654);
    CHECK(compute_fuel(istringstream("100756")) == 33583);
}

TEST_CASE("Part one") {
    CHECK(compute_fuel(ifstream(filename)) == 3160932);
}

// --------------------------------------------------------------------------
TEST_CASE("Part two examples") {
    CHECK(compute_fuel2(istringstream("12")) == 2);
    CHECK(compute_fuel2(istringstream("1969")) == 966);
    CHECK(compute_fuel2(istringstream("100756")) == 50346);
}

TEST_CASE("Part two") {
    CHECK(compute_fuel2(ifstream(filename)) == 4738549);
}

// --------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    doctest::Context context(argc, argv);

    filename = argv[1];

    int result = context.run();
    if (context.shouldExit())
        return result;

    { // part one
        auto total_fuel = compute_fuel(ifstream(filename));
        cout << "Result: " << total_fuel
            << "\n ---> part one: " << (total_fuel == 3160932 ? "Success" : "FAILED") << endl;
    }

    { // part two
        auto total_fuel = compute_fuel2(ifstream(filename));
        cout << "Result: " << total_fuel
            << "\n ---> part two: " << (total_fuel == 4738549 ? "Success" : "FAILED") << endl;
    }

   return result;
}

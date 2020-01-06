#include <iostream>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

using namespace std;

const char* filename = nullptr;

// --------------------------------------------------------------------------
struct ProgramAlarm {
    vector<int> op_codes;

    ProgramAlarm(const char* filename) : ProgramAlarm(ifstream(filename)) {
    }

    ProgramAlarm(istream&& is) {
        for(int op_code = 0; is >> op_code; is.ignore(512, ',')) {
            op_codes.push_back(op_code);
        }
    }

    int compute() {
        for(size_t i = 0; i < op_codes.size(); i += 4) {
            if (op_codes.at(i) == 99) break;
            else if (op_codes[i] == 1) {
                op_codes[op_codes[i + 3]] = op_codes[op_codes[i + 1]] + op_codes[op_codes[i + 2]];
            } else if (op_codes[i] == 2) {
                op_codes[op_codes[i + 3]] = op_codes[op_codes[i + 1]] * op_codes[op_codes[i + 2]];
            }
        }
        return op_codes[0];
    }

    string to_string() const {
        string tmp;
        for(int i = 0; i < op_codes.size(); ++i) {
            tmp += (i ? "," : "") + std::to_string(op_codes[i]);
        }
        return tmp;
    }
};

// --------------------------------------------------------------------------
TEST_CASE("Part one examples") {
    CHECK(ProgramAlarm(istringstream("1,9,10,3,2,3,11,0,99,30,40,50")).compute() == 3500);

    CHECK(ProgramAlarm(istringstream("1,0,0,0,99")).compute() == 2);

    SUBCASE("multiplication") {
        ProgramAlarm a(istringstream("2,3,0,3,99"));
        a.compute();
        CHECK(a.to_string() == "2,3,0,6,99");
    }

    SUBCASE("multiplication 2") {
        ProgramAlarm a(istringstream("2,4,4,5,99,0"));
        a.compute();
        CHECK(a.to_string() == "2,4,4,5,99,9801");
    }

    SUBCASE("More complete") {
        ProgramAlarm a(istringstream("1,1,1,4,99,5,6,0,99"));
        a.compute();
        CHECK(a.to_string() == "30,1,1,4,2,5,6,0,99");
    }
}

TEST_CASE("Part one") {
    ProgramAlarm alarm(filename);
    alarm.op_codes[1] = 12;
    alarm.op_codes[2] = 2;
    CHECK(alarm.compute() == 6568671);
}

// --------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    doctest::Context context(argc, argv);

    filename = argv[1]; // only to deal with final input filename

    int result = context.run();
    if (context.shouldExit())
        return result;

    { // Part one
        ProgramAlarm alarm(filename);
        alarm.op_codes[1] = 12;
        alarm.op_codes[2] = 2;
        auto value = alarm.compute();
        cout << "Day 02 - part one: " << value
                << "\n ---> " << (value == 6568671 ? "Success" : "FAILED") << endl;
    }

    { // Part two
        constexpr int EXPECTED_VALUE = 19690720;

        ProgramAlarm alarm(filename);

        int result = 0;
        for (int noun = 0; !result && noun < 100; ++noun) {
            for (int verb = 0; !result && verb < 100; ++verb) {
                auto tmp = alarm;
                tmp.op_codes[1] = noun;
                tmp.op_codes[2] = verb;

                if (tmp.compute() == EXPECTED_VALUE) {
                    result = 100 * noun + verb;
                }
            }
        }
        cout << "Day 02 - part two: " << result
                << "\n ---> " << (result == 3951 ? "Success" : "FAILED") << endl;
    }

    return result;
}

#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <algorithm>
#include <string>
#include <list>
#include <thread>
#include <array>
#include <set>

using namespace std;

using Eris = vector<string>;

set<Eris> previous;

void
complete(Eris& e) {
    e.insert(begin(e), string(begin(e)->size(), '+'));
    e.push_back(*begin(e));
    for (auto& r : e) {
        r = "+" + r + "+";
    }
}

unsigned long long
biodiversity(const Eris& e) {
    unsigned long long v = 0;
    unsigned long long index = 1;
    for (int r = 1; r <= 5; ++r) {
        for (int c = 1; c <= 5; ++c) {
            v += index * (e[r][c] == '#' ? 1 : 0);
            index *= 2;
        }
    }
    return v;
}

void
display(int i, const Eris& s) {
    cout << i << " -------------------- " << biodiversity(s) << "\n";
    for (auto&& r : s) {
        cout << r << "\n";
    }
}

void
evolve(Eris& e) {
    array<array<int, 5>, 5> counts = {};

    for (int r = 1; r <= 5; ++r) {
        for (int c = 1; c <= 5; ++c) {
            int count = (e[r][c - 1] == '#') +
                (e[r][c + 1] == '#') +
                (e[r - 1][c] == '#') +
                (e[r + 1][c] == '#');

            counts[r-1][c-1] = count;
        }
    }

    for (int r = 1; r <= 5; ++r) {
        for (int c = 1; c <= 5; ++c) {
            int count = counts[r - 1][c - 1];
            if (e[r][c] == '#' && count != 1) e[r][c] = '.';
            else if (e[r][c] == '.' && (count == 1 || count == 2)) e[r][c] = '#';
        }
    }
}

int
main(int argc, char* argv[])
{
    Eris eris;
    eris.push_back(".#.##");
    eris.push_back("...#.");
    eris.push_back("....#");
    eris.push_back(".#...");
    eris.push_back("..#..");
        
    complete(eris);
    
    for(int i = 0; true; ++i) {
        if (!previous.insert(eris).second) {
            break;
        }
        evolve(eris);
    }

    auto result = biodiversity(eris);
    cout << "Day 24 - part one: " << result
        << "\n ---> " << (result == 18842609 ? "Success" : "FAILED") << endl;
}

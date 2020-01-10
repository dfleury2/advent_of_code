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

using Eris = array<array<char, 5>, 5>;

set<Eris> previous;

unsigned long long
biodiversity(const Eris& e) {
    unsigned long long v = 0;
    unsigned long long index = 1;
    for (auto&& r : e) {
        for (auto&& c : r) {
            v += index * (c == '#' ? 1 : 0);
            index *= 2;
        }
    }
    return v;
}

void
display(int i, const Eris& s) {
    cout << i << " -------------------- " << biodiversity(s) << "\n";
    for (auto&& r : s) {
        for (auto&& c : r) {
            cout << c;
        }
        cout << "\n";
    }
}

void
evolve(Eris& e) {
    array<array<int, 5>, 5> counts = {};

    for (int r = 0; r < 5; ++r) {
        for (int c = 0; c < 5; ++c) {
            int count = (c ? (e[r][c - 1] == '#') : 0) +
                (c != 4 ? (e[r][c + 1] == '#') : 0) +
                (r ? (e[r - 1][c] == '#') : 0) +
                (r != 4 ? (e[r + 1][c] == '#') : 0);

            counts[r][c] = count;
        }
    }

    for (int r = 0; r < 5; ++r) {
        for (int c = 0; c < 5; ++c) {
            int count = counts[r][c];
            if (e[r][c] == '#' && count != 1) e[r][c] = '.';
            else if (e[r][c] == '.' && (count == 1 || count == 2)) e[r][c] = '#';
        }
    }
}

int
main(int argc, char* argv[])
{
    Eris eris = {{
        {'.', '#', '.', '#', '#'},
        {'.', '.', '.', '#', '.'},
        {'.', '.', '.', '.', '#'},
        {'.', '#', '.', '.', '.'},
        {'.', '.', '#', '.', '.'},
        }};
    
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

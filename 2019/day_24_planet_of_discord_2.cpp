#include <iostream>
#include <array>
#include <map>
#include <bitset>
#include <numeric>

using Eris = std::array<bool, 25>;
using Counts = std::array<int, 25>;

struct ErisData {
    Eris eris = {};
    Counts counts = {};

    int total_count() const noexcept {
        return std::accumulate(begin(eris), end(eris), 0) - (eris[12]);
    }
};

using Plutonian = std::map<int, ErisData>;

int
bug_count(const Plutonian& p) {
    int total_bug = 0;
    for (auto&& c : p) total_bug += c.second.total_count();
    return total_bug;
}

using namespace std;

void
display(int i, const Plutonian& p) {
    cout << i << " --------------------------------------\n";
    for (auto it = p.begin(); it != p.end(); ++it) {
        std::cout << "Depth " << it->first << ":\n";
        for (size_t i = 0; i < it->second.eris.size(); ++i) {
            if (i && !(i % 5)) std::cout << "\n";
            if (i == 12) std::cout << "?";
            else std::cout << (it->second.eris.operator[](i) ? '#' : '.');
        }
        std::cout << "\n\n";
    }
}

void
evolve(Plutonian& p) {
    int depth_min = p.begin()->first - 1;
    int depth_max = p.rbegin()->first + 1;

    for (int depth = depth_min; depth <= depth_max; ++depth) {
        auto& current = p[depth];
        for (size_t i = 0; i < current.eris.size(); ++i) {
            
            int count = 0;
            // Left
            if (!(i % 5)) { // A, F, K, P, U
                count += p[depth - 1].eris[11];
            }
            else if (i == 13) { // N
                auto& lower = p[depth + 1].eris;
                count += lower[4] + lower[9] + lower[14] + lower[19] + lower[24];
            }
            else {
                count += current.eris[i - 1];
            }

            // Right
            if (i % 5 == 4) { // E, J, O, T, Y
                count += p[depth - 1].eris[13];
            }
            else if (i == 11) { // L
                auto& lower = p[depth + 1].eris;
                count += lower[0] + lower[5] + lower[10] + lower[15] + lower[20];
            }
            else {
                count += current.eris[i + 1];
            }
            // Top
            if (i < 5) { // A, B, C, D, E
                count += p[depth - 1].eris[7];
            }
            else if (i == 17) { // R
                auto& lower = p[depth + 1].eris;
                count += lower[20] + lower[21] + lower[22] + lower[23] + lower[24];
            }
            else {
                count += current.eris[i - 5];
            }

            // Bottom
            if (i > 19) { // U, V, W, X, Y
                count += p[depth - 1].eris[17];
            }
            else if (i == 7) { // H
                auto& lower = p[depth + 1].eris;
                count += lower[0] + lower[1] + lower[2] + lower[3] + lower[4];
            }
            else {
                count += current.eris[i + 5];
            }

            current.counts[i] = count;
        }
    }

    for (auto it = p.rbegin(); it != p.rend(); ++it) {
        for (size_t i = 0; i < it->second.eris.size(); ++i) {
                int count = it->second.counts[i];
                if (it->second.eris[i] && count != 1) it->second.eris[i] = 0;
                else if (!it->second.eris[i] && (count == 1 || count == 2)) it->second.eris[i] = 1;
        }
    }

    for (auto it = begin(p); it != end(p);) {
        if (it->second.total_count() == 0) {
            it = p.erase(it);
        }
        else {
            ++it;
        }
    }
}

int
main(int argc, char* argv[])
{
    Plutonian plutonian;
    plutonian[0].eris = Eris{
        0,1,0,1,1,
        0,0,0,1,0,
        0,0,0,0,1,
        0,1,0,0,0,
        0,0,1,0,0
    };

    for (int i = 0; i < 200; ++i) {
        evolve(plutonian);
    }

    auto result = bug_count(plutonian);
    cout << "Day 24 - part two: " << result
        << "\n ---> " << (result == 2059 ? "Success" : "FAILED") << endl;
}

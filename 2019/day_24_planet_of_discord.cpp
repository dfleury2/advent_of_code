#include <iostream>
#include <vector>
#include <array>
#include <unordered_set>
#include <bitset>

using namespace std;

using Eris = std::bitset<25>;

unordered_set<Eris> previous;

unsigned long long
biodiversity(const Eris& e) {
    return e.to_ullong();
}

void
display(int i, const Eris& e) {
    cout << i << " -------------------- " << biodiversity(e) << "\n";
    for (size_t i = 0; i < e.size(); ++i) {
        if (i && !(i % 5)) cout << "\n";
        cout << (e[i] ? '#' : '.');
    }
    cout << "\n";
}

void
evolve(Eris& e) {
    array<int, 5 * 5> counts = {};

    for (size_t i = 0; i < e.size(); ++i) {
        int count = (i % 5 ? e[i - 1] : 0) +
            (i % 5 != 4 ? e[i + 1] : 0) +
            (i > 4 ? e[i - 5] : 0) +
            (i < 20 ? e[i + 5]: 0);

        counts[i] = count;
    }

    for (size_t i = 0; i < e.size(); ++i) {
        int count = counts[i];
        if (e[i] && count != 1) e[i] = 0;
        else if (!e[i] && (count == 1 || count == 2)) e[i] = 1;
    }
}

int
main(int argc, char* argv[])
{
    Eris eris{
        "01011"
        "00010"
        "00001"
        "01000"
        "00100"
    };
    
    for(int i = 0; previous.insert(eris).second; ++i) {
        display(i, eris);
        evolve(eris);
    }
    
    auto result = biodiversity(eris);
    cout << "Day 24 - part one: " << result
        << "\n ---> " << (result == 18842609 ? "Success" : "FAILED") << endl;
}

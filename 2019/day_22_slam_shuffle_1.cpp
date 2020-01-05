#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <algorithm>
#include <string>

using namespace std;

int
main(int argc, char* argv[])
{
    std::deque<int> cards;
    std::generate_n(std::back_inserter(cards), 10007, [i = 0]() mutable { return i++; });

    ifstream file(argv[1]);
    for (string line; getline(file, line);) {
        if (line.find("deal with increment") != std::string::npos) {
            int inc = stoi(line.substr(20));

            auto tmp = cards;
            for (int i = 0; !cards.empty(); i += inc, cards.pop_front()) {
                tmp[i % tmp.size()] = cards.front();
            }
            tmp.swap(cards);
        }
        else if (line == "deal into new stack") {
            std::reverse(begin(cards), end(cards));
        }
        else if (line.find("cut") != std::string::npos) {
            int cut = stoi(line.substr(4));
            if (cut < 0) {
                cut = cards.size() + cut;
            }
            auto  tmp = deque<int>(cards.begin(), cards.begin() + cut);
            cards.erase(cards.begin(), cards.begin() + cut);
            cards.insert(cards.end(), tmp.begin(), tmp.end());
        }
    }

    cout << std::distance(begin(cards),
                          std::find(begin(cards), end(cards), 2019))
        << endl;
}

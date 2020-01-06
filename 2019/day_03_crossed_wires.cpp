#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

int main(int argc, char* argv[])
{
    ifstream file(argv[1]);

    set<pair<int,int>> wire1, wire2;
    
    auto* wire = &wire1;
    for(string line; getline(file, line);) {
        int posX = 0, posY = 0;
        istringstream iss(line);
        for(string command; getline(iss, command, ',');) {
            istringstream iss(command);
            char direction; int distance;
            iss >> direction >> distance;
            for(int i = 0; i < distance; ++i) {
                switch(direction) {
                    case 'R': ++posX; break;
                    case 'L': --posX; break;
                    case 'U': ++posY; break;
                    case 'D': --posY; break;
                }
                wire->insert(make_pair(posX, posY));
            }
        }
        wire = &wire2;
    }
    
    std::vector<pair<int, int>> inters;
    std::set_intersection(wire1.begin(), wire1.end(),
                        wire2.begin(), wire2.end(),
                    std::back_inserter(inters));
    
    int min_distance = 2000000000;
    for(auto&& p : inters)
        min_distance = std::min(abs(p.first) + abs(p.second), min_distance);

    { // part one
        cout << "Day 03 - part one: " << min_distance
            << "\n ---> " << (min_distance == 3247 ? "Success" : "FAILED") << endl;
    }
}

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <utility>
#include <functional>

using namespace std;

struct Point {
    Point() = default;
    Point(int x, int y) : x(x), y(y) {}
    int x = 0, y = 0;
};

bool operator==(const Point& lh, const Point& rh) {
    return lh.x == rh.x && lh.y == rh.y;
}

struct PointHash {
    size_t operator()(const Point& p) const noexcept {
        return p.x * 10000 + p.y;
    }
};

int main()
{
    unordered_map<Point, int, PointHash> wire1, wire2;
    
    auto* wire = &wire1;
    for (string line; getline(cin, line);) {
        int posX = 0, posY = 0;
        int step = 0;
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
                wire->emplace(Point(posX, posY), ++step);
            }
        }
        wire = &wire2;
    }

    int min_distance = numeric_limits<int>::max();
    for (auto& [k, v] : wire1) {
        auto found = wire2.find(k);
        if (found != wire2.end())
            min_distance = std::min(v + found->second, min_distance);
    }
    cout << min_distance << std::endl;
}

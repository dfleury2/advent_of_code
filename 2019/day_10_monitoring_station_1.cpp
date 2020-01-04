#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

struct Asteroid;

double norme(const Asteroid& lh, const Asteroid& rh);
double scalaire(int x1, int y1, int x2, int y2);


struct Asteroid {
    int x = 0, y = 0;
    int visible_count = 0;

    bool
        is_visible_from(const Asteroid& other, const vector<Asteroid>& all) const noexcept {
        if (*this == other) return false;

        for (auto&& obstacle : all) {
            if (obstacle != *this && obstacle != other) {
                auto norme_u = norme(*this, other);
                auto norme_v = norme(*this, obstacle);
                auto norme_u_v = norme_u * norme_v;
                auto scale = norme_v / norme_u;
                auto scalaire_u_v = scalaire(other.x - this->x, other.y - this->y,
                    obstacle.x - this->x, obstacle.y - this->y);
                if (abs(norme_u_v - scalaire_u_v) < 0.000001 && scale < 1.0) {
                    return false;
                }
            }
        }
        return true;
    }

    bool operator==(const Asteroid& other) const noexcept {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Asteroid& other) const noexcept {
        return !this->operator==(other);
    }
};

double
norme(const Asteroid& lh, const Asteroid& rh) {
    return sqrt((rh.x - lh.x) * (rh.x - lh.x) + (rh.y - lh.y)*(rh.y - lh.y));
}

double
scalaire(int x1, int y1, int x2, int y2) {
    return x1 * x2 + y1 * y2;
}

int
main(int argc, char* argv[])
{
    ifstream file(argv[1]);
    auto asteroids = [&]() {
            vector<Asteroid> tmp;
            string line;
            for (int y = 0; getline(file, line); ++y)
                for (int x = 0; x < (int)line.size(); ++x)
                    if (line[x] == '#')
                        tmp.push_back(Asteroid{ x, y });
            return tmp;
        }();

    for (auto& asteroid : asteroids) {
        asteroid.visible_count = std::count_if(begin(asteroids), end(asteroids),
            [&](const Asteroid& other) { return other.is_visible_from(asteroid, asteroids);
        });
    }

    auto max_asteroid = std::max_element(begin(asteroids), end(asteroids),
        [](const Asteroid& lh, const Asteroid& rh) {
            return lh.visible_count < rh.visible_count;
    });

    cout << max_asteroid->x << ", " << max_asteroid->y << " = " << max_asteroid->visible_count << endl;
}

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

struct Asteroid;

double norme(const Asteroid& lh, const Asteroid& rh);
double scalaire(double x1, double y1, double x2, double y2);

struct Asteroid {
    double x = 0, y = 0;
    int visible_count = 0;
    bool is_destroyed = false;

    bool is_visible_from(const Asteroid& other, const vector<Asteroid>& all) const noexcept {
        if (*this == other) return false;

        for (auto&& obstacle : all) {
            if (obstacle != *this && obstacle != other && !obstacle.is_destroyed) {
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

    double distance_from(const Asteroid& other) const noexcept {
        return norme(*this, other);
    }

    double angle(const Asteroid& center) const noexcept {
        double o_x = center.x, o_y = center.y;
        double a_x = o_x, a_y = 0;
        double b_x = x, b_y = y;
        double s = acos(scalaire(a_x - o_x, a_y - o_y, b_x - o_x, b_y - o_y) /
            (norme(Asteroid{ a_x, a_y }, Asteroid{ o_x, o_y }) * norme(Asteroid{ b_x, b_y }, Asteroid{ o_x, o_y })));
        if (b_x < o_x) s = 2 * 3.1415926 - s;
        return s;
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
scalaire(double x1, double y1, double x2, double y2) {
    return x1 * x2 + y1 * y2;
}

int
main(int argc, char* argv[])
{
    ifstream file(argv[1]);
    auto asteroids = [&]() {
        vector<Asteroid> tmp;
        string line;
        for (double y = 0; getline(file, line); ++y)
            for (double x = 0; x < (int)line.size(); ++x)
                if (line[(int)x] == '#')
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

    Asteroid center = *max_asteroid;
    cout << center.x << ", " << center.y << " = " << center.visible_count << endl;
    cout << "Asteroid count: " << asteroids.size() << endl;
    asteroids.erase(std::remove(begin(asteroids), end(asteroids), center), end(asteroids));
    cout << "Asteroid count: " << asteroids.size() << endl;

    std::sort(begin(asteroids), end(asteroids),
        [&center](const Asteroid& lh, const Asteroid& rh) {
        auto lh_angle = lh.angle(center);
        auto rh_angle = rh.angle(center);
        if ((abs(lh_angle - rh_angle) < 0.00001)) return lh.distance_from(center) < rh.distance_from(center);
        return lh_angle < rh_angle;
    });

    int destroyed_count = 0;
    do {
        vector<std::reference_wrapper<Asteroid>> visible_asteroids;
        std::copy_if(begin(asteroids), end(asteroids),
            std::back_inserter(visible_asteroids),
            [&](const Asteroid& a) { return a.is_visible_from(center, asteroids); });

        for (auto&& a : visible_asteroids) {
            ++destroyed_count;
            cout << "destroying " << destroyed_count << ": " << a.get().x << ", " << a.get().y << endl;
            a.get().is_destroyed = true;
        }

        asteroids.erase(std::remove_if(begin(asteroids), end(asteroids),
                                        [](const Asteroid& a) { return a.is_destroyed; }),
                        end(asteroids));
    } while (destroyed_count < 200 && !asteroids.empty());
}

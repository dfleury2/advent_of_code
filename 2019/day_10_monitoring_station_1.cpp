#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

double
norme(int x1, int y1, int x2, int y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1)*(y2 - y1));
}

double
scalaire(int x1, int y1, int x2, int y2) {
    return x1 * x2 + y1 * y2;
}

int
main(int argc, char* argv[])
{
    vector<string> asteroids;
    for (string line; getline(cin, line);) asteroids.push_back(std::move(line));

    int max_visible = 0;
    int max_x = 0, max_y = 0;
    for (size_t y = 0; y < asteroids.size(); ++y) {
        for (size_t x = 0; x < asteroids[y].size(); ++x) {
            if (asteroids[y][x] == '#') {
                int visible_count = 0;
                for (size_t y1 = 0; y1 < asteroids.size(); ++y1) {
                    for (size_t x1 = 0; x1 < asteroids[y1].size(); ++x1) {
                        if ((y1 != y || x1 != x) && asteroids[y1][x1] == '#') {
                            //cout << "    " << x1 << ", " << y1 << endl;
                            bool is_visible = true;
                            for (size_t y2 = 0; is_visible && y2 < asteroids.size(); ++y2) {
                                for (size_t x2 = 0; is_visible && x2 < asteroids[y2].size(); ++x2) {
                                    if ((y2 != y || x2 != x) && (y2 != y1 || x2 != x1) && asteroids[y2][x2] == '#') {
                                        auto norme_u = norme(x, y, x1, y1);
                                        auto norme_v = norme(x, y, x2, y2);
                                        auto norme_u_v = norme_u * norme_v;
                                        auto scale = norme_v / norme_u;
                                        auto scalaire_u_v = scalaire(x1 - x, y1 - y, x2 - x, y2 - y);
                                        if (abs(norme_u_v - scalaire_u_v) < 0.0001 && scale < 1.0) {
                                            //cout << "        " << x2 << ", " << y2 << " = ";
                                            //cout << norme_u_v << ", " << scalaire_u_v << ", " << scale << endl;
                                            is_visible = false;
                                        }
                                    }
                                }
                            }
                            visible_count += is_visible;
                        }
                    }
                }
                if (visible_count > max_visible) {
                    max_x = x;
                    max_y = y;
                    max_visible = visible_count;
                }
            }
        }
    }
    cout << max_x << ", " << max_y << " = " << max_visible << endl;

}

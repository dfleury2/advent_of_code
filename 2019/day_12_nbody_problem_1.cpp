#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct Position {
    int x = 0, y = 0, z = 0;
    int vel_x = 0, vel_y = 0, vel_z = 0;
};

vector<Position> positions;

int
extract(const string& line, const char* name) {
    auto position = line.find(name);
    return stoi(line.substr(position + 2));
}

void
display(int step)
{
    cout << "After " << step << " steps:\n";
    for (auto&& p : positions) {
        cout << "pos=<x=" << p.x << ", y=" << p.y << ", z=" << p.z
            << ", vel=<x=" << p.vel_x << ", y=" << p.vel_y << ", z=" << p.vel_z << ">" << endl;
    }
}

void
gravity()
{
    for (size_t i = 0; i < positions.size() - 1; ++i) {
        for (size_t j = i + 1; j < positions.size(); ++j) {
            if (positions[i].x < positions[j].x) {
                positions[i].vel_x++;
                positions[j].vel_x--;
            } else if (positions[i].x > positions[j].x) {
                positions[i].vel_x--;
                positions[j].vel_x++;
            }
            if (positions[i].y < positions[j].y) {
                positions[i].vel_y++;
                positions[j].vel_y--;
            }
            else if (positions[i].y > positions[j].y) {
                positions[i].vel_y--;
                positions[j].vel_y++;
            }
            if (positions[i].z < positions[j].z) {
                positions[i].vel_z++;
                positions[j].vel_z--;
            }
            else if (positions[i].z > positions[j].z) {
                positions[i].vel_z--;
                positions[j].vel_z++;
            }
        }
    }
}

void
velocity()
{
    for (auto& p : positions) {
        p.x += p.vel_x;
        p.y += p.vel_y;
        p.z += p.vel_z;
    }
}

int
compute_energy()
{
    int energy = 0;
    for (auto&& p : positions) {
        energy += (abs(p.x) + abs(p.y) + abs(p.z)) *
            (abs(p.vel_x) + abs(p.vel_y) + abs(p.vel_z));
    }
    return energy;
}


int
main(int argc, char* argv[])
{
    ifstream file(argv[1]);
    
    for (string line; getline(file, line);) {
        Position pos;
        pos.x = extract(line, "x=");
        pos.y = extract(line, "y=");
        pos.z = extract(line, "z=");
        positions.push_back(pos);
    }
    
    for (int i = 0; i < 1000; ++i) {
        display(i);

        gravity();
        velocity();
    }

    cout << compute_energy() << endl;
}

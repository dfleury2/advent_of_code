/*
g++ -std=c++17 -o output output.cpp
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<string> lines;

bool check(int x, int y)
{
    for(int j = y; j < y + 100; ++j)
        for(int i = x; i < x + 100; ++i)
            if (lines[j][i] == '.') return false;
    
    return true;
}
    

int main()
{
    ifstream file("output.txt");
    for(string line; getline(file, line);) {
        lines.push_back(std::move(line));
    }
    
    int offset_x = 1650;
    int offset_y = 2060;
    
    for(auto&& l : lines) cout << l << endl;
    
    int min_value = 100000000;
    int min_x, min_y;
    for(int y = 0; y < lines.size() - 100; ++y)
        for(int x = 0; x < lines[y].size() - 100; ++x) {
            if (check(x, y)) {
                cout << "found something at " << x + offset_x << ", " << y + offset_y << endl;
                if ((x + offset_x) * 10000 + (y + offset_y) < min_value) {
                    min_value = (x + offset_x) * 10000 + (y + offset_y);
                    min_x = x + offset_x;
                    min_y = y + offset_y;
                }
                
            }
    }
    cout << min_x << ", " << min_y << "\n"
            << min_x * 10000 + min_y
            << endl;
    
}

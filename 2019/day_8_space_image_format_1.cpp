#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main()
{
    string line; getline(cin, line);

    string min_layer;
    int min_count = 150;
    for(int i = 0; i < line.size(); i += 25 * 6) {
        auto layer = line.substr(i, 25 * 6);
        auto count = std::count(begin(layer), end(layer), '0');
        if (count < min_count) {
            min_count = count;
            min_layer = layer;
        }
    }

    auto one_count = std::count(begin(min_layer), end(min_layer), '1');
    auto two_count = std::count(begin(min_layer), end(min_layer), '2');

    cout << min_layer << " (" << min_layer.size() << ")\n"
            << one_count << ", " << two_count << " = " << one_count * two_count
            <<endl;
}

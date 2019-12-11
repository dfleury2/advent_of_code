#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main()
{
    string line; getline(cin, line);

    vector<string> layers;
    for(int i = 0; i < line.size(); i += 25 * 6) {
        layers.push_back(line.substr(i, 25 * 6));
    }

    string image(layers.begin()->size(), '2');

    for(auto&& l : layers) {
        for(int i = 0; i < l.size(); ++i) {
            if (image[i] == '2' && l[i] != '2') {
                image[i] = l[i];
            }
        }
    }

    replace(begin(image), end(image), '1', ' ');
    replace(begin(image), end(image), '0', 'X');
    for(int i = 0; i < image.size(); i += 25) {
        cout << "[" << image.substr(i, 25) << "]" << endl;
    }
}

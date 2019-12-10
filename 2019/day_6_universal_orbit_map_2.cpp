#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <deque>

using namespace std;

using Nodes = std::unordered_map<std::string, std::unordered_set<std::string>>;
using Path = deque<string>;

Nodes nodes;
Path path_to_you, path_to_san;

void
display(const Nodes& nodes, const std::string& node_name, Path path = {})
{
    path.push_back(node_name);
    if (node_name == "YOU") path_to_you = path;
    else if (node_name == "SAN") path_to_san = path;

    auto childs = nodes.find(node_name);
    if (childs != nodes.end()) {
        for(auto&& child : childs->second) {
            display(nodes, child, path);
        }
    }
}

int main()
{
    for(string line; getline(cin, line);) {
        auto found = line.find(')');
        nodes[line.substr(0, found)].insert(line.substr(found + 1));
    }

    display(nodes, "COM");

    while (*path_to_you.begin() == *path_to_san.begin()) {
        path_to_you.pop_front();
        path_to_san.pop_front();
    }

    cout << path_to_you.size() + path_to_san.size() - 2 << endl;
}

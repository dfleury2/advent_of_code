#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <string>

using namespace std;

using Nodes = std::unordered_map<std::string, std::unordered_set<std::string>>;

Nodes nodes;
int total_depth = 0;

void
display(const Nodes& nodes, const std::string& node_name, int depth = 0) {

    cout << std::string(depth * 4, ' ') << node_name << " (" << depth/4 << ")" << endl;
    auto childs = nodes.find(node_name);
    if (childs != nodes.end()) {
        for(auto&& child : childs->second) {
            display(nodes, child, depth + 4);
        }
    }

    total_depth += depth /4;
}

int main()
{
    for(string line; getline(cin, line);) {
        auto found = line.find(')');
        nodes[line.substr(0, found)].insert(line.substr(found + 1));
    }

    display(nodes, "COM");

    cout << total_depth << endl;
}

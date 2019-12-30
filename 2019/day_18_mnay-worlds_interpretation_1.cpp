#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <array>
#include <stack>
#include <list>
#include <iomanip>
#include <unordered_map>

using namespace std;

using Space = vector<string>;

struct Player {
    int x = 0, y = 0;
};

struct KeyDoor {
    int key_x = 0, key_y = 0;
    int door_x = 0, door_y = 0;
    bool is_accessible = false;
    bool is_acquired = false;
    bool is_open = false;
};

using KeyDoors = unordered_map<char, KeyDoor>;

void
display_space(const Space& space, const KeyDoors& keyDoors, const Player& player)
{
    cout << "    ";
    for (size_t column = 0; column < space.begin()->size(); ++column) {
        cout << (!(column % 10) && (column / 10) ? std::to_string(column / 10) : " ");
    }
    cout << "    player: " << player.x << ", " << player.y;
    cout << "\n";

    cout << "    ";
    for (size_t column = 0; column < space.begin()->size(); ++column)
        cout << column % 10;
    cout << "\n";

    auto it_keyDoor = keyDoors.begin();

    for (size_t row = 0; row < space.size(); ++row) {
        cout << setw(2) << row << ": ";
        for (size_t column = 0; column < space[row].size(); ++column) {
            char c = space[row][column];
            cout << (c == '.' ? ' ' : c);
        }

        cout << "    ";
        if (it_keyDoor != keyDoors.end()) {
            const auto& kd = it_keyDoor->second;
            cout << it_keyDoor->first << "(" << setw(2) << kd.door_x << ", " << setw(2) << kd.door_y << "): "
                << "(" << setw(2) << kd.key_x << ", " << setw(2) << kd.key_y << ") "
                << (kd.is_accessible ? "+" : "-")
                << (kd.is_acquired ? "+" : "-")
                << (kd.is_open ? "O" : "X");
                ++it_keyDoor;
        }

        cout << "\n";
    }
}

bool is_key(char c) { return c >= 'a' && c <= 'z'; }
bool is_door(char c) { return c >= 'A' && c <= 'Z'; }
bool is_robot(char c) { return c == '@'; }
bool is_walkable(char c) { return c == '.' || is_key(c); }

void
detect_objects(const Space& space, KeyDoors& keyDoors, Player& player)
{
    for (size_t row = 0; row < space.size(); ++row) {
        for (size_t column = 0; column < space[row].size(); ++column) {
            char c = space[row][column];
            if (is_key(c) || is_door(c)) {
                auto& kd = keyDoors[toupper(c)];
                if (is_key(c)) {
                    kd.key_x = column;
                    kd.key_y = row;
                }
                else if (is_door(c)) {
                    kd.door_x = column;
                    kd.door_y = row;
                }
            }
            else if (is_robot(c)) {
                player.x = column;
                player.y = row;
            }
        }
    }

}

struct Node {
    int x = 0, y = 0;
    int cout = 0;
    double heuristique = 0.0;
    int from_x = -1, from_y = -1;

    friend bool operator<(const Node& lh, const Node& rh) {
        return lh.heuristique > rh.heuristique;
    }

    friend bool operator==(const Node& lh, const Node& rh) {
        return lh.x == rh.x && lh.y == rh.y;
    }

    friend bool operator!=(const Node& lh, const Node& rh) {
        return !(lh == rh);
    }

    friend ostream& operator<<(ostream& os, const Node& n) {
        return os << "x: " << n.x << ", y: " << n.y;
    }
};

using Graph = std::vector<std::string>;
using Nodes = std::vector<Node>;

double distance(const Node& u, const Node& v) {
    return std::sqrt((v.x - u.x) * (v.x - u.x) + (v.y - u.y) * (v.y - u.y));
}

Nodes
voisin(const Graph& g, const Node& u) {
    Nodes tmp;

    Node haut = u;
    haut.y++;
    if (haut.y < (int)g.size() && is_walkable(g[haut.y][haut.x])) tmp.push_back(haut);

    Node droite = u;
    droite.x++;
    if (droite.x < (int)g[0].size() && is_walkable(g[droite.y][droite.x])) tmp.push_back(droite);

    Node bas = u;
    bas.y--;
    if (bas.y >= 0 && is_walkable(g[bas.y][bas.x])) tmp.push_back(bas);

    Node gauche = u;
    gauche.x--;
    if (gauche.x >= 0 && is_walkable(g[gauche.y][gauche.x])) tmp.push_back(gauche);

    return tmp;
}

struct AccessiblePriorityQueue : std::priority_queue<Node> {
    std::priority_queue<Node>::container_type::iterator begin() { return c.begin(); }
    std::priority_queue<Node>::container_type::iterator end() { return c.end(); }
};

Node cheminPlusCourt(const Graph& g, const Node& objectif, const Node& depart)
{
    Nodes closedList;
    AccessiblePriorityQueue openList;

    openList.push(depart);
    while (openList.size()) {
        auto u = openList.top();
        openList.pop();

        if (u == objectif) {
            int cout = u.cout;
            // Reconstituer chemin
            Node previous;
            previous.x = u.from_x;
            previous.y = u.from_y;
            while (!(previous == depart)) {
                for (auto&& next : closedList) {
                    if (next == previous) {
                        u = previous;
                        previous.x = next.from_x;
                        previous.y = next.from_y;
                        break;
                    }
                }
            }
            u.cout = cout;
            return u;
        }

        for (Node v : voisin(g, u)) {
            v.cout = u.cout + 1;
            v.heuristique = v.cout + distance(v, objectif);

            auto foundClosed = std::find(begin(closedList), end(closedList), v);
            auto foundOpen = std::find(openList.begin(), openList.end(), v);
            if (foundClosed != closedList.end()) {
                if (foundClosed->cout > v.cout) {
                    foundClosed->cout = v.cout;
                    foundClosed->from_x = v.x;
                    foundClosed->from_y = v.y;
                }
            }
            else if (foundOpen != openList.end()) {
                if (foundOpen->cout > v.cout) {
                    foundOpen->cout = v.cout;
                    foundOpen->from_x = v.x;
                    foundOpen->from_y = v.y;
                }
            }
            else {
                v.from_x = u.x;
                v.from_y = u.y;
                openList.push(v);
            }
        }
        closedList.push_back(u);
    }

    return Node();
}


int
main(int argc, char* argv[])
{
    Space space;
    KeyDoors keyDoors;
    Player player;

    ifstream file(argv[1]);
    for (string line; getline(file, line);) space.push_back(std::move(line));

    detect_objects(space, keyDoors, player);

    Node depart;
    depart.x = player.x;
    depart.y = player.y;

    for (auto&[door, kd] : keyDoors) {
        Node objectif;
        objectif.x = kd.key_x;
        objectif.y = kd.key_y;

        Node path = cheminPlusCourt(Graph(space), objectif, depart);
        kd.is_accessible = (path != Node());
        if (kd.is_accessible) cout << path.cout << endl;
    }

    display_space(space, keyDoors, player);
}

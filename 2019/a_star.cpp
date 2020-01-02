#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>
#include <algorithm>

using namespace std;

int count = 0;

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
    if (haut.y < (int)g.size() && g[haut.y][haut.x] == '.') tmp.push_back(haut);

    Node droite = u;
    droite.x++;
    if (droite.x < (int)g[0].size() && g[droite.y][droite.x] == '.') tmp.push_back(droite);

    Node bas = u;
    bas.y--;
    if (bas.y >= 0 && g[bas.y][bas.x] == '.') tmp.push_back(bas);

    Node gauche = u;
    gauche.x--;
    if (gauche.x >= 0 && g[gauche.y][gauche.x] == '.') tmp.push_back(gauche);

    return tmp;
}

struct AccessiblePriorityQueue : std::priority_queue<Node> {
    std::priority_queue<Node>::container_type::iterator begin() { return c.begin(); }
    std::priority_queue<Node>::container_type::iterator end() { return c.end(); }
};

Node cheminPlusCourt(Graph& g, const Node& objectif, const Node& depart)
{
    Nodes closedList;
    AccessiblePriorityQueue openList;

    openList.push(depart);
    while (openList.size()) {
        auto u = openList.top();
        openList.pop();

        if (u == objectif) {
            // Reconstituer chemin
            Node previous;
            previous.x = u.from_x;
            previous.y = u.from_y;
            g[previous.y][previous.x] = '+';
            while (!(previous == depart)) {
                for (auto&& next : closedList) {
                    if (next == previous) {
                        u = previous;
                        previous.x = next.from_x;
                        previous.y = next.from_y;
                        g[previous.y][previous.x] = '+';
                        ++::count;
                        break;
                    }
                }
            }
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

int main()
{
    Graph g;
    //g.push_back("??????????");
    //g.push_back("??XXX?????");
    //g.push_back("XXX.XXXX??");
    //g.push_back(".      X??");
    //g.push_back("XXXX.XPX??");
    //g.push_back("???XXX X??");
    //g.push_back("?????XXX??");
    
    //g.push_back("??XXXXXX??");
    //g.push_back("??X    X??");
    //g.push_back("XXX XX X??");
    //g.push_back(".    X X??");
    //g.push_back("XXXX XPX??");
    //g.push_back("???X   X??");
    //g.push_back("???XXXXX??");

    //           0123456789   
    //g.push_back("          "); // 6
    //g.push_back("          "); // 5
    //g.push_back(" XXXXXXX  "); // 4
    //g.push_back("       X  "); // 3
    //g.push_back("       X  "); // 2
    //g.push_back("       X  "); // 1
    //g.push_back("P         "); // 0

    g.push_back("    ###################                         ");
    g.push_back("    #.#...............#                         ");
    g.push_back("    #.#.#######.#####.#                         ");
    g.push_back("    #.#.#.......#...#.#                         ");
    g.push_back("    #.#.#.#######.###.#                         ");
    g.push_back("    #.#.#.#     #.....#                         ");
    g.push_back("    #.#.#.#  ####.####                          ");
    g.push_back("    #...#.# #.....#                             ");
    g.push_back("    #.###.# #.####                              ");
    g.push_back("    #...#.# #.#                                 ");
    g.push_back("    ###.#.# #.#                                 ");
    g.push_back("    #...#.# ###                                 ");
    g.push_back("    #.###.#                                     ");
    g.push_back("    #.#...#                                     ");
    g.push_back("    #.#.####                                    ");
    g.push_back("    #.#.....#                                   ");
    g.push_back("    #.#####.##                                  ");
    g.push_back("    #.#...#...#                                 ");
    g.push_back("    #.#.#.###.##                                ");
    g.push_back("    #.#.#.# #...#                               ");
    g.push_back("    #.#.#.#  ##.#       ###                     ");
    g.push_back("    #...#.# #...#       #x#                     ");
    g.push_back("     ####.#.#.########  #.########              ");
    g.push_back("    #...#.#.#.........# #.........#             ");
    g.push_back("     ##.#.#.#########.###########.#             ");
    g.push_back("    #...#.#.....#.....#.....#...#.#             ");
    g.push_back("    #.###.#####.#.#.###.#.###.#.#.#             ");
    g.push_back("    #...#.......#.#.#...#.....#...#             ");
    g.push_back("    #.#.#####.###.#.#.######## ###              ");
    g.push_back("    #.#.......#...#.#.........#                 ");
    g.push_back("    #.#########.#.###########.#                 ");
    g.push_back("    #.....#.#...#.#.........#.#                 ");
    g.push_back("     ####.#.#.#####.#.#######.#                 ");
    g.push_back("    #...#...#.#.....#.......#.#                 ");
    g.push_back("    #.#.#.###.#.###########.#.#                 ");
    g.push_back("    #.#...#...#...#.....#...#.#                 ");
    g.push_back("    #.## ##.#.###.#.#####.###.#                 ");
    g.push_back("    #.....#.#.#...#.....#.#...#                 ");
    g.push_back("     ####.#.###.#######.#.#.##                  ");
    g.push_back("     .....#.............#...#                   ");
    g.push_back("      # ## ############# ###                    ");
    g.push_back("                                                ");
    
    //std::reverse(begin(g), end(g));
    
    Node depart;
    depart.x = 25;
    depart.y = 21;

    Node objectif;
    objectif.x = 5;
    objectif.y = 39;

    Node next = depart;
    do {
        cout << next << endl;
        next = cheminPlusCourt(g, objectif, next);
    } while (next != objectif && next != Node());
    cout << next << endl;

    int plus_count = 0;
    for (auto&& r : g) {
        cout << r << endl;
        plus_count += std::count(begin(r), end(r), '+');
    }
    cout << ::count << ", " << plus_count << endl;
}

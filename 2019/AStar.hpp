#pragma once

#include <ostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>

struct Node {
    Node() = default;
    Node(int x, int y) : x(x), y(y) {}

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

    friend std::ostream& operator<<(std::ostream& os, const Node& n) {
        return os << "x: " << n.x << ", y: " << n.y;
    }
};

using Graph = std::vector<std::string>;
using Nodes = std::vector<Node>;

class AStar
{
public:
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
                int cout = u.cout;
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

    Nodes voisin(const Graph& g, const Node& u) {
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

private:
    struct AccessiblePriorityQueue : std::priority_queue<Node> {
        std::priority_queue<Node>::container_type::iterator begin() { return c.begin(); }
        std::priority_queue<Node>::container_type::iterator end() { return c.end(); }
    };

    double distance(const Node& u, const Node& v) {
        return std::sqrt((v.x - u.x) * (v.x - u.x) + (v.y - u.y) * (v.y - u.y));
    }
};

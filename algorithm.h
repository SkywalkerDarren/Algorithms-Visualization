#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <vector>
#include <queue>
#include <string>
#include <iostream>
#include <cmath>

class algorithm
{
public:
    struct Node {
        int i, j, g, h;
        std::string val;
        Node(int i, int j) {
            this->i = i;
            this->j = j;
            g = -1;
            h = 0;
        }
        bool operator<(const Node &a) const {
            if ((a.g + a.h) == (g + h)) {
                if (a.h == h) {
                    return a.g < g;
                }
                return a.h < h;
            }
            return (a.g + a.h) < (g + h);
        }
    };
    typedef std::vector<std::vector<Node>> nodeMap;
    algorithm(nodeMap &p);
    class distance {
    public:
        int manhattan(int i, int j, int x2, int y2);
        int euclidean(int i, int j, int x2, int y2);
        int chebyshev(int i, int j, int x2, int y2);
        int bfs(int i, int j, int x2, int y2);
    };
    void setAlgo(int (algorithm::distance::*algo)(int, int, int, int));
    nodeMap start();
private:
    const int diri[4] = {0, 1, 0, -1};
    const int dirj[4] = {1, 0, -1, 0};
    const char dir[4][4] = {"←", "↑", "→", "↓"};
    int x1, x2, y1, y2;
    int (distance::*algo)(int, int, int, int) = NULL;
    nodeMap p;
    std::vector<std::vector<int>> g;
    std::vector<std::vector<int>> s;
    void print(nodeMap &p, int x2, int y2);
    nodeMap astar(int x1, int y1, int x2, int y2,
               nodeMap p,
               const std::vector<std::vector<int>> g, std::vector<std::vector<int>> s,
               int(distance::*algo)(int, int, int, int));
    bool overflow(int a, int b, int m, int n);
};

#endif // ALGORITHM_H

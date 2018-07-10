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
    typedef std::vector<std::vector<std::vector<std::string>>> list;
    algorithm(std::vector<std::vector<std::string>> &p);
    class distance {
    public:
        int manhattan(int i, int j, int x2, int y2);
        int euclidean(int i, int j, int x2, int y2);
        int chebyshev(int i, int j, int x2, int y2);
        int bfs(int i, int j, int x2, int y2);
    };
    void setAlgo(int (algorithm::distance::*algo)(int, int, int, int));
    list start();
private:
    struct Node {
        int i, j, g, h;
        bool operator<(const Node &a) const {
            if ((a.g + a.h) == (g + h)) {
                if(a.h == h) {
                    return a.g < g;
                }
                return a.h < h;
            }
            return (a.g + a.h) < (g + h);
        }
    };

    const int diri[4] = {0, 1, 0, -1};
    const int dirj[4] = {1, 0, -1, 0};
    const char dir[4][4] = {"←", "↑", "→", "↓"};
    int x1, x2, y1, y2;
    int (distance::*algo)(int, int, int, int) = NULL;
    std::vector<std::vector<std::string>> p;
    std::vector<std::vector<int>> g;
    std::vector<std::vector<int>> s;
    void print(std::vector<std::vector<std::string> > &p, int x2, int y2);
    list astar(int x1, int y1, int x2, int y2,
               std::vector<std::vector<std::string>> &p,
               const std::vector<std::vector<int>> g, std::vector<std::vector<int>> s,
               int(distance::*algo)(int, int, int, int));
    bool overflow(int a, int b, int m, int n);
};

#endif // ALGORITHM_H

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
    /*
    maze:
        startPoint
        endPoint
        printList:
            direction/state
            preNodePoint
            current cost
            predict cost
        total cost
        total distance
    */
    class Maze
    {

    };
    Maze maze;
    algorithm(Maze maze);
    class distance {
    public:
        int manhattan(int i, int j, int x2, int y2);
        int euclidean(int i, int j, int x2, int y2);
        int chebyshev(int i, int j, int x2, int y2);
        int bfs(int i, int j, int x2, int y2);
    };
    void setAlgo(int (algorithm::distance::*algo)(int, int, int, int));
    printList start();
private:


    const int diri[4] = {0, 1, 0, -1};
    const int dirj[4] = {1, 0, -1, 0};
    const char dir[4][4] = {"←", "↑", "→", "↓"};
    int x1, x2, y1, y2;
    int (distance::*algo)(int, int, int, int) = NULL;
    printList p;
    std::vector<std::vector<int>> g;
    std::vector<std::vector<int>> s;
    void print(std::vector<std::vector<std::string> > &p, int x2, int y2);
    printList astar(int x1, int y1, int x2, int y2, printList &p,
               const std::vector<std::vector<int>> g,
               std::vector<std::vector<int>> s,
               int(distance::*algo)(int, int, int, int));
    bool overflow(int a, int b, int m, int n);
};

#endif // ALGORITHM_H

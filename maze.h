#ifndef MAZE_H
#define MAZE_H


class Maze
{
public:
    Maze();

    struct Point
    {
        int i, j;
        Point(int i, int j);
    };

    struct Node {
        int g, h;
        Point p;
        Node(int i, int j);
        Node(Point p);
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
};

#endif // MAZE_H

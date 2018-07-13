#include "maze.h"

Maze::Maze()
{

}

Maze::Node(int i, int j)
{
    Point p(i,j);
    this->Node(p);
}

Maze::Node(Point p)
{
    this->Node.p = p;
    this->Node.g = 0;
    this->Node.h = 0;
}

Maze::Point(int i, int j)
{
   this->i = i;
   this->j = j;
}

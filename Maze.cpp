#include "Maze.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
bool visited(Cell & c, std::vector< Cell* > & visited)
{
    for (int i = 0; i < int(visited.size()); ++i)
    {
        if (c.r() == visited[i]->r() &&
            c.c() == visited[i]->c())
        {
            return 1;
        }
    }
    return 0;
}

//Cell Methods
std::ostream &operator<<(std::ostream &cout, const Cell &c)
{
    cout << "(" << c.c() << ' ' << c.r() << ')';
    return cout;
}


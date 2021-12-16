#ifndef MAZE_H
#define MAZE_H
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

class Cell
{
public:
Cell(int r = 0, int c = 0)
    :r_(r), c_(c)
    {}

    int &r()
    {return r_;}
    int &c()
    {return c_;}
    int r() const
    { return r_;}
    int c() const
    {return c_;}

    int FindNRow()
    {return r_ - 1;}
    int FindSRow()
    {return r_ + 1;}
    int FindERow()
    {return r_;}
    int FindWRow()
    {return r_;}
    
    int FindNCol()
    {return c_;}
    int FindSCol()
    {return c_;}
    int FindECol()
    {return c_ + 1;}
    int FindWCol()
    {return c_ - 1;}

    Cell FindN()
    {
        return Cell(FindNRow(), FindNCol());
    }

    Cell FindS()
    {
        return Cell(FindNRow(), FindNCol());
    }
    Cell FindE()
    {
        return Cell(FindNRow(), FindNCol());
    }
    Cell FindW()
    {
        return Cell(FindNRow(), FindNCol());
    }

    bool N = 1;
    bool S = 1;
    bool E = 1;
    bool W = 1;
    bool visable = 0;
private:
    int r_;
    int c_;
};

class PunchedWall
{
public:
    PunchedWall(Cell  a, Cell b)
    {
        c0.r() = a.r();
        c0.c() = a.c();
        c1.r() = b.r();
        c1.c() = b.c();
    }
    
    Cell c0, c1;
};


bool visited(Cell & c, std::vector< Cell* > &visited);
std::ostream &operator<<(std::ostream &, const Cell &);
#endif

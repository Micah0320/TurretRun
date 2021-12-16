#ifndef GLOBALS_H
#define GLOBALS_H
#include <vector>
#include <cmath>
#include <time.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "gl3d.h"
#include "View.h"
#include "bot.h"
//#include "bullet.h"

GLuint texture[4];
int WORLD_WIDTH = 1024;
int WORLD_HEIGHT = 768;
//World Information
int pathWidth = 1;
int depth_x = 0;
int depth_z = 0;
int size = 10;
void toggleAllOff();
void toggleAdjacent();
void toggleLineOfSight();


//Player Info
float Player_x = 2;
float Player_y = 0.5;
float Player_z = 2;
int PLAYER_CELL = 0;
//For computing the cells the player is in
float xTrav = 0;
float zTrav = 0;

//For Limiting movements in a cell
float LIM_NEG_X = -3;
float LIM_X = 3;
float LIM_NEG_Z = -3;
float LIM_Z = 3;


bool BirdsEye = 0;

//Camera Info
float horAngle = 3.14f; //Update -> mouseSpeed * deltaTime * float(1024/2 - xpos );
float verAngle = 0.0f; // Update -> mouseSpeed * deltaTime * float(768/2 - ypos );
//Movement of Player (More Camera)
float MoveSpeed = .005;
glm::vec3 Player(2.0f, 0.5f, 2.0f);
glm::vec3 Front(cos(verAngle) * sin(horAngle),
                sin(verAngle),
                cos(verAngle) * cos(horAngle));//1.0f, 0.0f, 0.0f);
glm::vec3 Up(0.0f, 1.0f, 0.0f);
glm::vec3 Right(sin(horAngle - 3.14f/2.0f),
                0,
                cos(horAngle - 3.14f/2.0f));

float deltaTime = 10.0f;	// Time between current frame and last frame
//float lastFrame = clock();
//Mouse Util
int xmousePos = 0;
int ymousePos = 0;
float mouseSpeed = 0.001;

void updateLook()
{
    horAngle = mouseSpeed * deltaTime * float(WORLD_WIDTH/2 - xmousePos );
    verAngle =  mouseSpeed * deltaTime * float(WORLD_HEIGHT/2 - ymousePos );


    
}

//Maze Things
std::vector< PunchedWall > punched_walls;
std::vector< Cell * > maze;
std::vector< int > bots_Cell_x;
std::vector< int > bots_Cell_y;
std::vector< int > bots_life;
std::vector< bool > bots_alive;

//Updates what Cell the player is in based of off pos data

void updateLimits()
{
    LIM_Z = 3.0;
    LIM_X = 3.0;
    LIM_NEG_Z = -3.0;
    LIM_NEG_X = -3.0;

    if((*maze[PLAYER_CELL]).N)
        {
            LIM_NEG_Z = -0.25;
        }
        if((*maze[PLAYER_CELL]).S)
        {
            LIM_Z = 0.25;
        }
        if((*maze[PLAYER_CELL]).E)
        {
            LIM_X = 0.25;
        }
        if((*maze[PLAYER_CELL]).W)
        {
            LIM_NEG_X = -0.25;
        }
    
}

void updatePlayer()
{
    //mygllib::View & view = *(mygllib::SingletonView::getInstance());
    if (zTrav >= 2)
    {
        zTrav = 0;
        PLAYER_CELL += size;
        toggleAllOff();
        toggleAdjacent();
        updateLimits();
        return;
    }
    if (zTrav <= -2)
    {
        zTrav = 0;
        PLAYER_CELL -= size;
        toggleAllOff();
        toggleAdjacent();
        updateLimits();
        return;
    }
    if (xTrav >= 2)
    {
        xTrav = 0;
        ++PLAYER_CELL;
        toggleAllOff();
        toggleAdjacent();
        updateLimits();
        return;
    }
    if (xTrav <= -2)
    {
        xTrav = 0;
        --PLAYER_CELL;
        toggleAllOff();
        toggleAdjacent();
        updateLimits();
        return;
    }
}


//World Updates
void toggleAllOff()
{
    for (int i = 0; i < int(maze.size()); ++i)
    {
        (*maze[i]).visable = 0;
    }

    //Sets the player Cell to visable
    (*maze[PLAYER_CELL]).visable = 1;
}
//Turns on all adjacent visable cells
void toggleAdjacent()
{
    //Toggle North
    if (PLAYER_CELL - size >= 0)
    {
        if (!(*maze[PLAYER_CELL]).N)
        {
            (*maze[PLAYER_CELL - size]).visable = 1;
        }
    }

    //Toggle South
    if (PLAYER_CELL + size < (size * size))
    {
        if (!(*maze[PLAYER_CELL]).S)
        {
            (*maze[PLAYER_CELL + size]).visable = 1;
        }
    }

    //Toggle East
    if (PLAYER_CELL % size != size - 1)
    {
        if (!(*maze[PLAYER_CELL]).E)
        {
            (*maze[PLAYER_CELL + 1]).visable = 1;
        }
    }

    //Toggle West
    if (PLAYER_CELL % size > 0)
    {
        if (!(*maze[PLAYER_CELL]).W)
        {
            (*maze[PLAYER_CELL - 1]).visable = 1;
        }
    }

    
}

void toggleLineOfSight()
{
    mygllib::View & view = *(mygllib::SingletonView::getInstance());
    int CurrentCell = PLAYER_CELL;
    if (view.refz() < 0)
        depth_x = 0;
    //Toggle North Cells
    while(1)
    {
        //Check North Cells
        if (CurrentCell - size >= 0 && view.refz() < 0)
        {
            //depth_x = 0;
            if (!(*maze[CurrentCell]).N)
            {
                (*maze[CurrentCell - size]).visable = 1;
                CurrentCell -= size;
                ++depth_x;
            }
            else
                break;
        }

        else
            break;
        
        
    }
    CurrentCell = PLAYER_CELL;
    if (view.refz() > 0)
        depth_x = 0;
    while(1)
    {
        //Check South Cells
        if (CurrentCell + size < size * size && view.refz() > 0)
        {
            
            if (!(*maze[CurrentCell]).S)
            {
                (*maze[CurrentCell + size]).visable = 1;
                CurrentCell += size;
                ++depth_x;
            }
            else
                break;
        }

        else
            break;
        
        
    }

    CurrentCell = PLAYER_CELL;
    if (view.refx() > 0)
        depth_z = 0;
    while(1)
    {
        //Check East Cells
        if ((CurrentCell + 1) % size < size - 1 && view.refx() > 0)
        {
            
            if (!(*maze[CurrentCell]).E)
            {
                (*maze[CurrentCell + 1]).visable = 1;
                ++CurrentCell;
                ++depth_z;
            }
            else
                break;
        }

        else
            break;
        
        
    }

    CurrentCell = PLAYER_CELL;
    if (view.refz() < 0)
        depth_z = 0;
    while(1)
    {
        //Check West Cells
        if ((CurrentCell - 1) % size > 0 && view.refx() <= 0)
        {
            //depth_z = 0;
            if (!(*maze[CurrentCell]).W)
            {
                (*maze[CurrentCell - 1]).visable = 1;
                --CurrentCell;
                ++depth_z;
            }
            else
                break;
        }

        else
            break;
        
        
    }
}

//Creates The World
std::vector< PunchedWall > build_maze(int n = size,
int r = 0, int c = 0);

void print_maze(int n, const std::vector<PunchedWall> & v);

//Functions for Creating the Maze
void chooseDirection(int n, std::vector< Cell* > &,
                     std::vector< Cell* > &, std::vector< PunchedWall > &,
                     Stack< Cell* > &);
bool inVisited(Cell &, std::vector< Cell* > &);
bool inPunched(PunchedWall &, const std::vector< PunchedWall > &);
bool findPunched(int x, int z, const std::vector< PunchedWall > &);
int find(Cell & c, std::vector< Cell * >collection);
void printHeight();
void printPunched();



void print()
{
    for (int i = 0; i < int(maze.size()); ++i)
    {
        std::cout << (*maze[i]).r() << ' ' << (*maze[i]).c()
                  << "N: " << (*maze[i]).N << ' '
                  << "S: " << (*maze[i]).S << ' '
                  << "E: " << (*maze[i]).E << ' '
                  << "W: " << (*maze[i]).W << ' '
                  << std::endl;
    }
}

std::vector< PunchedWall > build_maze(int n,
int r, int c)
{
    std::vector< PunchedWall >punchedWall;
    std::vector< Cell* >UNVISITED;
    std::vector< Cell* >VISITED;
    std::vector< Cell * >CELLS;
    Stack< Cell* >PATH;
    //std::vector< Cell* > PUNCHED;

    int k = 0;
    for (int i = 0; i < n; ++i)
    {
        for(int j = 0; j < n; ++j)
        {
            if (i == r && j == c)
            {
                VISITED.push_back(new Cell(i, j));
                CELLS.push_back(VISITED[0]);
                PATH.push(VISITED[0]);
            }
            else
            {
                UNVISITED.push_back(new Cell(i, j));
                CELLS.push_back(UNVISITED[k]);
                ++k;
            }
        }
    }
    while(punchedWall.size() < (n * n) && UNVISITED.size() > 0)
    {
            chooseDirection(n, VISITED, UNVISITED, punchedWall, PATH);
            PATH.pop();
            //std::cout << punchedWall.size() << ' ' << std::endl;
            //std::cout << PATH.size() << ' ' << std::endl;
    }
    //std::cout << punchedWall.size() << ' ' <<n * n << std::endl;
    return punchedWall;
}

void chooseDirection(int n, std::vector< Cell* > & Visited,
                     std::vector< Cell* > & Unvisited,
                     std::vector< PunchedWall > & Punched,
                     Stack< Cell* > & path)
{
    int x = path.top()->c();
    int y = path.top()->r();

    bool northBlock = 0;
    bool southBlock = 0;
    bool eastBlock = 0;
    bool westBlock = 0;

    Cell temp(x, y);

    while(1)
    {
        int direction = rand() % 4;
        //std::cout << direction <<std::endl;
        switch(direction)
        {
            //North
            case 0:
                temp.r() = path.top()->FindNRow();
                temp.c() = path.top()->FindNCol();
                //std::cout << temp << std::endl;
                if (temp.r() > -1 && inVisited(temp, Unvisited))
                {
                    Punched.push_back(PunchedWall(*(path.top()), temp));
                    path.push(new Cell(temp));
                    Visited.push_back(path.top());
                    Unvisited.erase(Unvisited.begin() +
                                    find(temp, Unvisited));
                }
                else
                    northBlock = 1;
                break;
                //South
            case 1:
                temp.r() = path.top()->FindSRow();
                temp.c() = path.top()->FindSCol();
                //std::cout << temp << std::endl;
                if (temp.r() < n && inVisited(temp, Unvisited))
                {
                    Punched.push_back(PunchedWall(*(path.top()), temp));
                    path.push(new Cell(temp));
                    Visited.push_back(path.top());
                    Unvisited.erase(Unvisited.begin() +
                                    find(temp, Unvisited));
                }
                else
                    southBlock = 1;
                break;
                //East
            case 2:
                temp.r() = path.top()->FindERow();
                temp.c() = path.top()->FindECol();
                
                // std::cout << temp << std::endl;
                if (temp.c() < n && inVisited(temp, Unvisited))
                {
                    Punched.push_back(PunchedWall(*(path.top()), temp));
                    path.push(new Cell(temp));
                    Visited.push_back(path.top());
                    Unvisited.erase(Unvisited.begin() +
                                    find(temp, Unvisited));
                }
                else
                    eastBlock = 1;
                break;
                //West
            case 3:
                temp.r() = path.top()->FindWRow();
                temp.c() = path.top()->FindWCol();
                //std::cout << temp << std::endl;
                if (temp.c() > -1 && inVisited(temp, Unvisited))
                {
                    Punched.push_back(PunchedWall(*(path.top()), temp));
                    path.push(new Cell(temp));
                    Visited.push_back(path.top());
                    Unvisited.erase(Unvisited.begin() +
                                    find(temp, Unvisited));
                }
                else
                    westBlock = 1;
                break;
        }
        //std::cout << Punched.size() << std::endl;
        //If everything is blocked, end this function
        if (northBlock && southBlock
            && eastBlock && westBlock)
        {
            //std::cout << "everything is blocked" <<std::endl;
            return;
        }
        
    }
    
    
}

bool inVisited(Cell & c, std::vector< Cell* > &visited)
{
    for (int i = 0; i < int(visited.size()); ++i)
    {
        if(c.r() == visited[i]->r() &&
           c.c() == visited[i]->c())
        {
            return 1;
        }
    }
    return 0;
}

bool findPunched(int x, int z, const std::vector< PunchedWall> &con)
{
    //std::cout << con.size() << std::endl;
    for (int i = 0; i < int(con.size()); ++i)
    {
        int x0 = con[i].c0.r();
        int z0 = con[i].c0.c();


        int x1 = con[i].c1.r();
        int z1 = con[i].c1.c();
        
        if((x ==  x0 &&
            z == z0) ||
           ((x == x1 &&
             z == z1)))
        {
            return 1;
        }
    }
    return 0;
}

bool inPunched(PunchedWall &p, const std::vector< PunchedWall> &con)
{
    for (int i = 0; i < int(con.size()); ++i)
    {
        if (p.c0.r() == con[i].c0.r() &&
            p.c0.c() == con[i].c0.c() &&
            p.c1.r() == con[i].c1.r() &&
            p.c1.c() == con[i].c1.c())
            return 1;
    }
    return 0;
}

int find(Cell & c, std::vector< Cell * >collection)
{
    for (int i = 0; i < int(collection.size()); ++i)
    {
        if(c.r() == collection[i]->r() &&
           c.c() == collection[i]->c())
        {
            return i;
        }
    }
    return -1;
}

void printHeight()
{
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            //std::cout << heightmap[i][j] << ' ';
        }
        std::cout << std::endl;
    }
}
void printPunched()
{
    for (int i = 0; i < int(punched_walls.size()); ++i)
    {
        std::cout <<"c0: (" <<punched_walls[i].c0.r() << ' ' << punched_walls[i].c0.c() << " ) ";
                 

        std::cout << "c1: (" <<punched_walls[i].c1.r() << ' ' << punched_walls[i].c1.c() << " ) "
                  << std::endl;
    }
}

void print_maze(int n, const std::vector<PunchedWall> & v)
{
    /*for (int i = 0; i < v.size(); ++i)
    {
        std::cout << v[i].c0 << ' ' << v[i].c1 << std::endl;
        }*/
    for(int x = 0; x < n; ++x)
    {
        if(x == 0)
        {
            for (int y = 0; y < n; ++y)
            {
                    std::cout << "+-";
            }
            std::cout << "+\n";
        }
            for (int y = 0; y < n - 1; ++y)
            {
                Cell it(x, y);
                Cell it2(x, y + 1);
                PunchedWall p(it, it2);
                PunchedWall p2(it2, it);
                /*
                if (x == n - 1)
                {
                    std::cout << it << ' ' << it2 << std::endl;
                    std::cout << inPunched(p, v) << ' '<<inPunched(p, v) <<std::endl;
                    continue;
                    }*/
                if (!y)
                {
                    std::cout << '|';
                }
                if (inPunched(p, v) || inPunched(p2,v))
                {
                    std::cout << "  ";
                }
                else
                    std::cout << " |";
            }
            std::cout << " |" << std::endl;
            for (int y = 0; y < n; ++y)
            {
                Cell it(x, y);
                Cell it2(x + 1, y);
                PunchedWall p(it, it2);
                PunchedWall p2(it2, it);
                if (inPunched(p, v) || inPunched(p2, v))
                    std::cout << "+ ";
                else
                    std::cout << "+-";
            }
            std::cout << "+\n";
    }
}
#endif

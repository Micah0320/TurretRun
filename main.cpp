#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "Maze.h"
#include "Stack.h"
#include <GL/freeglut.h>
//#include "glfw3.h"
#include "gl3d.h"
#include "View.h"
#include "SingletonView.h"

#include "Reshape.h"
#include "Material.h"
#include "Light.h"
#include "cube.h"
#include "globals.h"
#include "ImageFile.h"
#include "bullet.h"
mygllib::Light light;

std::vector<bullet * > bullets;


void popMaze()
{
    
    int i = 0;
    for (int x = 0; x < size; ++x)
    {
        for (int y = 0; y < size; ++y)
        {
            Cell * c = new Cell(x, y);
            maze.push_back(c);

            
            ++i;
            
        }
    }
    
    for (int i = 0; i < int(maze.size()) - 1; ++i)
    {
        Cell c(0, 0);
        
        //North
        c.r() = (*maze[i]).r() - 1;
        c.c() = (*maze[i]).c();
        PunchedWall p(c, (*maze[i]));
        PunchedWall p2((*maze[i]), c);
        //int f = 0;
        if ((inPunched(p, punched_walls) || inPunched(p2, punched_walls)) && (c.r() > 0 && c.r() < size))
        {
            //f = find(c, maze);
            //std::cout << f << std::endl;
            (*maze[i]).N = 0;
            (*maze[i - size]).S = 0;
        }

        //South
        c.r() = (*maze[i]).r() + 1;
        c.c() = (*maze[i]).c();
        p.c0.r() = c.r();
        p.c0.c() = c.c();
        p2.c1.r() = c.r();
        p2.c1.c() = c.c();
        if ((inPunched(p, punched_walls) || inPunched(p2, punched_walls)) && (c.r() > 0 && c.r() < size))
        {
            //f = find(c, maze);
            (*maze[i]).S = 0;
            (*maze[i + size]).N = 0;
        }
        //East
        c.r() = (*maze[i]).r();
        c.c() = (*maze[i]).c() + 1;
        p.c0.r() = c.r();
        p.c0.c() = c.c();
        p2.c1.r() = c.r();
        p2.c1.c() = c.c();
        if ((inPunched(p, punched_walls) || inPunched(p2, punched_walls)) && (c.c() > 0 && c.c() < size))
        {
            (*maze[i]).E = 0;
            (*maze[i + 1]).W = 0;
            
        }
        //West
        c.r() = (*maze[i]).r();
        c.c() = (*maze[i]).c() - 1;
        p.c0.r() = c.r();
        p.c0.c() = c.c();
        p2.c1.r() = c.r();
        p2.c1.c() = c.c();
        if ((inPunched(p, punched_walls) || inPunched(p2, punched_walls)) && (c.c() > 0 && c.c() < size))
        {
           
            (*maze[i]).W = 0;
            (*maze[i-1]).E = 0;
            
        }
    }

    //Populate maze with Bots now
    /*
    for (int i = 0; i < .45 * size * size; ++i)
    {
        bots_Cell_x.push_back(rand() % size);
        bots_Cell_y.push_back(rand() % size);
        bots_life.push_back(20);
        bots_alive.push_back(1);
    }
    for (int i = 0; i < int(bots_Cell_x.size()); ++i)
    {
        std::cout << bots_Cell_x[i] << ' '
                  << bots_Cell_y[i] << ' '
                  <<std::endl;
    }
    */
}


void load_external_texture(GLuint texid,
                           const std::string & filename)
{
    glBindTexture(GL_TEXTURE_2D, texid); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    BMPFile image(filename);
    glTexImage2D(GL_TEXTURE_2D,
                 0,                
                 GL_RGB,           
                 image.width(),    
                 image.height(),   
                 0,              
                 GL_RGB,           
                 GL_UNSIGNED_BYTE, 
                 image.data());    
}

void init()
{
    srand(time(NULL));
    std::cout << "What sized maze would you like? ";
    std::cin >> size;
    punched_walls = build_maze();
    
    popMaze();
    toggleAllOff();
    toggleAdjacent();
    updateLimits();
    
    
    //printHeight();
    mygllib::View & view = *(mygllib::SingletonView::getInstance());
    view.eyex() = Player_x;//10.0f;
    view.eyey() = Player_y;//35.0f;
    view.eyez() = Player_z;//10.0f;
    view.refx() = Front[0];//10.0f;
    view.refy() = Front[1];//35.0f;
    view.refz() = Front[2];//10.0f;
    view.upx() = Up[0];//10.0f;
    view.upy() = Up[1];//35.0f;
    view.upz() = Up[2];//10.0f;
    view.set_projection();
    view.lookat();
    
    glClearColor(0.04f, 0.53f, .84f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    
    mygllib::Light::all_on();    
    light.on();
    light.set();


    glGenTextures(3, texture);
    glEnable(GL_TEXTURE_2D);
    load_external_texture(texture[0], "wall.bmp");
    load_external_texture(texture[1], "path.bmp");
    load_external_texture(texture[2], "robo.bmp");
    load_external_texture(texture[3], "me.bmp");
    
}

void draw_square()
{
    mygllib::Light::all_off();
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glPushMatrix();
    {
        glTranslatef(-0.5 * pathWidth + pathWidth, 0, -0.5 * pathWidth + pathWidth);
    glBegin(GL_POLYGON);
    {

        glTexCoord2f(0.0, 0.0); glVertex3f( 0, 0, 0.0);
        glTexCoord2f(1.0, 0.0); glVertex3f( 0, 0, 2.0);
        glTexCoord2f(1.0, 1.0); glVertex3f( 2.0, 0.0, 2.0);
        glTexCoord2f(0.0, 1.0); glVertex3f( 2.0, 0.0, 0.0);
    }
    glEnd();
    }
    glPopMatrix();
    
    mygllib::Light::all_on();
        
}

void drawWall()
{
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    drawCube();
}

void drawMe()
{
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    drawCube();
}

void drawConnectors()
{
    
    //Draw Connectors in between
    for (int i = 0; i <= size; ++i)
    {
        for (int j = 0; j <= size; ++j)
        {
            if (i != 0 && j != 0 && i != size && j != size)
            {
                //if ((*maze[(i * size) + j]).visable)
                //{
                  Draw:
                //for (int k = 0; k < pathWidth; ++k)
                //{
                glPushMatrix();
                {
                    
                    glTranslatef(0.5 * pathWidth + (j * 2 * pathWidth), 0, 0.5 * pathWidth  + (i * 2 * pathWidth));
                            //glTranslatef(0, 0.5,  0);
                    draw_square();
                    drawWall();
                }
                glPopMatrix();
                //}
                
                    
            }
            else
                goto Draw;
        }
               
    }
}

        
void drawBot()
{
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glPushMatrix();
    {
        glTranslatef(.45, 0, 0.5);
        glScalef(0.25, 0.25, 0.25);
        
        drawCube();
    }
    glPopMatrix();
}
   
            
void drawMaze()
{
    
        //Draw the Top and Bottom Boundaries
    drawConnectors();

    for(int i = 0; i < size; ++i)
    {
        glPushMatrix();
        {
            glTranslatef(pathWidth, 0, 0);
            //glTranslatef(0, .5, 0);
            glTranslatef(0, 0, i * pathWidth + pathWidth);
            //draw_square();
            for (int j = 0; j < size; ++j)
            {
                if ((*maze[i * size + j]).visable)
                {
                    glPushMatrix();
                    {
                        glTranslatef(j * pathWidth, 0, 0);
                        //Values are in the upper Right corner of the cell
                        
                        
                        //Needs to translate to the center of the cell before moving
                        glTranslatef(pathWidth * 0.5, 0, pathWidth * 0.5);
                        /*
                        for(int bt = 0; bt < int(bots_Cell_x.size()); ++bt)
                        {
                            if(bots_alive[bt] && bots_Cell_x[bt] == i
                               && bots_Cell_y[bt] == j)
                            {
                                drawBot();
                                break;
                            }
                            continue;
                            }*/
                        
                        //Draw North
                        if(((*maze[(i * size) + j]).N))
                        {
                            glPushMatrix();
                            {
                                glTranslatef((j * pathWidth), 0, i * pathWidth - pathWidth);
                                drawWall();
                                draw_square();
                                //glTranslatef(0 , 0, pathWidth);
                                //drawCube();
                            }
                            glPopMatrix();
                        }
                        
                        if(((*maze[(i * size) + j]).S))
                        {
                            glPushMatrix();
                            {
                                glTranslatef((j * pathWidth), 0, i * pathWidth + pathWidth);
                                drawWall();
                                draw_square();
                                //glTranslatef(0 , 0, pathWidth);
                                //drawCube();
                            }
                            glPopMatrix();
                        }
                        
                        // /*
                        if(((*maze[(i * size) + j]).E))
                        {
                            glPushMatrix();
                            {
                                glTranslatef((j * pathWidth) + pathWidth, 0, i * pathWidth);
                                drawWall();
                                draw_square();
                                glTranslatef(0 , 0, pathWidth);
                                drawWall();
                            }
                            glPopMatrix();
                        }
                        //*/
                        
                        if((*maze[(i * size) + j]).W)
                        {
                            glPushMatrix();
                            {
                                glTranslatef((j * pathWidth ) - pathWidth , 0, i * pathWidth);
                                drawWall();
                                draw_square();
                                glTranslatef(0 , 0, pathWidth);
                                drawWall();
                                
                            }
                            glPopMatrix();
                        }
                        //*/ 
                        
                    }
                    glPopMatrix();
                }
                
            }
            
        }
        glPopMatrix();
    }
    
}

void drawBullet()
{
    glutSolidSphere(0.1, 20, 20);
    glutPostRedisplay();
}

void display()
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mygllib::Light::all_off();
    for (int i = 0; i < int(bullets.size()); ++i)
    {
        if ((*bullets[i]).Visible)
            (*bullets[i]).move();
        else
            bullets.erase(bullets.begin() + i);
        
    }
    //mygllib::draw_axes();
    //mygllib::draw_xz_plane();
    mygllib::Light::all_on();
    
    drawMaze();
    if (BirdsEye)
    {
        mygllib::View & view = *(mygllib::SingletonView::getInstance());
        glPushMatrix();
        {
            
            glTranslatef(view.eyex() - 0.5, 0.5, view.eyez());
            //glRotatef(90 * view.eyex(), 0, 1, 0);
            drawMe();
        }
        glPopMatrix();
    }

    for (int i = 0; i < int(bullets.size()); ++i)
    {
        glPushMatrix();
        {
            glTranslatef((*bullets[i]).x, Player_y, (*bullets[i]).z);
            drawBullet();
        }
        glPopMatrix();
    }
    
    glutSwapBuffers();
    
    
}

void keyboard(unsigned char key, int w, int h)
{
    mygllib::View & view = *(mygllib::SingletonView::getInstance());
    
    switch (key)
    {
        /*
        case 'x': view.eyex() -= 0.1; break;
        case 'X': view.eyex() += 0.1; break;
        case 'y': view.eyey() -= 0.1; break;
        case 'Y': view.eyey() += 0.1; break;
        case 'z': view.eyez() -= 0.1; break;
        case 'Z': view.eyez() += 0.1; break;
        */
            //Switches Views    
        case 'v':
        {
            BirdsEye = (!BirdsEye);
            if (BirdsEye)
            {
                Player_y = 10;
            }
            else
                Player_y = 0.5;
            view.eyey() = Player_y;
            break;

        }
        
        case 'w' :
        {
            glm::vec3 p(view.eyex(), view.eyey(), view.eyez());
            glm::vec3 f(view.refx(), view.refy(), view.refz());
            p += f * (MoveSpeed * deltaTime);
            //Finds the Sum distance Traversed in the Cell
            float xt, zt;
            xt = (p[0] - view.eyex());
            zt = (p[2] - view.eyez());
            xTrav += xt;
            zTrav += zt;

            
            if (xTrav > LIM_NEG_X && xTrav < LIM_X)
                view.eyex() = p[0];
            else
                xTrav -= xt;
            view.eyey() = Player_y;
            if (zTrav > LIM_NEG_Z && zTrav < LIM_Z)
                view.eyez() = p[2];
            else
                zTrav -= zt;
        
            
            break;
        }

        case 's' :
        {
            glm::vec3 p(view.eyex(), view.eyey(), view.eyez());
            
            glm::vec3 f(view.refx(), view.refy(), view.refz());
            
            p -= f * (MoveSpeed * deltaTime);
            float xt, zt;
            xt = (p[0] - view.eyex());
            zt = (p[2] - view.eyez());
            xTrav += xt;
            zTrav += zt;

            
            if (xTrav > LIM_NEG_X && xTrav < LIM_X)
                view.eyex() = p[0];
            else
                xTrav -= xt;
            view.eyey() = Player_y;
            if (zTrav > LIM_NEG_Z && zTrav < LIM_Z)
                view.eyez() = p[2];
            else
                zTrav -= zt;
        
            
            break;
        }
        
        case 'a' :
            {
                glm::vec3 p(view.eyex(), view.eyey(), view.eyez());
                
                p -= Right * (MoveSpeed * deltaTime);

                float xt, zt;
                xt = (p[0] - view.eyex());
                zt = (p[2] - view.eyez());
                xTrav += xt;
                zTrav += zt;
                
                
                if (xTrav > LIM_NEG_X && xTrav < LIM_X)
                    view.eyex() = p[0];
                else
                    xTrav -= xt;
                view.eyey() = Player_y;
                if (zTrav > LIM_NEG_Z && zTrav < LIM_Z)
                    view.eyez() = p[2];
                else
                    zTrav -= zt;
                
                
                
                break;
            }
            case 'd' :
            {
                glm::vec3 p(view.eyex(), view.eyey(), view.eyez());
                
                p += Right * (MoveSpeed * deltaTime);
                float xt, zt;
                xt = (p[0] - view.eyex());
                zt = (p[2] - view.eyez());
                xTrav += xt;
                zTrav += zt;
                
                
                if (xTrav > LIM_NEG_X && xTrav < LIM_X)
                    view.eyex() = p[0];
                else
                    xTrav -= xt;
                view.eyey() = Player_y;
                if (zTrav > LIM_NEG_Z && zTrav < LIM_Z)
                    view.eyez() = p[2];
                else
                    zTrav -= zt;
                
                break;
            }
            /*   
        case 'n': view.zNear() -= 0.1; break;
        case 'N': view.zNear() += 0.1; break;
        case 'f': view.zFar() -= 0.1; break;
        case 'F': view.zFar() += 0.1; break;
            */
        
    }

    view.set_projection();
    view.lookat();
    updatePlayer();
    toggleLineOfSight();
    glutPostRedisplay();
}

void processMouse(int x, int y)
{
    mygllib::View & view = *(mygllib::SingletonView::getInstance());
    glm::vec3 front;
    int xold = x;
    int yold = y;
    
    x = 1024 / 2;
    y = 768 / 2;
    xmousePos = xold - x;
    ymousePos = y - yold;
    updateLook();
    front.x = cos(verAngle) * sin(horAngle);
    front.y = sin(verAngle);
    front.z = cos(verAngle) * cos(horAngle);
    front = glm::normalize(front);
    view.refx() = front.x;
    view.refy() = front.y;
    view.refz() = front.z;
    Right.x = sin(horAngle - 3.14f/2.0f);
    Right.y = 0;
    Right.z = cos(horAngle - 3.14f/2.0f);
    Right = glm::normalize(glm::cross(front, Up));
    xmousePos = x;
    ymousePos = y;

    toggleAllOff();
    toggleAdjacent();
    toggleLineOfSight();
    view.set_projection();
    view.lookat();
    glutPostRedisplay();
}

void onMouseButton(int button, int state, int x, int y)
{
    //std::cout << "PEW PEW" << std::endl;
    bullet * bul = new bullet();
    bullets.push_back(bul);
    std::cout << bullets.size() <<std::endl;
}

int main(int argc, char ** argv)
{
    mygllib::WIN_W = WORLD_WIDTH;
    mygllib::WIN_H = WORLD_HEIGHT;
    mygllib::init3d();
    init();    
    glutDisplayFunc(display);
    glutReshapeFunc(mygllib::Reshape::reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(onMouseButton);
    glutPassiveMotionFunc(processMouse);
    glutMainLoop();
  
    return 0;
}



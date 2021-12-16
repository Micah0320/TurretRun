#ifndef BULLET_H
#define BULLET_H
#include "View.h"
#include "globals.h"
class bullet
{
  public:
    bullet()
    {
        mygllib::View & view = *(mygllib::SingletonView::getInstance());
        x = view.eyex();
        y = 0.3;
        z = view.eyez();
        bul_x_trav = xTrav;
        bul_z_trav = zTrav;
        bul_x_LIM = LIM_X + 10;
        bul_z_LIM = LIM_Z + 10;
        dir[0] = view.refx();
        dir[1] = 0;
        dir[2] = view.refz();
    }

    void move()
    {
        if(Visible)
        {
            glm::vec3 p(x, y, z);
            int xt = x;
            int zt = z;
            p += dir * (speed * deltaTime);
            x = p[0];
            y = p[1];
            z = p[2];
            bul_x_trav += p[0] - xt;
            bul_z_trav += p[2] - zt;
            if (abs(bul_x_trav) >= bul_x_LIM || abs(bul_x_trav) >= bul_z_LIM)
            {
                Visible = 0;
            }
        }
    }
    bool Visible = 1;
    float speed = 0.01;
    float x;
    float y;
    float z;
    float bul_x_trav;
    float bul_z_trav;

    float bul_x_LIM;
    float bul_z_LIM;
    glm::vec3 dir;
};
#endif

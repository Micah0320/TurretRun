#ifndef CUBE_H
#define CUBE_H
#include "ImageFile.h"
#include "globals.h"


void drawCube()
{
    
    // glDrawArrays:
    //   34 vertices with 10 repeats -- 34 * 6 * 4 = 816 bytes
    // glDrawElements:
    //   24 vertices                 -- 24 * 6 * 4 = 576 bytes
    float vertices[] = {0,1,0, 0,1,0, 0, 1,  // 4 (top)
                        0,1,1, 0,1,0, 0, 0, // 0 (top)
                        1,1,0, 0,1,0, 1, 1, // 5 (top)
                        1,1,1, 0,1,0, 1, 0, // 1 (top)
                        1,1,1, 0,1,0, 1, 0, // 1 (top)    -- repeat
                        0,1,1, 0,0,1, 0, 1, // 0 (front)  -- repeat
                        0,1,1, 0,0,1, 0, 1, // 0 (front)
                        0,0,1, 0,0,1, 0, 0, // 3 (front)
                        1,1,1, 0,0,1, 1, 1, // 1 (front)
                        1,0,1, 0,0,1, 1, 0, // 2 (front)                        
                        1,0,1, 0,0,1, 1, 0, // 2 (front) -- repeat
                        1,1,1, 1,0,0, 0, 1, // 1 (right) -- repeat
                        1,1,1, 1,0,0, 0, 1, // 1 (right)
                        1,0,1, 1,0,0, 0, 0, // 2 (right) 
                        1,1,0, 1,0,0, 1, 1, // 5 (right)
                        1,0,0, 1,0,0, 1, 0, // 6 (right)
                        1,0,0, 1,0,0, 1, 0, // 6 (right)  -- repeat
                        1,1,0, 0,0,-1, 0, 1, // 5 (back)   -- repeat
                        1,1,0, 0,0,-1, 0, 1,// 5 (back)
                        1,0,0, 0,0,-1, 0, 0,// 6 (back)
                        0,1,0, 0,0,-1, 1, 1,// 4 (back)
                        0,0,0, 0,0,-1, 1, 0,// 7 (back)
                        0,0,0, 0,0,-1, 1, 0,// 7 (back)   -- repeat
                        0,1,0, -1,0,0, 0, 1,// 4 (left)   -- repeat
                        0,1,0, -1,0,0, 0, 1,// 4 (left)
                        0,0,0, -1,0,0, 0, 0,// 7 (left)
                        0,1,1, -1,0,0, 1, 1,// 0 (left)
                        0,0,1, -1,0,0, 1, 0,// 3 (left)
                        0,0,1, -1,0,0, 1, 0,// 3 (left)   -- repeat
                        0,0,1, 0,0,-1, 0, 1,// 3 (bottom) -- repeat
                        0,0,1, 0,-1,0, 0, 1,// 3 (bottom)
                        0,0,0, 0,-1,0, 0, 0,// 7 (bottom)
                        1,0,1, 0,-1,0, 1, 1,// 2 (bottom)
                        1,0,0, 0,-1,0, 0, 1// 6 (botoom)
    };
    glFrontFace(GL_CCW); 
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    int stride = 8 * sizeof(GLfloat);
    int num_vertices = sizeof(vertices) / (8*sizeof(GLfloat));
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, stride, vertices);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, stride, vertices + 3);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 8 * sizeof(GLfloat), vertices + 6);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, num_vertices);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}



#endif

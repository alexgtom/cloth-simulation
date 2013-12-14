#ifndef _FALLING_CLOTH_FLOOR_H_ 
#define _FALLING_CLOTH_FLOOR_H_ 

#include "Scene.h"
#include <SOIL/SOIL.h>
#include <string>

class FallingClothFloor {
  public:
    static int timer;
    static Cloth cloth; 


        static void setup(string s) {
            GLuint tex_2d = SOIL_load_OGL_texture
            (
             s.c_str(),
             SOIL_LOAD_AUTO,
             SOIL_CREATE_NEW_ID,
             SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
             );
            
            if( 0 == tex_2d )
            {
                printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
            }
            cloth.addTexture(tex_2d);
        }

    static void display(void) {
      // calculating positions
      Vector3f ball_pos(5,-5,1010); // the center of one ball
      float ball_radius = 1005; // the radius of our one ball

      Vector3f ball_pos2(3,-5,3); //ball 2
      float ball_radius2 = 2;
      timer++;
      //ball_pos[2] = cos(timer/20.0)*5;  //used a cosine function to let the ball move forward and backward

      cloth.AddForce(Vector3f(0,0,9.8)); // add gravity 
      cloth.AddWind(Vector3f(10,0,10)); // generate wind 
      cloth.Time();                   // calculate the particle positions of the next step
      cloth.Intersect_Ball(ball_pos, ball_radius); // resolve collision with the ball
      cloth.Intersect_Ball(ball_pos2, ball_radius2);

      // OpenGL drawing
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glLoadIdentity();
      gluLookAt(
        15, -5.0, -0.0, 
        5.0, -5.0, 3.0, 
        0.0, 0.0, -1.0
      );

      cloth.drawCloth(); // draw the cloth with smooth shading

      glPushMatrix(); //  use glutSolidSphere to draw the ball
      glTranslatef(ball_pos[0],ball_pos[1],ball_pos[2]); // hence the translation of the sphere onto the ball position
      glColor3f(0.3f,0.6f,0.4f); //green ball
      glutSolidSphere(ball_radius-0.1,50,50); 
      glPopMatrix();

      glPushMatrix(); //  use glutSolidSphere to draw the ball
      glTranslatef(ball_pos2[0],ball_pos2[1],ball_pos2[2]); // hence the translation of the sphere onto the ball position
      glColor3f(0.9f,0.9f,0.9f); //gray
      glutSolidSphere(ball_radius2-0.1,50,50); 
      glPopMatrix();


      glFlush();
      glutSwapBuffers();
    }
};

int FallingClothFloor::timer = 0;
Cloth FallingClothFloor::cloth = Cloth(10,10,50,50); // one Cloth object of the Cloth class

#endif

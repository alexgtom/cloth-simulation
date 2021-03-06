#ifndef _FALLING_TO_FLOOR_SCENE_H_ 
#define _FALLING_TO_FLOOR_SCENE_H_ 

#include "Scene.h"

class FallingtoFloorScene {
  public:
    static int timer;
    static Cloth cloth; 

    static void setup(void) {
    }

    static void display(void) {
      // calculating positions
      Cloth cloth(20,10,20,20); // one Cloth object of the Cloth class
      Vector3f ball_1_pos(-4,-1,-5); // the center of our one ball
      float ball_1_radius = 2; // the radius of our one ball

      GLuint tex_2d;

      timer++;
      //ball_pos[2] = cos(timer/20.0)*5;  //used a cosine function to let the ball move forward and backward


      cloth.addTexture(tex_2d);  

      cloth.AddForce(Vector3f(0,0,9.8)); // add gravity 
      //cloth.AddWind(Vector3f(10,0,10)); // generate wind 
      cloth.Time();                   // calculate the particle positions of the next step
      cloth.Intersect_Ball(ball_1_pos, ball_1_radius); // resolve collision with the ball
	    cloth.Intersect_Floor(-5);

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
      glTranslatef(ball_1_pos[0],ball_1_pos[1],ball_1_pos[2]); // hence the translation of the sphere onto the ball position
      glColor3f(0.4f,0.8f,0.5f); //green ball
      glutSolidSphere(ball_1_radius-0.1,50,50); 
      glPopMatrix();

      glColor3f(1.0f,1.0f,1.0f);
	    glBegin(GL_POLYGON);
	    glVertex3f(-20,-5,-20);
	    glVertex3f(20,-5,-20);
    	glVertex3f(20,-5,0);
	    glVertex3f(-20,-5,0);
	    glEnd();

      glFlush();
      glutSwapBuffers();
    }
};

int FallingtoFloorScene::timer = 0;
Cloth FallingtoFloorScene::cloth = Cloth(20,10,20,20); // one Cloth object of the Cloth class

#endif

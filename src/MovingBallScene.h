#ifndef _MOVING_BALL_SCENE_H_ 
#define _MOVING_BALL_SCENE_H_ 

#include "Scene.h"

class MovingBallScene {
  public:
    static int timer;
    static Cloth cloth; 

    static void setup(void) {
	  // making the upper left most two and upper right most two particles unmovable
      cloth.getParticle(0,0)->makeUnmovable();
      cloth.getParticle(1,0)->makeUnmovable();
      cloth.getParticle(cloth.num_particles_x-1,0)->makeUnmovable();
      cloth.getParticle(cloth.num_particles_x-2,0)->makeUnmovable();
    }

    static void display(void) {
      // calculating positions
      Vector3f ball_pos(7,-5,0); // the center of our one ball
      float ball_radius = 4; // the radius of our one ball
      GLuint tex_2d;

      timer++;
      ball_pos[2] = cos(timer/20.0)*5;  //used a cosine function to let the ball move forward and backward


      cloth.addTexture(tex_2d);  

      cloth.AddForce(Vector3f(0,-9.8,0)); // add gravity 
      cloth.AddWind(Vector3f(1,0,1)); // generate wind 
      cloth.Time();                   // calculate the particle positions of the next step
      cloth.Intersect(ball_pos, ball_radius); // resolve collision with the ball

      // OpenGL drawing
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glLoadIdentity();

      glTranslatef(-6.5,6,-9.0f); 
      glRotatef(20,0,1,0);
      cloth.drawCloth(); // draw the cloth with smooth shading

      glPushMatrix(); //  use glutSolidSphere to draw the ball
      glTranslatef(ball_pos[0],ball_pos[1],ball_pos[2]); // hence the translation of the sphere onto the ball position
      glColor3f(0.4f,0.8f,0.5f); //green ball
      glutSolidSphere(ball_radius-0.1,50,50); 
      glPopMatrix();

      glFlush();
      glutSwapBuffers();
    }
};

int MovingBallScene::timer = 0;
Cloth MovingBallScene::cloth = Cloth(10,10,10,10); // one Cloth object of the Cloth class

#endif
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
#include <stdexcept>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include <time.h>
#include <math.h>

#include "Scene.h"
#include "Parser.h"

using namespace std;

Scene scene;
Cloth cloth(10,10,10,10); // one Cloth object of the Cloth class
Vector3f ball_pos(7,-5,0); // the center of our one ball
float ball_radius = 4; // the radius of our one ball

GLuint tex_2d;

/* ************************************************************************* */
/* Initialize Light Sources and Materials                                    */
/* ************************************************************************* */
void init()
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat Pos0[] = {-1.0,1.0,0.5,0.0};
	glLightfv(GL_LIGHT0,GL_POSITION, Pos0);
    
	glEnable(GL_LIGHT1);
    
	GLfloat Ambient1[] = {0.0,0.0,0.0,0.0};
	GLfloat Pos1[] = {1.0,0.0,-0.2,0.0};
	GLfloat Diffuse1[] = {0.5,0.5,0.3,0.0};
    
	glLightfv(GL_LIGHT1,GL_POSITION, Pos1);
	glLightfv(GL_LIGHT1,GL_AMBIENT, Ambient1);
	glLightfv(GL_LIGHT1,GL_DIFFUSE, Diffuse1);
    
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
    
	// define material properties:
	glEnable(GL_COLOR_MATERIAL);
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_diffuse[] = {0.0, 1.0, 0.0, 1.0};
	GLfloat mat_ambient[] = {0.0, 0.1, 0.1, 1.0};
	GLfloat mat_shininess = {20.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}


//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) {
  scene.width = w;
  scene.height = h;

  glViewport (0,0,w,h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective (80, (float )w /(float)h, 1.0,5000.0 );
  glMatrixMode(GL_MODELVIEW);  
  glLoadIdentity();
}

//****************************************************
// function that does the actual drawing of stuff
//***************************************************

float timer = 0; // counter for used to calculate the z position of the ball below

void myDisplay() {

  // calculating positions

  timer++;
  ball_pos[2] = cos(timer/20.0)*5;  //used a cosine function to let the ball move forward and backward


  cloth.addTexture(tex_2d);  

  cloth.AddForce(Vector3f(0,-9.8,0)); // add gravity 
  cloth.AddWind(Vector3f(1,0,1)); // generate wind 
  cloth.Time();                   // calculate the particle positions of the next step
  cloth.Intersect(ball_pos, ball_radius); // resolve collision with the ball

  // Transformations
  
  // OpenGL drawing
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  glTranslatef(-6.5,6,-9.0f); 
  glRotatef(20,0,1,0);
  
  // These were used for controling the camera via keyboard, but the shading is messsed up
  //// clear the color buffer and Z-buffer
  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	  

  //// indicate we are specifying camera transformations
  //glMatrixMode(GL_MODELVIEW);			

  //// Reset transformations
  //glLoadIdentity();

  //// Translations
  //glTranslatef(scene.translate_x, scene.translate_y, 0.0f);

  //// Zoom
  //glScalef(scene.scale, scene.scale, scene.scale);

  //// rotations
  //glRotatef(scene.rotate_x, 1.0, 0.0, 0.0);
  //glRotatef(scene.rotate_y, 0.0, 1.0, 0.0);

  // Start drawing stuff here
  //scene.render();
  cloth.drawCloth(); // draw the cloth with smooth shading

  glPushMatrix(); //  use glutSolidSphere to draw the ball
  glTranslatef(ball_pos[0],ball_pos[1],ball_pos[2]); // hence the translation of the sphere onto the ball position
  glColor3f(0.4f,0.8f,0.5f); //green ball
  glutSolidSphere(ball_radius-0.1,50,50); 
  glPopMatrix();

  glFlush();
  glutSwapBuffers();
}

void myFrameMove() {
  //nothing here for now
#ifdef _WIN32
  Sleep(10);                                   //give ~10ms back to OS (so as not to waste the CPU)
#endif
  glutPostRedisplay(); // forces glut to call the display function (myDisplay())
}

int main(int argc, char *argv[]) {
  // setup the scene
  scene.create(argc, argv);

  // ------------------------------------------------------------------------

  //This initializes glut
  glutInit(&argc, argv);

  //This tells glut to use a double-buffered window with red, green, and blue channels 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  //The size and position of the window
  glutInitWindowSize(scene.width, scene.height);
  glutInitWindowPosition(0,0);
  glutCreateWindow(argv[0]);
  glEnable(GL_DEPTH_TEST);

  init();
  glutDisplayFunc(myDisplay);                                // function to run when its time to draw something
  glutReshapeFunc(myReshape);	
  glutSpecialFunc(scene.keyboard);
  glutKeyboardFunc(scene.key);
  glutIdleFunc(myFrameMove);

  glutMainLoop();						// 999  //TODO: this is supposed to be infinityloop that will keep drawing and resizing
  // and whatever else

  return 0;
}

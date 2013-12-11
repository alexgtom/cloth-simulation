/********************************************************************************
	cs184-2013fall-Final Project-Cloth Simulation

	Kristen Curry, Zhongyin Hu, Alex Tom, Aaron Zhou
********************************************************************************/
#include "Scene.h"
#include "Shape.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
#include <stdexcept>
#include <cstdlib>

#define NOMINMAX

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

#include <SOIL/SOIL.h>

#include <Eigen/Dense>

using namespace Eigen;

class Viewport {
  public:
    int w, h; // width and height
};

Viewport viewport;
Scene scene;

//****************************************************
// Global Variables
//****************************************************

// define Cloth and Ball 
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
  
  tex_2d = SOIL_load_OGL_texture
  (
    "img.png", //use command line input???
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
  ); 

  if( 0 == tex_2d )
  {
    printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
  }

}

//****************************************************
// reshape viewport if the window is resized
//****************************************************

void myReshape(int w, int h) {
  viewport.w = w;
  viewport.h = h;

  glViewport (0,0,viewport.w,viewport.h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
}

//****************************************************
//Update the rendering
//***************************************************


//****************************************************
// function that does the actual drawing of stuff
//***************************************************

float timer = 0; // counter for used to calculate the z position of the ball below

/* display method called each frame*/
void myDisplay(void)
{
	// calculating positions
  
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
  
	glutSwapBuffers();
	glutPostRedisplay();
}

void reshape(int w, int h)  
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();  
	gluPerspective (80, (float )w /(float)h, 1.0,5000.0 );
	glMatrixMode(GL_MODELVIEW);  
	glLoadIdentity();
}

int main ( int argc, char** argv ) 
{
	glutInit( &argc, argv );
	
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH ); 
	glutInitWindowSize(400, 400 ); 

	glutCreateWindow( "CS184-Final Project-Cloth Simulation" );
	init();
	glutDisplayFunc(myDisplay);  
	glutReshapeFunc(reshape);
  glutSpecialFunc(scene.keyboard);
  glutKeyboardFunc(scene.key);

	glutMainLoop();
	
	return 0;
}

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
#include "MovingBallScene.h"
#include "FallingClothScene.h"
#include "Parser.h"

using namespace std;

Scene scene;

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
  // Display secenes here
  //MovingBallScene::setup();
  //glutDisplayFunc(MovingBallScene::display);
  FallingClothScene::setup();
  glutDisplayFunc(FallingClothScene::display);

  // Glut stuff
  glutReshapeFunc(myReshape);	
  glutSpecialFunc(scene.keyboard);
  glutKeyboardFunc(scene.key);
  glutIdleFunc(myFrameMove);

  glutMainLoop();						// 999  //TODO: this is supposed to be infinityloop that will keep drawing and resizing
  // and whatever else

  return 0;
}

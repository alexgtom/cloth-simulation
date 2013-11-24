
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

//****************************************************
// function that does the actual drawing of stuff
//***************************************************
void myDisplay() {

  // clear the color buffer and Z-buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	  

  // indicate we are specifying camera transformations
  glMatrixMode(GL_MODELVIEW);			

  // Reset transformations
  glLoadIdentity();

  // Translations
  glTranslatef(scene.translate_x, scene.translate_y, 0.0f);

  // Zoom
  glScalef(scene.scale, scene.scale, scene.scale);

  // rotations
  glRotatef(scene.rotate_x, 1.0, 0.0, 0.0);
  glRotatef(scene.rotate_y, 0.0, 1.0, 0.0);

  // Lighting
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_NORMALIZE); // normalize all the normal vectors

  //Add ambient light
  GLfloat ambientColor[] = {0.0f, 0.0f, 0.5f, 1.0f};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

  //Add directed light
  GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f};
  GLfloat lightPos0[] = {0.0f, 0.0f, 0.0f, 0.0f}; //0 is directional
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

  // Start drawing stuff here
  scene.render();

  glFlush();
  glutSwapBuffers();					    // swap buffers (we earlier set double buffer)
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

  glutDisplayFunc(myDisplay);				// function to run when its time to draw something
  glutSpecialFunc(scene.keyboard);
  glutIdleFunc(myFrameMove);

  glutMainLoop();						// 999  //TODO: this is supposed to be infinityloop that will keep drawing and resizing
  // and whatever else

  return 0;
}

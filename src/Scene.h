#ifndef SCENE_H_
#define SCENE_H_

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

#include "Parser.h"

#define ROTATE_STEP_SIZE 5.0f
#define ZOOM_STEP_SIZE 0.1f
#define TRANSLATE_STEP_SIZE 0.1f
#define RENDER_TIME_STEP 1

#define WIREFRAME 0
#define FILLED 1

using namespace std;

class Scene {
  public:
    int width, height; // width and height of the window
    string inputFile;
    vector<Shape*> shape_list;
    static float rotate_x, rotate_y;
    static float translate_x, translate_y;
    static float scale;
    static int render_mode;
    static int time;

    Scene() {
      width = 800;
      height = 600;
      rotate_x = rotate_y = 0;
    };
    
    // parse arguments from command line and set stuff
    void parseArgs(int argc, char *argv[]) {
      if (argc != 2) {
        cerr << "Invalid arguments" << endl;
        cerr << "USAGE: ./main inputfile" << endl;
        exit(1);
      }

      inputFile = argv[1];
    }
    
    // create everything for the scene
    void create(int argc, char *argv[]) {
      // parse arguements from command line
      parseArgs(argc, argv);
      
      // parse input file and return a patch list
      Parser parser;
      shape_list = parser.readFile(inputFile);
    }

    // render the scene in the GLUT loop
    void render() {
      for(int i = 0; i < shape_list.size(); i++) {
        if (render_mode == WIREFRAME)
          shape_list[i]->render_wireframe(time);
        else if (render_mode == FILLED)
          shape_list[i]->render_filled(time);
      }

      time += RENDER_TIME_STEP;
    }

    // keyboard controls for the scene from the keyboard
    static void keyboard(int key, int x, int y)
    {
      if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
        switch(key) {
          // translations
          case GLUT_KEY_UP:
            translate_y += TRANSLATE_STEP_SIZE;
            break;
          case GLUT_KEY_DOWN:
            translate_y -= TRANSLATE_STEP_SIZE;
            break;
          case GLUT_KEY_LEFT:
            translate_x -= TRANSLATE_STEP_SIZE;
            break;
          case GLUT_KEY_RIGHT:
            translate_x += TRANSLATE_STEP_SIZE;
            break;

            //zoom in
          case '+':
            scale += ZOOM_STEP_SIZE;
            break;
        }

      } else {
        switch(key) {
          // toggle between filled and wireframe mode
          case 'w':
            if (render_mode == WIREFRAME)
              render_mode = FILLED;
            else 
              render_mode = WIREFRAME;
            break;

          // rotations
          case GLUT_KEY_UP:
            rotate_x += ROTATE_STEP_SIZE;
            break;
          case GLUT_KEY_DOWN:
            rotate_x -= ROTATE_STEP_SIZE;
            break;
          case GLUT_KEY_LEFT:
            rotate_y -= ROTATE_STEP_SIZE;
            break;
          case GLUT_KEY_RIGHT:
            rotate_y += ROTATE_STEP_SIZE;
            break;

          // zoom out
          case '-':
            scale -= ZOOM_STEP_SIZE;
            break;

          // quit
          case 'q':
          case 27:
            exit(0);
            break;
        }
      }

      glutPostRedisplay();
    }
};

float Scene::rotate_x = 0.0f;
float Scene::rotate_y = 0.0f;
float Scene::translate_x = 0.0f;
float Scene::translate_y = 0.0f;
float Scene::scale = 0.3f;
int Scene::render_mode = FILLED;
int Scene::time = 0;

#endif

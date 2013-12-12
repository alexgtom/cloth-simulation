#ifndef SHAPE_H_
#define SHAPE_H_

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cmath>

#include <Eigen/Core>


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

#include "Point.h"
#include "Force.h"
#include "Particle.h"
#include "Spring.h"

//#include <SOIL/SOIL.h>

using namespace std;
using namespace Eigen;

class Shape {
  public:
    Vector3f world_pos;
    virtual void render_wireframe(int t, Force* external) = 0;
    virtual void render_filled(int t, Force* external) = 0;

    // Test if f intersects with the shape or not (in object space), if so,
    // return intersection point
    virtual bool intersect(Force& f, Point* intersection) = 0;

    // Test if f interspects with the shape or not
    virtual bool intersectP(Force& f) = 0;

};

class Sphere : public Shape {
  public:
    float radius;
    Point center;

    Sphere() {
    }

    Sphere(float radius) {
      this->radius = radius;
    }

    Sphere(float radius, Point center) {
      this->radius = radius;
      this->center = center;
    }

    void render_wireframe(int t, Force* external) {
      glPushMatrix();
      glTranslatef(center[0] , center[1], center[2]);
		  GLfloat color[] = {0.0f, 0.1f, 0.6f, 1.0f};
		  GLfloat emission[] = {0.0f, 0.2f, 0.6f, 1.0f};
		  GLfloat specular[] = {0.0f, 0.2f, 0.7f, 1.0f};

	  	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	  	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	  	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	  	glMaterialf(GL_FRONT, GL_SHININESS, 0.5);
	  	glutWireSphere(radius, 50.0, 50.0);
	    glPopMatrix();
    }

    void render_filled(int t, Force* external) {
      glPushMatrix();
      glTranslatef(center[0] , center[1], center[2]);
		  GLfloat color[] = {0.0f, 0.1f, 0.6f, 1.0f};
		  GLfloat emission[] = {0.0f, 0.2f, 0.6f, 1.0f};
		  GLfloat specular[] = {0.0f, 0.2f, 0.7f, 1.0f};

	  	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	  	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	  	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	  	glMaterialf(GL_FRONT, GL_SHININESS, 0.5);
	  	glutSolidSphere(radius, 50.0, 50.0);
	    glPopMatrix();
    }

    bool intersect(Force& f, Point* intersection) {
      // Implemented sphere intersection equations from PBD pg. 117
      // calculate A, B, C following Shirley
      float A = f.dir.dot(f.dir);
      float B = 2.0f * (f.dir.dot(f.pos - center));
      float C = (f.pos - center).dot(f.pos - center) - radius * radius;
      float t0;
      float t1;
      float thit;

      if (!quadraticSolver(A, B, C, &t0, &t1))
        return false;

      if (t0 > INFINITY || t1 < 0.0f)
        return false;
      thit = t0;
      if (t0 < 0.0f) {
        thit = t1;
        if (thit > INFINITY)
          return false;
      }

      *intersection = f.pos + thit * f.dir;

      return true;
    }

    bool intersectP(Force& f) {
      Point intersection;
      return intersect(f, &intersection);
    }

    bool quadraticSolver(float A, float B, float C, float *t0, float *t1) {
      float sq_term = B * B - 4 * A * C;
      if (sq_term < 0)
        return false;
      
      *t0 = 0.5f * (-B - sqrt(sq_term)) / A;
      *t1 = 0.5f * (-B + sqrt(sq_term)) / A;

      return true;
    }

};

class Cloth : public Shape {
  public:
    Cloth() {
    }
    
    //BROUGHT IN FROM CLOTH.H CLASS
    
    int num_particles_x; // number of particles in x direction
	  int num_particles_y; // number of particles in y direction
	
	  std::vector<Particle> List_Particles; //used to store the info of particle
	  std::vector<Spring> List_Springs;

    GLuint tex_2d; //texture

    
    Particle* getParticle(int x, int y) {return & List_Particles.at(x*num_particles_x + y);}
	
	  void AddSpring(Particle *p1, Particle *p2)
	  {   Spring newspring(p1,p2);
		List_Springs.push_back(newspring);
	  }
    
    void addTexture(GLuint t) {
      tex_2d = t;
    }

    Cloth(float width, float height, int num_width, int num_height)
	  {
      tex_2d = 0;

		  num_particles_x=num_width;
		  num_particles_y=num_height;
          
		  List_Particles.resize(num_particles_x*num_particles_y);
		
		  // creating a grid of particles from (0,0,0) to (width,-height,0)
		  for(int x=0; x<num_particles_x; x++)
		  {
			  for(int y=0; y<num_particles_y; y++)
			  {
				  float pos_x=width*(float)x/(float)num_particles_x;
				  float pos_y=-height*(float)y/(float)num_particles_y;
				  Vector3f pos = Vector3f(pos_x, pos_y, 0);
				  List_Particles.at(x*num_particles_x+y)=Particle(pos);
			  }
		  }
		
        
        
		  for(int x=0; x<num_particles_x; x++)
		  {
			  for(int y=0; y<num_particles_y; y++)
			  {
				  // 1 structural springs
				  if (x<num_particles_x-1)
					  AddSpring(getParticle(x,y),getParticle(x+1,y));
				  if (y<num_particles_y-1)
					  AddSpring(getParticle(x,y),getParticle(x,y+1));
                  
				  // 2 shear springs
				  if (x<num_particles_x-1 && y<num_particles_y-1)
					  AddSpring(getParticle(x,y),getParticle(x+1,y+1));
				  if (x<num_particles_x-1 && y<num_particles_y-1)
					  AddSpring(getParticle(x+1,y),getParticle(x,y+1));
                  
		          // 3 bend springs
				  if (x<num_particles_x-2)
					  AddSpring(getParticle(x,y),getParticle(x+2,y));
				  if (y<num_particles_y-2)
					  AddSpring(getParticle(x,y),getParticle(x,y+2));
				  if (x<num_particles_x-2 && y<num_particles_y-2)
					  AddSpring(getParticle(x,y),getParticle(x+2,y+2));
				  if (x<num_particles_x-2 && y<num_particles_y-2)
					  AddSpring(getParticle(x+2,y),getParticle(x,y+2));			
			}
		}        
	}
    
    
	Vector3f TriangleNormal(Particle *p1,Particle *p2,Particle *p3)
	{
		Vector3f pos1 = p1->getPos();
		Vector3f pos2 = p2->getPos();
		Vector3f pos3 = p3->getPos();
        
		Vector3f v1 = pos2-pos1;
		Vector3f v2 = pos3-pos1;
        
		Vector3f trianglenormal=v1.cross(v2);
		return trianglenormal;
	}
    
	
	void drawTriangle(Particle *p1, Particle *p2, Particle *p3, const Vector3f color)
	{
		glColor3f(color[0], color[1],color[2]);

		glNormal3f(p1->getNormal().normalized()[0], p1->getNormal().normalized()[1],p1->getNormal().normalized()[2]);
		glVertex3f(p1->getPos()[0], p1->getPos()[1], p1->getPos()[2]);
        
		glNormal3f(p2->getNormal().normalized()[0], p2->getNormal().normalized()[1],p2->getNormal().normalized()[2]);
		glVertex3f(p2->getPos()[0], p2->getPos()[1], p2->getPos()[2]);
        
		glNormal3f(p3->getNormal().normalized()[0], p3->getNormal().normalized()[1],p3->getNormal().normalized()[2]);
		glVertex3f(p3->getPos()[0], p3->getPos()[1], p3->getPos()[2]);
        
	}
    
  void drawTri(int x1, int y1, int x2, int y2, int x3, int y3) { 
    Particle *p1 = getParticle(x1,y1);
    Particle *p2 = getParticle(x2,y2);
    Particle *p3 = getParticle(x3,y3);

    glColor3f(1.0f, 1.0f, 1.0f);

		glNormal3f(p1->getNormal().normalized()[0], p1->getNormal().normalized()[1],p1->getNormal().normalized()[2]);
    glTexCoord2f((float) x1/ (float) num_particles_x,1.0-(float) y1/ (float) num_particles_y); 
		glVertex3f(p1->getPos()[0], p1->getPos()[1], p1->getPos()[2]);
        
		glNormal3f(p2->getNormal().normalized()[0], p2->getNormal().normalized()[1],p2->getNormal().normalized()[2]);
    glTexCoord2f((float) x2/ (float) num_particles_x,1.0-(float) y2/ (float) num_particles_y); 
		glVertex3f(p2->getPos()[0], p2->getPos()[1], p2->getPos()[2]);
        
		glNormal3f(p3->getNormal().normalized()[0], p3->getNormal().normalized()[1],p3->getNormal().normalized()[2]);
    glTexCoord2f((float) x3/ (float) num_particles_x,1.0-(float) y3/ (float) num_particles_y); 
		glVertex3f(p3->getPos()[0], p3->getPos()[1], p3->getPos()[2]);

    
  } 

	void drawCloth()
	{
		std::vector<Particle>::iterator it;
		for(it = List_Particles.begin(); it!= List_Particles.end(); it++)
		{
			(*it).resetNormal();
		}
        
        
		for(int x = 0; x<num_particles_x-1; x++)
		{
			for(int y=0; y<num_particles_y-1; y++)
			{
				Vector3f normal = TriangleNormal(getParticle(x+1,y),getParticle(x,y),getParticle(x,y+1));
				getParticle(x+1,y)->addToNormal(normal);
				getParticle(x,y)->addToNormal(normal);
				getParticle(x,y+1)->addToNormal(normal);
                
				normal = TriangleNormal(getParticle(x+1,y+1),getParticle(x+1,y),getParticle(x,y+1));
				getParticle(x+1,y+1)->addToNormal(normal);
				getParticle(x+1,y)->addToNormal(normal);
				getParticle(x,y+1)->addToNormal(normal);
			}
		}

    //draw texture
    if(tex_2d != 0) {
      glEnable(GL_TEXTURE_2D);
      glBindTexture( GL_TEXTURE_2D, tex_2d);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
      glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    } 
        
		glBegin(GL_TRIANGLES);
		for(int x = 0; x<num_particles_x-1; x++)
		{
			for(int y=0; y<num_particles_y-1; y++)
			{
				Vector3f color = Vector3f(0.5f, 0.5f, 0.5f);
        if(0 == tex_2d) { //no texture
				  drawTriangle(getParticle(x+1,y),getParticle(x,y),getParticle(x,y+1),color);
				  drawTriangle(getParticle(x+1,y+1),getParticle(x+1,y),getParticle(x,y+1),color);
        } else {
          drawTri(x+1,y,x,y,x,y+1);
          drawTri(x+1,y+1,x+1,y,x,y+1);
        }
			}
		}

		glEnd();
	}
    
#define NUMBER_ITERATIONS 10
    
	void Time()
	{
		std::vector<Spring>::iterator it;
		for(int i=0; i<NUMBER_ITERATIONS; i++) // iterate over all Springs several times
		{
			for(it = List_Springs.begin(); it != List_Springs.end(); it++ )
			{
				(*it).CheckSpring();
			}
		}
        
		std::vector<Particle>::iterator iter;
		for(iter= List_Particles.begin(); iter != List_Particles.end(); iter++)
		{
			(*iter).Time(); // calculate the position of each particle at the next time step.
		}
	}
    
	/* used to add gravity/other external force to all particles*/
	void AddForce(const Vector3f direction)
	{
		std::vector<Particle>::iterator it;
		for(it = List_Particles.begin(); it!= List_Particles.end(); it++)
		{
			(*it).AddForce(direction); // add the forces to each particle
		}
        
	}
    
	/* used to add wind forces to all particles*/
	//REFERENCE：http://www.cs.huji.ac.il/~irush/cloth/
	/*The wind adds force to the particle in the direction of its normal
     with size equal to the dot product between the normal and the wind's direction, multiplied by the power parameter
     */
    
	/* start by looking at the cloth as a collection of triangles of three particles;
     1. Solve the problem of adding wind one triangle at a time;
     2. Adding forces to the three particles of the triangle;
     3. The wind is coming in a certain direction, but forces should only be added in the direction of the normal of the triangle;
     4. The amount of force affecting the triangle should be proportional to the angle (dot product) between the triangle and the wind.
     */
    
	void AddWindForce(Particle *p1,Particle *p2,Particle *p3, const Vector3f direction)
	{
		Vector3f normal = TriangleNormal(p1,p2,p3);
		Vector3f d = normal.normalized();
		Vector3f force = normal*(d.dot(direction));
		p1->AddForce(force);
		p2->AddForce(force);
		p3->AddForce(force);
	}
    
	void AddWind(const Vector3f direction)
	{
		for(int x = 0; x<num_particles_x-1; x++)
		{
			for(int y=0; y<num_particles_y-1; y++)
			{
				AddWindForce(getParticle(x+1,y),getParticle(x,y),getParticle(x,y+1),direction);
				AddWindForce(getParticle(x+1,y+1),getParticle(x+1,y),getParticle(x,y+1),direction);
			}
		}
	}
    
	// used to detect the collision of the cloth with the ball.
	
	void Intersect(const Vector3f center,const float radius )
	{
		std::vector<Particle>::iterator it;
		for(it = List_Particles.begin(); it!= List_Particles.end(); it++)
		{
			Vector3f v = (*it).getPos()-center;
			float l = v.norm();
            float e = 0.001f;
			if ( v.norm() < radius + e) // if the particle is inside the ball
			{
				(*it).movePos(v.normalized()*(radius - l)); 
				// move the particle to the surface of the ball
				// add e for a better image
			}
		}
	}

    //FROM ORIGINAL
    
    Cloth(Vector3f world_pos) {
      this->world_pos = world_pos;
    }

    void render_wireframe(int t, Force* external) {
    }

    void render_filled(int t, Force* external) {
    }


    bool intersect(Force& f, Point* intersection) {
    }

    bool intersectP(Force& f) {
    }
};


#endif

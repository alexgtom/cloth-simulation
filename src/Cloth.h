/********************************************************************************
	cs184-2013fall-Final Project-Cloth Simulation

	Kristen Curry, Zhongyin Hu, Alex Tom, Aaron Zhou
********************************************************************************/
#ifndef CLOTH_H
#define CLOTH_H

#include <windows.h>
#include <GL/GL.h>
#include <GL/glut.h>
#include <GL/GLU.h>

#include <vector>
#include "Particle.h"
#include "Spring.h"

#include <Eigen/Dense>

using namespace Eigen;

class Cloth
{
public:

	int num_particles_x; // number of particles in x direction
	int num_particles_y; // number of particles in y direction
	

	std::vector<Particle> List_Particles; //used to store the info of particle
	std::vector<Spring> List_Springs; 

	Particle* getParticle(int x, int y) {return & List_Particles.at(x*num_particles_x + y);}
	
	void AddSpring(Particle *p1, Particle *p2) 
	{   Spring newspring(p1,p2);
		List_Springs.push_back(newspring);
	}
	
	Cloth(float width, float height, int num_width, int num_height) 
	{
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
		
		// making the upper left most two and upper right most two particles unmovable
			getParticle(0,0)->makeUnmovable(); 
			getParticle(1,0)->makeUnmovable(); 
			getParticle(num_particles_x-1,0)->makeUnmovable();
			getParticle(num_particles_x-2,0)->makeUnmovable();


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

		glBegin(GL_TRIANGLES);
		for(int x = 0; x<num_particles_x-1; x++)
		{
			for(int y=0; y<num_particles_y-1; y++)
			{
				Vector3f color = Vector3f(0.5f, 0.5f, 0.5f);

				drawTriangle(getParticle(x+1,y),getParticle(x,y),getParticle(x,y+1),color);
				drawTriangle(getParticle(x+1,y+1),getParticle(x+1,y),getParticle(x,y+1),color);
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
			if ( v.norm() < radius) // if the particle is inside the ball
			{
				float e=1;
				(*it).movePos(v.normalized()*((radius+e)-l)); 
				// move the particle to the surface of the ball
				// add e for a better image
			}
		}
	}


};
#endif 

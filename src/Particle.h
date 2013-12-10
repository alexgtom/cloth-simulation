/********************************************************************************
	cs184-2013fall-Final Project-Cloth Simulation

	Kristen Curry, Zhongyin Hu, Alex Tom, Aaron Zhou
********************************************************************************/

#ifndef PARTICLE_H
#define PARTICLE_H

#include <Eigen/Dense>

using namespace Eigen;

#define DAMPING 0.05
#define TIME_STEPSIZE 0.25

class Particle
{
  public:
	bool movable; // used to determine whether particle can be moved/ pin corners of the cloth

	float mass; // the mass of the particle 
	Vector3f pos; // the current position of the particle 
	Vector3f old_pos; // the position of the particle in the previous time step
	Vector3f velocity;
	Vector3f acceleration; 
	Vector3f all_normal; // used for OpenGL shading

	Particle(Vector3f position) 
	{
		movable=true; 
		pos=position;
		old_pos=position;
		acceleration=Vector3f(0,0,0);
		mass=100.0; 
		all_normal=Vector3f(0,0,0);
	}

	Particle(){}

	void AddForce(Vector3f f)
	{
		acceleration += f/mass;
	}

	void Time()
	{
		if(movable)
		{
			//1. damping account for gradual loss of velocity due to e.g. air resistance
			//2. term (old_pos-pos)implicitly represents a velocity vector

			velocity=pos-old_pos;
			Vector3f temp = pos;
            old_pos = temp;
			velocity+=acceleration*TIME_STEPSIZE;
			pos+= velocity*(1-DAMPING)*TIME_STEPSIZE ;
			
			acceleration = Vector3f(0,0,0); // acceleration is reset 

			//std::cout<<pos[0]<<" ";
		}
	}

	Vector3f& getPos() {return pos;}

	void resetAcceleration() {acceleration = Vector3f(0,0,0);}

	void movePos(const Vector3f v) { if(movable) pos += v;}

	void makeUnmovable() {movable = false;}

	void addToNormal(Vector3f normal)
	{
		all_normal += normal.normalized();
	}

	Vector3f& getNormal() { return all_normal;} 

	void resetNormal() {all_normal = Vector3f(0,0,0);}

};

#endif

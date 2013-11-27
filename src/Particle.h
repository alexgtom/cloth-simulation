/********************************************************************************
	cs184-2013fall-Final Project-Cloth Simulation

	Kristen Curry, Zhongyin Hu, Alex Tom, Aaron Zhou
********************************************************************************/

#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>
#include "Triangle.h"

using namespace std;

class Particle
{
private:
	float Mass; 
	Vector3f Position; 
	Vector3f OldPosition; 
	Vector3f Acceleration;

  vector<Triangle*> list_triangles;

public:
	Particle();
	
	Particle(Vector3f pos) 
	{
		Position=pos;
		OldPosition=pos;
		Acceleration=Vector3f(0,0,0);
		Mass=1.0;
	}
	
	float getMass() { return Mass; }
	void setMass(float m) { Mass = m; }

	Vector3f getPosition() {return Position; }
	void setPosition(Vector3f p) { Position = p; }
	void offsetPos(const Vector3f v) { Position += v;}

	Vector3f getVelocity() { return Position-OldPosition; }

	Vector3f getAcceleration() { return Acceleration; }
    void resetAcceleration() {Acceleration = Vector3f(0,0,0);}


    void AddTriange(Triangle * triangle) { list_triangles.push_back(triangle);}

	void AddForce(Vector3f f)
	{
        // Acceleration by Newton's second law
		Acceleration += f/Mass;
	}


	void Update(float dampingCoeff, float timeinterval)
	{
			Vector3f temp = Position;
			Position = Position + (Position-OldPosition)*(1.0-dampingCoeff) + Acceleration*timeinterval;
			OldPosition = temp;
            Acceleration = Vector3f(0,0,0);
	}
    

	// Compute the average normal of all triangles
	Vector3f getNormal()
	{
        Vector3f n = Vector3f(0,0,0);
        for(int i = 0; i < list_triangles.size(); i++)
        {
            n+= list_triangles[i]->getNormal();
        }

        n = n / list_triangles.size();

        return n;
	}
};
  

#endif // PARTICLE_H

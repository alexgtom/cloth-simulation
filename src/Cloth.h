/********************************************************************************
	cs184-2013fall-Final Project-Cloth Simulation

	Kristen Curry, Zhongyin Hu, Alex Tom, Aaron Zhou
********************************************************************************/

#ifndef CLOTH_H
#define CLOTH_H

#include <vector>
#include "Particle.h"
#include "Spring.h"
#include "Triangle.h"

#include <Eigen/Dense>

using namespace Eigen;

class Cloth
{
private:
	 float mass;    // mass of cloth
	 float width;  //width of cloth
     float height;  //height of cloth
	int HNum;  //number of horizontal particles
	int VNum;  //number of vertical particles
	float DampingCoeff;

	std::vector<Particle> list_particles; // list of all particles of the cloth
	std::vector<Spring> list_strings; //list of all constraints between particles of this cloth
    std::vector<Triangle*> list_triangles;  //list of all the triangles 

	Particle* getParticle(int x, int y) { return &list_particles[y*HNum + x]; }
	inline void addString(Particle *p1, Particle *p2) { list_strings.push_back(Spring(p1,p2)); }

    void prepareCloth();

public:

    Cloth(float widthvalue, float heightvalue, int horizontalParticles, int verticalParticles, float dampingCoeff)
    {
		width=widthvalue; 
		height=heightvalue;
		HNum=horizontalParticles;
	    VNum=verticalParticles;
		DampingCoeff=dampingCoeff;

        prepareCloth();
	}

    float getMass() { return mass; }
    void setMass(float m)
    {
        mass = m;
		float mass = m/(HNum*VNum);
		for(int i = 0; i < list_particles.size(); i++) list_particles[i].setMass(mass);
    }

    float getWidth() { return width; }
    void setWidth(float w) { width = w; prepareCloth();}
    
    float getHeight() { return height; }
    void setHeight(float h) { height = h; prepareCloth();}

    float getDampingCoeff() { return DampingCoeff; }
    void setDampingCoeff(float coeff) { DampingCoeff = coeff; }

	int getNumOfHorizontalParticles() { return HNum; }
    void setNumOfHorizontalParticles(float hPart) { HNum = hPart; prepareCloth();}

	int getNumOfVerticalParticles() { return VNum; }
    void setNumOfVerticalParticles(float vPart) { VNum = vPart; prepareCloth();}

	void Draw();

	void Update();

	void AddForce(const Vector3f direction); // adds forces to the particles 

	void Intersect(float r, Vector3f p);//Sphere Collision detection using the position and radius of the sphere 
    
};

#endif // CLOTH_H

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

#include "Cloth.h"
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

	Particle* getParticle(int x, int y) { return &list_particles[x*HNum + y]; }
	void AddSpring(Particle *p1, Particle *p2) { list_strings.push_back(Spring(p1,p2)); }

        void prepare();

public:

    Cloth(float widthvalue, float heightvalue, int horizontalParticles, int verticalParticles, float dampingCoeff)
    {
		width=widthvalue; 
		height=heightvalue;
		HNum=horizontalParticles;
	        VNum=verticalParticles;
		DampingCoeff=dampingCoeff;

                prepare();
	}

    float getMass() { return mass; }
    void setMass(float m)
    {
                mass = m;
		float mass = m/(HNum*VNum);
		for(int i = 0; i < list_particles.size(); i++) list_particles[i].setMass(mass);
    }

    float getWidth() { return width; }
    void setWidth(float w) { width = w; prepare();}
    
    float getHeight() { return height; }
    void setHeight(float h) { height = h; prepare();}

    float getDampingCoeff() { return DampingCoeff; }
    void setDampingCoeff(float coeff) { DampingCoeff = coeff; }

    int getNumOfHorizontalParticles() { return HNum; }
    void setNumOfHorizontalParticles(float hPart) { HNum = hPart; prepare();}
    
    int getNumOfVerticalParticles() { return VNum; }
    void setNumOfVerticalParticles(float vPart) { VNum = vPart; prepare();}

	void Draw();

	void Update();

	void AddForce(Vector3f direction); // adds forces to the particles 

	void Intersect(float r, Vector3f p);//Sphere Collision detection using the position and radius of the sphere 
    
};

void Cloth::prepare()
{
    list_particles.clear();
	list_particles.resize(HNum*VNum); 

   
	for(int x = 0; x < HNum; x++)
    {
        for(int y = 0; y < VNum; y++)
        {
            // Compute the initial cloth particle positions 
			Vector3f pos = Vector3f(width * (x/(float)HNum), height * (y/(float)VNum), 0);
			list_particles[x*HNum+y]= Particle(pos); 
        }
    }

    // Prepare the triangles that are being drawn
    for(int x = 0; x < HNum-1; x++)
    {
        for(int y = 0; y < VNum-1; y++)
        {
            // Construct triangles between the particles and add them to the cloth
            Triangle *tr1 = new Triangle(getParticle(x+1,y),getParticle(x,y),getParticle(x,y+1));
            Triangle *tr2 = new Triangle(getParticle(x+1,y+1),getParticle(x+1,y),getParticle(x,y+1));

            getParticle(x+1,y)->AddTriangle(tr1);
            getParticle(x,y)->AddTriangle(tr1);
            getParticle(x,y+1)->AddTriangle(tr1);

            getParticle(x+1,y+1)->AddTriangle(tr2);
            getParticle(x+1,y)->AddTriangle(tr2);
            getParticle(x,y+1)->AddTriangle(tr2);

            list_triangles.push_back(tr1);
            list_triangles.push_back(tr2);
        }
    }

       //construct springs bewteen different particles
	   
    for(int x = 0; x < HNum; x++)
    {
        for(int y = 0; y < VNum; y++)
        {
			// 1 structural springs
            if (x < HNum-1) AddSpring(getParticle(x,y),getParticle(x+1,y));
            if (y < VNum-1) AddSpring(getParticle(x,y),getParticle(x,y+1));
			
			// 2 shear springs
            if (x < HNum-1 && y < VNum-1) AddSpring(getParticle(x,y),getParticle(x+1,y+1));
            if (x < HNum-1 && y < VNum-1) AddSpring(getParticle(x+1,y),getParticle(x,y+1));
   
			//3 bending springs
            if (x < HNum-2) AddSpring(getParticle(x,y),getParticle(x+2,y));
            if (y < VNum-2) AddSpring(getParticle(x,y),getParticle(x,y+2));
            if (x < HNum-2 && y<VNum-2) AddSpring(getParticle(x,y),getParticle(x+2,y+2));
            if (x  <HNum-2 && y<VNum-2) AddSpring(getParticle(x+2,y),getParticle(x,y+2));			
	}
    }
}


void Cloth::Intersect(float r, Vector3f p)
{
    
    for(int i = 0; i < list_particles.size(); i++)
    {
        Particle &particle = list_particles[i];

        Vector3f v = particle.getPosition()-p;
        float l = v.norm();
        if ( v.norm() < r) // if the particle is inside the sphere
        {
            particle.movePos(v.normalized()*r);
        }
    }
}


void Cloth::AddForce(const Vector3f direction)
{
    std::vector<Particle>::iterator particle;
    for(particle = list_particles.begin(); particle != list_particles.end(); particle++)
    {
        (*particle).AddForce(direction); // add the forces to each particle
    }

}

void Cloth::Update()
{
		for(int j = 0; j < list_strings.size(); j++)
        {
            // satisfy constraint
			list_strings[j].Check();
        }

    std::vector<Particle>::iterator it;
	for(it= list_particles.begin(); it != list_particles.end(); it++)
    {
        (*it).Update(DampingCoeff, 0.25f);
    }
}


void Cloth::Draw()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);

    // Draw each triangle of the cloth
    for(int i = 0; i < list_triangles.size(); i++)
        list_triangles[i]->Draw();

    glEnd();
}




#endif // CLOTH_H

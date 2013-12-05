/********************************************************************************
	cs184-2013fall-Final Project-Cloth Simulation

	Kristen Curry, Zhongyin Hu, Alex Tom, Aaron Zhou
********************************************************************************/

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <windows.h>
#include <GL/GL.h>
#include <GL/glut.h>
#include <GL/GLU.h>

#include <Eigen/Dense>
#include "Particle.h"

using namespace Eigen;

class Triangle
{
private:
    Particle *Par1;
    Particle *Par2;
    Particle *Par3;

public:
	Triangle (Particle* p1, Particle* p2, Particle* p3)
    {
        Par1 = p1;
        Par2 = p2;
        Par3 = p3;
    }

    Vector3f  getNormal();

    void Draw();

    Particle* getParticle1() { return Par1; }
    Particle* getParticle2() { return Par2; }
    Particle* getParticle3() { return Par3; }

};


    // return the normal of the current triangle
 Vector3f Triangle::getNormal()
    {
		Vector3f v1 = Par2->getPos()-Par1->getPos();
		Vector3f v2 = Par3->getPos()-Par1->getPos();

        return v1.cross(v2).normalized();
    }

    // Draw the triangle
    void Triangle::Draw()
    {
		glNormal3fv((float*)&(Par1->getNormal().normalized()));
		glVertex3fv((float*)&(Par1->getPos()));

		glNormal3fv((float*)&(Par2->getNormal().normalized()));
		glVertex3fv((float*)&(Par2->getPos()));

		glNormal3fv((float*)&(Par3->getNormal().normalized()));
		glVertex3fv((float*)&(Par3->getPos()));
    }


#endif 

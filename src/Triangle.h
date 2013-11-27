/********************************************************************************
	cs184-2013fall-Final Project-Cloth Simulation

	Kristen Curry, Zhongyin Hu, Alex Tom, Aaron Zhou
********************************************************************************/

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <Eigen/Dense>

using namespace Eigen;

class Triangle
{
private:
    Particle *mParticle1;
    Particle *mParticle2;
    Particle *mParticle3;

    float mTextureCoord[3][2];
public:
    Triangle(Particle *p1, Particle *p2, Particle *p3);

    Vector3f  getNormal();

    void Draw();

    Particle* getParticle1() { return mParticle1; }
    Particle* getParticle2() { return mParticle2; }
    Particle* getParticle3() { return mParticle3; }
};


Triangle::Triangle(Particle *p1, Particle *p2, Particle *p3)
    {
        mParticle1 = p1;
        mParticle2 = p2;
        mParticle3 = p3;
    }

    // return the normal of the current triangle
    Vector3f Triangle::getNormal()
    {
		Vector3f v1 = mParticle2->getPos()-mParticle1->getPos();
		Vector3f v2 = mParticle3->getPos()-mParticle1->getPos();

        return v1.cross(v2).normalized();
    }

    // Draw the triangle
    void Triangle::Draw()
    {
		glNormal3fv((float*)&(mParticle1->getNormal().normalized()));
		glVertex3fv((float*)&(mParticle1->getPos()));

		glNormal3fv((float*)&(mParticle2->getNormal().normalized()));
		glVertex3fv((float*)&(mParticle2->getPos()));

		glNormal3fv((float*)&(mParticle3->getNormal().normalized()));
		glVertex3fv((float*)&(mParticle3->getPos()));
    }


#endif // TRIANGLE_H

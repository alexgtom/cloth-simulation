/********************************************************************************
	cs184-2013fall-Final Project-Cloth Simulation

	Kristen Curry, Zhongyin Hu, Alex Tom, Aaron Zhou
********************************************************************************/

#ifndef SPRING_H
#define SPRING_H

#include "Particle.h"

class Spring
{
public:
	float free_length; // the length between particle p1 and p2 without force
	float ks; //spring constant 
	float kd; //damping coefficient of the spring

	Particle *p1, *p2; // the two particles that are connected through this Spring

	Spring(Particle *par1, Particle *par2)
	{
		p1=par1;
		p2=par2;
		Vector3f vec = p1->getPos()-p2->getPos();
		free_length = vec.norm();
		ks=0.2;
		kd=0.2;
	}


	void CheckSpring()
	{
		Vector3f p1_to_p2 = p2->getPos()-p1->getPos(); 
		float current_distance = p1_to_p2.norm(); 
		float dist=current_distance-free_length;
		Vector3f dir_spring=p1_to_p2.normalized();  //direction of the spring force
		float springforce=-kd*abs((p2->velocity.norm()-p1->velocity.norm()))-ks*dist;
		//p1->AddForce(-springforce*dir_spring);
		//p2->AddForce(springforce*dir_spring);
		Vector3f correctionVector =0.5*p1_to_p2*(1-free_length/current_distance); // The offset vector that could moves p1 into a distance of free_length to p2
		p1->movePos(correctionVector); // should move p1 half the length needed
		p2->movePos(-correctionVector); // move p2 the negative direction of correctionVector
	}
};
#endif

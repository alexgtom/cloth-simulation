/********************************************************************************
	cs184-2013fall-Final Project-Cloth Simulation

	Kristen Curry, Zhongyin Hu, Alex Tom, Aaron Zhou
********************************************************************************/

#ifndef SPRING_H
#define SPRING_H

#include "Particle.h"

class Spring
{
private:
	float RestLength;
        Particle *Particle1, *Particle2;

public:
	Spring(Particle *p1, Particle *p2)  
	{
		Particle1=p1;
		Particle2=p2;
		Vector3f vec = Particle1->getPosition() - Particle2->getPosition();
                RestLength = vec.norm();
	}

    
	void Check()
	{
		Vector3f part1Part2 = Particle2->getPosition() - Particle1->getPosition(); // vector from p1 to p2
		float current_distance = part1Part2.norm(); // current distance between p1 and p2
		Vector3f moveVector = part1Part2*(1 - RestLength/current_distance); // The offset vector that could moves p1 into a distance of rest_distance to p2
		Vector3f halfmoveVector = moveVector*0.5f; // half that length, so that we can move BOTH p1 and p2.
		Particle1->movePos(halfmoveVector); // Halfmovevector is pointing from p1 to p2, so the length should move p1 half the length.
		Particle2->movePos(-halfmoveVector); // move p2 the negative direction of Halfmovevector since it points from p2 to p1	
	}

};

#endif

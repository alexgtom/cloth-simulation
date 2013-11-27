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

    
	void satisfyConstraint()
	{
		Vector3f part1ToPart2 = Particle2->getPosition() - Particle1->getPosition(); // vector from p1 to p2
		float current_distance = part1ToPart2.norm(); // current distance between p1 and p2
		Vector3f correctionVector = part1ToPart2*(1 - RestLength/current_distance); // The offset vector that could moves p1 into a distance of rest_distance to p2
		Vector3f correctionVectorHalf = correctionVector*0.5f; // Lets make it half that length, so that we can move BOTH p1 and p2.
		Particle1->offsetPos(correctionVectorHalf); // correctionVectorHalf is pointing from p1 to p2, so the length should move p1 half the length needed to satisfy the constraint.
		Particle2->offsetPos(-correctionVectorHalf); // we must move p2 the negative direction of correctionVectorHalf since it points from p2 to p1, and not p1 to p2.	
	}

};

#endif // SPRING_H

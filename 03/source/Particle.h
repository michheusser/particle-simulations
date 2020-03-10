/*
 *  Particle.h
 *  Exercise 3
 *
 *  Created by Diego Rossinelli on 3/17/11.
 *  Copyright 2011 ETH Zurich. All rights reserved.
 *
 */

#pragma once

#include <math.h>

#include "Force.h"

class Particle
{
protected:
	
	double x[3];
	double v[3];
	double a[3];
	double m;
	
	inline double _force_magnitude_LJ(double r) const;
	void _reproject();
	
public:
	
	Particle();
	//START MODIFYING HERE
	
	//Question 4
	//
	virtual void update(double dt);
	//
	int getType();
    
	// END MODIFYING HERE
	void operator += (const Force f);	
	double& operator[](int i);
	
	Force computeLocalInteraction(const Particle& p) const;
};

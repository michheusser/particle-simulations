/*
 *  TwinParticle.h
 *  Exercise 3
 *
 *  Created by Diego Rossinelli on 3/17/11.
 *  Copyright 2011 ETH Zurich. All rights reserved.
 *
 */

#pragma once
#include <math.h>
#include <stdlib.h>


#include "Particle.h"

class TwinParticle: public Particle
{
	enum TwinParticleState { Initialized, Connected };
	
	TwinParticleState state;
	double length_at_rest;
	const TwinParticle* twin;
	
	inline void _add_spring_force(const TwinParticle& p);
	
public:
	
	TwinParticle(double rest_length=1);
	
	void update(double dt);
	int getType();
	
	void connect(const TwinParticle* twin_);
};

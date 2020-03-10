/*
 *  Simulation.h
 *  Exercise 2
 *
 *  Created by Diego Rossinelli on 3/1/11.
 *  Copyright 2011 ETH Zurich. All rights reserved.
 *
 */

#pragma once

#include <string>

using namespace std;

#include "ParticleVector.h"
#include "InteractionMatrix.h"

class Simulation
{
	// Private class variables
	ParticleVector particles;
	InteractionMatrix interaction_matrix;
	
	// Private class methods (helpers)
	int _lines(string filename);
	ParticleVector _createParticlesFromFile(string filename);
	void _computeInteractionMatrix();
	void _updateParticleVector(double dt);
	void _dumpToFile(size_t stepid, double t);
	
public:
	
	// Constructor
	Simulation(string filename);
	
	// Public class method
	void run(double time_end, double dt);
};

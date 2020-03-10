/*
 *  InteractionMatrix.h
 *  Exercise 2
 *
 *  Created by Diego Rossinelli on 3/1/11.
 *  Copyright 2011 ETH Zurich. All rights reserved.
 *
 */

#pragma once

#include <assert.h>
#include <string.h>

#include "Force.h"

class InteractionMatrix
{
	// Class variables
	unsigned int n;
	Force * data; 
	
public:
	
	// Default constructor
	InteractionMatrix();
	// Constructor
	InteractionMatrix(int number_of_particles);
	// Destructor
	~InteractionMatrix();
	
	// Overloaded operator
	InteractionMatrix& operator=(const InteractionMatrix& c);
	
	// Accessor
	Force& getForce(int ip1, int ip2);
};

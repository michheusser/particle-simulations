#pragma once

#include "Particle.h"

class ParticleVector
{
	// Class variables
	int nparticles;
	/* Pointer to Particles (Careful!!! you need to ALLOCATE memory, 
	   DELETE memory, and ACCESS it properly!!!)	 
	 */
	Particle * data;	
	
public:
	
	// Default constructor
	ParticleVector();
	// Constructor
	ParticleVector(int nparticles);	
	// Destructor
	~ParticleVector();

	// Overloaded operator
	ParticleVector& operator=(const ParticleVector& c);
		
	// Accessor
	int getNumberOfParticles() const;
	
	// Overloaded operator
	Particle& operator[](int i) const;
};

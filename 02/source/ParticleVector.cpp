#include <assert.h>
#include <fstream>
#include <string.h>
#include <stdio.h>

#include "ParticleVector.h"

// Deafaul constructor 
/* Initialize pointer to NULL and zero particles */
ParticleVector::ParticleVector(): data(NULL), nparticles(0)
{
}

// Constructor
ParticleVector::ParticleVector(int nparticles): nparticles(nparticles), data(NULL)
{
	/* Check number of particles > zero */
	assert(nparticles>0);
	/* ALLOCATE particle vector */
	data = new Particle[nparticles];
	/* Check particle vector pointer is not NULL */
	assert(data != NULL);
}

// Destructor
ParticleVector::~ParticleVector()
{
	/* DEALLOCATE the memory allocate for the vector */
	if (data != NULL) 
		delete [] data;
}

// Accessor
int ParticleVector::getNumberOfParticles() const
{
	return nparticles;
}

// Overloaded operator [], to access particle i
Particle& ParticleVector::operator[](int i) const
{
	assert(i>=0 && i<nparticles);
	return data[i];
}

// Overloaded operator = to set particle vector
ParticleVector& ParticleVector::operator=(const ParticleVector& c)
{
	/* Deallocate memory for precaution */
	if (data != NULL) 
		delete [] data;
	
	/* Re-allocate data vector */
	nparticles = c.nparticles;
	data = new Particle[nparticles];
	
	/* Copy data */
	memcpy(data, c.data, sizeof(Particle)*nparticles);
	
	/* Return pointer to this object */
	return *this;
}

#pragma once

#include "Particle.h"

//structure that handle multiple objects of type Particle
class ParticleContainer
{
	Particle * array; //C-array containing all the particle data
	int nparticles; //number of particles
	
	//helper method for deallocation of the data
	void _dispose(); 
	//helper method for counting the lines of a text file
	int _lines(const char * const filename); 
	
public:
	
	ParticleContainer(); //constructor
	~ParticleContainer(); //destructor

	//load a set of particles from a file 
	void Load(const char * const filename);
	
	//get the particles data
	const Particle * const GetParticles() const;
	
	//get the number of allocated particles
	int GetNumberOfParticles() const;	
};
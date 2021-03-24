#pragma once

#include <math.h>
#include <assert.h>

class ParticleVector
{
	vector<Particle *> data;
	
public:
	
	ParticleVector(): data() {}
	
	void handleNewParticle(Particle * p)
	{
		assert(p != NULL);
		data.push_back(p);
	}
	
	~ParticleVector()
	{
		for(int i=0; i<data.size(); i++)
			delete data[i];
	}
	
	int getNumberOfParticles() const
	{
		return data.size();
	}
		
	Particle& operator[](const int i) const
	{
		assert(i>=0);
		assert(i<data.size());
		return *data[i];
	}
	
	const Particle& readParticle(const int i) const
	{
		//START FILLING IN HERE
		// Question 3 
	  assert(i>=0); 
	  assert(i<data.size());
	  return *data[i];

		//STOP FILLING IN HERE
	}
};

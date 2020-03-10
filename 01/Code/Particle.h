#pragma once

//structure containing the attributes of one particle
struct Particle
{
	double x,y; //position
	double u,v; //velocity
	double omega; //vorticity
	
public:
	
	//constructor, initializes all the particle attributes to zero
	Particle(); 
	
	//given the particle attributes, it computes the color, (r,g,b) are the output
	void ComputeColor(double& r, double& g, double& b) const;
};


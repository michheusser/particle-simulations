#pragma once

#include <assert.h>

class Force
{
	double data[3]; 
	
public:	
	Force(double fx = 0, double fy = 0, double fz = 0)
	{
		data[0] = fx;
		data[1] = fy;
		data[2] = fz;
	}
	
	Force(const Force& source)
	{
		for(int i=0; i<3; i++)
			data[i] = source.data[i];
	}
	
	// START FILLING IN HERE
	// QUESTION 1
	// Overload [] operator


	double & operator [] (int c){
	  
	  assert(c<3 && c>=0);
	  return data[c];

	}

 
	// STOP FILLING HERE 
	void operator += (const double val[3])
	{
		for(int i=0; i<3; i++)
			data[i] += val[i];
	}
};

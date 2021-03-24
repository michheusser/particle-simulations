# pragma once

#include <assert.h>
#include <math.h>
#include <string>

#include "Force.h"

/* Declare the use of the standrd namespace */
using namespace std;


struct Particle
{
	// Class variables
	double x[3], v[3], a[3], m;
	
public:
	
	// Constructor
	/* Basic class varibles can be initialized 
	   through the syntax ClassName(): variable(value)
	 */
	// FILL HERE: write the default constructor, where x,v,a are set to zero
	
  Particle(){

    m = 1;
    for(int i=0;i<3;i++){
      x[i] = 0;
      v[i] = 0;
      a[i] = 0;
    }

  }

         //            and mass to 1 
	
	// Class method to compute pairwise forces
	/* Note: the keyword "const" after the method signature
	   means that in the body of the method the STATE of the class (x,v,a,m) CANNOT be modified
	 */
	Force computeLocalInteraction(const Particle& p) const
	{
		/* Precaution to avoid division by zero (see magnitude calculation)
		 */
		const double smoothing_length = 1e-1;
		
		/* Compute vector connecting particles */
		const double r[3] = {
			p.x[0] - x[0], 
			p.x[1] - x[1],
			p.x[2] - x[2]
		};
		
		/* Compute distance, direction of the force and its magnitude */
		const double IrI = sqrt(pow(r[0],2) + pow(r[1],2) + pow(r[2],2));
		const double direction[3] = { r[0]/IrI, r[1]/IrI, r[2]/IrI };
		const double magnitude = m*p.m/(pow(r[0],2) + pow(r[1],2) + pow(r[2],2) + smoothing_length);
		
		/* Instantiate a new force */
		Force f;
		
		/* Set components through mutator */
		for(int i=0; i<3; i++)
			f.setComponent(i, magnitude*direction[i]);
		
		/* return forces */
		return f;
	}
	
	// Mutator
	void update(double dt)
	{
		/* Update particle's state */
		for (int i=0; i<3; i++)
		{
			x[i] += dt*v[i]; 
			v[i] += dt*a[i];
			/* Remember to set it to zero!! */
			a[i] = 0;
		}
	}
	
	// Mutator
	void addToAcceleration(const Force& f)
	{		
		// FILL HERE: add force contribution to the accelleration 
	 
	  a[0]+=f.getComponent(0)/m;
	  a[1]+=f.getComponent(1)/m;
	  a[2]+=f.getComponent(2)/m;

	}
};

/*
 *  Force.h
 *  Exercise 2
 *
 *  Created by Diego Rossinelli on 3/1/11.
 *  Copyright 2011 ETH Zurich. All rights reserved.
 *
 */

#pragma once

#include <assert.h>

class Force
{
	// Class variables
	double data[3]; 
	
public:
	
	// Default constructor 
	Force()
	{
		/* initialize force to zero */
		data[0] = data[1] = data[2] = 0;
	}
	
	// Copy constructor
	Force(const Force& source)
	{
		
		// FILL HERE: create a new force object as a copy of the other,
	  data[0] = source.getComponent(0);
	  data[1] = source.getComponent(1);
	  data[2] = source.getComponent(2);

	//            i.e. set data accordingly 
	}
	
	// Accessor
	double getComponent(int c) const
	{		
		/* check to avoid access out-of-bounds */
		assert(c>=0 && c<=2);
		
		/* return component */
		return data[c];
	}
	
	// Accessor
	Force opposite() const
	{
		// FILL HERE: compute a force opposite to the one stored 
	  Force oppforce;
	  oppforce.setComponent(0,data[0]*(-1));
	  oppforce.setComponent(1,data[1]*(-1));
	  oppforce.setComponent(2,data[2]*(-1));
	  return oppforce;
		//            in data and return it 
	}
	
	// Mutator
	void setComponent(int c, double val)
	{	
		/* check to avoid access out-of-bounds */
		assert(c>=0 && c<=2);
		
		/* set component value */ 
		data[c] = val;
	}
};

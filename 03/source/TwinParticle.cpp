/*
 *  TwinParticle.cpp
 *  Exercise 3
 *
 *  Created by Diego Rossinelli on 3/17/11.
 *  Copyright 2011 ETH Zurich. All rights reserved.
 *
 */

#include <math.h>

#include "TwinParticle.h"

void TwinParticle::_add_spring_force(const TwinParticle& p)
{
//START FILLING IN HERE
//Question 4
// Use Hooke's law to compute the spring contributions 
  /* r= sqrt ( pow((p.x[0]-x[0]),2) +pow((p.x[1]-x[1]),2) + pow((p.x[2]-x[2]),2) );

		  }
  for(int i=0; i<3; i++){
    a[i]+=(_force_magnitude_LJ(r)/m)*;
    }*/

double r[3];
for(int i=0; i<3; i++){
  r[i]= p.x[i]-x[i];
  }
 double distance=  sqrt ( pow((p.x[0]-x[0]),2) +pow((p.x[1]-x[1]),2) + pow((p.x[2]-x[2]),2) );
 
 double springforce= 2*(distance-length_at_rest)/length_at_rest;

 for (int i=0;i<3;i++){
     a[i]+= springforce/m * r[i]/distance;
   }


//END FILLING IN HERE
}

TwinParticle::TwinParticle(double rest_length): twin(NULL), state(Initialized), length_at_rest(rest_length) 
{
}

void TwinParticle::update(double dt)
{
// START FILLING IN HERE
//Question 4
//Ensure particle's state is connected
//Add the spring contributions 
//Update particle velocity/position with an explicit Euler
//Reset acceleration to 0
//
  assert(state==Connected); 
  _add_spring_force(*twin);
  
  for(int i=0;i<3;i++){
  v[i]+=dt*a[i];
  x[i]+=dt*v[i];
  a[i]=0;
  }

// END FILLING IN HERE
}

int TwinParticle::getType(){
	return 2;
}

void TwinParticle::connect(const TwinParticle* twin_)
{
//START FILLING IN HERE
//Question 4
// Connect an atom to another one
//Ensure:particle's state is initialized->advance it to connected 

  assert(state==Initialized);
    assert(twin_!=NULL);
  twin=twin_;
  length_at_rest = sqrt(pow(x[0]-twin_->x[0],2)+pow(x[1]-twin_->x[1],2)+pow(x[2]-twin_->x[2],2));
  state=Connected;

//END FILLING IN HERE
}

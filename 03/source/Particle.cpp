/*
 *  Particle.cpp
 *  Exercise 3
 *
 *  Created by Diego Rossinelli on 3/17/11.
 *  Copyright 2011 ETH Zurich. All rights reserved.
 *
 */

#include <math.h>

#include "Particle.h"

const int domain_size = 10;
const double rmin = domain_size/10.; 
const double cutoff = 2.5*rmin/pow(2, 1./6.); 
const double epsilon = 1.0;

double Particle::_force_magnitude_LJ(double r) const
{	
	return 4 * epsilon * (-12 * pow(rmin, 12) * pow(r, -13) + 6 * pow(rmin, 6) * pow(r, -7));
}

void Particle::_reproject()
{
	const int istart[3] = {
		floor(x[0]/domain_size), 
		floor(x[1]/domain_size),
		floor(x[2]/domain_size)
	};
	
	const double x_start[3] = {
		domain_size*istart[0], 
		domain_size*istart[1],
		domain_size*istart[2] 
	};
	
	x[0] = x[0] - x_start[0]; 
	x[1] = x[1] - x_start[1]; 
	x[2] = x[2] - x_start[2]; 	
}

Particle::Particle(): m(1)
{
	x[0] = x[1] = x[2] = 0;
	v[0] = v[1] = v[2] = 0;
	a[0] = a[1] = a[2] = 0;
}

 void Particle::update(double dt)
{
	x[0] += dt*v[0]; 
	x[1] += dt*v[1]; 
	x[2] += dt*v[2];
	
	v[0] += dt*a[0]; 
	v[1] += dt*a[1]; 
	v[2] += dt*a[2]; 
	
	a[0] = a[1] = a[2] = 0;
	
	_reproject();
}

int Particle::getType()
{
	return 1;
}

void Particle::operator += (const Force f)
{
//START FILLING IN HERE
// Question 2 
  assert(m>0);
  Force f_=f;
  for(int i=0; i<3; i++){
     a[i]+=f_[i]/m;
      }

//STOP FILLING IN HERE
}

double& Particle::operator[](int i)
{
	assert(i>=0);
	assert(i<3);
	
	return x[i];
}

Force Particle::computeLocalInteraction(const Particle& p) const
{			
	double fx = 0, fy = 0, fz = 0;
	
	for(int dz=-1; dz<2; dz++)
		for(int dy=-1; dy<2; dy++)
			for(int dx=-1; dx<2; dx++)
			{
				const double rx = domain_size * dx + p.x[0] - x[0];
				const double ry = domain_size * dy + p.x[1] - x[1];
				const double rz = domain_size * dz + p.x[2] - x[2];
				
				const double IrI = sqrt(rx*rx+ ry*ry + rz*rz);
				
				if (IrI > cutoff) continue;
				
				const double magnitude = _force_magnitude_LJ(IrI);
				const double invIrI  = 1./IrI;
				
				fx += magnitude*rx*invIrI;
				fy += magnitude*ry*invIrI;
				fz += magnitude*rz*invIrI;
			}
	
	return Force(fx, fy, fz);
}

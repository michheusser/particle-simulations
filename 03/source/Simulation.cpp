/*
 *  Simulation.cpp
 *  Exercise 3
 *
 *  Created by Florian Milde on 3/16/11.
 *  Copyright 2011 ETH Zurich. All rights reserved.
 *
 */

#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

#include "Simulation.h"

void Simulation::_computeForces()
{
    const int N = particles.getNumberOfParticles();
    
#pragma omp parallel for
    for(int i=0; i<N; i++)
		for(int j=0; j<N; j++)
		{
			if (i==j) continue;
			
			Particle& p = particles[i];
			
			const Force f = p.computeLocalInteraction(particles[j]);
			
			p += f;
		}
}

void Simulation::_updateParticleVector(double dt)
{
    const int N = particles.getNumberOfParticles();
    
#pragma omp parallel for
    for(int j=0; j<N; j++)
		particles[j].update(dt);
}

void Simulation::_dumpToFile(size_t stepid, double t)
{
    ofstream outputstream("output.xyz", stepid == 0 ? ios::out : ios::app);
    
    const int N = particles.getNumberOfParticles();
    
    outputstream << N << endl << "Atoms" << endl;
    
    for(int i=0; i<N; i++)
		outputstream << particles[i].getType() << " " << particles[i][0] 
		<< " " << particles[i][1] << " " << particles[i][2] << " " << endl;		
}

Simulation::Simulation(const char * const filename): 
particles()
{
    ifstream inputstream(filename);
	
	if (!inputstream.good())
	{
		cout << "I tried to load this file: " << filename << ", but I could not." << endl;
		exit(-1);
	}
	
    //simple particles
    {
		int N = -1;
		inputstream >> N;
		assert(N >= 0);
		
		for(int i=0; i<N; i++)
		{			
			particles.handleNewParticle(new Particle);

			inputstream >> particles[i][0];
			inputstream >> particles[i][1];
			inputstream >> particles[i][2];
		}
    }
    
    //twin particles
    {
		int N = -1;
		inputstream >> N;
		assert(N >= 0);
		
		for(int i=0; i<N; i++)
		{
			TwinParticle& p1 = *new TwinParticle();
			TwinParticle& p2 = *new TwinParticle();
			
			inputstream >> p1[0];
			inputstream >> p1[1];
			inputstream >> p1[2];
			inputstream >> p2[0];
			inputstream >> p2[1];
			inputstream >> p2[2];
			
			p1.connect(&p2);
			p2.connect(&p1);						
			
			particles.handleNewParticle(&p1);
			particles.handleNewParticle(&p2);
		}
    }
}

void Simulation::run(double time_end, double dt)
{    
    const size_t nsteps = (size_t)(time_end/dt);
    
    for(size_t step=0; step<nsteps; step++)
    {
		_computeForces();
		_updateParticleVector(dt);
		
		if (step % 20 == 0)
		{
			_dumpToFile(step, (step+1)*dt);
			
			cout << "Completion: " << setprecision(2) << step*100./nsteps << "%" << endl;
		}
    }    
}

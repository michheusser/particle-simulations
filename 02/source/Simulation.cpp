#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#include "Simulation.h"

// Helper
int Simulation::_lines(string filename)
{	
	/* Open file stream */
	ifstream filestream(filename.c_str());
	
	/* Check that the filestream is correctly open */
	if (!filestream.good())
	{
		cout << "ooops: file not found. Exiting now." << endl;
		exit(-1);
	}
	
	/* Count number of lines contained in the file*/
	int c = 0;
	string line;
	while( getline(filestream, line) ) c++;
	
	/* Return number of lines */
	return c;
}

// Helper
ParticleVector Simulation::_createParticlesFromFile(string filename)
{
	/* Get total number of particles */
	const int N = _lines(filename);
	
	/* Instantiate particle vector with the right number of particles */
	ParticleVector particles(N);
	
	/* Open file stream */
	ifstream filestream(filename.c_str());
	
	/* Parse input file and set particle attributes through */
	for(int i=0; i<N; i++)
	{
		assert(filestream.good());
		
		filestream >> particles[i].x[0];
		filestream >> particles[i].x[1];
		filestream >> particles[i].x[2];
		
		filestream >> particles[i].v[0];
		filestream >> particles[i].v[1];
		filestream >> particles[i].v[2];
	}
	
	/* Return particle vector */
	return particles;
}

// Helper
void Simulation::_computeInteractionMatrix()
{	
	// FILL HERE: fill the interaction matrix, i.e. compute pairwise forces

  for(int i=0; i<particles.getNumberOfParticles();i++){

    for(int j=0;j<particles.getNumberOfParticles();j++){
    
      if(j!=i){
      interaction_matrix.getForce(i,j).setComponent(0, particles[i].computeLocalInteraction(particles[j]).getComponent(0));
      
      interaction_matrix.getForce(i,j).setComponent(1, particles[i].computeLocalInteraction(particles[j]).getComponent(1));
      
      interaction_matrix.getForce(i,j).setComponent(2, particles[i].computeLocalInteraction(particles[j]).getComponent(2));
      }

    }
  }
}

// Helper
void Simulation::_updateParticleVector(double dt)
{
	/* Get number of particles */
	const int n = particles.getNumberOfParticles();
	
	/* Loop over all particles (and for each particle loop over all 
	   particles again) to compute accellerations
	 */
	for(int j=0; j<n; j++)
	{
		/* Retrieve particle */
		Particle& p = particles[j];
		
		/* Loop over all particles */
		for(int i=0; i<n; i++)
			p.addToAcceleration( interaction_matrix.getForce(j, i) );
		
		/* Update particle's state */
		p.update(dt);
	}	
}

// Helper
void Simulation::_dumpToFile(size_t stepid, double t)
{	
	/* Open OUTPUT streamfile */
	ofstream outputstream("output.xyz", stepid==0 ? ios::out : ios::app);
	
	/* Get number of particles */
	const int N = particles.getNumberOfParticles();

	/* Dump particles' attributes in VMD format */
	outputstream << N << endl << "Atoms" << endl;
		
	for(int i=0; i<N; i++)
	{
		outputstream << "1 ";
		
		for(int c=0; c<3; c++)
			outputstream << particles[i].x[c] << " ";
		
		outputstream << endl;
	}
}

// Constructor
Simulation::Simulation(string filename): particles(), interaction_matrix()
{
	/* Allocate and initialized particles given initial conditions */
	particles = _createParticlesFromFile(filename);
	
	/* Allocate Interaction matrix */
	interaction_matrix = InteractionMatrix( particles.getNumberOfParticles() );
}

// Actual simulation
void Simulation::run(double time_end, double dt)
{	
	/* Compute number of timesteps */
	const size_t nsteps = (size_t)(time_end/dt);
	
	/* Perform simulation repeating iteration matrix computation 
	   and particles' status update
	 */
	for(size_t step=0; step<nsteps; step++)
	{
		_computeInteractionMatrix();
		_updateParticleVector(dt);
		
		/* Dump output file every 20 steps */
		if (step % 20 == 0) 
		{
			_dumpToFile(step, (step+1)*dt);
			
			cout << "Completion: " << setprecision(2) << step*100./nsteps << "%" <<endl;
		}
	}
}

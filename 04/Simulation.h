/*
 *  Simulation.h
 *  Exercise 8
 *
 *  Created by Diego Rossinelli on 3/28/11.
 *  Copyright 2010 ETH Zurich. All rights reserved.
 *
 */

#pragma once

#include <iomanip>
#include <fstream>
using namespace std;

#include "ParticleVector.h"
#include "ProcessingPipe.h"
#include "DiatomicInfo.h"

#include "Processing_EulerTimestep.h"
#include "Processing_VelVerletTimestep.h"
#include "Processing_LJ.h"
#include "Processing_Spring.h"
#include "Processing_Project.h"
#include "Processing_Electrostatics.h"
#include "FieldViewer.h"
#include "screenshot.h"
#include "Kernels.h"

class Simulation
{
	const double dt, time_end, domain_size;
	ParticleVector particles;
	ProcessingPipe processing_pipe;
	vector< DiatomicInfo > diatomic_pairs;
	
	void _initialconditions(const char * filename);
	void _file_dump(size_t stepid, double t);
	
	//RENDERING
	FieldViewer field_viewer; // used to visualize the electrostatic potential
	void _paintSphere(double x, double y, double r, double g, double b);
	void _drawParticles(int iframe);
	
public:
	
	Simulation(const double time_end, const double dt, const char * filename, const int cells1D, bool bElectrostatics);
	
	void run();	
};

//IMPLEMENTATION
Simulation::Simulation(const double time_end, const double dt, const char * filename, const int cells1D, bool bElectrostatics): 
time_end(time_end), dt(dt), domain_size(2)
{
	const double domain_start = -1;
	const double eps = 0.01;
	const double rmin = domain_size/40.0;
	
	_initialconditions(filename);
	
	ProcessingPipe * force_calculation = new ProcessingPipe;
	
	//add dynamically the different forces calculators
	{
        //QUESTION 2
        // START FILLING HERE
	  
	  force_calculation->add( new Processing_LJ(cells1D, domain_size, rmin, eps, domain_start) ) ;
	  force_calculation->add( new Processing_Electrostatics<MP4>(field_viewer) );
	  force_calculation->add( new Processing_Spring(diatomic_pairs, domain_size) );

        // STOP FILLING HERE
	}
	
	Processing * verlet = new Processing_VelVerletTimestep(dt);
	
	processing_pipe.add( verlet );
	processing_pipe.add( force_calculation );
	processing_pipe.add( verlet );
	processing_pipe.add( new Processing_Projection(domain_size, domain_start) );	
	
	(*force_calculation)(particles);
}

void Simulation::run()
{	
	const size_t nsteps = (size_t)(time_end/dt);
	
	for(size_t step=0; step<nsteps; step++)
	{
		processing_pipe(particles);
		
		if (step % 50 == 0)
		{
			_file_dump(step, (step+1)*dt);
			_drawParticles(step/50);
			
			cout << "Completion: " << setprecision(2) << step*100./nsteps << "%" << endl;
		}
	}  
}

void Simulation::_initialconditions(const char * filename)
{
	ifstream inputstream(filename);
	
	if (!inputstream.good())
	{
		cout << "I tried to load this file: " << filename << ", but I could not open it." << endl;
		exit(-1);
	}
	
	vector<double> atoms;
	
	//read monoatomic data
	{
		int N = -1;
		inputstream >> N;
		assert(N >= 0);
		
		for(int i=0; i<N; i++)
		{					
			double x[2];
			
			inputstream >> x[0] >> x[1];
			
			atoms.insert(atoms.end(), x, x+2);			
		}
	}
	
	//read diatomic data, fill diatomic_pairs 
	{
		int N = -1;
		inputstream >> N;
		assert(N >= 0);
		
		for(int i=0; i<N; i++)
		{
			double x[4];
			
			inputstream >> x[0] >> x[1] >> x[2] >> x[3] ;
			
			atoms.insert(atoms.end(), x, x+4);
			
			const int id1 = atoms.size()/2-2;
			const int id2 = atoms.size()/2-1;
			
			diatomic_pairs.push_back(DiatomicInfo(x[0], x[1], x[2], x[3], id1, id2));
		}
	}
	
	//fill particle vector								 
	{
		const int NPARTICLES = atoms.size()/2;
		particles.setup(NPARTICLES, atoms.size()/2 - diatomic_pairs.size()*2);
		
		double * const xs = particles.getX();
		double * const ys = particles.getY();
		
		assert(NPARTICLES*2 == (int)atoms.size());
		for(int p=0; p<NPARTICLES; p++)
		{
			xs[p] = atoms[2*p + 0];
			ys[p] = atoms[2*p + 1];
		}
	}	
}

void Simulation::_file_dump(size_t stepid, double t)
{
	ofstream outputstream("output.xyz", stepid == 0 ? ios::out : ios::app);
	
	const int N = particles.size();
	
	outputstream << N << endl << "Atoms" << endl;
	
	const double * const xs = particles.getX();
	const double * const ys = particles.getY();
	
	for(int i=0; i<N; i++)
		outputstream << (particles.is_tagged(i) ? 2 : 1) << " " << xs[i] << " " << ys[i] << endl;
}

//RENDERING METHODS
void Simulation::_paintSphere(double x, double y, double r, double g, double b)
{
	GLfloat lightColor[] = {r*1.2,g*1.2,b*1.2,1};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	
	glColor3f(r,g,b);
	glPushMatrix();
	glTranslated(x,y,0);
	glutSolidSphere(0.02, 20,20);
	glPopMatrix();
}

void Simulation::_drawParticles(int iframe)
{
	const double * const xs = particles.getX();
	const double * const ys = particles.getY();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	field_viewer.view();
	
	glColor3f(1,1,1);
	glBegin(GL_LINES);
	
	for(int i=0; i<diatomic_pairs.size(); i++)
	{
		const int p1 = diatomic_pairs[i].particle_id1;
		const int p2 = diatomic_pairs[i].particle_id2;
		
		const double _rx = xs[p2] - xs[p1];
		const double _ry = ys[p2] - ys[p1];
		
		const double rx = _rx - domain_size*floorf(0.5f+_rx*(1.f/domain_size));
		const double ry = _ry - domain_size*floorf(0.5f+_ry*(1.f/domain_size));
		
		glVertex2f(xs[p1], ys[p1]);
		glVertex2f(xs[p1]+rx, ys[p1]+ry);
	}
	glEnd();
	
	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_LIGHTING);
	const int n = particles.size();
	for(int i=0; i<n; i++)
	{
		_paintSphere(xs[i], ys[i], 0,0.1,1);
	}
	
	for(vector< DiatomicInfo >::iterator it = diatomic_pairs.begin(); it!=diatomic_pairs.end(); ++it)
	{
		_paintSphere(xs[it->particle_id1], ys[it->particle_id1], 1,0.1,0.1);
		_paintSphere(xs[it->particle_id2], ys[it->particle_id2], 1,0.1,0.1);
	}
	
	glPopAttrib();
	
	glutSwapBuffers();
	
	char frame_name[300];
	sprintf(frame_name, "../output_images/md.%04d.tga", iframe);
	screenshot(frame_name);
}

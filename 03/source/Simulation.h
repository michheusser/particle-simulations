/*
 *  Simulation.h
 *  Exercise 3
 *
 *  Created by Florian Milde on 3/16/11.
 *  Copyright 2011 ETH Zurich. All rights reserved.
 *
 */

#pragma once

#include <vector>

using namespace std;

#include "Force.h"
#include "Particle.h"
#include "TwinParticle.h"
#include "ParticleVector.h"

class Simulation
{
    ParticleVector particles;
    
    void _computeForces();
    void _updateParticleVector(double dt);
    void _dumpToFile(size_t stepid, double t);
    
public:
    
    Simulation(const char * const filename);
    
    void run(double time_end, double dt);
};

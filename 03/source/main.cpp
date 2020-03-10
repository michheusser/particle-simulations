/*
 *  main.cpp
 *  Exercise 3
 *
 *  Created by Diego Rossinelli on 3/1/11.
 *  Copyright 2011 ETH Zurich. All rights reserved.
 *
 */

#include <iostream>
#include <stdlib.h>

using namespace std;

#include "Simulation.h"

int main (int argc, char * const argv[]) 
{
    if (argc != 4)
    {
		cout << "arguments: <path-to-particle-file> <end-time> <timestep>" << endl;
		exit(-1);
    }
    
    const char * const FILEPATH = argv[1];
    const float TEND = atof(argv[2]);
    const float DT = atof(argv[3]); 
	
	assert(TEND > 0);
	assert(DT > 0);
    
    cout << "Hello!\n" << endl;
    cout << "File is " << FILEPATH << endl;
    cout << "End-time is " << TEND << endl;
    cout << "Time step is " << DT << endl;
	
    Simulation sim(FILEPATH);
    sim.run(TEND, DT);
	
    return 0;
}
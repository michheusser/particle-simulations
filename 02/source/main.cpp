#include <assert.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

#include "Simulation.h"

int main (int argc, char * const argv[]) 
{
	// Parse command line
	if (argc != 4)
	{
		cout << "arguments: <path-to-particle-file> <end-time> <timestep>" << endl;
		exit(-1);
	}
		
	const string FILEPATH = string(argv[1]);
	const float TEND = atof(argv[2]);
	const float DT = atof(argv[3]); 
	
	cout << "Hello!\n" << endl;
	cout << "File is " << FILEPATH.c_str() << endl;
	cout << "End-time is " << TEND << endl;
	cout << "Time step is " << DT << endl;
	
	// Run the simulation
	Simulation sim(FILEPATH);
	sim.run(TEND, DT);
	
    return 0;
}

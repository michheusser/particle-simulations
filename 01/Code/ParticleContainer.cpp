#include <fstream>
#include <string>
#include <assert.h>

#include "ParticleContainer.h"

using namespace std;

void ParticleContainer::_dispose()
{
	//INSERT HERE: if array is allocated, deallocate it
  
  if(array != NULL){
    array = NULL;
  }

    //STOP HERE
}

int ParticleContainer::_lines(const char * const filename)
{	
	ifstream filestream(filename);
	
	int c = 0;
	string line;
	
	//count the lines
	while( getline(filestream, line) ) 
		c++;
	
	return c;
}

//set nparticles to zero, array to NULL
ParticleContainer::ParticleContainer()
{
//INSERT HERE: 

  array = NULL;
  nparticles=0;

//STOP HERE

}

//deconstructor, deallocate possible allocations
ParticleContainer::~ParticleContainer()
{
	_dispose();
}

void ParticleContainer::Load(const char * const filename)
{
	//INSERT HERE: if array is allocated, deallocate it
         _dispose();

	//STOP HERE
    
	//INSERT HERE: retrieve number of particles from the file
         nparticles = _lines(filename);
        //STOP HERE
    
	//INSERT HERE: allocation of the particles 
	 array = new Particle[nparticles];
        //STOP HERE
    
	//INSERT HERE: create file stream
	 ifstream file;
	 
    //STOP HERE
	
	//INSERT HERE: parse particles' attributes
	 file.open(filename);
	 for(int i=0; i<nparticles; i++){
	   file >> array[i].x;
	   file >> array[i].y;
	   file >> array[i].u;
	   file >> array[i].v;
	   file >> array[i].omega;
	 }


    //STOP HERE
}

const Particle * const ParticleContainer::GetParticles() const
{
	//INSERT HERE: checks is particle data is allocated and 
    //              return the pointer to the data 

  if (array!=NULL){
    return array;
  }

    //STOP HERE
}

int ParticleContainer::GetNumberOfParticles() const
{
	//INSERT HERE: return the number of allocated particles 

  return nparticles;

    //STOP HERE
}


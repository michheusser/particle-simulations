#include "InteractionMatrix.h"

// Default constructor
/* pointer data to NULL and dimension set to zero */
InteractionMatrix::InteractionMatrix(): data(NULL), n(0)
{
}

// Constructor
InteractionMatrix::InteractionMatrix(int number_of_particles):
n(number_of_particles), data(NULL)
{
	// FILL HERE: allocate matrix memory and check for 
  
  assert(n>0);
  data = new Force[n*n];

  
	//            strictly positive matrix dimension 
}

// Destructor
InteractionMatrix::~InteractionMatrix()
{
	/* Deallocate memory */
	if (data!=NULL) delete [] data;
}

// Overloaded operator
InteractionMatrix& InteractionMatrix::operator=(const InteractionMatrix& c)
{
	/* Deallocate data */
	if (data != NULL) delete [] data;
	
	/* Re-allocate data*/
	n = c.n;
	data = new Force[n*n];
	
	/* Copy data */
	memcpy(data, c.data, sizeof(Force)*n*n);
	
	/* Return pointer to this object*/
	return *this;
}

// Accessor
Force& InteractionMatrix::getForce(int index_particle_1, int index_particle_2)
{
	/* Check matrix entry validity */
	assert(index_particle_1>=0 && index_particle_1<n);
	assert(index_particle_2>=0 && index_particle_2<n);
	
	/* return corresponing data value */
	return data[index_particle_1+index_particle_2*n];
}

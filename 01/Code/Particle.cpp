#include <math.h>
#include <algorithm>

using namespace std;

#include "Particle.h"

//constructor, initialize all attributes to zero
Particle::Particle(): x(0), y(0), u(0), v(0), omega(0)
{
}

//given the particles attributes, compute r,g and b
//note that r,g,b must be in [0,1]
void Particle::ComputeColor(double& r, double& g, double& b) const
{
    r = g = b = 1.;
    
	//INSERT HERE: Compute r, g and b as a function of the particle's attributes
    double VortSkaliert= omega*400; //Werte von Omega sind der ordnung 10^(-3). Gesucht ist aber eine Zahl zwischen 0 und 4.

    if(VortSkaliert > 4){
      VortSkaliert=4;
    }
    if(VortSkaliert>3){
      r=1;
      g=(1-(VortSkaliert-3));
      b=0;
    }else if(VortSkaliert>2){
      r=VortSkaliert-2;
    g=1;
    b=0;
    }else if(VortSkaliert>1){
      r=0;
      g=1;
      b=1-(VortSkaliert-1);
    }else{
      r=0;
      g=VortSkaliert;
      b=1;
    }


    //STOP HERE
}

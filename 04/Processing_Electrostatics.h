#include "Grid.h"
#include "PoissonSolverFFT.h"
#include "FieldViewer.h"

template<typename ParticleKernel>
class Processing_Electrostatics: public Processing, Viewer 
{	
	bool bTextureAllocated;
	//stuff related with the Particle Kernel (used in particles-grid and grid-particles communication)
	static const int KS = ParticleKernel::support;
	static const int KSS = ParticleKernel::support_start;
	static const int KSE = ParticleKernel::support_end;	
	
	//stuff for poisson solver
	static const int NPOINTS = 128;
	Grid2D<double, NPOINTS, NPOINTS > charge_density; //right hand side
	Grid2D<double, NPOINTS, NPOINTS > electric_potential; // solution of the poisson equation
	Grid2D<double, NPOINTS, NPOINTS, 2>  electric_field; // E = -grad(PHI), PHI is the electric_potential
	
	//the fftw-based poisson solver
	PoissonSolverFFT< Grid2D<double, NPOINTS, NPOINTS> > * poisson_solver;
	
public:

	//constructor: 
	// - allocate the poisson solver
	// - register the callback function for visualizing the electric_potential
	// - allocate texture for that visualization
	Processing_Electrostatics(FieldViewer& field_viewer):
		charge_density(), electric_potential(), electric_field(), poisson_solver(NULL), bTextureAllocated(false)
	{
		poisson_solver = new PoissonSolverFFT< Grid2D<double, NPOINTS, NPOINTS> > (charge_density, electric_potential);
		
		field_viewer.register_view(this);
	}
	
	//deconstructor: deallocate the poisson solver
	~Processing_Electrostatics()
	{
		delete poisson_solver;
	}
	
	//the MOST important part: how do we solve for the electrostatics effect??
	//1. Transfer charges from particles to charge_density (P2M)
	//2. Find the electric potential, by solving the Poisson equation on the grid
	//3. Compute the electric field, which is the gradient of the electric potential
	//4. Reconstruct/sample the electric field at the particle locations (M2P)
	void operator()(ParticleVector & p)
	{
		if (!bTextureAllocated)
		{
			allocate_texture(NPOINTS, NPOINTS);
			bTextureAllocated = true;
		}
		
		const double charge_value = 2.0;
		
		//1.
		_p2m_density(p, charge_value);
		
		//2.-3.
		_compute_electric_field();
		
		//4.
		_m2p_electric_field(p, charge_value);
	}
	
private:
	
	//transfer the charges from the particles "p" to the grid "charge_density". 
	//we assume that all the particles have a constant charge of value "charge_value"
	//this is performed by evaluating ParticleKernel::eval(.)
	void _p2m_density(const ParticleVector & p, const double charge_value)
	{
		const double * const xs = p.getX();
		const double * const ys = p.getY();
		
		charge_density.clear();
		
		const double h = 2./NPOINTS;
		const double invh = 1/h;
		const int n = p.size();
		
		const double epsilon_0 = 8.854187817e-3; //in Faraday/nanometers;
		
		for(int i=0; i<n; i++)
		{			
			const double xp = (xs[i] - (2*floor((xs[i]+1)/2) - 1))*invh;
			const double yp = (ys[i] - (2*floor((ys[i]+1)/2) - 1))*invh;
			
			const int x = (int)floor(xp);
			const int y = (int)floor(yp);
			
			assert(x>=0 && x<NPOINTS);
			assert(y>=0 && y<NPOINTS);			
			
			double wx[KS];

            //QUESTION 1: compute x-kernel weights P2M, using Kernel Support KS
            // START FILLING HERE
            //
			for(int j=0; j<KS; j++)
			  wx[j] = ParticleKernel::eval(xp - x - KSS - j);
            // STOP FILLING HERE
                
			double wy[KS];
            //QUESTION 1: compute y-kernel weights P2M, using Kernel Support KS
            // START FILLING HERE
			for(int j=0; j<KS; j++)
			  wy[j] = ParticleKernel::eval(yp - y - KSS - j);
			

            // STOP FILLING HERE
            
			for(int dy=KSS; dy<KSE; dy++)
			for(int dx=KSS; dx<KSE; dx++)
			{
				const int gx = (x + dx + NPOINTS) % NPOINTS;
				const int gy = (y + dy + NPOINTS) % NPOINTS;
                
                /*
                 QUESTION 1: compute charge density field, given particle locations.
                 P2M is performed as a tensorial product of x- and y-weights times the electrical charge
                 divided by the permittivity: -wx[...]*wy[...]*charge_value*(1./epsilon_0)
                 */
                // START FILLING HERE
		charge_density(gx, gy) += - wx[dx-KSS]*wy[dy-KSS]*charge_value*(1./epsilon_0);
		//
                // STOP FILLING HERE
			}
		}
	}
	
	//solve the poisson equation by using "poisson_solver"
	//compute "electric_field" by taking the grad of "electric_potential"
	void _compute_electric_field()
	{
		const double h = 2./NPOINTS;
		const double factor = 0.5/h;
		
        //QUESTION 1: call Poisson sover
        // START FILLING HERE
        poisson_solver->solve();
        // STOP FILLING HERE
        
		for(int iy=0; iy<NPOINTS; iy++)
			for(int ix=0; ix<NPOINTS; ix++)
			{
				const int ixm = (ix-1+NPOINTS) % NPOINTS;
				const int iym = (iy-1+NPOINTS) % NPOINTS;
				const int ixp = (ix+1+NPOINTS) % NPOINTS;
				const int iyp = (iy+1+NPOINTS) % NPOINTS;			
				
                //QUESTION 1: compute electric field as: -1*gradient(electric_potential)
                // START FILLING HERE
                electric_field(ix, iy,0) = -(electric_potential(ixp, iy) - electric_potential(ixm, iy))*factor;
		electric_field(ix, iy,1) = -(electric_potential(ix, iyp) - electric_potential(ix, iym))*factor;
                // STOP FILLING HERE
			}
	}
	
	//sample the electric field "electric_field" at the particle locations of "p", assuming that all the particles
	//have the same charge "charge_value".
	//this is performed by evaluating ParticleKernel::eval(.)
	void _m2p_electric_field(ParticleVector & p, const double charge_value) 
	{
		const double * const xs = p.getX();
		const double * const ys = p.getY();
		double * const axs = p.getAX();
		double * const ays = p.getAY();

		const double h = 2./(NPOINTS);
		const double invH = 1./h;
		
		const int n = p.size();
		
		for(int i=0; i<n; i++)
		{			
			const double xp = (xs[i] - (2*floor((xs[i]+1)/2) - 1))*invH;
			const double yp = (ys[i] - (2*floor((ys[i]+1)/2) - 1))*invH;
			
			const int x = (int)floor(xp);
			const int y = (int)floor(yp);
			
			assert(x>=0 && x<NPOINTS);
			assert(y>=0 && y<NPOINTS);		
			
			double wx[KS];
            //QUESTION 1: compute interpolating x-weights
            // START FILLING HERE
            for(int j=0; j<KS; j++)
	      wx[j] = ParticleKernel::eval(xp - x - KSS - j);
            // STOP FILLING HERE
            
			double wy[KS];
            //QUESTION 1: compute interpolating y-weights
            // START FILLING HERE
            //
	     for(int j=0; j<KS; j++)
	       wy[j] = ParticleKernel::eval(yp - y - KSS - j);


            // STOP FILLING HERE
            
			for(int dy=KSS; dy<KSE; dy++)
				for(int dx=KSS; dx<KSE; dx++)
				{
					const int gx = (x + dx + NPOINTS) % NPOINTS;
					const int gy = (y + dy + NPOINTS) % NPOINTS;
                    
                    /*
                     QUESTION 1: compute the x-component of the force acting on the particle 
                     due to the electric field, interpolating 
                     from the electric field to the particle location
                     force = charge_value*electric_field(...)*wx[...]*wy[...]
                     */
                    // START FILLING HERE
					*axs += charge_value*electric_field(gx, gy, 0)*wx[dx-KSS]*wy[dy-KSS];
                    // STOP FILLING HERE
				}
			
			for(int dy=KSS; dy<KSE; dy++)
				for(int dx=KSS; dx<KSE; dx++)
				{
					const int gx = (x + dx + NPOINTS) % NPOINTS;
					const int gy = (y + dy + NPOINTS) % NPOINTS;
					
                    /*
                     QUESTION 1: compute the y-component of the force acting on the particle 
                     due to the electric field, interpolating 
                     from the electric field to the particle location
                     force = charge_value*electric_field(...)*wx[...]*wy[...]
                     */
                    // START FILLING HERE
					*axs += charge_value*electric_field(gx, gy, 1)*wx[dx-KSS]*wy[dy-KSS];
                    // STOP FILLING HERE
				}
		}
	}
	
public:
	
	//DONT LOOK AT ME - i barely exist
	void view()
	{
		//fill texture data
		{
			const double maxval = +0.02;
			const double minval = 0;
			
			for(int iy=0; iy<NPOINTS; iy++)
				for(int ix=0; ix<NPOINTS; ix++)
					texture_data[ix + iy*NPOINTS] = min(1., max(0., (electric_potential(ix, iy)-minval)/(maxval - minval)));
			
			upload_texture();
		}
			
		//draw texture
		{
			glPushAttrib(GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);
			glEnable(GL_TEXTURE_2D);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBindTexture(GL_TEXTURE_2D, texture_handle);
			
			glColor3f(0.77,0.98,0.28);
			const double half_h = 0.5*2./NPOINTS;
			glBegin(GL_QUADS);
				glTexCoord2f(0, 0);
				glVertex2f(-1, -1);
				
				glTexCoord2f(1+half_h, 0);
				glVertex2f(1, -1);
				
				glTexCoord2f(1+half_h, 1+half_h);
				glVertex2f(1, 1);
				
				glTexCoord2f(0, 1+half_h);
				glVertex2f(-1, 1);
			glEnd();
			
			glPopAttrib();
		}
	}
};

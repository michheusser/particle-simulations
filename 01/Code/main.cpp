//standard libraries
#include <assert.h>
#include <math.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

//graphics
#ifndef __APPLE__
#include "GL/glut.h"
#else
#include "GLUT/glut.h"
#endif

//our code
#include "screenshot.h"
#include "Particle.h"
#include "ParticleContainer.h"

//this object will contain all the particles to visualize
ParticleContainer * container = NULL;

void run()
{
	glutPostRedisplay();
}

//this is the function that render the particles
void display()
{
	//let's make sure that the container is allocated
	assert(container != NULL);
	
	//get the data of the particles, and the number of particles
	const Particle * const particles = container->GetParticles();
	const int N = container->GetNumberOfParticles();
	
	//clear the framebuffer, then we paint the particles
	glClear(GL_COLOR_BUFFER_BIT);
		
	glPointSize(2.0);
	
	//loop over all the particles, which are painted as colored points
	glBegin(GL_POINTS);
	for(int i=0; i<N; i++)
	{
		//compute the color of the particle
		double R,G,B;
		particles[i].ComputeColor(R,G,B);
		
		//display the color and the position of the point
		glColor3f(R,G,B);
		glVertex2d(particles[i].x,particles[i].y);
	}
	
	glEnd();
	
	//we are done, let's show the content of the framebuffer
	glutSwapBuffers();	
	
	//save a snapshot
	screenshot("output.tga");
	
	//ciao
	exit(0);
}

//initialization of the graphics system and rendering call
void opengl_setup(int argc, char ** argv)
{
	const double maxWindowSize = 512;
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE );
	glutInitWindowSize(maxWindowSize,maxWindowSize);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Test");
	
	glutDisplayFunc(display);
	glutIdleFunc(run);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
		
	glutMainLoop();
}

//we allocate the container, we read the particle data, 
//then we are ready to render the particles
int main (int argc, char * const argv[]) 
{
	printf("Hello!\n");
	
	const string FILEPATH = string(argv[1]);
	
	cout << "File is " << FILEPATH.c_str() << endl;
	
	container = new ParticleContainer();
	container->Load(FILEPATH.c_str());
	
	opengl_setup(argc, const_cast<char**>(argv));
	
    return 0;
}

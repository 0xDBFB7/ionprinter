#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#define grid_width 1000
#define pixel_size 10
#define grid_step_size 1
#define num_particles 2
long double timestep = 0.00001; //seconds

long double k_constant = 8990000000.0;
long double particle_locations_x[200] = {0,grid_width,50.0}; //m
long double particle_locations_y[200] = {grid_width/2,grid_width/2,50};//m
long double particle_locations_z[200] = {grid_width/2,grid_width/2,50}; //m
long double particle_charge[200] = {0.00000000000000000016021766,0.00000000000000000016021766};
long double particle_mass[200] = {0.0000000000000000000000000016726218,0.0000000000000000000000000016726218};

int button = 0;
int mouse_x = 0;
int mouse_y = 0;

int rotation_x = 45;
int rotation_y = 45;

float shading_multiplier = 1.0;
float shading_threshold = 2.0;

char key;
int shot_counter = 0;
float zoom = -grid_width;

void update() {
   glutPostRedisplay();
}

void initGL() {
   glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background color to black and opaque
   glClearDepth(1.0f);                   // Set background depth to farthest
   glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
   glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
   glShadeModel(GL_SMOOTH);   // Enable smooth shading
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

void mouse( int x, int y){
	mouse_x = x;
	mouse_y = y;
}

void keyboard(unsigned char pressed, int x, int y){
	key = pressed;
	mouse_x = x;
	mouse_y = y;
}

void TakeScreenshot(float time)
{
	unsigned char *buffer;
	char filename[200];
	int w = 1920;
	int h = 1080;
	int buf_size = 18 + (w * h * 3);
	int i;
	unsigned char temp;
	FILE *out_file;
	char filename2[200];
	sprintf(filename2,"video/%i.tga",shot_counter);
	if (!(out_file = fopen(filename2, "wb")))
	{
		return;
	}

	// allocate mem to read from frame buf
	if (!(buffer = (unsigned char *) calloc(1, buf_size)))
	{
		return;
	}

	// set header info
	buffer[2] = 2;	// uncompressed
	buffer[12] = w & 255;
	buffer[13] = w >> 8;
	buffer[14] = h & 255;
	buffer[15] = h >> 8;
	buffer[16] = 24;	// 24 bits per pix

	// read frame buf
	glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, buffer + 18);

	// RGB to BGR
	for (i = 18; i < buf_size; i += 3)
	{
		temp = buffer[i];
		buffer[i] = buffer[i + 2];
		buffer[i + 2] = temp;
	}
	// write header + color buf to file
	fwrite(buffer, sizeof(unsigned char), buf_size, out_file);
	// cleanup
	fclose(out_file);
	free(buffer);
	shot_counter++;
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glEnable(GL_MULTISAMPLE);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("Solver");
    glViewport(0, 0, 1920, 1080);
    initGL();
    float t = 0;

    while(1){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glutPassiveMotionFunc(mouse);
        glMatrixMode(GL_PROJECTION);
        glutKeyboardFunc(keyboard);
        glLoadIdentity();
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        double aspect = (double)viewport[2] / (double)viewport[3];
        gluPerspective(60, aspect, 1, 3000);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();


  	    glTranslatef(0,0, zoom);
  	    glRotatef(rotation_y, 0, 0.1, 0);
  	    glRotatef(rotation_x, 0.1, 0, 0);
  	    glTranslatef(-grid_width/2,-grid_width/2, -grid_width/2);
        for(int particle = 0;particle<num_particles;particle++){
          long double dx = 0;
          long double dy = 0;
          long double dz = 0;
          long double accel_x = 0;
          long double accel_y = 0;
          long double accel_z = 0;

          //Coloumb's law solver, sum force over all particles
          printf("Particle: %i\n",particle);
          for(int second_particle = 0;second_particle<num_particles;second_particle++){
            if(!(second_particle == particle)){
              long double magnitude = (k_constant*particle_charge[particle]*particle_charge[second_particle]);
              printf("Magnitude: %.30Lf\n",magnitude);
              printf("Distance X: %.30Lf\n",powl((particle_locations_x[particle]-particle_locations_x[second_particle]),2.0));
              long double force_x = 0;
              long double force_y = 0;
              long double force_z = 0;
              force_x += (magnitude/powl((particle_locations_x[particle]-particle_locations_x[second_particle]),2.0));
              force_y += (magnitude/powl((particle_locations_y[particle]-particle_locations_y[second_particle]),2.0));
              force_z += (magnitude/powl((particle_locations_z[particle]-particle_locations_z[second_particle]),2.0));
              printf("Force X: %.30Lf\n",force_x);
              printf("Force Y: %.30Lf\n",force_y);
              printf("Force Z: %.30Lf\n",force_z);
              accel_x = force_x/particle_mass[particle];
              accel_y = force_y/particle_mass[particle];
              accel_z = force_z/particle_mass[particle];
              printf("Accel X: %.30Lf\n",accel_x);
              printf("Accel Y: %.30Lf\n",accel_y);
              printf("Accel Z: %.30Lf\n",accel_z);
            }
          }

          // dx = accel_x*timestep;
          // dy = accel_y*timestep;
          // dz = accel_z*timestep;
          // printf("Dx: %.30Lf\n",dx);
          // printf("Dy: %.30Lf\n",dy);
          // printf("Dz: %.30Lf\n",dz);
          // particle_locations_x[particle] += dx;
          // particle_locations_y[particle] += dy;
          // particle_locations_z[particle] += dz;
          // printf("X: %.30Lf\n",particle_locations_x[particle]);
          // printf("Y: %.30Lf\n",particle_locations_y[particle]);
          // printf("Z: %.30Lf\n\n",particle_locations_z[particle]);
          // glPushMatrix();
          // glTranslatef(particle_locations_x[particle]*pixel_size,particle_locations_y[particle]*pixel_size,particle_locations_z[particle]*pixel_size);
          // glColor3f(1,0,0);
          // glutSolidCube(pixel_size);
          // glPopMatrix();
        }

        // glutSwapBuffers();
        // update(0);
        // TakeScreenshot(t);
        // t+=timestep;
        // glPopMatrix();
        // glutMainLoopEvent();
        }
}

//		for (int i = 0; i < num_waves; i++)
	    // {
	    // 	wave_locations_x[i] = (rand()/(float)RAND_MAX)*grid_width;
	    //     wave_locations_y[i] = (rand()/(float)RAND_MAX)*grid_width;
	    // }

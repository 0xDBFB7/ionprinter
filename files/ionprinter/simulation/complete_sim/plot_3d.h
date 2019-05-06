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
#define pixel_size 1
#define num_waves 2
#define damping_multiplier 0.0000001
#define amplitude_multiplier 1
#define timestep .001
#define wave_speed 1000
#define grid_step_size 1

float wave_locations_x[200] = {0,grid_width,50};
float wave_locations_y[200] = {grid_width/2,grid_width/2,50};
float wave_locations_z[200] = {grid_width/2,grid_width/2,50};
float wave_period[num_waves] = {0.1,0.1,1};

float wave_frequencies[200] = {600,900,500};
float wave_phases[200] = {0,0,0};

int button = 0;
int mouse_x = 0;
int mouse_y = 0;

int rotation_x = 45;
int rotation_y = 45;

float shading_multiplier = 1.0;
float shading_threshold = 2.0;

float slice_width = 1000;
float slice_z = 0;
float slice_z_dx = 0;
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
    glutCreateWindow("Genetic");
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
        if(key == 'w'){
        	rotation_x += 30;
        }
        if(key == 's'){
        	rotation_x -= 30;
        }
        if(key == 'a'){
        	rotation_y += 30;
        }
        	if(key == 'd'){
        	rotation_y -= 30;
        }
        if(key == 'd'){
        	rotation_y -= 30;
        }
        if(key == '.'){
        	t+=0.1;
        }
        if(key == ','){
        	t-=0.1;
        }
        if(key == 'm'){
        	shading_multiplier+=15;
        }
        if(key == 'n'){
        	shading_multiplier-=15;
        }
        // if(key == 'x'){
        // 	slice_width+=5;
        // }
        // if(key == 'z'){
        // 	slice_z_dx += 10;
        // }
        if(key == 'g'){
        	zoom+=50;
        }
        if(key == 'h'){
        	zoom -= 50;
        }
    		key = '`';
    		slice_z = slice_z+slice_z_dx;
    		if(slice_z+slice_width > grid_width){
    			slice_z = 0;
    		}
        rotation_y += 2;
  	    glTranslatef(0,0, zoom);
  	    glRotatef(rotation_y, 0, 0.1, 0);
  	    glRotatef(rotation_x, 0.1, 0, 0);
  	    glTranslatef(-grid_width/2,-grid_width/2, -grid_width/2);
        for(int x=0;x<grid_width;x+=grid_step_size){//y
            for(int y=slice_z;y<slice_z+slice_width;y+=grid_step_size){//x
                for(int z=0;z<slice_z+slice_width;z+=grid_step_size){//x
                    float amplitude = 0;
                    for(int wave = 0;wave<num_waves;wave++){
                      float distance = pow(wave_locations_x[wave]-x,2)+pow(wave_locations_y[wave]-y,2)+pow(wave_locations_z[wave]-z,2);
                      if(distance < pow(t*10.0,2)*wave_speed){
                      //amplitude += sin(t*wave_frequencies[wave]+wave_phases[wave])*sin(sqrtf(distance)*wave_period[wave])*(1/(damping_multiplier*distance)*amplitude_multiplier); //* for some cool patterns
                        amplitude += sin((t-(sqrtf(distance)/wave_speed))*(wave_frequencies[wave]/(3.14159*2.0)))*(1/(damping_multiplier*distance)*amplitude_multiplier);
                      }
                }
                if(fabs(amplitude) > shading_threshold){
                  if(amplitude < shading_threshold-10.0){
                    glPushMatrix();
                    glTranslatef(x*pixel_size,y*pixel_size,z*pixel_size);
                    glColor3f((amplitude/shading_multiplier),x/(float)grid_width,y/(float)grid_width);
                    glutSolidCube(pixel_size);
                    glPopMatrix();
                  }
                  else{
                    glPushMatrix();
                    glTranslatef(x*pixel_size,y*pixel_size,z*pixel_size);
                    glColor3f((amplitude/shading_multiplier),x/(float)grid_width,y/(float)grid_width);
                    glutSolidCube(pixel_size);
                    glPopMatrix();
                  }
                }
              }
            }
        }
        glutSwapBuffers();
        update(0);
        TakeScreenshot(t);
        t+=timestep;
        glPopMatrix();
        glutMainLoopEvent();
        }
}

//		for (int i = 0; i < num_waves; i++)
	    // {
	    // 	wave_locations_x[i] = (rand()/(float)RAND_MAX)*grid_width;
	    //     wave_locations_y[i] = (rand()/(float)RAND_MAX)*grid_width;
	    // }

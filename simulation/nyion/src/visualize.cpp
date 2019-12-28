#include "nyion.hpp"
#include "visualize.hpp"

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/glx.h>

#include "TGraph.h"


float camera_x = 0.0;
float camera_y = 0.0;
float camera_z = 0.0;

float camera_target_x = 0.0;
float camera_target_y = 0.0;
float camera_target_z = 0.0;

float camera_angle_x = 0.0;
float camera_angle_y = 1.0;

GLXDrawable mesh_window;
GLXDrawable graph_window;
GLXContext glContext;
Display * display;
XVisualInfo* vi;

void keyboard_handler(unsigned char key,__attribute__((unused)) int x,__attribute__((unused)) int y){
  if(key == '2') camera_angle_z = 10;
  if(key == '8') camera_target_y = -10;
  if(key == '4') camera_target_z = 10;
  if(key == '6') opengl_angle_y = -10;
  if(key == '+') opengl_zoom = -10;
  if(key == '-') opengl_zoom = +10;
  if(key == '0'){
      opengl_delta_angle_x -= opengl_current_angle_x;
      opengl_delta_angle_y -= opengl_current_angle_y;
  }
}

void special_keyboard_handler(int key,__attribute__((unused)) int x,__attribute__((unused)) int y){
  if(key == GLUT_KEY_F6){

  }
}

void initialize_opengl(){

  int sngBuf[] = { GLX_RGBA,
                   GLX_RED_SIZE, 1,
                   GLX_GREEN_SIZE, 1,
                   GLX_BLUE_SIZE, 1,
                   GLX_DEPTH_SIZE, 12,
                   None
  };

  display = XOpenDisplay(0);
  vi = glXChooseVisual(display, DefaultScreen(display), sngBuf);
  glContext = glXCreateContext(display, vi, 0, GL_TRUE);


  int argc = 0;
  char *argv[1];
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  glEnable(GL_MULTISAMPLE);
  glutInitWindowSize(OPENGL_3D_WINDOW_X, OPENGL_3D_WINDOW_Y);
  glutCreateWindow("Nyion Mesh");
  glViewport(0, 0, OPENGL_3D_WINDOW_X, OPENGL_3D_WINDOW_Y);

  glutKeyboardFunc(keyboard_handler);
  glutSpecialFunc(special_keyboard_handler);

  mesh_window = glXGetCurrentDrawable();

  // glutInitWindowSize(OPENGL_GRAPH_WINDOW_X, OPENGL_GRAPH_WINDOW_X);
  // glutCreateWindow("Nyion Graph");
  // glViewport(0, 0, OPENGL_GRAPH_WINDOW_X, OPENGL_GRAPH_WINDOW_Y);

  // glutKeyboardFunc(keyboard_handler);
  // glutSpecialFunc(special_keyboard_handler);

  // graph_window = glXGetCurrentDrawable();
  //
  // std::this_thread::sleep_for(std::chrono::milliseconds(100));
  //
  // opengl_switch_to_mesh_window();


  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  // glEnable(GL_DEPTH_TEST);
  // glDepthFunc(GL_LEQUAL);
  glShadeModel(GL_SMOOTH);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  glEnable (GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* -----------------------------------------------------------------------------
  Determine the longest side of the mesh and configure the frustrum appropriately.
  ----------------------------------------------------------------------------- */

}

void opengl_switch_to_graph_window(){
  glXMakeCurrent(display,graph_window,glContext);
}

void opengl_switch_to_mesh_window(){
  glXMakeCurrent(display,mesh_window,glContext);
}

void opengl_3d_mode(){
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  double aspect = (double)viewport[2] / (double)viewport[3];

  gluPerspective(60, aspect, opengl_current_z_translate-1.5*opengl_z_extent, opengl_current_z_translate+opengl_z_extent); //all parameters must be positive.
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


void opengl_2d_mode(){
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0.0, OPENGL_3D_WINDOW_X, OPENGL_3D_WINDOW_Y, 0.0, -1.0, 10.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


void opengl_clear_screen(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPopMatrix();
}


void opengl_draw_axis_cross(){
  /* -----------------------------------------------------------------------------
  Draws a pretty RGB cross denoting X,Y,Z.
  ----------------------------------------------------------------------------- */

  glColor3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_LINE_STRIP);
  glVertex3f(300,0,0);
  glVertex3f(0,0,0);
  glEnd();

  glColor3f(0.0f, 1.0f, 0.0f);
  glBegin(GL_LINE_STRIP);
  glVertex3f(0,300,0);
  glVertex3f(0,0,0);
  glEnd();

  glColor3f(0.0f, 0.0f, 1.0f);
  glBegin(GL_LINE_STRIP);
  glVertex3f(0,0,300);
  glVertex3f(0,0,0);
  glEnd();
}


// void draw_mesh(std::vector<std::vector<double>>& input_mesh){
//   /* -----------------------------------------------------------------------------
//   Determine min and max for color scaling
//   ----------------------------------------------------------------------------- */
//   double max = *std::max_element(potentials, potentials+(s_x*s_y*s_z));
//   double min = *std::min_element(potentials, potentials+(s_x*s_y*s_z));
//   /* -----------------------------------------------------------------------------
//   Draw cubes
//   ----------------------------------------------------------------------------- */
//   for(int x = 0; x < SIZE_X; x++){
//     for(int y = 0; y < SIZE_X; y++){
//       for(int z = 0; z < SIZE_X; z++){
//
//   glPushMatrix();
//     glTranslatef((r_x*mesh_geometry.root_scale+x*fine_scale)/OPENGL_WORLD_SCALE+(cube_size/2.0),
//                   (r_y*mesh_geometry.root_scale+y*fine_scale)/OPENGL_WORLD_SCALE+(cube_size/2.0),
//                   (r_z*mesh_geometry.root_scale+z*fine_scale)/OPENGL_WORLD_SCALE+(cube_size/2.0));
//
//     if(input_mesh[root_idx][idx(x,y,z,sub_len,sub_len)] > 0){
//       glColor4f((255.0*(input_mesh[root_idx][idx(x,y,z,sub_len,sub_len)]/max)),0,255,
//                                   (input_mesh[root_idx][idx(x,y,z,sub_len,sub_len)]/max));
//     }
//     else{
//       glColor4f(0,(255.0*(input_mesh[root_idx][idx(x,y,z,sub_len,sub_len)]/min)),0,
//                                   (input_mesh[root_idx][idx(x,y,z,sub_len,sub_len)]/min));
//     }
//     glutSolidCube((mesh_geometry.root_scale/sub_len)/OPENGL_WORLD_SCALE);
//   glPopMatrix();
// }


void update_screen(){
  gluLookAt(0,0,0,
            0,0,0,
            0,1,0);

  glutMainLoopEvent();
  glutSwapBuffers();
  glutPostRedisplay();

  // TakeScreenshot(t);
}




//
// void mouse_handler(int x, int y){
//   opengl_angle_x = ((x-mouse_previous_x)/(double)OPENGL_3D_WINDOW_X)*360.0;
//   opengl_angle_y = ((y-mouse_previous_y)/(double)OPENGL_3D_WINDOW_Y)*360.0;
//   mouse_previous_x = x;
//   mouse_previous_y = y;
//
// }




// void TakeScreenshot(double time)
// {
// 	unsigned char *buffer;
// 	char filename[200];
// 	int w = 1920;
// 	int h = 1080;
// 	int buf_size = 18 + (w * h * 3);
// 	int i;
// 	unsigned char temp;
// 	FILE *out_file;
// 	char filename2[200];
// 	sprintf(filename2,"video/%i.tga",shot_counter);
// 	if (!(out_file = fopen(filename2, "wb")))
// 	{
// 		return;
// 	}
//
// 	// allocate mem to read from frame buf
// 	if (!(buffer = (unsigned char *) calloc(1, buf_size)))
// 	{
// 		return;
// 	}
//
// 	// set header info
// 	buffer[2] = 2;	// uncompressed
// 	buffer[12] = w & 255;
// 	buffer[13] = w >> 8;
// 	buffer[14] = h & 255;
// 	buffer[15] = h >> 8;
// 	buffer[16] = 24;	// 24 bits per pix
//
// 	// read frame buf
// 	glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, buffer + 18);
//
// 	// RGB to BGR
// 	for (i = 18; i < buf_size; i += 3)
// 	{
// 		temp = buffer[i];
// 		buffer[i] = buffer[i + 2];
// 		buffer[i + 2] = temp;
// 	}
// 	// write header + color buf to file
// 	fwrite(buffer, sizeof(unsigned char), buf_size, out_file);
// 	// cleanup
// 	fclose(out_file);
// 	free(buffer);
// 	shot_counter++;
// }

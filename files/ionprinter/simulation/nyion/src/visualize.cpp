#include "multiphysics.hpp"
#include "visualize.hpp"
#include <thread>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/glx.h>


float opengl_zoom = 30;
float opengl_z_extent = 10;
float opengl_current_angle_x = 45;
float opengl_current_angle_y = 25.0;
float opengl_current_x_translate = 0.0;
float opengl_current_y_translate = 0.0;
float opengl_current_z_translate = 0;
float opengl_delta_zoom = 0;
float opengl_delta_angle_x = 0;
float opengl_delta_angle_y = 0;
float opengl_delta_x_translate = 0.0;
float opengl_delta_y_translate = 0.0;
float opengl_delta_z_translate = 0.0;

GLXDrawable mesh_window;
GLXDrawable graph_window;
GLXContext glContext;
Display * display;
XVisualInfo* vi;

void keyboard_handler(unsigned char key, int x, int y){
  if(key == '2') opengl_delta_angle_x = 10;
  if(key == '8') opengl_delta_angle_x = -10;
  if(key == '4') opengl_delta_angle_y = 10;
  if(key == '6') opengl_delta_angle_y = -10;
  if(key == '+') opengl_delta_zoom = -10;
  if(key == '-') opengl_delta_zoom = +10;
  if(key == '0'){
      opengl_delta_angle_x -= opengl_current_angle_x;
      opengl_delta_angle_y -= opengl_current_angle_y;
  }
}


void initialize_opengl(root_mesh_geometry mesh_geometry){

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

  mesh_window = glXGetCurrentDrawable();

  glutInitWindowSize(OPENGL_GRAPH_WINDOW_X, OPENGL_GRAPH_WINDOW_X);
  glutCreateWindow("Nyion Graph");
  glViewport(0, 0, OPENGL_GRAPH_WINDOW_X, OPENGL_GRAPH_WINDOW_Y);

  glutKeyboardFunc(keyboard_handler);

  graph_window = glXGetCurrentDrawable();

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  opengl_switch_to_mesh_window();

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glShadeModel(GL_SMOOTH);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  glEnable (GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* -----------------------------------------------------------------------------
  Determine the longest side of the mesh and configure the frustrum appropriately.
  ----------------------------------------------------------------------------- */
  int min_extent = std::min(std::min(mesh_geometry.x_min_bound,mesh_geometry.y_min_bound),mesh_geometry.z_min_bound)/OPENGL_WORLD_SCALE;
  int max_extent = std::max(std::max(mesh_geometry.x_max_bound,mesh_geometry.y_max_bound),mesh_geometry.z_max_bound)/OPENGL_WORLD_SCALE;
  opengl_z_extent = max_extent-min_extent;

  opengl_current_z_translate = 2*opengl_z_extent;
  opengl_current_x_translate = -(((mesh_geometry.x_max_bound-mesh_geometry.x_min_bound)/2)+mesh_geometry.x_min_bound)/OPENGL_WORLD_SCALE;
  opengl_current_y_translate = -(((mesh_geometry.y_max_bound-mesh_geometry.y_min_bound)/2)+mesh_geometry.y_min_bound)/OPENGL_WORLD_SCALE;
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
  // printf("%f,%f\n",opengl_current_z_translate-1.5*opengl_z_extent,opengl_current_z_translate+opengl_z_extent);
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


void draw_geometry_outline(root_mesh_geometry mesh_geometry){
  /* -----------------------------------------------------------------------------
  Draw a wireframe box encompassing mesh_geometry.
  GL_LINE_STRIPs should be drawn between pixels; hence the +0.5.
  ----------------------------------------------------------------------------- */
  glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    glVertex3f(((int)(mesh_geometry.x_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_min_bound/OPENGL_WORLD_SCALE)+0.5));
    glVertex3f(((int)(mesh_geometry.x_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_min_bound/OPENGL_WORLD_SCALE)+0.5));
    glVertex3f(((int)(mesh_geometry.x_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_max_bound/OPENGL_WORLD_SCALE)+0.5));
    glVertex3f(((int)(mesh_geometry.x_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_max_bound/OPENGL_WORLD_SCALE)+0.5));
    glVertex3f(((int)(mesh_geometry.x_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_min_bound/OPENGL_WORLD_SCALE)+0.5));
    glVertex3f(((int)(mesh_geometry.x_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_min_bound/OPENGL_WORLD_SCALE)+0.5));
    glVertex3f(((int)(mesh_geometry.x_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_min_bound/OPENGL_WORLD_SCALE)+0.5));
    glVertex3f(((int)(mesh_geometry.x_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_max_bound/OPENGL_WORLD_SCALE)+0.5));
    glVertex3f(((int)(mesh_geometry.x_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_max_bound/OPENGL_WORLD_SCALE)+0.5));
    glVertex3f(((int)(mesh_geometry.x_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_min_bound/OPENGL_WORLD_SCALE)+0.5));
    glVertex3f(((int)(mesh_geometry.x_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_min_bound/OPENGL_WORLD_SCALE)+0.5));
    glVertex3f(((int)(mesh_geometry.x_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_min_bound/OPENGL_WORLD_SCALE)+0.5));
    glVertex3f(((int)(mesh_geometry.x_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_max_bound/OPENGL_WORLD_SCALE)+0.5));
    glVertex3f(((int)(mesh_geometry.x_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_max_bound/OPENGL_WORLD_SCALE)+0.5));
    glVertex3f(((int)(mesh_geometry.x_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_max_bound/OPENGL_WORLD_SCALE)+0.5));
    glVertex3f(((int)(mesh_geometry.x_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_max_bound/OPENGL_WORLD_SCALE)+0.5));
  glEnd();

}

template<typename T>
void draw_mesh(std::vector<std::vector<T>>& input_mesh, root_mesh_geometry mesh_geometry){
  /* -----------------------------------------------------------------------------
  Determine min and max for color scaling
  ----------------------------------------------------------------------------- */
  float min = mesh_min(input_mesh,mesh_geometry);
  float max = mesh_max(input_mesh,mesh_geometry);
  /* -----------------------------------------------------------------------------
  Draw cubes
  ----------------------------------------------------------------------------- */
  for(int r_x = 0; r_x < mesh_geometry.root_x_len; r_x++){
    for(int r_y = 0; r_y < mesh_geometry.root_y_len; r_y++){
      for(int r_z = 0; r_z < mesh_geometry.root_z_len; r_z++){
        int root_idx = (mesh_geometry.root_x_len*mesh_geometry.root_y_len*r_z) + (mesh_geometry.root_x_len*r_y) + r_x;

        if(input_mesh[root_idx].size()){
          /* -----------------------------------------------------------------------------
          Draw submesh outline
          ----------------------------------------------------------------------------- */
          glPushMatrix();
            int cube_size = mesh_geometry.root_scale/OPENGL_WORLD_SCALE;
            glTranslatef((r_x*mesh_geometry.root_scale)/OPENGL_WORLD_SCALE+(cube_size/2.0),
                          (r_y*mesh_geometry.root_scale)/OPENGL_WORLD_SCALE+(cube_size/2.0),
                          (r_z*mesh_geometry.root_scale)/OPENGL_WORLD_SCALE+(cube_size/2.0));
            glColor4f(1, 0, 0, 1);
            glutWireCube(cube_size);
          glPopMatrix();
          /* -----------------------------------------------------------------------------
          Draw fine mesh cubes
          ----------------------------------------------------------------------------- */
          int sub_len = submesh_side_length(input_mesh[root_idx]);

          cube_size = (mesh_geometry.root_scale/sub_len)/OPENGL_WORLD_SCALE;
          float fine_scale = mesh_geometry.root_scale/sub_len;
          for(int x = 0; x < sub_len; x++){
            for(int y = 0; y < sub_len; y++){
              for(int z = 0; z < sub_len; z++){
                glPushMatrix();

                  glTranslatef((r_x*mesh_geometry.root_scale+x*fine_scale)/OPENGL_WORLD_SCALE+(cube_size/2.0),
                                (r_y*mesh_geometry.root_scale+y*fine_scale)/OPENGL_WORLD_SCALE+(cube_size/2.0),
                                (r_z*mesh_geometry.root_scale+z*fine_scale)/OPENGL_WORLD_SCALE+(cube_size/2.0));
                  if(input_mesh[idx(x,y,z,sub_len,sub_len] > 0){
                    glColor4f(0,(255.0*(max)),0,1.0);
                  }
                  else{
                    glColor4f((255.0*()),0,0,1.0);
                  }
                  glutSolidCube((mesh_geometry.root_scale/sub_len)/OPENGL_WORLD_SCALE);
                glPopMatrix();
              }
            }
          }
        }
        else{
          /* -----------------------------------------------------------------------------
          Draw inactive submesh outlines
          ----------------------------------------------------------------------------- */
          glPushMatrix();
            int cube_size = mesh_geometry.root_scale/OPENGL_WORLD_SCALE;
            glTranslatef((r_x*mesh_geometry.root_scale)/OPENGL_WORLD_SCALE+(cube_size/2.0),
                          (r_y*mesh_geometry.root_scale)/OPENGL_WORLD_SCALE+(cube_size/2.0),
                          (r_z*mesh_geometry.root_scale)/OPENGL_WORLD_SCALE+(cube_size/2.0));
            // glColor3f(255,255,255);
            glColor4f(1, 1, 1, 0.02);
            glutSolidCube(cube_size);
          glPopMatrix();
        }
      }
    }
  }
}
template void draw_mesh(std::vector<std::vector<float>>& input_mesh,root_mesh_geometry mesh_geometry);
template void draw_mesh(std::vector<std::vector<int>>& input_mesh,root_mesh_geometry mesh_geometry);


void opengl_apply_camera_rotation(){
  opengl_current_z_translate += opengl_delta_zoom; //FIXME: no longer required.
  opengl_current_angle_x += opengl_delta_angle_x;
  opengl_current_angle_y += opengl_delta_angle_y;
  /* -----------------------------------------------------------------------------
  Redo model rotation and translation
  ----------------------------------------------------------------------------- */
  glTranslatef(opengl_current_x_translate, opengl_current_y_translate, -opengl_current_z_translate);
  glRotatef(opengl_current_angle_x, 1.0, 0, 0);
  glRotatef(opengl_current_angle_y, 0, 1.0, 0);
  opengl_delta_zoom = 0;
  opengl_delta_angle_x = 0;
  opengl_delta_angle_y = 0;
}


void opengl_graph_1d_vector(std::vector<float> &input, const std::string& title, int index){
  float min = *std::min_element(input.begin(),input.end());
  float max = *std::max_element(input.begin(),input.end());

  opengl_switch_to_graph_window();

  float graph_y_position = (OPENGL_GRAPH_WINDOW_Y/ ((float)OPENGL_GRAPH_COUNT))*index + OPENGL_GRAPH_Y_OFFSET;
  float graph_height = OPENGL_GRAPH_WINDOW_Y/((float)OPENGL_GRAPH_COUNT);
  glRasterPos2i(OPENGL_GRAPH_X_OFFSET,graph_y_position);
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char *) title.c_str());

  glColor3f(1.0f, 1.0f, 0.0f);
  glBegin(GL_LINE_STRIP);
  float graph_x_scale = (((float)OPENGL_GRAPH_WINDOW_X-OPENGL_GRAPH_X_OFFSET)/input.size());
  float graph_y_scale = graph_height/(max-min);
  for(int i = 0; i < input.size(); i++){
    glVertex2f(OPENGL_GRAPH_X_OFFSET+(graph_x_scale*i),(graph_y_position+graph_height)-(graph_y_scale*(input[i]-min)));
  }
  glEnd();

}

void update_screen(){
  glutMainLoopEvent();

  glutSwapBuffers();
  glutPostRedisplay();

  // TakeScreenshot(t);
}




//
// void mouse_handler(int x, int y){
//   opengl_angle_x = ((x-mouse_previous_x)/(float)OPENGL_3D_WINDOW_X)*360.0;
//   opengl_angle_y = ((y-mouse_previous_y)/(float)OPENGL_3D_WINDOW_Y)*360.0;
//   mouse_previous_x = x;
//   mouse_previous_y = y;
//
// }




// void TakeScreenshot(float time)
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




void to_csv(std::vector<std::vector<float>> &original, root_mesh_geometry mesh_geometry){
  std::ofstream output_file;
  output_file.open ("out.csv");

  for(int r_x = 0; r_x < mesh_geometry.root_x_len; r_x++){
    for(int r_y = 0; r_y < mesh_geometry.root_y_len; r_y++){
      for(int r_z = 0; r_z < mesh_geometry.root_z_len; r_z++){
        int root_idx = (mesh_geometry.root_x_len*mesh_geometry.root_y_len*r_z) + (mesh_geometry.root_x_len*r_y) + r_x;

        if(original[root_idx].size()){
          int fine_sub_len = submesh_side_length(original[root_idx]);
          for(int x = 0; x < fine_sub_len; x++){
            for(int y = 0; y < fine_sub_len; y++){
              for(int z = 0; z < fine_sub_len; z++){
                int sub_idx = (fine_sub_len*fine_sub_len*z) + (fine_sub_len*y) + x;
                float world_x = (r_x*mesh_geometry.root_scale)+(x*(mesh_geometry.root_scale/fine_sub_len));
                float world_y = (r_y*mesh_geometry.root_scale)+(y*(mesh_geometry.root_scale/fine_sub_len));
                float world_z = (r_z*mesh_geometry.root_scale)+(z*(mesh_geometry.root_scale/fine_sub_len));
                // if(original[root_idx][sub_idx]){
                  output_file << world_x << ","
                              << world_y << ","
                              << world_z << ","
                              << original[root_idx][sub_idx] << "," << "\n";
                // }
              }
            }
          }
        }
      }
    }
  }
  output_file.close();
}

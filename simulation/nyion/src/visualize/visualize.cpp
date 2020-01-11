#include "nyion.hpp"
#include "visualize.hpp"
#include "host_data_structure.hpp"



float camera_distance = -10;

float camera_x = 0;
float camera_y = 0.0;
float camera_z = 0;

float camera_angle_pitch = -0.785398;
float camera_angle_yaw = -0.785398;



GLXDrawable mesh_window;
GLXDrawable graph_window;
GLXContext glContext;
Display * display;
XVisualInfo* vi;

void keyboard_handler(unsigned char key,__attribute__((unused)) int x,__attribute__((unused)) int y){
  if(key == '2') camera_angle_pitch += 0.1;
  if(key == '8') camera_angle_pitch += -0.1;
  if(key == '4') camera_angle_yaw += 0.1;
  if(key == '6') camera_angle_yaw += -0.1;
  if(key == 'a') camera_x += 0.1;
  if(key == 'd') camera_x += -0.1;
  if(key == 'w') camera_y += 0.1;
  if(key == 's') camera_y += -0.1;
  if(key == '0'){ camera_angle_pitch = -0.785398; camera_angle_yaw = 0; camera_x = 0; camera_y = 0; camera_z = 0; camera_distance = -10;};
  if(key == '+') camera_distance += -1;
  if(key == '-') camera_distance += +1;
}

void special_keyboard_handler(int key,__attribute__((unused)) int x,__attribute__((unused)) int y){
  if(key == GLUT_KEY_F6){

  }
}

void initialize_opengl(){

  // int sngBuf[] = { GLX_RGBA,
  //                  GLX_RED_SIZE, 1,
  //                  GLX_GREEN_SIZE, 1,
  //                  GLX_BLUE_SIZE, 1,
  //                  GLX_DEPTH_SIZE, 12,
  //                  None
  // };

  // display = XOpenDisplay(0);
  // vi = glXChooseVisual(display, DefaultScreen(display), sngBuf);
  // glContext = glXCreateContext(display, vi, 0, GL_TRUE);
  //

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
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  double aspect = (double)viewport[2] / (double)viewport[3];

  gluPerspective(45, aspect, 0.1, 30); //all parameters must be positive.
  glMatrixMode(GL_MODELVIEW);
  // glLoadIdentity();

}

void opengl_switch_to_graph_window(){
  glXMakeCurrent(display,graph_window,glContext);
}

void opengl_switch_to_mesh_window(){
  glXMakeCurrent(display,mesh_window,glContext);
}

void opengl_3d_mode(){


}


void opengl_2d_mode(){
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0.0, OPENGL_3D_WINDOW_X, OPENGL_3D_WINDOW_Y, 0.0, -1.0, 30.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


void opengl_clear_screen(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glFlush();

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

void opengl_test_prism() {
  //thanks to mina michael
  //https://gamedev.stackexchange.com/questions/150802/drawing-a-simple-triangular-prism-in-opengl-in-3d
  glBegin(GL_QUADS);
      glVertex3f(0.5, 0, 0.5);
      glVertex3f(0.5, 0, -0.5);
      glVertex3f(-0.5, 0, -0.5);
      glVertex3f(-0.5, 0, 0.5);

      glVertex3f(0.5,0,-0.5);
      glVertex3f(0.5,1,-0.5);
      glVertex3f(-0.5,1,-0.5);
      glVertex3f(-0.5,0,-0.5);

      glVertex3f(0.5,1,-0.5);
      glVertex3f(-0.5,1,-0.5);
      glVertex3f(-0.5,0,0.5);
      glVertex3f(0.5,0,0.5);
  glEnd();
  glBegin(GL_TRIANGLES);
      glVertex3f(0.5,0,0.5);
      glVertex3f(0.5,1,-0.5);
      glVertex3f(0.5,0,-0.5);

      glVertex3f(-0.5,0,0.5);
      glVertex3f(-0.5,1,-0.5);
      glVertex3f(-0.5,0,-0.5);
  glEnd();
}

void draw_mesh(__attribute__((unused)) float * array, int * refined_indices, int (&mesh_sizes)[MAX_DEPTH], bool level_splitting){

  //level split: render meshes with an offset corresponding to their level
  traverse_state state;

  for(init_state(state, mesh_sizes); breadth_first(state, refined_indices, MAX_DEPTH, 0, mesh_sizes); xyz_traverse(state, mesh_sizes, 0)){

    glPushMatrix();

      float current_scale = state.world_scale[state.current_depth];

      float level_split_offset = 0;

      float cube_size = current_scale*OPENGL_SCALE*0.95;

      if(state.current_depth){
        level_split_offset = (level_splitting * state.world_scale[state.current_depth-1]*1.5);
      }

      float gl_x,gl_y,gl_z;
      cell_world_lookup(state, gl_x, gl_y, gl_z, mesh_sizes);
      pretty_print_named_value(gl_x);
      gl_x *= OPENGL_SCALE;
      gl_y *= OPENGL_SCALE;
      gl_z *= OPENGL_SCALE;

      gl_x += level_split_offset*OPENGL_SCALE + 0.5*cube_size;
      gl_y += 0.5*cube_size;
      gl_z += 0.5*cube_size;


      glTranslatef(gl_x, gl_y, gl_z);

      // if(user_state.x == state.x &&
      //     user_state.y == state.y &&
      //     user_state.z == state.z &&
      //     user_state.current_depth == state.current_depth){
      //
      // }
      // else{
      if(is_ghost(state,mesh_sizes)){
        glColor4f(0,255.0,0,255);
      }
      else{
        glColor4f(255.0,0,0,255);
      }
      // }

      glutWireCube(cube_size); //make slightly smaller to
    glPopMatrix();
  }
}


void update_screen(){
  glLoadIdentity();
  gluLookAt(camera_distance * sin(camera_angle_pitch)*cos(camera_angle_yaw) + camera_x,
            camera_distance * cos(camera_angle_pitch) + camera_y,
            camera_distance * sin(camera_angle_pitch)*sin(camera_angle_yaw) + camera_z,
            camera_x,camera_y,camera_z,
            0,1,0);

  // gluLookAt(-10,0,0, 0,0,0, 0,1,0);

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

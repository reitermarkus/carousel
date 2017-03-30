/******************************************************************
*
* carousel.c
*
* Description: This example demonstrates a colored, rotating
* cube in shader-based OpenGL. The use of transformation
* matrices, perspective projection, and indexed triangle sets
* are shown.
*
* Computer Graphics Proseminar SS 2017
*
* Interactive Graphics and Simulation Group
* Institute of Computer Science
* University of Innsbruck
*
*******************************************************************/


/* Standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "opengl.h"

/* Local includes */
#include "matrix.h"
#include "helper.h"
#include "create_shader_program.h"
#include "draw.h"
#include "polygon.h"
#include "vertex.h"


/*----------------------------------------------------------------*/

/* Define handle to a vertex buffer object */
GLuint VBO;

/* Define handle to an index buffer object */
GLuint IBO;

float projection_matrix[16]; /* Perspective projection matrix */
float view_matrix[16]; /* Camera view matrix */
float model_matrix[16]; /* Model matrix */

/* Transformation matrices for initial position */
float translate_origin[16];
float translate_down[16];
float rotate_x[16];
float rotate_z[16];
float initial_transform[16];

struct vertex* vertex_buffer_data;

GLushort* index_buffer_data;

/*----------------------------------------------------------------*/


/******************************************************************
*
* display
*
* This function is called when the content of the window needs to be
* drawn/redrawn. It has been specified through 'glutDisplayFunc()';
* Enable vertex attributes, create binding between C program and
* attribute name in shader
*
*******************************************************************/

void display() {
  /* Clear window; color specified in 'initialize()' */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* Put linked shader program into drawing pipeline */
  GLuint shader_program = create_shader_program("vertexshader.vs", "fragmentshader.fs");
  draw(VBO, IBO, shader_program, projection_matrix, view_matrix, model_matrix);

  /* Swap between front and back buffer */
  glutSwapBuffers();
}


/******************************************************************
*
* on_idle
*
*
*
*******************************************************************/

void on_idle() {
  float rotations_per_second = 0.25f;
  float elapsed_time = glutGet(GLUT_ELAPSED_TIME); // ms
  float angle = elapsed_time * rotations_per_second * 360.0 / 1000.0; // deg

  float rotation_matrix_anim[16];

  /* Time dependent rotation */
  set_rotation_y(-angle, rotation_matrix_anim);

  /* Apply model rotation; finally move cube down */
  multiply_matrix(rotation_matrix_anim, initial_transform, model_matrix);
  multiply_matrix(translate_down, model_matrix, model_matrix);

  /* Request redrawing forof window content */
  glutPostRedisplay();
}


/******************************************************************
*
* setup_data_buffers
*
* Create buffer objects and load data into buffers
*
*******************************************************************/

void setup_data_buffers() {
  long vertex_buffer_size;
  long index_buffer_size;

  polygon(7, 1.5, .25, &vertex_buffer_data, &vertex_buffer_size, &index_buffer_data, &index_buffer_size);

  // generates 1 object name and stores it in VBO
  glGenBuffers(1, &VBO);
  // creates an ARRAY_BUFFER and names it VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // this function allocates memory on the GPU for our data and returns a pointer to the array, holding the data
  glBufferData(GL_ARRAY_BUFFER, vertex_buffer_size, vertex_buffer_data, GL_STATIC_DRAW);

  GLuint vao;
  glGenVertexArrays(1, &vao); // generates 1 array object name and stores it in VAO
  glBindVertexArray(vao); // binds the vertex array to given name -> VAO

  glGenBuffers(1, &IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_size, index_buffer_data, GL_STATIC_DRAW);

  free(vertex_buffer_data);
  free(index_buffer_data);
}


/******************************************************************
*
* initialize
*
* This function is called to initialize rendering elements, setup
* vertex buffer objects, and to setup the vertex and fragment shader
*
*******************************************************************/

void initialize(int window_width, int window_height) {
  /* Set background (clear) color to dark blue */
  glClearColor(0.0, 0.0, 0.4, 0.0);

  /* Enable depth testing */
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  /* Initialize matrices */
  set_identity_matrix(projection_matrix);
  set_identity_matrix(view_matrix);
  set_identity_matrix(model_matrix);

  /* Set projection transform */
  float fovy = 45.0;
  float aspect = (float)window_width / (float)window_height;
  float nearPlane = 1.0;
  float farPlane = 50.0;
  set_perspective_matrix(fovy, aspect, nearPlane, farPlane, projection_matrix);

  /* Set viewing transform */
  float camera_disp = -5.0;
  set_translation(0.0, 0.0, camera_disp, view_matrix);

  /* Translate and rotate cube onto tip */
  set_translation(0, 0, 0, translate_origin);
  set_rotation_x(0, rotate_x);
  set_rotation_z(0, rotate_z);

  /* Translate down */
  set_translation(0, -sqrtf(sqrtf(2.0) * 1.0), 0, translate_down);

  /* Initial transformation matrix */
  multiply_matrix(rotate_x, translate_origin, initial_transform);
  multiply_matrix(rotate_z, initial_transform, initial_transform);

  /* Setup vertex, color, and index buffer objects */
  setup_data_buffers();
}


/******************************************************************
*
* main
*
* Main function to setup GLUT, GLEW, and enter rendering loop
*
*******************************************************************/

int main(int argc, char** argv) {
  /* Initialize GLUT; set double buffered window and RGBA color model */
  glutInit(&argc, argv);

  #ifdef GLUT_CORE_PROFILE
  glutInitContextVersion(3, 3);
  glutInitContextProfile(GLUT_CORE_PROFILE);
  #endif

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH
    #ifdef GLUT_3_2_CORE_PROFILE
    | GLUT_3_2_CORE_PROFILE // macOS
    #endif
  );

  int window_width = 800;
  int window_height = 600;
  int window_position_x = (glutGet(GLUT_SCREEN_WIDTH) - window_width) / 2;
  int window_position_y = (glutGet(GLUT_SCREEN_HEIGHT) - window_height) / 2;
  glutInitWindowSize(window_width, window_height);
  glutInitWindowPosition(window_position_x, window_position_y);
  glutCreateWindow("Carousel");

  #ifdef __GLEW_H__
  /* Initialize GL extension wrangler */
  glewExperimental = GL_TRUE;
  GLenum res = glewInit();
  if (res != GLEW_OK) {
    fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
    return EXIT_FAILURE;
  }
  #endif

  /* Setup scene and rendering parameters */
  initialize(window_width, window_height);

  /* Specify callback functions;enter GLUT event processing loop,
   * handing control over to GLUT */
  glutIdleFunc(on_idle);
  glutDisplayFunc(display);
  glutMainLoop();

  return EXIT_SUCCESS;
}

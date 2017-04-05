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
#include <time.h>

#include "opengl.h"

/* Local includes */
#include "matrix.h"
#include "helper.h"
#include "create_shader_program.h"
#include "draw.h"
#include "cylinder.h"
#include "polygon.h"
#include "vertex.h"


/*----------------------------------------------------------------*/

float projection_matrix[16]; /* Perspective projection matrix */
float view_matrix[16]; /* Camera view matrix */

/* Transformation matrices for initial position */
float translate_origin[16];
float translate_down[16];
float rotation_x[16];
float rotation_z[16];
float initial_transform[16];

enum { number_of_sides = 7 };
struct object_data base;
struct object_data top;
struct object_data pillars[number_of_sides];
const float PILLAR_HEIGHT = 1.5;
const float BASE_HEIGHT = .15;
const float BASE_RADIUS = 1.5;

/*----------------------------------------------------------------*/

/******************************************************************
*
* setup_data_buffers
*
* Create buffer objects and load data into buffers
*
*******************************************************************/

void setup_data_buffers(struct object_data* object) {
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Bind buffer object for vertices and colors.
  glGenBuffers(1, &(object->vbo));
  glBindBuffer(GL_ARRAY_BUFFER, object->vbo);
  glBufferData(GL_ARRAY_BUFFER, object->vertices_size, object->vertices, GL_STATIC_DRAW);
  free(object->vertices);

  // Bind buffer object for indices.
  glGenBuffers(1, &(object->ibo));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, object->indices_size, object->indices, GL_STATIC_DRAW);
  free(object->indices);
}

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

void display_object(struct object_data* object) {
  /* Put linked shader program into drawing pipeline */
  object->shader_program = create_shader_program(object->vertex_shader_file, object->fragment_shader_file);
  draw(object, projection_matrix, view_matrix);
}

void display() {
  /* Clear window; color specified in 'initialize()' */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  display_object(&base);

  for (int i = 0; i < number_of_sides; i++) {
    display_object(&(pillars[i]));
  }

  display_object(&top);

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
  float rotations_per_second = 1.0f / 8;
  long elapsed_time = glutGet(GLUT_ELAPSED_TIME); // ms
  float angle = elapsed_time * rotations_per_second * 360.0 / 1000.0; // deg

  float rotation_matrix_anim[16];
  float transform_matrix[16];

  // Time dependent rotation.
  set_rotation_y(-angle, rotation_matrix_anim);

  multiply_matrix(rotation_matrix_anim, initial_transform, base.translation_matrix);
  multiply_matrix(translate_down, base.translation_matrix, base.translation_matrix);

  multiply_matrix(rotation_matrix_anim, initial_transform, top.translation_matrix);
  multiply_matrix(translate_down, top.translation_matrix, top.translation_matrix);

  // Move roof up above the pillars.
  set_translation(0, PILLAR_HEIGHT + BASE_HEIGHT, 0, transform_matrix);
  multiply_matrix(transform_matrix, top.translation_matrix, top.translation_matrix);

  for (int i = 0; i < number_of_sides; i++) {
    // Move pillars onto the base and outwards.
    set_translation(0, BASE_HEIGHT, -(BASE_RADIUS / 7 * 6), transform_matrix);

    multiply_matrix(transform_matrix, initial_transform, pillars[i].translation_matrix);
    set_rotation_y(360.0 / (float)number_of_sides * (float)i, transform_matrix);
    multiply_matrix(transform_matrix, pillars[i].translation_matrix, pillars[i].translation_matrix);
    multiply_matrix(rotation_matrix_anim, pillars[i].translation_matrix, pillars[i].translation_matrix);
    multiply_matrix(translate_down, pillars[i].translation_matrix, pillars[i].translation_matrix);
  }

  /* Request redrawing forof window content */
  glutPostRedisplay();
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
  glClearColor(R(220), G(220), B(220), 0.0);

  /* Enable depth testing */
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  /* Initialize matrices */
  set_identity_matrix(projection_matrix);
  set_identity_matrix(view_matrix);

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
  set_rotation_x(0, rotation_x);
  set_rotation_z(0, rotation_z);

  /* Translate down */
  set_translation(0, -sqrtf(sqrtf(2.0)), 0, translate_down);

  /* Initial transformation matrix */
  multiply_matrix(rotation_x, translate_origin, initial_transform);
  multiply_matrix(rotation_z, initial_transform, initial_transform);

  cylinder(number_of_sides, BASE_RADIUS, BASE_HEIGHT, &(base.vertices), &(base.vertices_size), &(base.indices), &(base.indices_size));

  /* Setup vertex, color, and index buffer objects */
  setup_data_buffers(&base);
  base.vertex_shader_file = "vertexshader.vs";
  base.fragment_shader_file = "fragmentshader.fs";
  set_identity_matrix(base.translation_matrix);


  cylinder(number_of_sides, BASE_RADIUS, BASE_HEIGHT, &(top.vertices), &(top.vertices_size), &(top.indices), &(top.indices_size));
  setup_data_buffers(&top);
  top.vertex_shader_file = "vertexshader.vs";
  top.fragment_shader_file = "fragmentshader.fs";
  set_identity_matrix(top.translation_matrix);


  for (int i = 0; i < number_of_sides; i++) {
    struct object_data pillar;

    cylinder(7, .1, PILLAR_HEIGHT, &(pillar.vertices), &(pillar.vertices_size), &(pillar.indices), &(pillar.indices_size));

    /* Setup vertex, color, and index buffer objects */
    setup_data_buffers(&pillar);
    pillar.vertex_shader_file = "vertexshader.vs";
    pillar.fragment_shader_file = "fragmentshader.fs";
    set_identity_matrix(pillar.translation_matrix);
    pillars[i] = pillar;
  }
}


/******************************************************************
*
* main
*
* Main function to setup GLUT, GLEW, and enter rendering loop
*
*******************************************************************/

int main(int argc, char** argv) {
  srand(time(NULL));

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

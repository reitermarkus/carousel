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
#include "cube.h"
#include "polygon.h"
#include "vertex.h"


/*----------------------------------------------------------------*/

float projection_matrix[16];
float view_matrix[16];

enum { number_of_sides = 8 };
struct object_data base;
struct object_data center_pillar;
struct object_data roof;
struct object_data pillars[number_of_sides];
struct object_data cubes[number_of_sides];

const float PILLAR_HEIGHT = 1.5;
const float BASE_HEIGHT = .15;
const float BASE_RADIUS = 2.5;
const float CENTER_PILLAR_RADIUS = 0.5;

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

void setup_shader_program(struct object_data* object) {
  // Put linked shader program into drawing pipeline.
  object->shader_program = create_shader_program(object->vertex_shader_file, object->fragment_shader_file);
}

void display_object(struct object_data* object) {
  draw(object, projection_matrix, view_matrix);
}

void display() {
  // Clear window with color specified in `initialize`.
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  display_object(&base);
  display_object(&center_pillar);
  display_object(&roof);

  for (int i = 0; i < number_of_sides; i++) {
    display_object(&(pillars[i]));
    display_object(&(cubes[i]));
  }

  // Swap between front and back buffer.
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
  float angle = elapsed_time * rotations_per_second * 360.0 / 1000; // deg

  // Rotate clock-wise by using negative angle.
  float rotation = -angle;

  // Initialize center pillar matrix.
  set_identity_matrix(center_pillar.translation_matrix);

  // Rotate center pillar.
  rotate_y(rotation, center_pillar.translation_matrix);

  // Initialize base matrix.
  set_identity_matrix(base.translation_matrix);

  // Rotate base.
  rotate_y(rotation, base.translation_matrix);

  // Initialize roof matrix.
  set_identity_matrix(roof.translation_matrix);

  // Rotate roof.
  rotate_y(rotation, roof.translation_matrix);

  // Move roof up above the pillars.
  translate_y(PILLAR_HEIGHT + BASE_HEIGHT, roof.translation_matrix);

  for (int i = 0; i < number_of_sides; i++) {
    // Initialize pillar matrix.
    set_identity_matrix(pillars[i].translation_matrix);

    // Move pillar onto the base.
    translate_y(BASE_HEIGHT, pillars[i].translation_matrix);

    // Move pillar towards the edge.
    translate_z(-(BASE_RADIUS / 7 * 6), pillars[i].translation_matrix);

    // Rotate pillar around the center to the corresponding edge.
    rotate_y(360.0 / (float)number_of_sides * (float)i, pillars[i].translation_matrix);

    // Apply general rotation to pillar.
    rotate_y(rotation, pillars[i].translation_matrix);
  }

  for(int i=0; i < number_of_sides; i++){
    // Initialize cube matrix.
    set_identity_matrix(cubes[i].translation_matrix);

    // Move cube towards the edge.
    translate_z(-(BASE_RADIUS / 7 * 6), cubes[i].translation_matrix);

    // Move cube up and down.
    translate_y((sin(rotation / 10.0 + i * M_PI) / 5) + PILLAR_HEIGHT / 2.175, cubes[i].translation_matrix);

    // Rotate cube around the center to the corresponding edge.
    rotate_y(360.0 / (float)number_of_sides * (float)i, cubes[i].translation_matrix);

    // Apply general rotation to cube.
    rotate_y(rotation, cubes[i].translation_matrix);
  }
  // Request redrawing of window content.
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

void initialize() {
  // Set background color based on system time.
  time_t current_time = time(NULL);
  int hour = localtime(&current_time)->tm_hour;
  if (hour >= 20 || hour <= 6) {
    glClearColor(R(0), G(25), B(50), 0.0);
  } else {
    glClearColor(R(220), G(220), B(220), 0.0);
  }

  // Enable depth testing.
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // Initialize view matrix.
  set_identity_matrix(view_matrix);
  float camera_distance = -7.0;
  set_translation(0.0, -1, camera_distance, view_matrix);

  float top_center_y_offset_roof = 1.0;

  /* Setup vertex, color, and index buffer objects for ROOF*/
  cylinder(20, BASE_RADIUS , 0.01, &(roof.vertices), &(roof.vertices_size), &(roof.indices), &(roof.indices_size),top_center_y_offset_roof);
  setup_data_buffers(&roof);
  roof.vertex_shader_file = "vertexshader.vs";
  roof.fragment_shader_file = "fragmentshader.fs";
  setup_shader_program(&roof);
  set_identity_matrix(roof.translation_matrix);

   float top_center_y_offset_pillar = 0.0;
  /* Setup vertex, color, and index buffer objects for CENTER PILLAR*/
  cylinder(15, CENTER_PILLAR_RADIUS, BASE_HEIGHT + PILLAR_HEIGHT, &(center_pillar.vertices), &(center_pillar.vertices_size), &(center_pillar.indices), &(center_pillar.indices_size),top_center_y_offset_pillar);
  setup_data_buffers(&center_pillar);
  center_pillar.vertex_shader_file = "vertexshader.vs";
  center_pillar.fragment_shader_file = "fragmentshader.fs";
  setup_shader_program(&center_pillar);
  set_identity_matrix(center_pillar.translation_matrix);

  float top_center_y_offset_base = 0.0;
   /* Setup vertex, color, and index buffer objects for BASE*/
  cylinder(20, BASE_RADIUS, BASE_HEIGHT, &(base.vertices), &(base.vertices_size), &(base.indices), &(base.indices_size),top_center_y_offset_base);
  setup_data_buffers(&base);
  base.vertex_shader_file = "vertexshader.vs";
  base.fragment_shader_file = "fragmentshader.fs";
  setup_shader_program(&base);
  set_identity_matrix(base.translation_matrix);

  for (int i = 0; i < number_of_sides; i++) {
    struct object_data cube_object;

    cube(0.15, &(cube_object.vertices), &(cube_object.vertices_size), &(cube_object.indices), &(cube_object.indices_size));
    /* Setup vertex, color, and index buffer objects for cubes*/
    setup_data_buffers(&cube_object);
    cube_object.vertex_shader_file = "vertexshader.vs";
    cube_object.fragment_shader_file = "fragmentshader.fs";
    setup_shader_program(&cube_object);
    set_identity_matrix(cube_object.translation_matrix);
    cubes[i] = cube_object;
  }

  float top_center_y_offset_pillars = 0.0;

  for (int i = 0; i < number_of_sides; i++) {
    struct object_data pillar;

    cylinder(7, .03, PILLAR_HEIGHT, &(pillar.vertices), &(pillar.vertices_size), &(pillar.indices), &(pillar.indices_size),top_center_y_offset_pillars);
    /* Setup vertex, color, and index buffer objects */
    setup_data_buffers(&pillar);
    pillar.vertex_shader_file = "vertexshader.vs";
    pillar.fragment_shader_file = "fragmentshader.fs";
    setup_shader_program(&pillar);
    set_identity_matrix(pillar.translation_matrix);
    pillars[i] = pillar;
  }
}


void resize_window(int width, int height) {
  // Initialize projection matrix.
  glViewport(0, 0, width, height);

  set_identity_matrix(projection_matrix);
  float fovy = 45.0;
  float aspect = (float)width / (float)height;
  float nearPlane = 1.0;
  float farPlane = 50.0;
  set_perspective_matrix(fovy, aspect, nearPlane, farPlane, projection_matrix);
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

  // Setup scene and rendering parameters.
  initialize();

  // Specify GLUT callback functions.
  glutIdleFunc(on_idle);
  glutReshapeFunc(resize_window);
  glutDisplayFunc(display);

  // Enter GLUT event processing loop.
  glutMainLoop();

  return EXIT_SUCCESS;
}

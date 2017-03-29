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
#include <string.h>
#include <math.h>

#include "opengl.h"

/* Local includes */
#include "load_shader.h"   /* Provides loading function for shader code */
#include "matrix.h"
#include "helper.h"
#include "polygon.h"
#include "vertex.h"


/*----------------------------------------------------------------*/

/* Define handle to a vertex buffer object */
GLuint VBO;

/* Define handle to a vertex array object */
GLuint VAO;

/* Define handle to an index buffer object */
GLuint IBO;


/* Indices to vertex attributes; in this case positon and color */
enum data_id {
  v_position = 0,
  v_color = 1
};

/* Strings for loading and storing shader code */
static const char* vertex_shader_string;
static const char* fragment_shader_string;

GLuint shader_program;

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

  glEnableVertexAttribArray(v_position);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // second and third parameter specify, that the array points are 3 dimensional of type GL_FLOAT
  glVertexAttribPointer(
    v_position,
    3,          // dimension of array points
    GL_FLOAT,   // type of the points
    GL_FALSE,   // whether the points should be normalized
    sizeof(struct vertex),
    0         // see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml
  );

  glEnableVertexAttribArray(v_color);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(v_color, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex), (void*)sizeof(struct position));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  GLint size;
  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

  /* Associate program with shader matrices */
  GLint projection_unifom = glGetUniformLocation(shader_program, "projection_matrix");
  if (projection_unifom == -1) {
    fprintf(stderr, "Could not bind uniform projection_matrix\n");
    exit(EXIT_FAILURE);
  }
  glUniformMatrix4fv(projection_unifom, 1, GL_TRUE, projection_matrix);

  GLint view_uniform = glGetUniformLocation(shader_program, "view_matrix");
  if (view_uniform == -1) {
    fprintf(stderr, "Could not bind uniform view_matrix\n");
    exit(EXIT_FAILURE);
  }
  glUniformMatrix4fv(view_uniform, 1, GL_TRUE, view_matrix);

  GLint rotation_uniform = glGetUniformLocation(shader_program, "model_matrix");
  if (rotation_uniform == -1) {
    fprintf(stderr, "Could not bind uniform model_matrix\n");
    exit(EXIT_FAILURE);
  }
  glUniformMatrix4fv(rotation_uniform, 1, GL_TRUE, model_matrix);

  /* Issue draw command, using indexed triangle list */
  glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

  /* Disable attributes */
  glDisableVertexAttribArray(v_position);
  glDisableVertexAttribArray(v_color);

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

  polygon(25, 1.5, &vertex_buffer_data, &vertex_buffer_size, &index_buffer_data, &index_buffer_size);

  // generates 1 object name and stores it in VBO
  glGenBuffers(1, &VBO);
  // creates an ARRAY_BUFFER and names it VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // this function allocates memory on the GPU for our data and returns a pointer to the array, holding the data
  glBufferData(GL_ARRAY_BUFFER, vertex_buffer_size, vertex_buffer_data, GL_STATIC_DRAW);

  // generates 1 array object name and stores it in VAO
  glGenVertexArrays(1, &VAO);
  // binds the vertex array to given name -> VAO
  glBindVertexArray(VAO);

  glGenBuffers(1, &IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_size, index_buffer_data, GL_STATIC_DRAW);

  free(vertex_buffer_data);
  free(index_buffer_data);
}


/******************************************************************
*
* add_shader
*
* This function creates and adds individual shaders
*
*******************************************************************/

void add_shader(GLuint shader_program, const char* shader_code, GLenum shader_type) {
  /* Create shader object */
  GLuint shader_obj = glCreateShader(shader_type);

  if (shader_obj == 0) {
    fprintf(stderr, "Error creating shader type %d\n", shader_type);
    exit(EXIT_FAILURE);
  }

  /* Associate shader source code string with shader object */
  glShaderSource(shader_obj, 1, &shader_code, NULL);

  GLint status;
  GLchar info_log[1024];

  /* Compile shader source code */
  glCompileShader(shader_obj);
  glGetShaderiv(shader_obj, GL_COMPILE_STATUS, &status);

  unless (status == GL_TRUE) {
    glGetShaderInfoLog(shader_obj, 1024, NULL, info_log);
    fprintf(stderr, "Error compiling shader type %d: '%s'\n", shader_type, info_log);
    exit(EXIT_FAILURE);
  }

  /* Associate shader with shader program */
  glAttachShader(shader_program, shader_obj);
}


/******************************************************************
*
* create_shader_program
*
* This function creates the shader program; vertex and fragment
* shaders are loaded and linked into program; final shader program
* is put into the rendering pipeline
*
*******************************************************************/

void create_shader_program() {
  /* Allocate shader object */
  shader_program = glCreateProgram();

  if (shader_program == 0) {
    fprintf(stderr, "Error creating shader program\n");
    exit(EXIT_FAILURE);
  }

  /* Load shader code from file */
  vertex_shader_string = load_shader("vertexshader.vs");
  fragment_shader_string = load_shader("fragmentshader.fs");

  /* Separately add vertex and fragment shader to program */
  add_shader(shader_program, vertex_shader_string, GL_VERTEX_SHADER);
  add_shader(shader_program, fragment_shader_string, GL_FRAGMENT_SHADER);

  GLint success = 0;
  GLchar error_log[1024];

  /* Link shader code into executable shader program */
  glLinkProgram(shader_program);

  /* Check results of linking step */
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

  if (success == 0) {
    glGetProgramInfoLog(shader_program, sizeof(error_log), NULL, error_log);
    fprintf(stderr, "Error linking shader program: '%s'\n", error_log);
    exit(EXIT_FAILURE);
  }

  /* Check if shader program can be executed */
  glValidateProgram(shader_program);
  glGetProgramiv(shader_program, GL_VALIDATE_STATUS, &success);

  unless (success) {
    glGetProgramInfoLog(shader_program, sizeof(error_log), NULL, error_log);
    fprintf(stderr, "Invalid shader program: '%s'\n", error_log);
    exit(EXIT_FAILURE);
  }

  /* Put linked shader program into drawing pipeline */
  glUseProgram(shader_program);
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

  /* Setup vertex, color, and index buffer objects */
  setup_data_buffers();

  /* Setup shaders and shader program */
  create_shader_program();

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

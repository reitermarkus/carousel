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

/* OpenGL includes */
#if __APPLE__
  #include "TargetConditionals.h"
  #define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
  #include <OpenGL/gl3.h>
  #include <GLUT/glut.h>
#else
  #include <GL/glew.h>
  #include <GL/freeglut.h>
#endif


/* Local includes */
#include "load_shader.h"   /* Provides loading function for shader code */
#include "matrix.h"


/*----------------------------------------------------------------*/

/* Define handle to a vertex buffer object */
GLuint VBO;

/* Define handle to a vertex array object */
GLuint VAO;

/* Define handle to a color buffer object */
GLuint CBO;

/* Define handle to an index buffer object */
GLuint IBO;


/* Indices to vertex attributes; in this case positon and color */
enum data_id {v_position = 0, v_color = 1};

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


GLfloat vertex_buffer_data[] = { /* 8 cube vertices XYZ */
  -1.0, -1.0,  1.0,
   1.0, -1.0,  1.0,
   1.0,  1.0,  1.0,
  -1.0,  1.0,  1.0,
  -1.0, -1.0, -1.0,
   1.0, -1.0, -1.0,
   1.0,  1.0, -1.0,
  -1.0,  1.0, -1.0,
};

GLfloat color_buffer_data[] = { /* RGB color values for 8 vertices */
  0.0, 0.0, 1.0,
  1.0, 0.0, 1.0,
  1.0, 1.0, 1.0,
  0.0, 1.0, 1.0,
  0.0, 0.0, 0.0,
  1.0, 0.0, 0.0,
  1.0, 1.0, 0.0,
  0.0, 1.0, 0.0,
};

GLushort index_buffer_data[] = { /* Indices of 6*2 triangles (6 sides) */
  0, 1, 2,
  2, 3, 0,
  1, 5, 6,
  6, 2, 1,
  7, 6, 5,
  5, 4, 7,
  4, 0, 3,
  3, 7, 4,
  4, 5, 1,
  1, 0, 4,
  3, 2, 6,
  6, 7, 3,
};

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
  glVertexAttribPointer(v_position, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glEnableVertexAttribArray(v_color);
  glBindBuffer(GL_ARRAY_BUFFER, CBO);
  glVertexAttribPointer(v_color, 3, GL_FLOAT,GL_FALSE, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  GLint size;
  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

  /* Associate program with shader matrices */
  GLint projectionUniform = glGetUniformLocation(shader_program, "projection_matrix");
  if (projectionUniform == -1) {
    fprintf(stderr, "Could not bind uniform projection_matrix\n");
    exit(EXIT_FAILURE);
  }
  glUniformMatrix4fv(projectionUniform, 1, GL_TRUE, projection_matrix);

  GLint ViewUniform = glGetUniformLocation(shader_program, "view_matrix");
  if (ViewUniform == -1) {
    fprintf(stderr, "Could not bind uniform view_matrix\n");
    exit(EXIT_FAILURE);
  }
  glUniformMatrix4fv(ViewUniform, 1, GL_TRUE, view_matrix);

  GLint RotationUniform = glGetUniformLocation(shader_program, "model_matrix");
  if (RotationUniform == -1) {
    fprintf(stderr, "Could not bind uniform model_matrix\n");
    exit(EXIT_FAILURE);
  }
  glUniformMatrix4fv(RotationUniform, 1, GL_TRUE, model_matrix);

  /* Issue draw command, using indexed triangle list */
  glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

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
  float angle = (glutGet(GLUT_ELAPSED_TIME) / 1000.0) * (180.0/M_PI);
  float RotationMatrixAnim[16];

  /* Time dependent rotation */
  set_rotation_y(angle, RotationMatrixAnim);

  /* Apply model rotation; finally move cube down */
  multiply_matrix(RotationMatrixAnim, initial_transform, model_matrix);
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
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_buffer_data), index_buffer_data, GL_STATIC_DRAW);

  glGenBuffers(1, &CBO);
  glBindBuffer(GL_ARRAY_BUFFER, CBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data), color_buffer_data, GL_STATIC_DRAW);
}


/******************************************************************
*
* add_shader
*
* This function creates and adds individual shaders
*
*******************************************************************/

void add_shader(GLuint shader_program, const char* ShaderCode, GLenum ShaderType) {
  /* Create shader object */
  GLuint ShaderObj = glCreateShader(ShaderType);

  if (ShaderObj == 0) {
    fprintf(stderr, "Error creating shader type %d\n", ShaderType);
    exit(0);
  }

  /* Associate shader source code string with shader object */
  glShaderSource(ShaderObj, 1, &ShaderCode, NULL);

  GLint success = 0;
  GLchar InfoLog[1024];

  /* Compile shader source code */
  glCompileShader(ShaderObj);
  glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
    fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
    exit(1);
  }

  /* Associate shader with shader program */
  glAttachShader(shader_program, ShaderObj);
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
    exit(1);
  }

  /* Load shader code from file */
  vertex_shader_string = load_shader("vertexshader.vs");
  fragment_shader_string = load_shader("fragmentshader.fs");

  /* Separately add vertex and fragment shader to program */
  add_shader(shader_program, vertex_shader_string, GL_VERTEX_SHADER);
  add_shader(shader_program, fragment_shader_string, GL_FRAGMENT_SHADER);

  GLint success = 0;
  GLchar ErrorLog[1024];

  /* Link shader code into executable shader program */
  glLinkProgram(shader_program);

  /* Check results of linking step */
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

  if (success == 0) {
    glGetProgramInfoLog(shader_program, sizeof(ErrorLog), NULL, ErrorLog);
    fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
    exit(1);
  }

  /* Check if shader program can be executed */
  glValidateProgram(shader_program);
  glGetProgramiv(shader_program, GL_VALIDATE_STATUS, &success);

  if (!success) {
    glGetProgramInfoLog(shader_program, sizeof(ErrorLog), NULL, ErrorLog);
    fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
    exit(1);
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

void initialize(void) {
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
  float aspect = 1.0;
  float nearPlane = 1.0;
  float farPlane = 50.0;
  set_perspective_matrix(fovy, aspect, nearPlane, farPlane, projection_matrix);

  /* Set viewing transform */
  float camera_disp = -10.0;
  set_translation(0.0, 0.0, camera_disp, view_matrix);

  /* Translate and rotate cube onto tip */
  set_translation(1, 1, 1, translate_origin);
  set_rotation_x(-45, rotate_x);
  set_rotation_z(35, rotate_z);

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
  glutInitWindowSize(600, 600);
  glutInitWindowPosition(400, 400);
  glutCreateWindow("CG Proseminar - Rotating Cube");

  #ifdef __GLEW_H__
  /* Initialize GL extension wrangler */
  GLenum res = glewInit();
  if (res != GLEW_OK) {
    fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
    return EXIT_FAILURE;
  }
  #endif

  /* Setup scene and rendering parameters */
  initialize();

  /* Specify callback functions;enter GLUT event processing loop,
   * handing control over to GLUT */
  glutIdleFunc(on_idle);
  glutDisplayFunc(display);
  glutMainLoop();

  return EXIT_SUCCESS;
}

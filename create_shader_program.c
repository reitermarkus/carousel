#include <stdio.h>
#include <stdlib.h>

#include "opengl.h"
#include "helper.h"
#include "load_shader.h"

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

GLuint create_shader_program(const char* vertexshader, const char* fragmentshader) {
  /* Allocate shader object */
  GLuint shader_program = glCreateProgram();

  if (shader_program == 0) {
    fprintf(stderr, "Error creating shader program.\n");
    exit(EXIT_FAILURE);
  }

  /* Load shader code from file */
  const char* vertex_shader_string = load_shader(vertexshader);
  const char* fragment_shader_string = load_shader(fragmentshader);

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

  free((char*)vertex_shader_string);
  free((char*)fragment_shader_string);

  glUseProgram(shader_program);

  return shader_program;
}

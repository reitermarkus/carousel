#include <stdio.h>
#include <stdlib.h>

#include "shader_program.h"

#include "helper/load_file.h"

void add_shader(GLuint shader_program, const char* shader_code, GLenum shader_type) {
  GLuint shader = glCreateShader(shader_type);

  if (shader == 0) {
    fprintf(stderr, "Error creating shader type %d.\n", shader_type);
    exit(EXIT_FAILURE);
  }

  // Associate shader source code string with shader object.
  glShaderSource(shader, 1, &shader_code, NULL);

  GLint status;
  GLchar info_log[1024];

  // Compile shader source code.
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

  if (status != GL_TRUE) {
    glGetShaderInfoLog(shader, 1024, NULL, info_log);
    fprintf(stderr, "Error compiling shader type %d: '%s'\n", shader_type, info_log);
    exit(EXIT_FAILURE);
  }

  // Attach shader to the shader program.
  glAttachShader(shader_program, shader);
}

GLuint create_shader_program(const char* vertex_shader, const char* fragment_shader) {
  GLuint shader_program = glCreateProgram();

  if (shader_program == 0) {
    fprintf(stderr, "Error creating shader program.\n");
    exit(EXIT_FAILURE);
  }

  // Load shader code from file.
  const char* vertex_shader_string = load_file(vertex_shader);
  const char* fragment_shader_string = load_file(fragment_shader);

  // Attach vertex and fragment shader to shader program.
  add_shader(shader_program, vertex_shader_string, GL_VERTEX_SHADER);
  add_shader(shader_program, fragment_shader_string, GL_FRAGMENT_SHADER);

  free((char*)vertex_shader_string);
  free((char*)fragment_shader_string);

  GLint success = 0;
  GLchar error_log[1024];

  // Link shader code into executable shader program.
  glLinkProgram(shader_program);
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

  if (success == 0) {
    glGetProgramInfoLog(shader_program, sizeof(error_log), NULL, error_log);
    fprintf(stderr, "Error linking shader program: '%s'\n", error_log);
    exit(EXIT_FAILURE);
  }

  return shader_program;
}


void use_shader(GLuint shader_program) {
  // Check if shader program can be executed.
  glValidateProgram(shader_program);

  GLint success = 0;
  GLchar error_log[1024];
  glGetProgramiv(shader_program, GL_VALIDATE_STATUS, &success);

  if (success == 0) {
    glGetProgramInfoLog(shader_program, sizeof(error_log), NULL, error_log);
    fprintf(stderr, "Invalid shader program: '%s'\n", error_log);
    exit(EXIT_FAILURE);
  }

  glUseProgram(shader_program);
}

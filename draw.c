#include <stdio.h>
#include <stdlib.h>

#include "opengl.h"
#include "vertex.h"

static void set_shader_matrix(GLuint shader_program, const char* matrix_name, float* matrix) {
  GLint uniform_location = glGetUniformLocation(shader_program, matrix_name);

  if (uniform_location == -1) {
    fprintf(stderr, "Could not bind uniform '%s'.\n", matrix_name);
    exit(EXIT_FAILURE);
  }

  glUniformMatrix4fv(uniform_location, 1, GL_TRUE, matrix);
}

void draw(GLuint vbo, GLuint ibo, GLuint shader_program, float* projection_matrix, float* view_matrix, float* model_matrix) {
  glEnableVertexAttribArray(v_position);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // second and third parameter specify, that the array points are 3 dimensional of type GL_FLOAT
  glVertexAttribPointer(
    v_position,
    3,          // dimension of array points
    GL_FLOAT,   // type of the points
    GL_FALSE,   // whether the points should be normalized
    sizeof(struct vertex),
    0           // see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml
  );

  glEnableVertexAttribArray(v_color);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(v_color, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex), (void*)sizeof(struct position));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  GLint size;
  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

  // Associate program with shader matrices
  set_shader_matrix(shader_program, "projection_matrix", projection_matrix);
  set_shader_matrix(shader_program, "view_matrix", view_matrix);
  set_shader_matrix(shader_program, "model_matrix", model_matrix);

  // Issue draw command, using indexed triangle list
  glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

  // Disable attributes
  glDisableVertexAttribArray(v_position);
  glDisableVertexAttribArray(v_color);
}

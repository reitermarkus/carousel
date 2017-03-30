#include <stdio.h>
#include <stdlib.h>

#include "opengl.h"
#include "vertex.h"

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

  // Issue draw command, using indexed triangle list
  glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

  // Disable attributes
  glDisableVertexAttribArray(v_position);
  glDisableVertexAttribArray(v_color);
}

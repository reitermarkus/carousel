#include "draw.h"

#include <stdio.h>
#include <stdlib.h>

#include "helper/shared_headers.h"
#include "helper/shader_program.h"

static void set_shader_matrix(GLuint shader_program, const char* matrix_name, matrix matrix) {
  GLint uniform_location = glGetUniformLocation(shader_program, matrix_name);

  if (uniform_location == -1) {
    fprintf(stderr, "Could not bind uniform '%s'.\n", matrix_name);
    exit(EXIT_FAILURE);
  }

  glUniformMatrix4fv(uniform_location, 1, GL_TRUE, (float*)matrix);
}

void draw(struct object_data* object, matrix projection_matrix, matrix view_matrix) {
  glBindVertexArray(object->vao);

  use_shader(object->shader_program);

  // Bind texture if there is one.
  if (object->texture != 0) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, object->texture);

    glUniform1i(glGetUniformLocation(object->shader_program, "texture_enabled"), 1);
  } else {
    glUniform1i(glGetUniformLocation(object->shader_program, "texture_enabled"), 0);
  }

  // Associate program with shader matrices.
  set_shader_matrix(object->shader_program, "projection_matrix", projection_matrix);
  set_shader_matrix(object->shader_program, "view_matrix", view_matrix);
  set_shader_matrix(object->shader_program, "model_matrix", (*object).translation_matrix);

  // Draw triangles using index array.
  GLint size;
  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

  glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
  glBindVertexArray(0);
}

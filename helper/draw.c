#include "draw.h"

#include <stdio.h>
#include <stdlib.h>

#include "helper/shared_headers.h"

static void set_shader_matrix(GLuint shader_program, const char* matrix_name, const matrix matrix) {
  GLint uniform_location = glGetUniformLocation(shader_program, matrix_name);

  if (uniform_location == -1) {
    fprintf(stderr, "Could not bind uniform '%s'.\n", matrix_name);
    exit(EXIT_FAILURE);
  }

  glUniformMatrix4fv(uniform_location, 1, GL_TRUE, (float*)matrix);
}

void draw(struct object_data* object, const matrix projection_matrix, const matrix view_matrix) {
  // Bind vertex array.
  glEnableVertexAttribArray(v_position);
  glBindBuffer(GL_ARRAY_BUFFER, (*object).vbo);
  glVertexAttribPointer(
    v_position,                    // attribute index
    3,                             // attribute length (x, y, z)
    GL_FLOAT,                      // attribute type
    GL_FALSE,                      // normalize points
    sizeof(struct vertex),         // offset between indices
    0                              // offset to vertex values
  );

  // Bind color array.
  glEnableVertexAttribArray(v_color);
  glBindBuffer(GL_ARRAY_BUFFER, (*object).vbo);
  glVertexAttribPointer(
    v_color,                       // attribute index
    3,                             // attribute length (r, g, b)
    GL_FLOAT,                      // attribute type
    GL_FALSE,                      // normalize points
    sizeof(struct vertex),         // offset between indices
    (void*)sizeof(struct position) // offset to color values
  );

  // Bind index array.
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*object).ibo);

  // Associate program with shader matrices.
  set_shader_matrix(object->shader_program, "projection_matrix", projection_matrix);
  set_shader_matrix(object->shader_program, "view_matrix", view_matrix);
  set_shader_matrix(object->shader_program, "model_matrix", (*object).translation_matrix);

  // Draw triangles using index array.
  GLint size;
  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
  glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

  glDisableVertexAttribArray(v_position);
  glDisableVertexAttribArray(v_color);
}

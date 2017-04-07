#ifndef __H_VERTEX__
#define __H_VERTEX__

#include "helper/shared_headers.h"

#define SET_VERTEX_COLOR(vertex, r_val, g_val, b_val) do { \
  (vertex).color.r = (r_val);                              \
  (vertex).color.g = (g_val);                              \
  (vertex).color.b = (b_val);                              \
} while(0)

#define SET_VERTEX_POSITION(vertex, x_val, y_val, z_val) do { \
  (vertex).position.x = (x_val);                              \
  (vertex).position.y = (y_val);                              \
  (vertex).position.z = (z_val);                              \
} while(0)

struct position {
  GLfloat x, y, z;
};

struct color {
  GLfloat r, g, b;
};

struct index {
  GLuint a, b, c;
};

struct vertex {
  struct position position;
  struct color color;
};

enum attribute_index {
  v_position = 0,
  v_color = 1
};

struct object_data {
  GLuint vbo;
  struct vertex* vertices;
  long vertices_size;

  GLuint ibo;
  GLushort* indices;
  long indices_size;

  GLuint shader_program;
  const char* vertex_shader_file;
  const char* fragment_shader_file;

  float translation_matrix[16];
};

#endif

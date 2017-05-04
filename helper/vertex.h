#ifndef __VERTEX_H__
#define __VERTEX_H__

#include "helper/shared_headers.h"
#include "helper/matrix.h"

#define SET_VERTEX_COLOR(vertex, r_val, g_val, b_val, a_val) do { \
  (vertex).color.r = (r_val);                              \
  (vertex).color.g = (g_val);                              \
  (vertex).color.b = (b_val);                              \
  (vertex).color.a = (a_val);                              \
} while (0)

#define SET_VERTEX_POSITION(vertex, x_val, y_val, z_val) do { \
  (vertex).position.x = (x_val);                              \
  (vertex).position.y = (y_val);                              \
  (vertex).position.z = (z_val);                              \
} while (0)

struct position {
  GLfloat x, y, z;
};

struct color {
  GLfloat r, g, b, a;
};

struct index {
  GLushort a, b, c;
};

struct texture {
  GLfloat u, v;
};

struct vertex {
  struct position position;
  struct color color;
};

enum attribute_index {
  v_position = 0,
  v_color = 1,
  v_texture = 2
};

struct object_data {
  GLuint vao;

  GLuint vbo;
  struct vertex* vertices;
  long vertex_count;

  GLuint ibo;
  struct index* indices;
  long index_count;

  GLuint tbo;
  struct texture* textures;
  long texture_count;

  GLuint shader_program;
  const char* vertex_shader_file;
  const char* fragment_shader_file;

  matrix translation_matrix;
};

#endif // __VERTEX_H__

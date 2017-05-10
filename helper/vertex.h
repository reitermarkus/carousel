#ifndef __VERTEX_H__
#define __VERTEX_H__

#include <stdlib.h>

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

struct normal {
  GLfloat a, b, c;
};

struct vertex {
  struct position position;
  struct color color;
  struct normal normal;
};

enum attribute_index {
  v_position = 0,
  v_color = 1,
  v_texture = 2,
  v_normal = 3
};

struct object_data {
  GLuint vao;

  GLuint vbo;
  struct vertex* vertices;
  size_t vertex_count;

  GLuint ibo;
  struct index* indices;
  size_t index_count;

  GLuint tbo;
  struct texture* textures;
  size_t texture_count;

  GLuint nbo;
  struct normal* face_normals;

  GLuint shader_program;
  const char* vertex_shader_file;
  const char* fragment_shader_file;

  matrix translation_matrix;
};

void calculate_normals(struct object_data* obj);
void init_object_data(struct object_data* obj);

#endif // __VERTEX_H__

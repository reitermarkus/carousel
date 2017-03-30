#ifndef __H_VERTEX__
#define __H_VERTEX__

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

enum data_id {
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

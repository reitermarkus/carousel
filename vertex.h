#ifndef __H_VERTEX__
#define __H_VERTEX__

struct position {
  GLfloat x, y, z;
};

struct color {
  GLfloat r, g, b;
};

struct vertex {
  struct position position;
  struct color color;
};

enum data_id {
  v_position = 0,
  v_color = 1
};

#endif

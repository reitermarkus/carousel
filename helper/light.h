#ifndef __LIGHT_DATA_H__
#define __LIGHT_DATA_H__

struct rgb {
  GLfloat r, g, b;
};

struct light {
  struct position position;
  struct rgb color;
};

#endif // __LIGHT_DATA_H__

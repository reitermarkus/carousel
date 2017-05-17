#ifndef __LIGHT_DATA_H__
#define __LIGHT_DATA_H__

#include "helper/shared_headers.h"
#include "helper/vertex.h"

struct rgb {
  GLfloat r, g, b;
};

struct light {
  struct position position;
  struct rgb color;
};

int rand(void);
void srand(unsigned int seed);
void get_random_color(struct rgb* light_color);

#endif // __LIGHT_DATA_H__

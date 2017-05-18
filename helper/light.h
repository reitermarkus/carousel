#ifndef __LIGHT_DATA_H__
#define __LIGHT_DATA_H__

#include "helper/shared_headers.h"
#include "helper/vertex.h"
#include "helper/color.h"
#include <stdio.h>
#include <stdlib.h>

struct light {
  struct position position;
  struct hsv color;
};

int rand(void);
void srand(unsigned int seed);
void get_random_color(struct hsv* light_color);

void calc_hsv_tmp(struct hsv* hsv, int h);
void calc_hsv(struct hsv* hsv, int m);
void hsv_to_rgb(struct hsv* hsv, float h, float s, float v);
float fmodf(float x, float y);

#endif // __LIGHT_DATA_H__

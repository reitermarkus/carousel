#ifndef __COLOR_H__
#define __COLOR_H__

#include "helper/shared_headers.h"

/* Struct for rgb values */
struct frgb {
    float r;
    float g;
    float b;
};

/* Struct for hsv values */
struct hsv {
  int hue;
  int saturation;
  int value;
};

#endif
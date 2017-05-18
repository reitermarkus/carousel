#ifndef __LIGHT_DATA_H__
#define __LIGHT_DATA_H__

#include "helper/shared_headers.h"
#include "helper/vertex.h"
#include "helper/color.h"

struct light {
  struct position position;
  struct hsv color;
};

#endif // __LIGHT_DATA_H__

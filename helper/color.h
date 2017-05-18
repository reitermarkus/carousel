#ifndef __COLOR_H__
#define __COLOR_H__

struct rgb {
  float r, g, b;
};

struct hsv {
  float h, s, v;
};

void rgb_to_hsv(struct rgb color, struct hsv* out);
void hsv_to_rgb(struct hsv color, struct rgb* out);

#endif

#include "color.h"

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void rgb_to_hsv(struct rgb color, struct hsv* out) {
  float max = fmax(fmax(color.r, color.g), color.b);
  float min = fmin(fmin(color.r, color.g), color.b);
  float delta = max - min;

  out->h = 0;
  out->s = 0;
  out->v = max;

  if (delta > 0) {
    if (max > 0) { out->s = delta / max; }

    if (max == color.r) { out->h = 60.0 * fmod(((color.g - color.b) / delta), 6); } else
    if (max == color.g) { out->h = 60.0 * (((color.b - color.r) / delta) + 2.0); } else
    if (max == color.b) { out->h = 60.0 * (((color.r - color.g) / delta) + 4.0); }

    while (out->h < 0) { out->h += 360.0; }
  }
}

void hsv_to_rgb(struct hsv color, struct rgb* out) {
  float hue = color.h;

  while (hue < 0) { hue += 360; }

  float chroma = color.v * color.s;
  float hue_mod = fmod(hue / 60.0, 6);
  float m = color.v - chroma;
  float x = chroma * (1.0 - fabs(1.0 - fmod(hue_mod, 2))) + m;

  switch ((int)hue_mod) {
    case 0: out->r = color.v; out->g = x;       out->b = m;       break;
    case 1: out->r = x;       out->g = color.v; out->b = m;       break;
    case 2: out->r = m;       out->g = color.v; out->b = x;       break;
    case 3: out->r = m;       out->g = x;       out->b = color.v; break;
    case 4: out->r = x;       out->g = m;       out->b = color.v; break;
    case 5: out->r = color.v; out->g = m;       out->b = x;       break;
  }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "opengl.h"
#include "helper.h"
#include "vertex.h"

void circle(int detail, double radius, struct position** vertices, long* vertices_size, struct color** colors, long* colors_size, GLushort** indices, long* indices_size) {
  float step = 2.0 * M_PI / (float)detail; // circumfence divided by parts

  *vertices_size = (detail + 1) * sizeof(struct position);
  *vertices = malloc(*vertices_size);

  *colors_size = (detail + 1) * sizeof(struct color);
  *colors = malloc(*colors_size);

  *indices_size = detail * 3 * sizeof(GLushort);
  *indices = malloc(*indices_size);

  struct position* position = (struct position*)(*vertices);

  (*position).x = 0.0;
  (*position).y = 0.0;
  (*position).z = 0.0;

  srand(time(NULL));

  struct color* color = (struct color*)(*colors);

  (*color).r = (double)rand() / (double)RAND_MAX;
  (*color).g = (double)rand() / (double)RAND_MAX;
  (*color).b = (double)rand() / (double)RAND_MAX;

  float angle = 0;
  for (int i = 0; i < detail; i++) {
    position = (struct position*)(*vertices + i + 1);

    (*position).x = cosf(angle) * radius;
    (*position).y = 0.0;
    (*position).z = sinf(angle) * radius;

    printf("Angle: %8.2f, Position: (%8.2f, %8.2f, %8.2f)\n", rad_to_deg(angle), (*position).x, (*position).y, (*position).z);
    angle += step;

    color = (struct color*)(*colors + i + 1);

    (*color).r = (double)rand() / (double)RAND_MAX;
    (*color).g = (double)rand() / (double)RAND_MAX;
    (*color).b = (double)rand() / (double)RAND_MAX;


    int curr_i = i + 1;
    int next_i = (curr_i % detail) + 1;
    printf("Connecting %i (origin) with %i and %i.\n", 0, curr_i, next_i);

    int idx = i * 3;
    (*indices)[idx + 0] = 0;
    (*indices)[idx + 1] = curr_i;
    (*indices)[idx + 2] = next_i;
  }
}

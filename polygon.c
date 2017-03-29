#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "opengl.h"
#include "helper.h"
#include "vertex.h"

void polygon(int detail, double radius, struct vertex** vertices, long* vertices_size, GLushort** indices, long* indices_size) {
  float step = 2.0 * M_PI / (float)detail; // circumfence divided by parts

  *vertices_size = (detail + 1) * sizeof(struct vertex);
  *vertices = malloc(*vertices_size);

  *indices_size = detail * 3 * sizeof(GLushort);
  *indices = malloc(*indices_size);

  struct vertex* vertex = (struct vertex*)(*vertices);

  (*vertex).position.x = 0.0;
  (*vertex).position.y = 0.0;
  (*vertex).position.z = 0.0;

  srand(time(NULL));

  (*vertex).color.r = (double)rand() / (double)RAND_MAX;
  (*vertex).color.g = (double)rand() / (double)RAND_MAX;
  (*vertex).color.b = (double)rand() / (double)RAND_MAX;

  float angle = 0;
  for (int i = 0; i < detail; i++) {
    vertex = (struct vertex*)(*vertices + i + 1);

    (*vertex).position.x = cosf(angle) * radius;
    (*vertex).position.y = 0.0;
    (*vertex).position.z = sinf(angle) * radius;

    printf("Angle: %8.2f, Position: (%8.2f, %8.2f, %8.2f)\n", rad_to_deg(angle), (*vertex).position.x, (*vertex).position.y, (*vertex).position.z);
    angle += step;

    (*vertex).color.r = (double)rand() / (double)RAND_MAX;
    (*vertex).color.g = (double)rand() / (double)RAND_MAX;
    (*vertex).color.b = (double)rand() / (double)RAND_MAX;

    int curr_i = i + 1;
    int next_i = (curr_i % detail) + 1;
    printf("Connecting %i (origin) with %i and %i.\n", 0, curr_i, next_i);

    int idx = i * 3;
    (*indices)[idx + 0] = 0;
    (*indices)[idx + 1] = curr_i;
    (*indices)[idx + 2] = next_i;
  }
}

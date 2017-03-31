#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "opengl.h"
#include "helper.h"
#include "vertex.h"

void polygon(int detail, double radius, double height, struct vertex** vertices, long* vertices_size, GLushort** indices, long* indices_size) {
  float step = 2.0 * M_PI / (float)detail; // circumfence divided by parts

  *vertices_size = (detail + 1) * (height == 0 ? 1 : 2) * sizeof(struct vertex);
  *vertices = malloc(*vertices_size);

  *indices_size = detail * (height == 0 ? 1 : 4) * 3 * sizeof(GLushort);
  *indices = malloc(*indices_size);

  struct vertex* vertex = (struct vertex*)(*vertices);

  (*vertex).position.x = 0.0;
  (*vertex).position.y = 0.0;
  (*vertex).position.z = 0.0;

  srand(time(NULL));

  (*vertex).color.r = (double)rand() / (double)RAND_MAX;
  (*vertex).color.g = (double)rand() / (double)RAND_MAX;
  (*vertex).color.b = (double)rand() / (double)RAND_MAX;

  unless (height == 0) {
    vertex = (struct vertex*)(*vertices + (detail + 1));

    (*vertex).position.x = 0.0;
    (*vertex).position.y = height;
    (*vertex).position.z = 0.0;

    (*vertex).color.r = (double)rand() / (double)RAND_MAX;
    (*vertex).color.g = (double)rand() / (double)RAND_MAX;
    (*vertex).color.b = (double)rand() / (double)RAND_MAX;
  }

  float angle = -(M_PI / 2.0); // start with first edge on top
  for (int i = 0; i < detail; i++) {
    vertex = (struct vertex*)(*vertices + i + 1);

    (*vertex).position.x = cosf(angle) * radius;
    (*vertex).position.y = 0.0;
    (*vertex).position.z = sinf(angle) * radius;

    printf("Angle: %8.2f, Position: (%8.2f, %8.2f, %8.2f)\n", rad_to_deg(angle), (*vertex).position.x, (*vertex).position.y, (*vertex).position.z);

    (*vertex).color.r = (double)rand() / (double)RAND_MAX;
    (*vertex).color.g = (double)rand() / (double)RAND_MAX;
    (*vertex).color.b = (double)rand() / (double)RAND_MAX;

    // top circle
    unless (height == 0) {
      vertex = (struct vertex*)(*vertices + i + 1 + (detail + 1));

      (*vertex).position.x = cosf(angle) * radius;
      (*vertex).position.y = height;
      (*vertex).position.z = sinf(angle) * radius;

      (*vertex).color.r = (double)rand() / (double)RAND_MAX;
      (*vertex).color.g = (double)rand() / (double)RAND_MAX;
      (*vertex).color.b = (double)rand() / (double)RAND_MAX;
    }

    int bottom_curr_i = i + 1;
    int bottonm_next_i = (bottom_curr_i % detail) + 1;

    int j = i * 3;
    (*indices)[j + 0] = 0;
    (*indices)[j + 1] = bottom_curr_i;
    (*indices)[j + 2] = bottonm_next_i;
    printf("Connecting %i (origin bottom) with %i and %i.\n", 0, bottom_curr_i, bottonm_next_i);

    // top circle
    unless (height == 0) {
      int k = (i + detail) * 3;

      int top_curr_i = bottom_curr_i + detail + 1;
      int top_next_i = bottonm_next_i + detail + 1;

      (*indices)[k + 0] = 0 + detail + 1;
      (*indices)[k + 1] = top_curr_i;
      (*indices)[k + 2] = top_next_i;
      printf("Connecting %i (origin top) with %i and %i.\n", 0, top_curr_i, top_next_i);

      // sides
      (*indices)[k * 2 + 0] = bottom_curr_i;
      (*indices)[k * 2 + 1] = bottonm_next_i;
      (*indices)[k * 2 + 2] = top_curr_i;
      printf("Connecting %i and %i and %i.\n", bottom_curr_i, bottonm_next_i, top_curr_i);
      (*indices)[k * 2 + 3] = top_curr_i;
      (*indices)[k * 2 + 4] = top_next_i;
      (*indices)[k * 2 + 5] = bottonm_next_i;
      printf("Connecting %i and %i and %i.\n", top_curr_i, top_next_i, bottonm_next_i);
    }

    angle += step;
  }

}

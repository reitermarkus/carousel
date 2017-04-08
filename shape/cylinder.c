#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "cylinder.h"

#include "helper/macros.h"

void cylinder(int edges, double radius, double height, struct vertex** vertices, long* vertices_size, GLushort** indices, long* indices_size, float top_center_y_offset) {
  float step = 2.0 * M_PI / (float)edges; // circumfence divided by parts

  // Vertex count is number of edges + 1 (center vertex) if height is 0,
  // otherwise (number of edges + 1) * 2.
  *vertices_size = (edges + 1) * (height == 0 ? 1 : 2) * sizeof(struct vertex);
  *vertices = malloc(*vertices_size);

  // Index count is number of edges if height is 0,
  // otherwise (number of edges) * 4, to account for the side panels.
  *indices_size = edges * (height == 0 ? 1 : 4) * 3 * sizeof(GLushort);
  *indices = malloc(*indices_size);

  struct vertex* vertex = (struct vertex*)(*vertices);

  // Assign coordinates to the center vertex of the base polygon.
  SET_VERTEX_POSITION(*vertex, 0, 0, 0);

  // Assign color values to the center vertex of the base polygon.
  SET_VERTEX_COLOR(*vertex, R(RGB_RAND), G(RGB_RAND), B(RGB_RAND));

  if (height > 0) {
    vertex = (struct vertex*)(*vertices + (edges + 1));

    // Assign coordinates to the center vertex of the top polygon.
    SET_VERTEX_POSITION(*vertex, 0, height + top_center_y_offset, 0);

    // Assign color values to the center vertex of the top polygon.
    SET_VERTEX_COLOR(*vertex, R(RGB_RAND), G(RGB_RAND), B(RGB_RAND));
  }

  // Make sure the first polygon vertex is at the top.
  float angle = -(M_PI / 2.0);

  for (int i = 0; i < edges; i++) {
    vertex = (struct vertex*)(*vertices + i + 1);

    // Assign coordinates to the nth vertex of the base polygon.
    SET_VERTEX_POSITION(*vertex, cosf(angle) * radius, 0, sinf(angle) * radius);
    // printf("Angle: %8.2f, Position: (%8.2f, %8.2f, %8.2f)\n", rad_to_deg(angle), (*vertex).position.x, (*vertex).position.y, (*vertex).position.z);

    // Assign color values to the nth vertex of the base polygon.
    SET_VERTEX_COLOR(*vertex, R(RGB_RAND), G(RGB_RAND), B(RGB_RAND));

    if (height > 0) {
      vertex = (struct vertex*)(*vertices + i + 1 + (edges + 1));

      // Assign coordinates to the nth vertex of the top polygon.
      SET_VERTEX_POSITION(*vertex, cosf(angle) * radius, height, sinf(angle) * radius);

      // Assign color values to the nth vertex of the top polygon.
      SET_VERTEX_COLOR(*vertex, R(RGB_RAND), G(RGB_RAND), B(RGB_RAND));
    }

    int j = i * 3;
    int curr_i_base = i + 1;
    int next_i_base = (curr_i_base % edges) + 1;

    // Connect the triangle between the center vertex,
    // the current and next vertex of the base polygon.
    (*indices)[j + 0] = 0;
    (*indices)[j + 1] = curr_i_base;
    (*indices)[j + 2] = next_i_base;
    // printf("Connecting %i (origin base) with %i and %i.\n", 0, curr_i_base, next_i_base);

    if (height > 0) {
      int k = (i + edges) * 3;
      int curr_i_top = curr_i_base + edges + 1;
      int next_i_top = next_i_base + edges + 1;

      // Connect the triangle between the center vertex,
      // the current and next vertex of the top polygon.
      (*indices)[k + 0] = 0 + edges + 1;
      (*indices)[k + 1] = curr_i_top;
      (*indices)[k + 2] = next_i_top;
      // printf("Connecting %i (origin top) with %i and %i.\n", 0, curr_i_top, next_i_top);

      // Connect the first triangle of the side panel.
      (*indices)[k * 2 + 0] = curr_i_base;
      (*indices)[k * 2 + 1] = next_i_base;
      (*indices)[k * 2 + 2] = curr_i_top;
      // printf("Connecting %i and %i and %i.\n", curr_i_base, next_i_base, curr_i_top);

      // Connect the second triangle of the side panel.
      (*indices)[k * 2 + 3] = curr_i_top;
      (*indices)[k * 2 + 4] = next_i_top;
      (*indices)[k * 2 + 5] = next_i_base;
      // printf("Connecting %i and %i and %i.\n", curr_i_top, next_i_top, next_i_base);
    }

    angle += step;
  }

}

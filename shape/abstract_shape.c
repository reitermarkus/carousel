#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "abstract_shape.h"

#include "helper/macros.h"

void abstract_shape(int edges, float bottom_radius, float top_radius, float height, float bottom_center_y_offset, float top_center_y_offset, struct vertex** vertices, long* vertex_count, struct index** indices, long* index_count) {
  float step = 2.0 * M_PI / (float)edges; // circumfence divided by parts

  // Vertex count is number of edges + 1 (center vertex) if height is 0,
  // otherwise (number of edges + 1) * 2.
  *vertex_count = edges + 1;
  if (height != 0) {
    *vertex_count *= 2;
  } else if (top_center_y_offset != 0) {
    *vertex_count += 1;
  }

  *vertices = malloc(*vertex_count * sizeof(**vertices));

  // Index count is number of edges if height is 0,
  // otherwise (number of edges) * 4, to account for the side panels.
  *index_count = edges;
  if (top_center_y_offset != 0) {
    *index_count *= 2;
  } else if (height != 0) {
    *index_count *= 4;
  }

  *indices = malloc(*index_count * sizeof(**indices));

  struct vertex* vertex = (struct vertex*)(*vertices);

  // Assign coordinates to the center vertex of the base polygon.
  SET_VERTEX_POSITION(*vertex, 0, bottom_center_y_offset, 0);

  // Assign color values to the center vertex of the base polygon.
  SET_VERTEX_COLOR(*vertex, R(RGB_RAND), G(RGB_RAND), B(RGB_RAND));

  if (height != 0 || top_center_y_offset != 0) {
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
    SET_VERTEX_POSITION(*vertex, cosf(angle) * bottom_radius, 0, sinf(angle) * bottom_radius);
    // printf("Angle: %8.2f, Position: (%8.2f, %8.2f, %8.2f)\n", rad_to_deg(angle), (*vertex).position.x, (*vertex).position.y, (*vertex).position.z);

    // Assign color values to the nth vertex of the base polygon.
    SET_VERTEX_COLOR(*vertex, R(RGB_RAND), G(RGB_RAND), B(RGB_RAND));

    if (height != 0) {
      vertex = (struct vertex*)(*vertices + i + 1 + (edges + 1));

      // Assign coordinates to the nth vertex of the top polygon.
      SET_VERTEX_POSITION(*vertex, cosf(angle) * top_radius, height, sinf(angle) * top_radius);

      // Assign color values to the nth vertex of the top polygon.
      SET_VERTEX_COLOR(*vertex, R(RGB_RAND), G(RGB_RAND), B(RGB_RAND));
    }

    int curr_i_base = i + 1;
    int next_i_base = (curr_i_base % edges) + 1;

    // Connect the triangle between the center vertex,
    // the current and next vertex of the base polygon.
    (*indices)[i].a = 0;
    (*indices)[i].b = curr_i_base;
    (*indices)[i].c = next_i_base;
    // printf("Connecting %i (origin base) with %i and %i.\n", 0, curr_i_base, next_i_base);

    if (height != 0 || top_center_y_offset != 0) {
      int k = i + edges;
      int curr_i_top = curr_i_base + edges + 1;
      int next_i_top = next_i_base + edges + 1;

      if (height == 0) {
        // Connect the triangle between the top center vertex
        // with the current and next vertex of the base polygon.
        (*indices)[k].a = 0 + edges + 1;
        (*indices)[k].b = curr_i_base;
        (*indices)[k].c = next_i_base;
      } else if (height != 0) {
        // Connect the triangle between the top center vertex
        // with the current and next vertex of the top polygon.
        (*indices)[k].a = 0 + edges + 1;
        (*indices)[k].b = curr_i_top;
        (*indices)[k].c = next_i_top;
        // printf("Connecting %i (origin top) with %i and %i.\n", 0, curr_i_top, next_i_top);

        // Connect the first triangle of the side panel.
        (*indices)[k * 2 + 0].a = curr_i_base;
        (*indices)[k * 2 + 0].b = next_i_base;
        (*indices)[k * 2 + 0].c = curr_i_top;
        // printf("Connecting %i and %i and %i.\n", curr_i_base, next_i_base, curr_i_top);

        // Connect the second triangle of the side panel.
        (*indices)[k * 2 + 1].a = curr_i_top;
        (*indices)[k * 2 + 1].b = next_i_top;
        (*indices)[k * 2 + 1].c = next_i_base;
        // printf("Connecting %i and %i and %i.\n", curr_i_top, next_i_top, next_i_base);
      }
    }

    angle += step;
  }

}

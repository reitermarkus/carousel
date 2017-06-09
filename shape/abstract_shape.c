#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "abstract_shape.h"

#include "helper/macros.h"

void abstract_shape(int edges, float bottom_radius, float top_radius, float height, float bottom_center_y_offset, float top_center_y_offset, struct object_data* obj) {
  float step = 2.0 * M_PI / (float)edges; // circumfence divided by parts

  // Vertex count is number of edges + 1 (center vertex) if height is 0,
  // otherwise (number of edges + 1) * 2.
  if (height != 0) {
    obj->vertex_count = edges * 4 + 2;
  } else if (top_center_y_offset != 0) {
    obj->vertex_count = edges * 2 + 2;
  } else {
    obj->vertex_count = edges + 1;
  }

  obj->vertices = malloc(obj->vertex_count * sizeof(*obj->vertices));

  // Index count is number of edges if height is 0,
  // otherwise (number of edges) * 4, to account for the side panels.
  if (height != 0) {
    obj->index_count = edges * 4;
  } else if (top_center_y_offset != 0) {
    obj->index_count = edges * 2;
  } else {
    obj->index_count = edges;
  }

  obj->indices = malloc(obj->index_count * sizeof(*obj->indices));

  size_t bci = 0;
  size_t tci = edges + 1;

  // Assign coordinates to the center vertex of the base polygon.
  SET_VERTEX_POSITION(obj->vertices[bci], 0, bottom_center_y_offset, 0);

  if (height != 0 || top_center_y_offset != 0) {
    // Assign coordinates to the center vertex of the top polygon.
    SET_VERTEX_POSITION(obj->vertices[tci], 0, height + top_center_y_offset, 0);
  }

  // Make sure the first polygon vertex is at the top.
  float angle = -(M_PI / 2.0);

  for (int i = 0; i < edges; i++) {
    // Assign coordinates to the nth vertex of the base polygon.
    SET_VERTEX_POSITION(obj->vertices[i + 1], cosf(angle) * bottom_radius, 0, sinf(angle) * bottom_radius);
    // printf("Angle: %8.2f, Position: (%8.2f, %8.2f, %8.2f)\n", rad_to_deg(angle), (*vertex).position.x, (*vertex).position.y, (*vertex).position.z);

    if (height != 0 || top_center_y_offset != 0) {
      // Assign coordinates to the nth vertex of the top polygon.
      SET_VERTEX_POSITION(obj->vertices[tci + (i + 1)], cosf(angle) * top_radius, height, sinf(angle) * top_radius);

      if (height != 0) {
        SET_VERTEX_POSITION(obj->vertices[tci + (i + 1) + (edges * 1)], cosf(angle) * bottom_radius, 0, sinf(angle) * bottom_radius);
        SET_VERTEX_POSITION(obj->vertices[tci + (i + 1) + (edges * 2)], cosf(angle) * top_radius, height, sinf(angle) * top_radius);
      }
    }

    int curr_i = i + 1;
    int next_i = (curr_i % edges) + 1;

    // Connect the triangle between the center vertex,
    // the current and next vertex of the base polygon.
    obj->indices[i].a = bci;
    obj->indices[i].b = curr_i;
    obj->indices[i].c = next_i;
    // printf("Connecting %i (origin base) with %i and %i.\n", 0, curr_i, next_i);

    if (height != 0 || top_center_y_offset != 0) {
      // Connect the triangle between the top center vertex
      // with the current and next vertex of the top polygon.
      obj->indices[i + edges].a = tci;
      obj->indices[i + edges].b = next_i + tci;
      obj->indices[i + edges].c = curr_i + tci;

      if (height != 0) {
        // Connect the first triangle of the side panel.
        obj->indices[i + edges * 2].a = curr_i + tci + (edges * 1);
        obj->indices[i + edges * 2].b = curr_i + tci + (edges * 2);
        obj->indices[i + edges * 2].c = next_i + tci + edges;

        // Connect the second triangle of the side panel.
        obj->indices[i + edges * 3].a = curr_i + tci + (edges * 2);
        obj->indices[i + edges * 3].b = next_i + tci + (edges * 2);
        obj->indices[i + edges * 3].c = next_i + tci + edges;
      }
    }

    angle += step;
  }

  for (size_t i = 0; i < obj->vertex_count; i++) {
    SET_VERTEX_COLOR(obj->vertices[i], R(RGB_RAND), G(RGB_RAND), B(RGB_RAND), ALPHA_RAND);
  }
}

#include "flattened_cone.h"

#include "abstract_shape.h"

void flattened_cone(int edges, float bottom_radius, float top_radius, float height, struct vertex** vertices, long* vertices_size, GLushort** indices, long* indices_size) {
  abstract_shape(edges, bottom_radius, top_radius, height, 0, 0, vertices, vertices_size, indices, indices_size);
}

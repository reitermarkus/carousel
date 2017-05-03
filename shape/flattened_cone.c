#include "flattened_cone.h"

#include "abstract_shape.h"

void flattened_cone(int edges, float bottom_radius, float top_radius, float height, struct vertex** vertices, long* vertex_count, struct index** indices, long* index_count) {
  abstract_shape(edges, bottom_radius, top_radius, height, 0, 0, vertices, vertex_count, indices, index_count);
}

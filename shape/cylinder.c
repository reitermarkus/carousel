#include "cylinder.h"

#include "abstract_shape.h"

void cylinder(int edges, float radius, float height, struct vertex** vertices, long* vertex_count, struct index** indices, long* index_count) {
  abstract_shape(edges, radius, radius, height, 0, 0, vertices, vertex_count, indices, index_count);
}

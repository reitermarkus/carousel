#include "cone.h"

#include "abstract_shape.h"

void cone(int edges, float radius, float height, struct vertex** vertices, long* vertex_count, struct index** indices, long* index_count) {
  abstract_shape(edges, radius, radius, 0, 0, height, vertices, vertex_count, indices, index_count);
}

#include "polygon.h"

#include "abstract_shape.h"

void polygon(int edges, float radius, struct vertex** vertices, long* vertex_count, struct index** indices, long* index_count) {
  abstract_shape(edges, radius, radius, 0, 0, 0, vertices, vertex_count, indices, index_count);
}

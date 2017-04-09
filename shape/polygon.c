#include "polygon.h"

#include "abstract_shape.h"

void polygon(int edges, float radius, struct vertex** vertices, long* vertices_size, GLushort** indices, long* indices_size) {
  abstract_shape(edges, radius, 0, 0, 0, vertices, vertices_size, indices, indices_size);
}

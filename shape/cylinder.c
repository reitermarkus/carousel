#include "cylinder.h"

#include "abstract_shape.h"

void cylinder(int edges, double radius, double height, struct vertex** vertices, long* vertices_size, GLushort** indices, long* indices_size) {
  abstract_shape(edges, radius, height, 0, 0, vertices, vertices_size, indices, indices_size);
}

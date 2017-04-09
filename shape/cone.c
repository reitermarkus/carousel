#include "cone.h"

#include "abstract_shape.h"

void cone(int edges, double radius, double height, struct vertex** vertices, long* vertices_size, GLushort** indices, long* indices_size) {
  abstract_shape(edges, radius, 0, 0, height, vertices, vertices_size, indices, indices_size);
}

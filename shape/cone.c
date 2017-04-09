#include "polygon.h"

#include "cylinder.h"

void cone(int edges, double radius, double height, struct vertex** vertices, long* vertices_size, GLushort** indices, long* indices_size) {
  cylinder(edges, radius, 0, vertices, vertices_size, indices, indices_size, height);
}

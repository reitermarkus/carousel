#include "opengl.h"
#include "vertex.h"
#include "cylinder.h"

void polygon(int edges, double radius, struct vertex** vertices, long* vertices_size, GLushort** indices, long* indices_size) {
  double height = 0;
  cylinder(edges, radius, height, vertices, vertices_size, indices, indices_size);
}

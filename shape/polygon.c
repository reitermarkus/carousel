#include "helper/shared_headers.h"
#include "helper/vertex.h"
#include "shape/cylinder.h"

void polygon(int edges, double radius, struct vertex** vertices, long* vertices_size, GLushort** indices, long* indices_size,float top_center_y_offset) {
  double height = 0;
  cylinder(edges, radius, height, vertices, vertices_size, indices, indices_size, top_center_y_offset);
}

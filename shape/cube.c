#include "cube.h"

#include "cuboid.h"

void cube(float h, struct vertex** vertices, long* vertices_size, struct index** indices, long* indices_size) {
  cuboid(h, h, h, vertices, vertices_size, indices, indices_size);
}

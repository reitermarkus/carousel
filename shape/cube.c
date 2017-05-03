#include "cube.h"

#include "cuboid.h"

void cube(float h, struct vertex** vertices, long* vertex_count, struct index** indices, long* index_count) {
  cuboid(h, h, h, vertices, vertex_count, indices, index_count);
}

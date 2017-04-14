#include "hyper_rectangle.h"

#include "cuboid.h"

void hyper_rectangle(float w, float h, float d, struct vertex** vertices, long* vertices_size, GLushort** indices, long* indices_size) {
  cuboid(w, h, d, vertices, vertices_size, indices, indices_size);
}

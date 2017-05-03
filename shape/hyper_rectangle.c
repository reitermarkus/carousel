#include "hyper_rectangle.h"

#include "cuboid.h"

void hyper_rectangle(float w, float h, float d, struct vertex** vertices, long* vertex_count, struct index** indices, long* index_count) {
  cuboid(w, h, d, vertices, vertex_count, indices, index_count);
}

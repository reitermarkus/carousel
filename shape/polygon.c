#include "polygon.h"

#include "abstract_shape.h"

void polygon(int edges, float radius, struct object_data* obj) {
  abstract_shape(edges, radius, radius, 0, 0, 0, obj);
}

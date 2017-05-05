#include "cylinder.h"

#include "abstract_shape.h"

void cylinder(int edges, float radius, float height, struct object_data* obj) {
  abstract_shape(edges, radius, radius, height, 0, 0, obj);
}

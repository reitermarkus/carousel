#include "cone.h"

#include "abstract_shape.h"

void cone(int edges, float radius, float height, struct object_data* obj) {
  abstract_shape(edges, radius, radius, 0, 0, height, obj);
}

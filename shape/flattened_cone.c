#include "flattened_cone.h"

#include "abstract_shape.h"

void flattened_cone(int edges, float bottom_radius, float top_radius, float height, struct object_data* obj) {
  abstract_shape(edges, bottom_radius, top_radius, height, 0, 0, obj);
}

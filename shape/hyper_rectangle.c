#include "hyper_rectangle.h"

#include "cuboid.h"

void hyper_rectangle(float w, float h, float d, struct object_data* obj) {
  cuboid(w, h, d, obj);
}

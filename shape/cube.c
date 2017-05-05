#include "cube.h"

#include "cuboid.h"

void cube(float h, struct object_data* obj) {
  cuboid(h, h, h, obj);
}

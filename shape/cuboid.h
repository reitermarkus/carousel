#ifndef __CUBOID_H__
#define __CUBOID_H__

#include "helper/shared_headers.h"
#include "helper/vertex.h"

void cuboid(float d, float h, float w, struct vertex** vertices, long* vertex_count, struct index** indices, long* index_count);

#endif // __CUBOID_H__

#ifndef __CYLINDER_H__
#define __CYLINDER_H__

#include "helper/shared_headers.h"
#include "helper/vertex.h"

void cylinder(int edges, float radius, float height, struct vertex** vertices, long* vertex_count, struct index** indices, long* index_count);

#endif // __CYLINDER_H__

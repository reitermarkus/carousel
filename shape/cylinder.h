#ifndef __CYLINDER_H__
#define __CYLINDER_H__

#include "helper/shared_headers.h"
#include "helper/vertex.h"

void cylinder(int edges, float radius, float height, struct vertex** vertices, long* vertices_size, GLushort** indices, long* indices_size);

#endif // __CYLINDER_H__

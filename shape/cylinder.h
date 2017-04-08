#ifndef __CYLINDER_H__
#define __CYLINDER_H__

#include "helper/shared_headers.h"
#include "helper/vertex.h"

void cylinder(int edges, double radius, double height, struct vertex** vertices, long* vertices_size, GLushort** indices, long* indices_size, float top_center_y_offset);

#endif

#ifndef __H_CYLINDER__
#define __H_CYLINDER__

#include "opengl.h"
#include "vertex.h"

void cylinder(int edges, double radius, double height, struct vertex** vertices, long* vertices_size, GLushort** indices, long* indices_size, float top_center_y_offset);

#endif

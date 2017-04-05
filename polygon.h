#ifndef __H_POLYGON__
#define __H_POLYGON__

#include "opengl.h"
#include "vertex.h"

void polygon(int edges, double radius, struct vertex** vertices, long* vertices_size, GLushort** indices, long* indices_size);

#endif

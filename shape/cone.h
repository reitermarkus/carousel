#ifndef __POLYGON_H__
#define __POLYGON_H__

#include "helper/shared_headers.h"
#include "helper/vertex.h"

void cone(int edges, double radius, double height, struct vertex** vertices, long* vertices_size, GLushort** indices, long* indices_size);

#endif

#ifndef __POLYGON_H__
#define __POLYGON_H__

#include "helper/shared_headers.h"
#include "helper/vertex.h"

void polygon(int edges, float radius, struct vertex** vertices, long* vertices_size, GLushort** indices, long* indices_size);

#endif // __POLYGON_H__

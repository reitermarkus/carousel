#ifndef __CONE_H__
#define __CONE_H__

#include "helper/shared_headers.h"
#include "helper/vertex.h"

void cone(int edges, double radius, double height, struct vertex** vertices, long* vertices_size, GLushort** indices, long* indices_size);

#endif // __CONE_H__

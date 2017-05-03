#ifndef __POLYGON_H__
#define __POLYGON_H__

#include "helper/shared_headers.h"
#include "helper/vertex.h"

void polygon(int edges, float radius, struct vertex** vertices, long* vertex_count, struct index** indices, long* index_count);

#endif // __POLYGON_H__

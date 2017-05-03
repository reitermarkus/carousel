#ifndef __CONE_H__
#define __CONE_H__

#include "helper/shared_headers.h"
#include "helper/vertex.h"

void cone(int edges, float radius, float height, struct vertex** vertices, long* vertex_count, struct index** indices, long* index_count);

#endif // __CONE_H__

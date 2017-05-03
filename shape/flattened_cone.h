#ifndef __FLATTENED_CONE_H__
#define __FLATTENED_CONE_H__

#include "helper/shared_headers.h"
#include "helper/vertex.h"

void flattened_cone(int edges, float bottom_radius, float top_radius, float height, struct vertex** vertices, long* vertex_count, struct index** indices, long* index_count);

#endif // __FLATTENED_CONE_H__

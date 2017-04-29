#ifndef __FLATTENED_CONE_H__
#define __FLATTENED_CONE_H__

#include "helper/shared_headers.h"
#include "helper/vertex.h"

void flattened_cone(int edges, float bottom_radius, float top_radius, float height, struct vertex** vertices, long* vertices_size, struct index** indices, long* indices_size);

#endif // __FLATTENED_CONE_H__

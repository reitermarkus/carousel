#ifndef __ABSTRACT_SHAPE_H__
#define __ABSTRACT_SHAPE_H__

#include "helper/shared_headers.h"
#include "helper/vertex.h"

// Abstract funciton to create cylindrical and conical shapes.
void abstract_shape(int edges, float bottom_radius, float top_radius, float height, float bottom_center_y_offset, float top_center_y_offset, struct vertex** vertices, long* vertices_size, struct index** indices, long* indices_size);

#endif // __ABSTRACT_SHAPE_H__

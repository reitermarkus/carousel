#ifndef __HYPER_RECTANGLE_H__
#define __HYPER_RECTANGLE_H__

#include "helper/shared_headers.h"
#include "helper/vertex.h"

void hyper_rectangle(float w, float h, float d, struct vertex** vertices, long* vertex_count, struct index** indices, long* index_count);

#endif // __HYPER_RECTANGLE_H__

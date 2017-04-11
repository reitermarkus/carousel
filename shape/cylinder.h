#ifndef __CYLINDER_H__
#define __CYLINDER_H__

#include "helper/shared_headers.h"
#include "helper/vertex.h"
#include "shape.h"

class Cylinder final : virtual public Shape {
	private:
		int edges;
		double radius;
		double height;
		struct vertex** vertices;
		long* vertices_size;
		GLushort** indices;
		long* indices_size;
		float top_center_y_offset;

  public:
  	virtual ~Cylinder() { }
    Cylinder(int edges, double radius, double height, struct vertex** vertices, long* vertices_size, GLushort** indices, long* indices_size, float top_center_y_offset);
    void create() override;
};

#endif

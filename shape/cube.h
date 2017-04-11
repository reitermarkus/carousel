#ifndef __CUBE_H__
#define __CUBE_H__

#include "helper/shared_headers.h"
#include "helper/vertex.h"
#include "shape.h"

class Cube final : virtual public Shape {
	private:
		double h;
		struct vertex** vertices;
		long* vertices_size;
		GLushort** indices;
		long* indices_size;
		float top_center_y_offset;

  public:
  	virtual ~Cube() { }
    Cube(double h, struct vertex** vertices, long* vertices_size, GLushort** indices, long* indices_size);
    void create() override;
};

#endif

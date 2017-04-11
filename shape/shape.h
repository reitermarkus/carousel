#ifndef __SHAPE_H__
#define __SHAPE_H__

class Shape
{
	public:
		virtual ~Shape() { }
		virtual void create() = 0;
};

#endif
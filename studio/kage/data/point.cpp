
#include "point.h"

PointData::PointData() {
	x = 0;
	y = 0;
}

PointData::PointData(Gdk::Point p) {
	x = (double) p.get_x();
	y = (double) p.get_y();
}

PointData::PointData(double p_x, double p_y) {
	x = p_x;
	y = p_y;
}

PointData::~PointData() {
	//
}

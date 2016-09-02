
#include "point.h"

PointData::PointData() {
	x = 0;
	y = 0;
}

PointData::PointData(GdkPoint p) {
	x = (double) p.x;
	y = (double) p.y;
}

PointData::PointData(double p_x, double p_y) {
	x = p_x;
	y = p_y;
}

PointData::~PointData() {
	//
}

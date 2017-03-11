
#include "point.h"
#include "../../kage.h"
#include <iostream>

PointData::PointData() {
	x = 0;
	y = 0;
	debug_id = PointData::debug_pts++;
}

PointData::PointData(GdkPoint p) {
	x = (double) (((double)((int)(p.x*100)))/100);
	y = (double) (((double)((int)(p.y*100)))/100);
	debug_id = PointData::debug_pts++;
}

PointData::PointData(double p_x, double p_y) {
	x = (double) (((double)((int)(p_x*100)))/100);
	y = (double) (((double)((int)(p_y*100)))/100);
	debug_id = PointData::debug_pts++;
}

PointData PointData::clone() {
	PointData l_p(x, y);
	return l_p;
}

PointData::~PointData() {
	//
}


#include "point.h"
#include "kage.h"
#include <iostream>

PointData::PointData() {
	x = 0;
	y = 0;
	debug_id = PointData::debug_pts++;
}

PointData::PointData(GdkPoint p) {
	x = (double) p.x;
	y = (double) p.y;
	debug_id = PointData::debug_pts++;
}

PointData::PointData(double p_x, double p_y) {
	x = p_x;
	y = p_y;
	debug_id = PointData::debug_pts++;
}

PointData PointData::clone() {
	PointData l_p(x, y);
	return l_p;
}
const PointData& PointData::operator=(const PointData& p_origin) {
	PointData l_p(p_origin.x, p_origin.y);
	return l_p;
}

PointData::~PointData() {
	//
}

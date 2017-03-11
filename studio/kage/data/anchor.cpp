
#include "anchor.h"
#include <iostream>

AnchorData::AnchorData() : PointData() {
	rotation = 0;
}

AnchorData::AnchorData(GdkPoint p) : PointData(p) {
	rotation = 0;
}

AnchorData::AnchorData(double p_x, double p_y) : PointData(p_x, p_y) {
	rotation = 0;
}

AnchorData AnchorData::clone() {
	AnchorData l_p(x, y);
	return l_p;
}

AnchorData::~AnchorData() {
	//
}

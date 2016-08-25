
#include "vectordata.h"

VectorData::VectorData() {
	//
}
VectorData::VectorData(type p_type) {
	setType(p_type);
}

VectorData::~VectorData() {
	points.clear();
}

void VectorData::setType(type p_type) {
	if (p_type >= TYPE_FILL || p_type <= TYPE_IMAGE) {
		vectorType = p_type;
	}
}
void VectorData::setPoints(vector<PointData> p_points) {
	points = p_points;
	count = p_points.size();
}

vector<PointData> VectorData::getPoints() {
	return points;
}

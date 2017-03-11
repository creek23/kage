
#include "vectordata.h"
#include "../../kage.h"

VectorData::VectorData() {
	count = 0;
}
VectorData::VectorData(type p_type) {
	count = 0;
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

VectorData::type VectorData::getType() {
	return vectorType;
}

void VectorData::setPoints(vector<PointData> p_points) {
	points = p_points;
	count = p_points.size();
}

vector<PointData> VectorData::getPoints() {
	return points;
}

VectorData VectorData::clone() {
	VectorData l_vectorData(getType());
	vector<PointData> l_points;
		for (unsigned int i = 0; i < count; ++i) {
			l_points.push_back(points[i].clone());
		}
	l_vectorData.setPoints(l_points);
	l_vectorData.stroke = stroke.clone();
	l_vectorData.fillColor = fillColor.clone();
	
	return l_vectorData;
}

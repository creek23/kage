
#include "vectordatamanager.h"
#include "../kage.h"
#include "stage.h"

VectorDataManager::VectorDataManager() {
//	init(KageStage::fillColor, KageStage::stroke);
// ^ depracated: TYPE_INIT to be used as SHAPE identifier
}

VectorDataManager::VectorDataManager(vector<VectorData> p_vectorData) {
	setVectorData(p_vectorData);
}

VectorDataManager::~VectorDataManager() {
	vectorData.clear();
}

void VectorDataManager::setVectorData(vector<VectorData> p_vectorData) {
	vectorData = p_vectorData;
}

vector<VectorData> VectorDataManager::getVectorData() {
	return vectorData;
}

void VectorDataManager::clear() {
	vectorData.clear();
}

void VectorDataManager::push(VectorDataManager p_vectorsData) {
	vector<VectorData> l_v = p_vectorsData.getVectorData();
	int isrc = l_v.size();
	
	for (int i = 0; i < isrc; ++i) {
		vectorData.push_back(l_v[i]);
	}
}

void VectorDataManager::add(VectorData::type p_type, ColorData p_fill, StrokeColorData p_stroke) {
	VectorData l_vectorData;
	
		l_vectorData.vectorType = p_type;
		l_vectorData.stroke = p_stroke;
		l_vectorData.fillColor = p_fill;
	
	vectorData.push_back(l_vectorData);
}

void VectorDataManager::addInit() {
	add(VectorData::TYPE_INIT, KageStage::fillColor, KageStage::stroke);
}
void VectorDataManager::addFill(ColorData p_color) {
	VectorData l_vectorData;
	if (vectorData.size() > 0) {
		l_vectorData = vectorData[vectorData.size()-1];
	} else {
		l_vectorData.setType(VectorData::TYPE_INIT);
		l_vectorData.fillColor = KageStage::fillColor;
		l_vectorData.stroke = KageStage::stroke;
	}
	add(VectorData::TYPE_FILL, p_color, l_vectorData.stroke);
}

void VectorDataManager::addEndFill() {
	ColorData p_fill(0,0,0,0);
	StrokeColorData p_stroke(0.0);
		p_stroke.setA(0);
	add(VectorData::TYPE_ENDFILL, p_fill, p_stroke);
}

VectorDataManager VectorDataManager::clone() {
	vector<VectorData> l_vectorData;
	unsigned int count = vectorData.size();
		for (unsigned int i = 0; i < count; ++i) {
			l_vectorData.push_back(vectorData[i].clone());
		}
	VectorDataManager l_vectorDataManager(l_vectorData);
	
	return l_vectorDataManager;
}

void VectorDataManager::addCubic(PointData p_point1, PointData p_point2, PointData p_point3) {
	addCurve(p_point1, p_point2, p_point3, VectorData::TYPE_CURVE_CUBIC);
}

void VectorDataManager::addQuadratic(PointData p_point1, PointData p_point2, PointData p_point3) {
	addCurve(p_point1, p_point2, p_point3, VectorData::TYPE_CURVE_QUADRATIC);
}

void VectorDataManager::addLinePoly(PointData p_point, double p_x, double p_y) {
	VectorData l_vectorData;
	if (vectorData.size() > 0) {
		l_vectorData = vectorData[vectorData.size()-1];
	} else {
		l_vectorData.setType(VectorData::TYPE_INIT);
		l_vectorData.fillColor = KageStage::fillColor;
		l_vectorData.stroke = KageStage::stroke;
	}
	add(VectorData::TYPE_CURVE_CUBIC, l_vectorData.fillColor, l_vectorData.stroke);
	
	vector<PointData> ps;
		ps.push_back(
			PointData(
				(p_x + p_point.x)/2,
				(p_y + p_point.y)/2
			)
				);
		ps.push_back(
			PointData(
				(p_x + p_point.x)/2,
				(p_y + p_point.y)/2
			)
				);
		ps.push_back(p_point);
	vectorData[vectorData.size()-1].setPoints(ps);
}

void VectorDataManager::addLine(PointData p_point) {
	vector<PointData> p_prevPoints = vectorData[vectorData.size()-1].getPoints();
	
	VectorData l_vectorData = vectorData[vectorData.size()-1];
	add(VectorData::TYPE_CURVE_CUBIC, l_vectorData.fillColor, l_vectorData.stroke);
	
	vector<PointData> ps;
		ps.push_back(
			PointData(
				(p_prevPoints[p_prevPoints.size()-1].x + p_point.x)/2,
				(p_prevPoints[p_prevPoints.size()-1].y + p_point.y)/2
			)
				);
		ps.push_back(
			PointData(
				(p_prevPoints[p_prevPoints.size()-1].x + p_point.x)/2,
				(p_prevPoints[p_prevPoints.size()-1].y + p_point.y)/2
			)
				);
		ps.push_back(p_point);
	vectorData[vectorData.size()-1].setPoints(ps);
}

void VectorDataManager::addMove(PointData p_point) {
	VectorData l_vectorData;
	if (vectorData.size() > 0) {
		l_vectorData = vectorData[vectorData.size()-1];
	} else {
		l_vectorData.setType(VectorData::TYPE_INIT);
		l_vectorData.fillColor = KageStage::fillColor;
		l_vectorData.stroke = KageStage::stroke;
	}
	add(VectorData::TYPE_MOVE, l_vectorData.fillColor, l_vectorData.stroke);
	
	vector<PointData> ps;
		ps.push_back(p_point);
	vectorData[vectorData.size()-1].setPoints(ps);
}

void VectorDataManager::addLineStyle(StrokeColorData p_stroke) {
	VectorData l_vectorData;
	if (vectorData.size() > 0) {
		l_vectorData = vectorData[vectorData.size()-1];
	} else {
		l_vectorData.setType(VectorData::TYPE_INIT);
		l_vectorData.fillColor = KageStage::fillColor;
		l_vectorData.stroke = KageStage::stroke;
	}
	add(VectorData::TYPE_STROKE, l_vectorData.fillColor, p_stroke);
}

void VectorDataManager::addClosePath() {
	VectorData l_vectorData;
	if (vectorData.size() > 0) {
		l_vectorData = vectorData[vectorData.size()-1];
	} else {
		l_vectorData.setType(VectorData::TYPE_INIT);
		l_vectorData.fillColor = KageStage::fillColor;
		l_vectorData.stroke = KageStage::stroke;
	}
	add(VectorData::TYPE_CLOSE_PATH, l_vectorData.fillColor, l_vectorData.stroke);
}

void VectorDataManager::addCurve(PointData p_point1, PointData p_point2, PointData p_point3, VectorData::type p_curveType) {
	VectorData l_vectorData;
	if (vectorData.size() > 0) {
		l_vectorData = vectorData[vectorData.size()-1];
	} else {
		l_vectorData.setType(VectorData::TYPE_INIT);
		l_vectorData.fillColor = KageStage::fillColor;
		l_vectorData.stroke = KageStage::stroke;
	}
	add(p_curveType, l_vectorData.fillColor, l_vectorData.stroke);
	
	vector<PointData> ps;
		ps.push_back(p_point1);
		ps.push_back(p_point2);
		ps.push_back(p_point3);
	vectorData[vectorData.size()-1].setPoints(ps);
}

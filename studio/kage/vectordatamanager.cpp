
#include "vectordatamanager.h"
#include "kage.h"
#include "stage.h"

VectorDataManager::VectorDataManager() {
	init(KageStage::fillColor, KageStage::stroke);
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
	
	for (int i = 1; i <= isrc-1; ++i) {
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

void VectorDataManager::init(ColorData p_fill, StrokeColorData p_stroke) {
	if (vectorData.size() == 0) {
		add(VectorData::TYPE_INIT, p_fill, p_stroke);
	}
}
void VectorDataManager::addFill(ColorData p_color) {
	std::cout << " VectorDataManager::addFill()" << std::endl;
	std::cout << p_color.getR() << std::endl;
	std::cout << p_color.getG() << std::endl;
	std::cout << p_color.getB() << std::endl;
	add(VectorData::TYPE_FILL, p_color, vectorData[vectorData.size() - 1].stroke);
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
/*
const VectorDataManager& VectorDataManager::operator=( const VectorDataManager& s ) {
	vector<VectorData> l_vectorData;
	unsigned int count = vectorData.size();
		for (unsigned int i = 1; i < count; ++i) {
			l_vectorData.push_back(vectorData[i-1]);
		}
	VectorDataManager l_vectorDataManager(l_vectorData);
	
	return l_vectorDataManager;
}*/

void VectorDataManager::addCubic(PointData p_point1, PointData p_point2) {
	addCurve(p_point1, p_point2, VectorData::TYPE_CURVE_CUBIC);
}

void VectorDataManager::addQuadratic(PointData p_point1, PointData p_point2) {
	addCurve(p_point1, p_point2, VectorData::TYPE_CURVE_QUADRATIC);
}

void VectorDataManager::addLine(PointData p_point) {
	VectorData l_vectorData = vectorData[vectorData.size()-1];
	add(VectorData::TYPE_LINE, l_vectorData.fillColor, l_vectorData.stroke);
	
	vector<PointData> ps;
		ps.push_back(p_point);
	vectorData[vectorData.size()-1].setPoints(ps);
}

void VectorDataManager::addMove(PointData p_point) {
	VectorData l_vectorData = vectorData[vectorData.size()-1];
	add(VectorData::TYPE_MOVE, l_vectorData.fillColor, l_vectorData.stroke);
	
	vector<PointData> ps;
		ps.push_back(p_point);
	vectorData[vectorData.size()-1].setPoints(ps);
}

void VectorDataManager::addLineStyle(StrokeColorData p_stroke) {
	add(VectorData::TYPE_STROKE, vectorData[vectorData.size() - 1].fillColor, p_stroke);
}

void VectorDataManager::addCurve(PointData p_point1, PointData p_point2, VectorData::type p_curveType) {
	VectorData l_vectorData = vectorData[vectorData.size()-1];
	add(p_curveType, l_vectorData.fillColor, l_vectorData.stroke);
	
	vector<PointData> ps;
		ps.push_back(p_point1);
		ps.push_back(p_point2);
	vectorData[vectorData.size()-1].setPoints(ps);
}

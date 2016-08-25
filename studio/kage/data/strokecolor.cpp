
#include "strokecolor.h"

StrokeColorData::StrokeColorData() {
	//
}

StrokeColorData::StrokeColorData(unsigned int p_r, unsigned int p_g, unsigned int p_b) :
	ColorData(p_r, p_g, p_b) {
	//
}

StrokeColorData::StrokeColorData(unsigned int p_r, unsigned int p_g, unsigned int p_b, unsigned int p_a) :
	ColorData(p_r, p_g, p_b, p_a) {
	//
}

StrokeColorData StrokeColorData::clone() {
	StrokeColorData s(getR(), getG(), getB(), getA());
	return s;
}

StrokeColorData::StrokeColorData(double p_thickness) {
	setThickness(p_thickness);
}

StrokeColorData::~StrokeColorData() {
	//
}

void StrokeColorData::setThickness(double p_thickness) {
	if (p_thickness < 0) {
		p_thickness = 0;
	}
	thickness = p_thickness;
}

double StrokeColorData::getThickness() {
	return thickness;
}

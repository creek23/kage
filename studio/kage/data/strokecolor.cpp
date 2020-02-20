
#include "strokecolor.h"

StrokeColorData::StrokeColorData() {
	thickness = 0;
}

StrokeColorData::StrokeColorData(unsigned int p_r, unsigned int p_g, unsigned int p_b) : ColorData(p_r, p_g, p_b) {
	thickness = 0;
}

StrokeColorData::StrokeColorData(unsigned int p_r, unsigned int p_g, unsigned int p_b, unsigned int p_a) : ColorData(p_r, p_g, p_b, p_a) {
	thickness = 0;
}

StrokeColorData StrokeColorData::clone() {
	StrokeColorData s(getR(), getG(), getB(), getA());
	s.setThickness(getThickness());
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

double StrokeColorData::getThickness() const {
	return thickness;
}

std::string StrokeColorData::toString() const {
	std::ostringstream l_ostringstream;
		l_ostringstream << "(" << R << ", " << G << ", " << B << ", " << A << ", " << thickness << ")";
	return l_ostringstream.str();
}

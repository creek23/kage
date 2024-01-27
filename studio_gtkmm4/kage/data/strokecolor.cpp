/*
 * Kage Studio - a simple free and open source vector-based 2D animation software
 * Copyright (C) 2011~2022  Mj Mendoza IV
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.  Or, see <https://www.gnu.org/licenses/>.
 * 
 */

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

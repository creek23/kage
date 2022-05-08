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

#include "color.h"

ColorData::ColorData() {
	setR(255);
	setG(255);
	setB(255);
	setA(255);
}

ColorData::ColorData(unsigned int p_r, unsigned int p_g, unsigned int p_b) {
	setR(p_r);
	setG(p_g);
	setB(p_b);
	setA(255);
}

ColorData::ColorData(unsigned int p_r, unsigned int p_g, unsigned int p_b, unsigned int p_a) {
	setR(p_r);
	setG(p_g);
	setB(p_b);
	setA(p_a);
}

ColorData::~ColorData() {
	//
}

ColorData ColorData::clone() {
	ColorData c(getR(), getG(), getB(), getA());
	return c;
}

bool ColorData::equalTo(ColorData p_color) {
	return (getR() == p_color.getR()
				&& getG() == p_color.getG()
				&& getB() == p_color.getB()
				&& getA() == p_color.getA());
}

void ColorData::copy(ColorData p_color) {
	setR(p_color.getR());
	setG(p_color.getG());
	setB(p_color.getB());
	setA(p_color.getA());
}

std::string ColorData::toString() {
	std::ostringstream l_ostringstream;
		l_ostringstream << "(" << getR() << ", " << getG() << ", " << getB() << ", " << getA() << ")";
	return l_ostringstream.str();
}

void ColorData::setR(unsigned int p_r) {
	if (p_r > 255) { p_r = 255; }
	R = p_r;
}

void ColorData::setG(unsigned int p_g) {
	if (p_g > 255) { p_g = 255; }
	G = p_g;
}

void ColorData::setB(unsigned int p_b) {
	if (p_b > 255) { p_b = 255; }
	B = p_b;
}

void ColorData::setA(unsigned int p_a) {
	if (p_a > 255) { p_a = 255; }
	A = p_a;
}

unsigned int ColorData::getR() const {
	return R;
}

unsigned int ColorData::getG() const {
	return G;
}

unsigned int ColorData::getB() const {
	return B;
}

unsigned int ColorData::getA() const {
	return A;
}

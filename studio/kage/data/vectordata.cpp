/*
 * Kage Studio - a simple free and open source vector-based 2D animation software
 * Copyright (C) 2011~2024  Mj Mendoza IV
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

VectorData::type VectorData::getType() const {
	return vectorType;
}

void VectorData::setPoints(std::vector<PointData> p_points) {
	points = p_points;
	count = p_points.size();
}

std::vector<PointData> VectorData::getPoints() {
	return points;
}

VectorData VectorData::clone() {
	VectorData l_vectorData(getType());
	std::vector<PointData> l_points;
		for (unsigned int i = 0; i < count; ++i) {
			l_points.push_back(points[i].clone());
		}
	l_vectorData.setPoints(l_points);
	l_vectorData.stroke = stroke.clone();
	l_vectorData.fillColor = fillColor.clone();
	
	return l_vectorData;
}

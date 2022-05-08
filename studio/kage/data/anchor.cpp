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

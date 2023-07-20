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

#include "point.h"
#include "../../kage.h"
#include <iostream>

PointData::PointData() {
	x = 0;
	y = 0;
	debug_id = PointData::debug_pts++;
}

PointData::PointData(GdkPoint p) {
	x = (double) (((double)((int)(p.x*100.0f)))/100.0f);
	y = (double) (((double)((int)(p.y*100.0f)))/100.0f);
	debug_id = PointData::debug_pts++;
}

PointData::PointData(double p_x, double p_y) {
	x = p_x;
	y = p_y;
	debug_id = PointData::debug_pts++;
}

PointData PointData::clone() {
	PointData l_p(x, y);
	return l_p;
}

PointData::~PointData() {
	//
}

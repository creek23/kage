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

#ifndef GTKMM_KAGE_DATA_ANCHOR_H
	#define GTKMM_KAGE_DATA_ANCHOR_H
	
	#include <gdk/gdk.h>
	#include "point.h"
	
	class AnchorData: public PointData {
		public:
			enum type {
				TYPE_NONE,
				TYPE_NORTH,
				TYPE_EAST,
				TYPE_WEST,
				TYPE_SOUTH,
				TYPE_NORTH_EAST,
				TYPE_NORTH_WEST,
				TYPE_SOUTH_EAST,
				TYPE_SOUTH_WEST,
				TYPE_MOVE,
				TYPE_ROTATE
			};
			AnchorData();
			AnchorData(GdkPoint p);
			AnchorData(double p_x, double p_y);
			virtual ~AnchorData();
			
			AnchorData clone();
			//const AnchorData& operator=( const AnchorData& p_origin );
			
			unsigned int rotation;
		protected:
	};
#endif //GTKMM_KAGE_DATA_POINT_H

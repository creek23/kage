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

#ifndef GTKMM_KAGE_DATA_VECTOR_H
	#define GTKMM_KAGE_DATA_VECTOR_H
	
	#include <vector>
	#include "color.h"
	#include "point.h"
	#include "strokecolor.h"
	
	class VectorData {
		public:
			enum type {
				TYPE_INIT,            //holds initial fill/stroke; no vectors to use
				TYPE_FILL,            //holds fillColor; no vectors to use
				TYPE_CLOSE_PATH,      //no vectors to use
				TYPE_ENDFILL,         //holds transparent fillColor; no vectors to use
				TYPE_STROKE,          //holds strokeColor
				TYPE_MOVE,            //moves p0 to p1; p1 is to be stored
				TYPE_LINE,            //draws a line from p0 to p1
				TYPE_CURVE_QUADRATIC, //draws a curve from p0 with kink to p1 before ending at p2 then; p1 and p2 is to be stored
				TYPE_CURVE_CUBIC,     //draws a curve from p0 with kink to p1 before ending at p2 and another from p2 with kink to p3 then ends at p4
				TYPE_TEXT,            //TODO: handle string content
				TYPE_IMAGE            //TODO: think of how to handle blits from imageB to imageA
			};
			
			VectorData();
			VectorData(type p_type);
			virtual ~VectorData();
			
			void setType(type p_type);
			type getType() const;
			void setPoints(std::vector<PointData> p_points);
			std::vector<PointData> getPoints();
			StrokeColorData stroke;
			ColorData fillColor;
			std::string fillColorGradientID;
			unsigned int count;
			
			type vectorType;
			std::vector<PointData> points;
			
			VectorData clone();
		protected:
			
			long lImg;  //index reference to array of typeImage
	};
#endif //GTKMM_KAGE_DATA_VECTOR_H

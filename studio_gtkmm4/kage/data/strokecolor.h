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

#ifndef GTKMM_KAGE_DATA_STROKECOLOR_H
	#define GTKMM_KAGE_DATA_STROKECOLOR_H
	
	#include "color.h"

	class StrokeColorData: public ColorData {
		public:
			StrokeColorData();
			StrokeColorData(unsigned int p_r, unsigned int p_g, unsigned int p_b);
			StrokeColorData(unsigned int p_r, unsigned int p_g, unsigned int p_b, unsigned int p_a);
			StrokeColorData(double p_thickness);
			virtual ~StrokeColorData();
			
			//ColorData color;
			void setThickness(double p_thickness);
			double getThickness() const;
			StrokeColorData clone();
			std::string toString() const;
		protected:
			double thickness;
	};
#endif //GTKMM_KAGE_DATA_STROKECOLOR_H

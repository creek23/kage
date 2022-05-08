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

#ifndef GTKMM_KAGE_DATA_COLOR_H
	#define GTKMM_KAGE_DATA_COLOR_H
	
	#include <string>
	#include <sstream>
	
	class ColorData {
		public:
			ColorData();
			ColorData(unsigned int p_r, unsigned int p_g, unsigned int p_b);
			ColorData(unsigned int p_r, unsigned int p_g, unsigned int p_b, unsigned int p_a);
			virtual ~ColorData();
			
			void setR(unsigned int p_r);
			void setG(unsigned int p_g);
			void setB(unsigned int p_b);
			void setA(unsigned int p_a);
			unsigned int getR() const;
			unsigned int getG() const;
			unsigned int getB() const;
			unsigned int getA() const;
			ColorData clone();
			std::string toString();
			void copy(ColorData p_color);
			bool equalTo(ColorData p_color);
		protected:
			unsigned int R;
			unsigned int G;
			unsigned int B;
			unsigned int A;
	};
#endif //GTKMM_KAGE_DATA_COLOR_H

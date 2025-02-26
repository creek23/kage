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

#ifndef GTKMM_KAGE_MANAGER_UNRE_KAGEDO_H
	#define GTKMM_KAGE_MANAGER_UNRE_KAGEDO_H
	
	#include <iostream>
	
	#include "../data/vectordata.h"
	
	using namespace std;
	
	class KageDo {
		public:
			unsigned int _frame;
			unsigned int _layer;
			KageDo();
			KageDo(unsigned int p_layer, unsigned int p_frame);
			virtual ~KageDo();
			
			void setVectorData(vector<VectorData> p_vectorData);
			vector<VectorData> getVectorData();
			KageDo clone();
			
			void clear();
			
			void push(KageDo p_vectorsData);
			
			bool isEmpty();
		protected:
			void add(VectorData::type p_type, ColorData p_fill, StrokeColorData p_stroke);
			
			vector<VectorData> _vectorData;
	};
#endif //GTKMM_KAGE_MANAGER_UNRE_KAGEDO_H

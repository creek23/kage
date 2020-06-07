/*
 * Kage Studio - a simple free and open source vector-based 2D animation software
 * Copyright (C) 2011~2020  Mj Mendoza IV
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

#ifndef GTKMM_KAGE_MANAGER_DATA_VECTOR_Hx
	#define GTKMM_KAGE_MANAGER_DATA_VECTOR_Hx
	
	#include <vector>
	#include <iostream>
	
	#include "data/color.h"
	#include "data/point.h"
	#include "data/strokecolor.h"
	#include "data/vectordata.h"
	
	class Kage; //forward declaration
	
	using namespace std;
	
	class VectorDataManager {
		public:
			static unsigned int idmaker;
			VectorDataManager();
			VectorDataManager(vector<VectorData> p_vectorData);
			virtual ~VectorDataManager();
			
			void setVectorData(vector<VectorData> p_vectorData);
			vector<VectorData> getVectorData();
			VectorDataManager clone();
			
			void clear();
			void addInit();
			void addInit(PointData p_point);
			void addFill(ColorData p_color);
			void addEndFill();
			void addClosePath();
			void addCubic(PointData p_point1, PointData p_point2, PointData p_point3);
			void addQuadratic(PointData p_point1, PointData p_point2, PointData p_point3);
			void addLine(PointData p_point);
			void addLinePoly(PointData p_point, double p_x, double p_y);
			void addMove(PointData p_point);
			void addLineStyle(StrokeColorData p_stroke);
			
//			void init(ColorData p_fill, StrokeColorData p_stroke);
			void push(VectorDataManager p_vectorsData);
			
			bool isEmpty();
			
			vector<unsigned int> raiseSelectedShape(vector<unsigned int> p_selectedShapes);
			vector<unsigned int> lowerSelectedShape(vector<unsigned int> p_selectedShapes);
			vector<unsigned int> raiseToTopSelectedShape(vector<unsigned int> p_selectedShapes);
			vector<unsigned int> lowerToBottomSelectedShape(vector<unsigned int> p_selectedShapes);
			vector<unsigned int> groupSelectedShapes(vector<unsigned int> p_selectedShapes);
			vector<unsigned int> ungroupSelectedShapes(vector<unsigned int> p_selectedShapes);
			vector<unsigned int> duplicateShapes(vector<unsigned int> p_selectedShapes);
			bool flipHorizontalSelectedShape(vector<unsigned int> p_selectedShapes);
			bool flipVerticalSelectedShape(vector<unsigned int> p_selectedShapes);
			
			bool recenterRotationPoint(vector<unsigned int> p_selectedShapes);
		protected:
			void add(VectorData::type p_type, ColorData p_fill, StrokeColorData p_stroke);
			
			vector<VectorData> vectorData;
			
			void addCurve(PointData p_point1, PointData p_point2, PointData p_point3, VectorData::type p_curveType);
			
			//for use on z-ordering~ish
			vector<VectorData> copySelectedShapes(vector<unsigned int> p_selectedShapes);
			vector<unsigned int> pasteSelectedShapes(vector<VectorData> p_vectorDataCopyBuffer);
			bool deleteSelectedShapes(vector<unsigned int> p_selectedShapes);
	};
#endif //GTKMM_KAGE_MANAGER_DATA_VECTOR_Hx

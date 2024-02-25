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

#ifndef GTKMM_KAGE_MANAGER_DATA_VECTOR_Hx
	#define GTKMM_KAGE_MANAGER_DATA_VECTOR_Hx
	
	#include <vector>
	#include <iostream>
	
	#include "data/color.h"
	#include "data/point.h"
	#include "data/strokecolor.h"
	#include "data/vectordata.h"
	
	class Kage; //forward declaration
	
	class VectorDataManager {
		public:
			static unsigned int idmaker;
			VectorDataManager();
			VectorDataManager(std::vector<VectorData> p_vectorData);
			virtual ~VectorDataManager();
			
			void setVectorData(std::vector<VectorData> p_vectorData);
			std::vector<VectorData> getVectorData();
			VectorDataManager clone();
			
			void clear();
			void addInit();
			void addInit(PointData p_point);
			void addFill(ColorData p_color);
			void addFill(std::string p_gradientID);
			void addEndFill();
			void addClosePath();
			void addCubic(PointData p_point1, PointData p_point2, PointData p_point3);
			void addQuadratic(PointData p_point1, PointData p_point2, PointData p_point3);
			void addLine(PointData p_point);
			void addLinePoly(PointData p_point, double p_x, double p_y);
			void addMove(PointData p_point);
			void addLineStyle(StrokeColorData p_stroke);
			
			void addImage(PointData p_IDandBuff, PointData p_xy, PointData p_size, PointData p_scale, PointData p_rotateAlpha);

//			void init(ColorData p_fill, StrokeColorData p_stroke);
			void push(VectorDataManager p_vectorsData);
			
			bool isEmpty();
			
			//for use on z-ordering~ish
			std::vector<unsigned int> raiseSelectedShape(std::vector<unsigned int> p_selectedShapes);
			std::vector<unsigned int> lowerSelectedShape(std::vector<unsigned int> p_selectedShapes);
			std::vector<unsigned int> raiseToTopSelectedShape(std::vector<unsigned int> p_selectedShapes);
			std::vector<unsigned int> lowerToBottomSelectedShape(std::vector<unsigned int> p_selectedShapes);
			std::vector<unsigned int> groupSelectedShapes(std::vector<unsigned int> p_selectedShapes);
			std::vector<unsigned int> ungroupSelectedShapes(std::vector<unsigned int> p_selectedShapes);
			std::vector<unsigned int> duplicateShapes(std::vector<unsigned int> p_selectedShapes);
			bool flipHorizontalSelectedShape(std::vector<unsigned int> p_selectedShapes);
			bool flipVerticalSelectedShape(std::vector<unsigned int> p_selectedShapes);
			
			bool recenterRotationPoint(std::vector<unsigned int> p_selectedShapes);
			
			const unsigned int _NO_SELECTION = -1;
			std::vector<unsigned int> _selectedNodes;
			std::vector<unsigned int> _selectedShapes;
			unsigned int getShape(unsigned int p_index);
			bool isSelectedShape(unsigned int p_index);
			void addSelectedShape(unsigned int p_index);
			std::vector<unsigned int> tryMultiSelectShapes_populateShapes();
			std::vector<unsigned int> selectAllShapes();
			bool cutSelectedShapes();
			std::vector<VectorData> copySelectedShapes(std::vector<unsigned int> p_selectedShapes);
			std::vector<unsigned int> pasteSelectedShapes(std::vector<VectorData> p_vectorDataCopyBuffer);
			bool deleteSelectedShapes(std::vector<unsigned int> p_selectedShapes);
			
			void add(VectorData::type p_type, ColorData p_fill, StrokeColorData p_stroke);
			void add(VectorData::type p_type, std::string p_fillGradientID, StrokeColorData p_stroke);
			
			std::vector<VectorData> vectorData;
			
			void addCurve(PointData p_point1, PointData p_point2, PointData p_point3, VectorData::type p_curveType);
	};
#endif //GTKMM_KAGE_MANAGER_DATA_VECTOR_Hx

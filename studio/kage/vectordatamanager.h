
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
			
			vector<unsigned int> raiseSelectedShape(vector<unsigned int> p_selectedShapes);
			vector<unsigned int> lowerSelectedShape(vector<unsigned int> p_selectedShapes);
			vector<unsigned int> raiseToTopSelectedShape(vector<unsigned int> p_selectedShapes);
			vector<unsigned int> lowerToBottomSelectedShape(vector<unsigned int> p_selectedShapes);
			vector<unsigned int> groupSelectedShapes(vector<unsigned int> p_selectedShapes);
			vector<unsigned int> ungroupSelectedShapes(vector<unsigned int> p_selectedShapes);
			vector<unsigned int> duplicateShapes(vector<unsigned int> p_selectedShapes);
			bool flipHorizontalSelectedShape(vector<unsigned int> p_selectedShapes);
			bool flipVerticalSelectedShape(vector<unsigned int> p_selectedShapes);
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

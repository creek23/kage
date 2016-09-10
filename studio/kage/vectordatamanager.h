
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
			void addFill(ColorData p_color);
			void addEndFill();
			void addCubic(PointData p_point1, PointData p_point2);
			void addQuadratic(PointData p_point1, PointData p_point2);
			void addLine(PointData p_point);
			void addMove(PointData p_point);
			void addLineStyle(StrokeColorData p_stroke);
			
			void init(ColorData p_fill, StrokeColorData p_stroke);
			void push(VectorDataManager p_vectorsData);
		protected:
			void add(VectorData::type p_type, ColorData p_fill, StrokeColorData p_stroke);
			
			vector<VectorData> vectorData;
			
			void addCurve(PointData p_point1, PointData p_point2, VectorData::type p_curveType);
	};
#endif //GTKMM_KAGE_MANAGER_DATA_VECTOR_Hx

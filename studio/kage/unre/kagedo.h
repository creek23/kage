
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

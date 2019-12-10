
#ifndef GTKMM_KAGE_MANAGER_UNRE_UNRE_H
	#define GTKMM_KAGE_MANAGER_UNRE_UNRE_H
	
	#include <vector>
	#include <iostream>
	
	#include "../data/vectordata.h"
	#include "../data/point.h"
	
	#include "kagedo.h"
	
	using namespace std;
	
	class UnRe {
		public:
			UnRe();
			virtual ~UnRe();
			
			void stackDo(unsigned int p_layer, unsigned int p_frame, vector<VectorData> p_data);
			KageDo undo();
			KageDo redo();
			
			void clear();
			
			void applyZoom(PointData p_originBefore, PointData p_originAfter, PointData p_zoomReference, double p_zoomRatio);
		protected:
			unsigned int _stackIndex;
			vector<KageDo> _undoStack;
			
			///for use of applyZoom
			PointData applyZoomRatio(PointData p_zoomReference, double p_zoomRatio, PointData p_value);
	};
#endif //GTKMM_KAGE_MANAGER_UNRE_UNRE_H

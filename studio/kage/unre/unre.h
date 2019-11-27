
#ifndef GTKMM_KAGE_MANAGER_UNRE_UNRE_H
	#define GTKMM_KAGE_MANAGER_UNRE_UNRE_H
	
	#include <vector>
	#include <iostream>
	
	#include "../data/vectordata.h"
	
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
		protected:
			unsigned int _stackIndex;
			vector<KageDo> _undoStack;
	};
#endif //GTKMM_KAGE_MANAGER_UNRE_UNRE_H

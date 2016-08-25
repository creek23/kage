#ifndef GTKMM_KAGE_MANAGER_FRAME_H
	#define GTKMM_KAGE_MANAGER_FRAME_H
	
	#include <gtkmm/box.h>
	#include "frame.h"
	
	class KageFramesManager; //forward declaration
	
	class KageFrameManager : public Gtk::HBox {
		public:
			KageFrameManager(KageFramesManager *p_fsm, unsigned int p_layerID, unsigned int p_frameCount);
			virtual ~KageFrameManager();
			unsigned int getID();
			bool selectAll(bool p_selectAll);
			unsigned int getFrameCount();
			void addFrame();
			void extendFrame(unsigned int p_frameID);
			void setCurrentFrame(unsigned int p_frame);
			void focusFrame(unsigned int p_frame);
			
			KageFramesManager *getFsm();
			KageFrame *getFrameAt(unsigned int p_frame);
		protected:
			unsigned int layerID;
//			unsigned int layerCtr;
			unsigned int frameCtr;
			std::vector<KageFrame*> frames;
			
			KageFramesManager *fsm;
	};
#endif // GTKMM_KAGE_MANAGER_FRAME_H

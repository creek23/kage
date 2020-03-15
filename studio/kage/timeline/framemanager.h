#ifndef GTKMM_KAGE_MANAGER_FRAME_H
	#define GTKMM_KAGE_MANAGER_FRAME_H
	
	#include <gtkmm/box.h>
	#include "frame.h"
	#include "frame_padding.h"
	
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
			
			void removeFrame(unsigned int p_frameID);
			void removeAllFrames();
			
			KageFramesManager *getFsm();
			KageFrame *getFrameAt(unsigned int p_frame);
			
			void setSelected(KageFrame *p_frame);
			
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
			unsigned int layerID;
//			unsigned int layerCtr;
			unsigned int frameCtr;
			std::vector<KageFrame*> frames;
			KageFramePadding frame_padding;
			
			KageFramesManager *fsm;
			unsigned int _currentFrameID;
			unsigned int _currentFrameIndex;
	};
#endif // GTKMM_KAGE_MANAGER_FRAME_H

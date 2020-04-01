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
			void duplicateFrame();
			void extendFrame();
			unsigned int getCurrentFrame();
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
				bool doFlipVerticalSelectedShapeOn(unsigned int p_frameIndex, vector<unsigned int> p_selectedShapes);
				bool doFlipVerticalSelectedShapeOnExtendedFrame(unsigned int p_frameIndex, vector<unsigned int> p_selectedShapes);
			
			bool addDataToFrame(VectorDataManager p_vectorsData);
			bool setFrameData(VectorDataManager p_vectorsData);
			VectorDataManager getFrameData();
			VectorDataManager getFrameDataAt(unsigned int p_frame); ///NOTE: "Frame" -- NOT "frame ID"
			VectorDataManager getPreviousFrameData(unsigned int p_frameID); ///Called by KageFrame if KageFrame is Extended
			bool setFrameDataToPreviousFrame(VectorDataManager p_vectorsData, unsigned int p_frameID); ///Called by KageFrame if KageFrame is Extended
			bool addDataToPreviousFrame(VectorDataManager v, unsigned int p_frameID); ///Called by KageFrame if KageFrame is Extended
			
			bool switchToPreviousFrame(unsigned int p_frameID); ///Called by KageFrame for navigating to Previous Frame
			bool switchToNextFrame(unsigned int p_frameID); ///Called by KageFrame for navigating to Next Frame
			bool isCurrentFrame(unsigned int p_frameID); ///Called by KageFrame when rendering
		protected:
			unsigned int layerID;
//			unsigned int layerCtr;
			unsigned int frameCtr;
			std::vector<KageFrame*> frames;
			KageFramePadding frame_padding;
			
			KageFramesManager *fsm;
			unsigned int _currentFrameID;
			unsigned int _currentFrameIndex;
			
			bool moveToLeft();                       ///Moves Current Frame to Left
			bool moveToLeftAt(unsigned int p_frame); ///Moves Current Frame to Left on indicated Frame
			bool moveToLeftMost();                   ///Moves Current Frame to Left
	};
#endif // GTKMM_KAGE_MANAGER_FRAME_H

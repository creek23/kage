#ifndef GTKMM_KAGE_MANAGER_FRAMES_H
	#define GTKMM_KAGE_MANAGER_FRAMES_H
	
	#include <gtkmm/box.h>
	#include <gtkmm/window.h>
	#include <gtkmm/label.h>
	#include "framemanager.h"
	
	class Kage;
	
	class KageFramesManager : public Gtk::VBox {
		public:
			static unsigned int currentFrame;
			
			KageFramesManager(Kage *p_win);
			virtual ~KageFramesManager();
			void addFrameManager(unsigned int p_layer);
			void deleteFrameManager(unsigned int p_layer);
			bool addFrame();
			bool extendFrame(unsigned int p_frameID);
			void setCurrentFrame(unsigned int p_currentFrame);
			unsigned int getCurrentFrame();
			void setCurrentLayer(unsigned int p_currentLayer);
			void setCurrentLayerByID(unsigned int p_layerID);
			unsigned int getCurrentLayer(); //currently used by getFrame()
			unsigned int layerCount();
			unsigned int frameCount();
			void selectAll(bool p_selectAll); //used by KageFrame
			KageFrame *getFrame();
			
			void renderStage(); //used by KageFrame
			
			bool removeFrame(unsigned int p_frameID);
			bool removeAllFrames();
			
			bool moveToTop();
			bool moveUp();
			bool moveDown();
			bool moveToBottom();
		protected:
			Kage *win;
			Gtk::Label place_holder;
			std::vector<KageFrameManager*> framemanager;
	};
#endif // GTKMM_KAGE_MANAGER_FRAMES_H

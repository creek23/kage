/*
 * Kage Studio - a simple free and open source vector-based 2D animation software
 * Copyright (C) 2011~2022  Mj Mendoza IV
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

#ifndef GTKMM_KAGE_MANAGER_FRAMES_H
	#define GTKMM_KAGE_MANAGER_FRAMES_H
	
	#include <gtkmm/box.h>
	#include <gtkmm/window.h>
	#include <gtkmm/label.h>
	#include "frameset.h"
	
	class Kage;
	
	class KageFramesetManager : public Gtk::Box {
		public:
			static bool LOADING_MODE;
			KageFramesetManager(Kage *p_win);
			virtual ~KageFramesetManager();
			void addFrameset(unsigned int p_layer);
			void deleteFrameset(unsigned int p_layer);
			bool addFrame();
			bool extendFrame();
			bool duplicateFrame();
			bool removeFrame();
			void setCurrentFrame(unsigned int p_frame);
			void setCurrentFrameByID(unsigned int p_frameID);
			unsigned int getCurrentFrame();
			void setCurrentLayer(unsigned int p_currentLayer);
			void setCurrentLayerByID(unsigned int p_layerID);
			unsigned int getCurrentLayer(); //currently used by getFrame()
			unsigned int layerCount();
			unsigned int frameCount();
			void selectAll(bool p_selectAll); //used by KageFrame
			KageFrame *getFrame();
			KageFrame *getFrameAt(unsigned int p_frame);
			
			void renderStage(); //used by KageFrame
			
			bool removeAllFrames();
			
			bool moveToTop();
			bool moveUp();
			bool moveDown();
			bool moveToBottom();
			
			vector<unsigned int> raiseSelectedShape(vector<unsigned int> p_selectedShapes);
			vector<unsigned int> lowerSelectedShape(vector<unsigned int> p_selectedShapes);
			vector<unsigned int> raiseToTopSelectedShape(vector<unsigned int> p_selectedShapes);
			vector<unsigned int> lowerToBottomSelectedShape(vector<unsigned int> p_selectedShapes);
			vector<unsigned int> groupSelectedShapes(vector<unsigned int> p_selectedShapes);
			vector<unsigned int> ungroupSelectedShapes(vector<unsigned int> p_selectedShapes);
			vector<unsigned int> duplicateShapes(vector<unsigned int> p_selectedShapes);
			bool flipHorizontalSelectedShape(vector<unsigned int> p_selectedShapes);
			bool flipVerticalSelectedShape(vector<unsigned int> p_selectedShapes);
			
			bool recenterRotationPoint(vector<unsigned int> p_selectedShapes);
			
			bool addDataToFrame(VectorDataManager p_vectorsData);
			bool setFrameData(VectorDataManager p_vectorsData);
			VectorDataManager getFrameData();
			VectorDataManager getFrameDataAt(unsigned int p_frame);  ///NOTE: "Frame" -- NOT "frame ID"
			
			bool forceSetTween(unsigned int p_tween);
			bool setTween(unsigned int p_tween);
			unsigned int getTween();
			
			void switchToPreviousFrame(); ///Called by Kage for navigating to Previous Frame
			void switchToPreviousFrame(unsigned int p_frameID); ///Called by KageFrame for navigating to Previous Frame
			void switchToNextFrame(); ///Called by Kage for navigating to Next Frame
			void switchToNextFrame(unsigned int p_frameID); ///Called by KageFrame for navigating to Next Frame
			
			void setFrameExtension(KageFrame::extension p_extension);
		protected:
			Kage *win;
			std::vector<KageFrameset*> _framesets;
	};
#endif // GTKMM_KAGE_MANAGER_FRAMES_H

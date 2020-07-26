/*
 * Kage Studio - a simple free and open source vector-based 2D animation software
 * Copyright (C) 2011~2020  Mj Mendoza IV
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

#ifndef GTKMM_KAGE_MANAGER_FRAME_H
	#define GTKMM_KAGE_MANAGER_FRAME_H
	
	#include <gtkmm/box.h>
	#include "frame.h"
	#include "frame_padding.h"
	
	class KageFramesetManager; //forward declaration
	
	class KageFrameset : public Gtk::HBox {
		public:
			KageFrameset(KageFramesetManager *p_fsm, unsigned int p_layerID, unsigned int p_frameCount);
			virtual ~KageFrameset();
			unsigned int getID();
			bool selectAll(bool p_selectAll);
			unsigned int getFrameCount();
			bool addFrame();
			void duplicateFrame();
			void extendFrame();
			bool removeFrame();
			unsigned int getCurrentFrame();
			void setCurrentFrame(unsigned int p_frame);
			void setCurrentFrameByID(unsigned int p_frameID);
			void focusFrame(unsigned int p_frame);
			
			void removeAllFrames();
			
			KageFramesetManager *getFsm();
			KageFrame *getFrameAt(unsigned int p_frame);
			unsigned int getFrameNumberByID(unsigned int p_frameID);
			
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
			
			bool recenterRotationPoint(vector<unsigned int> p_selectedShapes);
			
			bool addDataToFrame(VectorDataManager p_vectorsData);
			bool setFrameData(VectorDataManager p_vectorsData);
			VectorDataManager getFrameTweenData(unsigned int p_frameIndex);
			VectorDataManager getFrameData();
			VectorDataManager getFrameDataAt(unsigned int p_frame); ///NOTE: "Frame" -- NOT "frame ID"
			VectorDataManager getPreviousFrameData(unsigned int p_frameID); ///Called by KageFrame if KageFrame is Extended
			bool setFrameDataToPreviousFrame(VectorDataManager p_vectorsData, unsigned int p_frameID); ///Called by KageFrame if KageFrame is Extended
			bool addDataToPreviousFrame(VectorDataManager v, unsigned int p_frameID); ///Called by KageFrame if KageFrame is Extended
			
			bool setPreviousFrameTween(unsigned int p_frameID, unsigned int p_tween);
			bool setExtendedFrameTween(unsigned int p_frameID, unsigned int p_tween);
			bool forceSetTween(unsigned int p_tween);
			bool setTween(unsigned int p_tween);
			unsigned int getTween();
			
			bool switchToPreviousFrame(); ///Called by FrameSet for navigating to Previous Frame
			bool switchToPreviousFrame(unsigned int p_frameID); ///Called by KageFrame for navigating to Previous Frame
			bool switchToNextFrame(); ///Called by FrameSet for navigating to Next Frame
			bool switchToNextFrame(unsigned int p_frameID); ///Called by KageFrame for navigating to Next Frame
			bool isCurrentFrame(unsigned int p_frameID); ///Called by KageFrame when rendering
			bool canReUseNextFrame(); ///Called by KageFramesetManager when duplicating Frame
			
			void setFrameExtension(KageFrame::extension p_extension);
		protected:
			unsigned int layerID;
//			unsigned int layerCtr;
			unsigned int frameCtr;
			std::vector<KageFrame*> _frames;
			KageFramePadding frame_padding;
			
			KageFramesetManager *fsm;
			unsigned int _currentFrameID;
			unsigned int _currentFrameIndex;
			
			bool moveToLeft();                       ///Moves Current Frame to Left
			bool moveToLeftAt(unsigned int p_frame); ///Moves Current Frame to Left on indicated Frame
			bool moveToLeftMost();                   ///Moves Current Frame to Left
	};
#endif // GTKMM_KAGE_MANAGER_FRAME_H

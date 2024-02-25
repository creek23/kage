/*
 * Kage Studio - a simple free and open source vector-based 2D animation software
 * Copyright (C) 2011~2024  Mj Mendoza IV
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
#ifndef GTKMM_KAGE_LAYER_H
	#define GTKMM_KAGE_LAYER_H
	
	#include <vector>
	#include "color.h"

	#include "frame.h"
	#include "../vectordatamanager.h"

	#include "../../util/string/stringhelper.h"
	
	#include <gdkmm/color.h>
	class Kage; //forward declaration
	
	class KageScene; //forward declaration

	class KageLayer {
		public:
			unsigned int layerID;
			std::vector<KageFrame*> Frames;
			KageFrame* _framePtr;

			KageLayer(KageScene *p_scene, unsigned int p_layerID, unsigned int p_frameCount);
			KageLayer operator=(const KageLayer &p_layer);
			virtual ~KageLayer();
			unsigned int getID();
			bool selectAllFrame(bool p_selectAll);
			bool addFrame();
			void duplicateFrame();
			void extendFrame();
			bool removeFrame();
			unsigned int getCurrentFrame();
			void setCurrentFrame(unsigned int p_frame, bool p_addSelected);
			void setCurrentFrameByID(unsigned int p_frameID);
			
			bool removeAllFrames();
			
			KageScene *getScene();
			KageFrame *getFrame();
			KageFrame *getFrameAt(unsigned int p_frame);
			unsigned int getFrameNumberByID(unsigned int p_frameID);
			
			void setSelected(KageFrame *p_frame);
			
			std::vector<unsigned int> raiseSelectedShape(std::vector<unsigned int> p_selectedShapes);
			std::vector<unsigned int> lowerSelectedShape(std::vector<unsigned int> p_selectedShapes);
			std::vector<unsigned int> raiseToTopSelectedShape(std::vector<unsigned int> p_selectedShapes);
			std::vector<unsigned int> lowerToBottomSelectedShape(std::vector<unsigned int> p_selectedShapes);
			std::vector<unsigned int> groupSelectedShapes(std::vector<unsigned int> p_selectedShapes);
			std::vector<unsigned int> ungroupSelectedShapes(std::vector<unsigned int> p_selectedShapes);
			std::vector<unsigned int> duplicateShapes(std::vector<unsigned int> p_selectedShapes);
			bool flipHorizontalSelectedShape(std::vector<unsigned int> p_selectedShapes);
			bool flipVerticalSelectedShape(std::vector<unsigned int> p_selectedShapes);
				bool doFlipVerticalSelectedShapeOn(unsigned int p_frameIndex, std::vector<unsigned int> p_selectedShapes);
				bool doFlipVerticalSelectedShapeOnExtendedFrame(unsigned int p_frameIndex, std::vector<unsigned int> p_selectedShapes);
			
			bool recenterRotationPoint(std::vector<unsigned int> p_selectedShapes);
			
			bool addDataToFrame(VectorDataManager p_vectorsData);
			bool setFrameData(VectorDataManager p_vectorsData);
			VectorDataManager getFrameTweenData(unsigned int p_frameIndex);
			VectorDataManager getFrameData();
			VectorDataManager getFrameDataAt(unsigned int p_frame, bool p_frameOnion, bool p_layerOnion); ///NOTE: "Frame" -- NOT "frame ID"
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
			bool canReUseNextFrame(); ///Called by KageScene when duplicating Frame
			
			void setFrameExtension(KageFrame::extension p_extension);
			unsigned int getActiveFrameID();

			unsigned int frameCtr;

			KageScene *_scene;
			unsigned int _currentFrameID;
			unsigned int _currentFrameIndex;
			
			bool moveToLeft();                       ///Moves Current Frame to Left
			bool moveToLeftAt(unsigned int p_frame); ///Moves Current Frame to Left on indicated Frame
			bool moveToLeftMost();                   ///Moves Current Frame to Left

			unsigned int _activeFrame;

			//GUI-related
			bool _selected;
			bool _visible;
			bool _lock;
			std::string _label;

			//GUI-related
			void setSelected(bool p_selected);
			bool isSelected();
			
			void setVisible(bool p_visible);
			bool isVisible();
			void toggleVisibility();
			
			void setLock(bool p_lock);
			bool isLocked();
			void toggleLock();
			
			void setLabel(std::string p_label);
			std::string getLabel();
	};
#endif //GTKMM_KAGE_LAYER_H
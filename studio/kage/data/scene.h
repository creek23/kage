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
#ifndef GTKMM_KAGE_SCENE_H
	#define GTKMM_KAGE_SCENE_H
	
	#include <vector>
	#include "color.h"
    
    #include "frame.h"
    #include "layer.h"
    #include "../vectordatamanager.h"

	#include "../../util/string/stringhelper.h"
	
	#include <gdkmm/color.h>
	//class Kage; //forward declaration
	
	class KageFrame; //forward declaration
	class KageLayer; //forward declaration

	class KageDocument; //forward declaration

	class KageScene {
		public:
			void init(KageDocument *p_document, unsigned int p_sceneID);

			unsigned int sceneID;
			bool _selected;
			bool _saved;
			unsigned int _activeLayerID;
			unsigned int _activeLayerIndex;
			unsigned int _activeLayer; //effectively _activeLayerIndex+1

			unsigned int layerCtr; ///will be used to create Unique LayerUI IDs
			
			std::string _label; //Scene label
		
			static bool LOADING_MODE;
			KageScene(KageDocument *p_document, unsigned int p_sceneID, std::string p_filepath);
			KageScene operator=(const KageScene &p_scene);

			unsigned int getID();

			std::vector<KageLayer *> Layers;
			KageLayer* _layerPtr;

			bool open(std::string p_filepath);
			bool save();
			
			void setSelected(bool p_selected);
			bool isSelected();
			bool isSaved();
			
			KageScene(KageDocument *p_document, unsigned int p_sceneID);
			KageScene(KageDocument *p_document, unsigned int p_sceneID, KageScene *p_scene);
			virtual ~KageScene();
			void addLayer();
			void deleteLayer(unsigned int p_layerID);
			bool removeLayerAt(unsigned int p_layerIndex);
			bool addLayerFrame();
			bool extendLayerFrame();
			bool duplicateLayerFrame();
			bool removeLayerFrame();
			void setLayerCurrentFrame(unsigned int p_frame, bool p_addSelected);
			void setCurrentFrameByID(unsigned int p_frameID);
			unsigned int getCurrentFrame();
			void setCurrentLayer(unsigned int p_currentLayer, bool p_addSelected);
			void setCurrentLayerByID(unsigned int p_layerID);
			unsigned int getCurrentLayer(); //currently used by getFrame()
			unsigned int frameCount();
			void selectAllLayerFrame(bool p_selectAll);
			KageFrame *getFrame();
			KageFrame *getFrameAt(unsigned int p_frame);
			KageLayer *getLayer();
			KageLayer *getLayerAt(unsigned int p_layer);
			
			bool removeAllFrames();
			
			bool moveToTop();
			bool moveUp();
			bool moveDown();
			bool moveToBottom();
			
			std::vector<unsigned int> raiseSelectedShape(std::vector<unsigned int> p_selectedShapes);
			std::vector<unsigned int> lowerSelectedShape(std::vector<unsigned int> p_selectedShapes);
			std::vector<unsigned int> raiseToTopSelectedShape(std::vector<unsigned int> p_selectedShapes);
			std::vector<unsigned int> lowerToBottomSelectedShape(std::vector<unsigned int> p_selectedShapes);
			std::vector<unsigned int> groupSelectedShapes(std::vector<unsigned int> p_selectedShapes);
			std::vector<unsigned int> ungroupSelectedShapes(std::vector<unsigned int> p_selectedShapes);
			std::vector<unsigned int> duplicateShapes(std::vector<unsigned int> p_selectedShapes);
			bool flipHorizontalSelectedShape(std::vector<unsigned int> p_selectedShapes);
			bool flipVerticalSelectedShape(std::vector<unsigned int> p_selectedShapes);
			
			bool recenterRotationPoint(std::vector<unsigned int> p_selectedShapes);
			
			bool addDataToFrame(VectorDataManager p_vectorsData);
			bool setFrameData(VectorDataManager p_vectorsData);
			VectorDataManager getFrameData();
			VectorDataManager getFrameDataAt(unsigned int p_frame, bool p_frameOnion, bool p_layerOnion);  ///NOTE: "Frame" -- NOT "frame ID"
			
			bool forceSetTween(unsigned int p_tween);
			bool setTween(unsigned int p_tween);
			unsigned int getTween();
			
			bool layerSwitchToPreviousFrame(); ///Called by Kage for navigating to Previous Frame
			bool layerSwitchToPreviousFrame(unsigned int p_frameID); ///Called by KageFrame for navigating to Previous Frame
			bool layerSwitchToNextFrame(); ///Called by Kage for navigating to Next Frame
			bool layerSwitchToNextFrame(unsigned int p_frameID); ///Called by KageFrame for navigating to Next Frame
			
			void setFrameExtension(KageFrame::extension p_extension);

			void extendFrameAt(unsigned int p_layerIndex, unsigned int p_frameIndex, unsigned int p_frameCount);
			//Kage *_kage;
			KageDocument *_document;
			KageScene *_parent;

			unsigned int getActiveLayerID();
			std::string getLayerLabel();
			void setLayerLabel(std::string p_label);
			void toggleLayerVisibility();
			bool isLayerVisible();
			void setLayerVisible(bool p_visible);
			void toggleLayerLock();
			bool isLayerLocked();
			void setLayerLocked(bool p_locked);

			std::string getLabel();
			void setLabel(std::string p_label);
			
	};
#endif //GTKMM_KAGE_SCENE_H
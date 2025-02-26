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
#ifndef GTKMM_KAGE_FRAME_H
	#define GTKMM_KAGE_FRAME_H
	
	#include <vector>
	#include "color.h"
	#include "../vectordatamanager.h"

	#include "../../util/string/stringhelper.h"
	
	#include <gdkmm/color.h>
	
	class KageLayer; //forward declaration

	class KageFrame {
		public:
			enum extension {
				EXTENSION_NOT,
				EXTENSION_START,
				EXTENSION_MID,
				EXTENSION_END
			};
			
			static bool DEBUG_ON;
			static bool mouseIsDown;
			
			//Glib::RefPtr<Gdk::Window> window;
			
			KageFrame(KageLayer *p_layer, unsigned int p_frameID);
			KageFrame operator=(const KageFrame &p_frame);
			virtual ~KageFrame();
			
			bool isEmpty();
			int data; //what's this for?
			void setSelected(bool p_selected);
			bool isSelected();
			void setCurrent(bool p_current);
			bool isCurrent();
			VectorDataManager getFrameData();
			void setFrameData(VectorDataManager p_vectorsData);
			void setCode(bool p_newCode);
			bool getCode();
			
			unsigned int frameID;     //which frame among frames in a layer?
			void setNull(bool p_null);
			bool isNull();
			void setExtension(KageFrame::extension p_extension);
			KageFrame::extension getExtension();
			void forceSetTween(unsigned int p_tween);
			void setTween(unsigned int p_tween);
			unsigned int getTween();
			
			//void forceRender();
			
			static bool _gotFocus;
			
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
			
			void addDataToFrame(VectorDataManager v);
			std::vector<VectorData> copySelectedShapes(std::vector<unsigned int> p_selectedShapes);
			std::vector<unsigned int> pasteSelectedShapes(std::vector<VectorData> p_copiedShapes);
			bool deleteSelectedShapes(std::vector<unsigned int> p_selectedShapes);

			bool _null;
			bool _selected;
			bool _current;
			unsigned int _tweenX;
			unsigned int _tweenY;
			
			KageFrame::extension _extension;
			
			KageLayer *_layer;
			VectorDataManager vectorsData;
			
			static unsigned int _mouseLocationShapeIndex;
	};

#endif //GTKMM_KAGE_FRAME_H
/*
 * Kage Studio - a simple free and open source vector-based 2D animation software
 * Copyright (C) 2011~2023  Mj Mendoza IV
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
	
	using namespace std;

	class KageLayer; //forward declaration

	class KageFrame {
		unsigned int ID;
		//================================
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
			
			KageFrame(KageLayer *p_layer, unsigned p_layerID, unsigned int p_frameID);
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
			
			unsigned int layerID;     //which layer does it reside?
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
			
			void addDataToFrame(VectorDataManager v);
		protected:
			bool _null;
			bool _selected;
			bool _current;
			unsigned int _tweenX;
			unsigned int _tweenY;
			
			KageFrame::extension _extension;
			
			KageLayer *_layer;
			VectorDataManager vectorsData;
	};

#endif //GTKMM_KAGE_FRAME_H
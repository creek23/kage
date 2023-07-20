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

#ifndef GTKMM_KAGE_MANAGER_LAYER_H
	#define GTKMM_KAGE_MANAGER_LAYER_H
	
	#include <gtkmm/box.h>
	#include <gtkmm/label.h>
	#include <gtkmm/button.h>
	#include "layer.h"
	#include "../../layer_rename.h"
	
	class Kage;
	
	class KageLayerManager : public Gtk::VBox {
		public:
			KageLayerManager(Kage *p_kage);
			virtual ~KageLayerManager();
			unsigned int layerCount();
			unsigned int addLayer(Glib::ustring p_name = "");
			void deleteLayer();
			bool removeAllLayers();
			void setSelected(KageLayerUI *p_layer);
			KageLayerUI *getLayer();
			unsigned int getCurrentLayer();
			void setCurrentLayer(unsigned int p_layer);
			void setCurrentLayerByID(unsigned int p_layerID);
			void renderStage();
			
			void toggleLock();
			bool isLayerLocked();
			void setLock(bool p_lock);
			
			void toggleVisibility();
			bool isLayerVisible();
			void setVisible(bool p_visible);
			
			void renameLayer(KageLayerUI *p_layer);
			void renameLayer();
			void setLabel(string p_label);
			string getLabel();
			
			bool moveToTop();
			bool moveUp();
			bool moveDown();
			bool moveToBottom();
			
		protected:
			Kage *_kage;
			unsigned int layerCtr;
			std::vector<Gtk::Button*> labels;
			std::vector<KageLayerUI*> layers;
			unsigned int _currentLayerID;
			unsigned int _currentLayerIndex;
	};
#endif // GTKMM_KAGE_MANAGER_LAYER_H

/*
 * layermanager.h
 * 
 * Copyright 2011-2020 Mj Mendoza IV <mj.mendoza.iv@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
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
 * MA 02110-1301, USA.
 * 
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
			KageLayerManager(Kage *p_win);
			virtual ~KageLayerManager();
			unsigned layerCount();
			void addLayer(Glib::ustring p_name = "");
			void deleteLayer();
			bool removeAllLayers();
			void setSelected(KageLayer *p_layer);
			KageLayer *getLayer();
			unsigned int getCurrentLayer();
			void setCurrentLayer(unsigned int p_layer);
			void renderStage();
			
			void toggleLock();
			bool isLayerLocked();
			void setLock(bool p_lock);
			
			void toggleVisibility();
			bool isLayerVisible();
			void setVisible(bool p_visible);
			
			void renameLayer(KageLayer *p_layer);
			void renameLayer();
			void setLabel(string p_label);
			string getLabel();
			
		protected:
			Kage *win;
			unsigned int layerCtr;
			std::vector<Gtk::Button*> labels;
			std::vector<KageLayer*> layers;
			unsigned int _currentLayerID;
			unsigned int _currentLayerIndex;
	};
#endif // GTKMM_KAGE_MANAGER_LAYER_H

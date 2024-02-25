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

#ifndef GTKMM_KAGE_ASSET_H
	#define GTKMM_KAGE_ASSET_H
	
	#include <gtkmm/drawingarea.h>
	#include <gtkmm/entry.h>
	#include <time.h>
	#include "../../util/string/stringhelper.h"
	
	class KageAssetManager; ///forward declaration
	
	class KageAsset : public Gtk::DrawingArea {
		public:
			enum AssetType {
				ASSET_NULL,
				ASSET_IMAGE,
				ASSET_KAGE,
				ASSET_VIDEO
			};

			static bool mouseIsDown;
			static Glib::RefPtr<Gdk::Pixbuf> iconASSET_IMAGE;
			static Glib::RefPtr<Gdk::Pixbuf> iconASSET_KAGE;
			static Glib::RefPtr<Gdk::Pixbuf> iconASSET_VIDEO;
			
			Glib::RefPtr<Gdk::Window> window;
			
			KageAsset(KageAssetManager *p_assetManager, unsigned p_assetID);
			virtual ~KageAsset();
			
			void setAssetHash(std::string p_assetHash);
			std::string getAssetHash();

			void setFilePath(std::string p_filePath);
			std::string getFilePath();
			void setFileName(std::string p_fileName);
			std::string getFileName();

			void setLabel(std::string p_label);
			std::string getLabel();
			void setSelected(bool p_selected);
			bool isSelected();
			void setFocus();
			
			void setAssetType(KageAsset::AssetType p_assetType);
			KageAsset::AssetType getAssetType();
			
			unsigned int assetID;
			
			void forceRender();
			virtual bool invalidateToRender();
			Gtk::Entry _txtLabel;
			void txtLabel_onEnter();
			
			void addEventsListener();

			void render(unsigned int p_assetID);
			void render();
		protected:
			std::string _assetHash;

			//Override default signal handler:
			virtual bool on_expose_event(GdkEventExpose *e);
			virtual bool on_key_press_event(GdkEventKey *e) override;
			virtual bool on_key_release_event(GdkEventKey *e) override;
			virtual bool on_event(GdkEvent *e) override;
			
			bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
			bool _selected;
			KageAsset::AssetType _assetType;
			std::string _label;
			
			KageAssetManager *_assetManager;
			std::string _filePath;
			std::string _fileName;
			
			unsigned int _renderAssetID;
			bool _CTRL;
	};
#endif // GTKMM_KAGE_ASSET_H

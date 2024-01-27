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

#ifndef GTKMM_KAGE_LAYERS_UI_H
	#define GTKMM_KAGE_LAYERS_UI_H
	
	#include <gtkmm/drawingarea.h>
	#include <time.h>
	#include "../vectordatamanager.h"
	#include "../data/point.h"
	#include "../../util/string/stringhelper.h"
	#include "../../label_rename.h"
	
	class Kage; ///forward declaration
	
	class KageLayersUI : public Gtk::DrawingArea {
		public:
			static bool mouseIsDown;
			static Glib::RefPtr<Gdk::Pixbuf> imageVISIBLE_TRUE;
			static Glib::RefPtr<Gdk::Pixbuf> imageVISIBLE_FALSE;
			static Glib::RefPtr<Gdk::Pixbuf> imageLOCKED_TRUE;
			static Glib::RefPtr<Gdk::Pixbuf> imageLOCKED_FALSE;
			
			Glib::RefPtr<Gdk::Window> window;
			
			KageLayersUI(Kage *p_kage);
			virtual ~KageLayersUI();
			
			void setFocus();
			void renderStage();
			
			//unsigned int layerID;
			
			void forceRender();
			virtual bool invalidateToRender();
			
			static bool _gotFocus;

			void addEventsListener();
		protected:
			Kage* _kage;
			//Override default signal handler:
			virtual bool on_expose_event(GdkEventExpose *e);
			virtual bool on_key_press_event(GdkEventKey *e) override;
			virtual bool on_key_release_event(GdkEventKey *e) override;
			virtual bool on_event(GdkEvent *e) override;
			
			bool on_draw(const Cairo::RefPtr<Cairo::Context>& p_context) override;
			
			PointData draw1;
			PointData draw2;
	};
#endif // GTKMM_KAGE_LAYERS_UI_H

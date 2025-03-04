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

#ifndef GTKMM_KAGE_LAYER_H
	#define GTKMM_KAGE_LAYER_H
	
	#include <gtkmm/drawingarea.h>
	#include <gtkmm/entry.h>
	#include <gtkmm/gesturedrag.h>
	#include <gtkmm/gesturestylus.h>
	#include <gtkmm/gestureclick.h>
	#include <time.h>
	#include "../../util/string/stringhelper.h"
	
	class KageLayerManager; ///forward declaration
	
	class KageLayer : public Gtk::DrawingArea {
		public:
			static bool mouseIsDown;
			static Glib::RefPtr<Gdk::Pixbuf> imageVISIBLE_TRUE;
			static Glib::RefPtr<Gdk::Pixbuf> imageVISIBLE_FALSE;
			static Glib::RefPtr<Gdk::Pixbuf> imageLOCKED_TRUE;
			static Glib::RefPtr<Gdk::Pixbuf> imageLOCKED_FALSE;
			
			Glib::RefPtr<Gtk::Window> window;
			
			KageLayer(KageLayerManager *p_layerManager, unsigned p_layerID);
			virtual ~KageLayer();
			
			void setLabel(string p_label);
			string getLabel();
			void setSelected(bool p_selected);
			bool isSelected();
			void setFocus();
			bool isVisible();
			bool isLocked();
			void toggleVisibility();
			void toggleLock();
			
			void setVisible(bool p_visible);
			void setLock(bool p_lock);
			
			unsigned int layerID;
			
			void forceRender();
			virtual bool render();
			Gtk::Entry _txtLabel;
			void txtLabel_onEnter();
			
			void addEventsListener();
		protected:
			//Override default signal handler:
			Glib::RefPtr<Gtk::GestureDrag> controllerDrag;
			Glib::RefPtr<Gtk::GestureStylus> controllerStylus;
			Glib::RefPtr<Gtk::GestureClick> controllerClick;
			void onDragBegin(double p_x, double p_y);
			void onDragUpdate(double p_x, double p_y);
			void onDragEnd(double p_x, double p_y);
			void onStylusDown(double p_x, double p_y);
			void onStylusMotion(double p_x, double p_y);
			void onStylusUp(double p_x, double p_y);
			void onClick(int n_press, double p_x, double p_y);
			void draw_brush(double p_x, double p_y, double size = 0.5);
			bool on_key_pressed(guint keyval, guint keycode, Gdk::ModifierType state);
			double start_x, start_y;
			
			void on_draw(const Cairo::RefPtr<Cairo::Context>& p_cr, int p_width, int p_height);
			Cairo::RefPtr<Cairo::Context> cr;
			
			bool on_event(GdkEvent *e);
			
			//bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
			
			bool _selected;
			bool _visible;
			bool _lock;
			string _label;
			
			KageLayerManager *_layerManager;
	};
#endif // GTKMM_KAGE_LAYER_H

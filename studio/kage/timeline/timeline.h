/*
 * Kage Studio - a simple free and open source vector-based 2D animation software
 * Copyright (C) 2023-2024  Mj Mendoza IV
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

#ifndef GTKMM_KAGE_TIMELINE_H
	#define GTKMM_KAGE_TIMELINE_H
	
	#include <gtkmm/drawingarea.h>
	#include <time.h>
	#include "../vectordatamanager.h"
	#include "../data/point.h"
	
	class Kage; ///forward declaration
	
	class KageTimeline : public Gtk::DrawingArea {
		public: 
			static bool mouseIsDown;
			
			static Glib::RefPtr<Gdk::Pixbuf> imageNULL;
			static Glib::RefPtr<Gdk::Pixbuf> imageNULL_CUR;
			static Glib::RefPtr<Gdk::Pixbuf> imageNULL_X;
			static Glib::RefPtr<Gdk::Pixbuf> imageNULL_X_CUR;
			static Glib::RefPtr<Gdk::Pixbuf> imageBLANK;
			static Glib::RefPtr<Gdk::Pixbuf> imageBLANK_CUR;
			static Glib::RefPtr<Gdk::Pixbuf> imageBLANK_CUR_SEL;
			static Glib::RefPtr<Gdk::Pixbuf> imageBLANK_CUR_TWN;
			static Glib::RefPtr<Gdk::Pixbuf> imageBLANK_CUR_SEL_TWN;
			static Glib::RefPtr<Gdk::Pixbuf> imageBLANK_SEL;
			static Glib::RefPtr<Gdk::Pixbuf> imageBLANK_SEL_TWN;
			static Glib::RefPtr<Gdk::Pixbuf> imageBLANK_TWN;
			static Glib::RefPtr<Gdk::Pixbuf> imageBLANK_X;
			static Glib::RefPtr<Gdk::Pixbuf> imageBLANK_X_CUR;
			static Glib::RefPtr<Gdk::Pixbuf> imageBLANK_X_CUR_SEL;
			static Glib::RefPtr<Gdk::Pixbuf> imageBLANK_X_CUR_TWN;
			static Glib::RefPtr<Gdk::Pixbuf> imageBLANK_X_CUR_SEL_TWN;
			static Glib::RefPtr<Gdk::Pixbuf> imageBLANK_X_SEL;
			static Glib::RefPtr<Gdk::Pixbuf> imageBLANK_X_SEL_TWN;
			static Glib::RefPtr<Gdk::Pixbuf> imageBLANK_X_TWN;
			static Glib::RefPtr<Gdk::Pixbuf> imageDRAWN;
			static Glib::RefPtr<Gdk::Pixbuf> imageDRAWN_CUR;
			static Glib::RefPtr<Gdk::Pixbuf> imageDRAWN_CUR_SEL;
			static Glib::RefPtr<Gdk::Pixbuf> imageDRAWN_CUR_TWN;
			static Glib::RefPtr<Gdk::Pixbuf> imageDRAWN_CUR_SEL_TWN;
			static Glib::RefPtr<Gdk::Pixbuf> imageDRAWN_SEL;
			static Glib::RefPtr<Gdk::Pixbuf> imageDRAWN_SEL_TWN;
			static Glib::RefPtr<Gdk::Pixbuf> imageDRAWN_TWN;
			static Glib::RefPtr<Gdk::Pixbuf> imageDRAWN_X;
			static Glib::RefPtr<Gdk::Pixbuf> imageDRAWN_X_CUR;
			static Glib::RefPtr<Gdk::Pixbuf> imageDRAWN_X_CUR_SEL;
			static Glib::RefPtr<Gdk::Pixbuf> imageDRAWN_X_CUR_TWN;
			static Glib::RefPtr<Gdk::Pixbuf> imageDRAWN_X_CUR_SEL_TWN;
			static Glib::RefPtr<Gdk::Pixbuf> imageDRAWN_X_SEL;
			static Glib::RefPtr<Gdk::Pixbuf> imageDRAWN_X_SEL_TWN;
			static Glib::RefPtr<Gdk::Pixbuf> imageDRAWN_X_TWN;
			
			static bool DEBUG_ON;
			
			Glib::RefPtr<Gdk::Window> window;
			
			KageTimeline(Kage* p_kage);
			virtual ~KageTimeline();
			
			void setSelected(bool p_selected);
			bool isSelected();
			VectorDataManager getFrameData();
			void setFrameData(VectorDataManager p_vectorsData);
			void setFocus();
			
			void forceRender();
			virtual bool invalidateToRender();
			
			static bool _gotFocus;
			
			std::vector<unsigned int> raiseSelectedShape(vector<unsigned int> p_selectedShapes);
			
			void addDataToFrame(VectorDataManager v);
		protected:
			Kage* _kage;
			//Override default signal handler:
			virtual bool on_expose_event(GdkEventExpose *e);
			virtual bool on_key_press_event(GdkEventKey *e) override;
			virtual bool on_key_release_event(GdkEventKey *e) override;
			virtual bool on_event(GdkEvent *e) override;
			
			bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
			
			bool _null;
			unsigned int _tweenX;
			unsigned int _tweenY;
			bool _selected;
			bool _current;
			
			VectorDataManager vectorsData;
			
			PointData draw1;
			PointData draw2;
	};
#endif // GTKMM_KAGE_LIBRARY_H

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

#ifndef GTKMM_KAGE_FRAME_H
	#define GTKMM_KAGE_FRAME_H
	
	#include <gtkmm/drawingarea.h>
	#include <gtkmm/eventcontrollerkey.h>
	#include <gdkmm.h>
	#include <gtkmm/gesturedrag.h>
	#include <gtkmm/gesturestylus.h>
	#include <gtkmm/gestureclick.h>
	#include <time.h>
	#include "../vectordatamanager.h"
	
	class KageFrameset; ///forward declaration
	
	class KageFrame : public Gtk::DrawingArea {
		public:
			/// is Frame extended or not.  If extended which part? start/mid/end?
			enum extension {
				EXTENSION_NOT,
				EXTENSION_START,
				EXTENSION_MID,
				EXTENSION_END
			};
			
			static bool DEBUG_ON;
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
			
			Glib::RefPtr<Gtk::Window> window;
			
			KageFrame(KageFrameset *p_frameset, unsigned p_layerID, unsigned int p_frameID);
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
			void setFocus();
			
			unsigned int layerID;     //which layer does it reside?
			unsigned int frameID;     //which frame among frames in a layer?
			void setNull(bool p_null);
			bool isNull();
			void setExtension(KageFrame::extension p_extension);
			KageFrame::extension getExtension();
			void forceSetTween(unsigned int p_tween);
			void setTween(unsigned int p_tween);
			unsigned int getTween();
			
			void forceRender();
			virtual bool render();
			
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
			//Override default signal handler:
			//virtual bool on_draw(GdkEventExpose *e);
			//bool on_key_press_event(GdkEventKey *e);
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
			//bool on_event(GdkEvent *e); ??? what replaced this in gtkmm4
			Glib::ustring sCode;
			bool _null;
			unsigned int _tweenX;
			unsigned int _tweenY;
			bool _selected;
			bool _current;
			
			KageFrame::extension _extension;
			
			KageFrameset *_frameset;
			VectorDataManager vectorsData;
	};
#endif // GTKMM_KAGE_FRAME_H

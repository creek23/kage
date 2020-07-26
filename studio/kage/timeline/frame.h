/*
 * Kage Studio - a simple free and open source vector-based 2D animation software
 * Copyright (C) 2011~2020  Mj Mendoza IV
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
			static Glib::RefPtr<Gdk::Pixbuf> imageBLANK_X;
			static Glib::RefPtr<Gdk::Pixbuf> imageBLANK_X_CUR;
			static Glib::RefPtr<Gdk::Pixbuf> imageDRAWN;
			static Glib::RefPtr<Gdk::Pixbuf> imageDRAWN_CUR;
			static Glib::RefPtr<Gdk::Pixbuf> imageDRAWN_X;
			static Glib::RefPtr<Gdk::Pixbuf> imageDRAWN_X_CUR;
			static Glib::RefPtr<Gdk::Pixbuf> imageTWEEN;
			static Glib::RefPtr<Gdk::Pixbuf> imageTWEEN_CUR;
			static Glib::RefPtr<Gdk::Pixbuf> imageTWEEN_X;
			static Glib::RefPtr<Gdk::Pixbuf> imageTWEEN_X_CUR;
			static Glib::RefPtr<Gdk::Pixbuf> imageSELECTED;
			static Glib::RefPtr<Gdk::Pixbuf> imageSELECTED_CUR;
			static Glib::RefPtr<Gdk::Pixbuf> imageSELECTED_X;
			static Glib::RefPtr<Gdk::Pixbuf> imageSELECTED_X_CUR;
			
			Glib::RefPtr<Gdk::Window> window;
			
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
			virtual bool on_expose_event(GdkEventExpose *e);
			virtual bool on_key_press_event(GdkEventKey *e);
			virtual bool on_event(GdkEvent *e);
			
			bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
			
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

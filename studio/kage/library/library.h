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

#ifndef GTKMM_KAGE_LIBRARY_H
	#define GTKMM_KAGE_LIBRARY_H
	
	#include <gtkmm/drawingarea.h>
	#include <time.h>
	#include "../vectordatamanager.h"
	
	class Kage; ///forward declaration
	
	class KageLibrary : public Gtk::DrawingArea {
		protected:
			Kage* _kage;
		public:
			/// is Frame extended or not.  If extended which part? start/mid/end?
			enum extension {
				EXTENSION_NOT
			};
			
			static bool DEBUG_ON;
			static bool mouseIsDown;
			
			static Glib::RefPtr<Gdk::Pixbuf> imageNULL;
			
			Glib::RefPtr<Gdk::Window> window;
			
			KageLibrary(Kage* p_kage);
			virtual ~KageLibrary();
			
			bool isEmpty();
			int data; //what's this for?
			void setSelected(bool p_selected);
			bool isSelected();
			VectorDataManager getFrameData();
			void setFrameData(VectorDataManager p_vectorsData);
			void setFocus();
			
			void setExtension(KageLibrary::extension p_extension);
			KageLibrary::extension getExtension();
			
			void forceRender();
			virtual bool invalidateToRender();
			
			static bool _gotFocus;
			
			std::vector<unsigned int> raiseSelectedShape(std::vector<unsigned int> p_selectedShapes);

			bool recenterRotationPoint(std::vector<unsigned int> p_selectedShapes);
			
			void addDataToFrame(VectorDataManager v);

			void render(unsigned int p_assetID);

			void resetAssetID();

			unsigned int _renderAssetID;
		protected:
			//Override default signal handler:
			virtual bool on_expose_event(GdkEventExpose *e);
			virtual bool on_key_press_event(GdkEventKey *e) override;
			virtual bool on_event(GdkEvent *e) override;
			
			bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
			
			Glib::ustring sCode;
			bool _null;
			unsigned int _tweenX;
			unsigned int _tweenY;
			bool _selected;
			bool _current;
			
			VectorDataManager vectorsData;
	};
#endif // GTKMM_KAGE_LIBRARY_H

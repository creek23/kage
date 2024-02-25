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

#ifndef GTKMM_KAGE_PROP_NODE_XY_H
	#define GTKMM_KAGE_PROP_NODE_XY_H
	
	#include <gtkmm/box.h>
	#include <gtkmm/label.h>
	#include <gtkmm/entry.h>
	#include <gtkmm/colorbutton.h>
	#include <gtkmm/colorselection.h>
	
	class Kage; //forward declaration

	class PropertyNodeXY : public Gtk::HBox {
		public:
			PropertyNodeXY(Kage* p_kage);
			virtual ~PropertyNodeXY();
		
		protected:
			Kage* _kage;
			//Member widgets:
			Gtk::HBox m_propNodeXY;
			Gtk::VBox m_propNodeXYV1;
			Gtk::VBox m_propNodeXYV2;
			Gtk::Label m_LabelNodeX;
			Gtk::Entry m_EntryNodeX;
			Gtk::Label m_LabelNodeY;
			Gtk::Entry m_EntryNodeY;
			Gtk::Label m_LabelToggleLine;
			Gtk::Button _btnToggleLine;
				Gtk::Image                _btnToggleLine_img;
				Glib::RefPtr<Gdk::Pixbuf> _btnToggleLine_pixbuf;
		public:
				void EntryNodeX_onEnter();
				void EntryNodeY_onEnter();
				void ToggleLine_onClick();
				
				void setNodeXText(double p_nodeX);
				void setNodeYText(double p_nodeY);
	};
#endif //GTKMM_KAGE_PROP_NODE_XY_H

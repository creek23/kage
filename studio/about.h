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

#ifndef GTKMM_KAGE_ABOUT_H
	#define GTKMM_KAGE_ABOUT_H
	
	#include <gtkmm/window.h>
	#include <gtkmm/dialog.h>
	#include <gtkmm/box.h>
	#include <gtkmm/label.h>
	#include <gtkmm/stock.h>
	#include <gtkmm/image.h>
	#include <gtkmm/table.h>
	
	using namespace std;
	
	class KageAbout : public Gtk::Dialog {
		public:
			KageAbout(Gtk::Window& parent);
			virtual ~KageAbout();
			static string app_version;
			static string app_title;
			static string app_author;
			static string app_contact;
			static string app_website;
			static string app_copyright;
			static string app_comment;
			static Glib::RefPtr<Gdk::Pixbuf> imageABOUT;
		
		protected:
			//Member widgets:
			Gtk::HBox m_HBox;
			Gtk::Table m_Table;
			Gtk::Label m_Label1, m_Label2;
			Gtk::Image m_Image;
	};	
#endif //GTKMM_KAGE_ABOUT_H

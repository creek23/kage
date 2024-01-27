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

#ifndef GTKMM_KAGE_APP_H
	#define GTKMM_KAGE_APP_H
	
	#include <gtkmm.h>
	#include <string>
	
	#include<giomm-2.68/giomm/simpleactiongroup.h>

	#include "about.h"
	
	using namespace std;

	class Kage;

	class GtkKageApp: public Gtk::Application {
		protected:
			GtkKageApp(string p_path);
			string _path;
			
		public:
			static Glib::RefPtr<GtkKageApp> create(string p_path);

		protected:
			// Override default signal handlers:
			void on_startup() override;
  			void on_activate() override;
			void on_open(const Gio::Application::type_vec_files& files,
			const Glib::ustring& hint) override;

			Glib::RefPtr<Gtk::Builder> m_refBuilder;

		private:
			Kage* create_appwindow();
			void on_hide_window(Gtk::Window* window);
	};

#endif //GTKMM_KAGE_APP_H

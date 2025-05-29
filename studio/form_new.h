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

#ifndef GTKMM_KAGE_FORM_NEW_H
	#define GTKMM_KAGE_FORM_NEW_H
	
	#include <gtkmm/window.h>
	#include <gtkmm/dialog.h>
	#include <gtkmm/box.h>
	#include <gtkmm/label.h>
	#include <gtkmm/entry.h>
	#include <gtkmm/comboboxtext.h>
	#include <gtkmm/stock.h>
	#include <iostream>
	#include "util/xml/xml.h"
	#include "util/string/stringhelper.h"
	
	class FormNewDialog : public Gtk::Dialog {
		public:
		FormNewDialog(Gtk::Window& parent);
			virtual ~FormNewDialog();
			
			unsigned int getFPS();
			double getWidth();
			double getHeight();
		protected:
			//Member widgets:
			Gtk::HBox m_HBox;
			Gtk::Label _label;

			Gtk::VBox m_VBox1;
			Gtk::VBox m_VBox2;
			Gtk::Label _lblOrientation;
			Gtk::ComboBoxText _cmbOrientation;
			void Orientation_onChange();
			Gtk::Label _lblTemplate;
			Gtk::ComboBoxText _cmbTemplate;
			void Template_onChange();
			Gtk::Label _lblFPS;
			Gtk::Entry _txtFPS;
			void txtFPS_onEnter();
			void setFPSText(unsigned int p_fps);
			Gtk::Label _lblWidth;
			Gtk::Entry _txtWidth;
			void txtWidth_onEnter();
			void setWidthText(double p_width);
			Gtk::Label _lblHeight;
			Gtk::Entry _txtHeight;
			void txtHeight_onEnter();
			void setHeightText(double p_height);

			struct TemplateProfile {
				std::string _name;
				bool _square;
				bool _landscape;
				unsigned int _fps;
				double _width;
				double _height;
			};
			
			std::vector<TemplateProfile> _templateProfiles; 
			TemplateProfile _templateProfile;
			bool doOpenTemplateProfile();

	};	
#endif //GTKMM_KAGE_FORM_NEW_H

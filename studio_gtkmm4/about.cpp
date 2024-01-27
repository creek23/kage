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

#include <iostream>
#include <gtkmm-4.0/gtkmm/picture.h>

#include "about.h"

string KageAbout::app_title = "Kage Studio";
string KageAbout::app_version = "0.7.221227";
string KageAbout::app_author = "Mj Mendoza IV";
string KageAbout::app_contact = "mj.mendoza.iv@gmail.com";
string KageAbout::app_website = "https://kage.sourceforge.io";
string KageAbout::app_copyright = "Copyleft 2011~2022";
string KageAbout::app_comment = "A Simple Free and Open Source vector-based 2D Animation Software.";
Glib::RefPtr<Gdk::Pixbuf> KageAbout::imageABOUT;

KageAbout::KageAbout(Gtk::Window& parent)
		: Gtk::Dialog("About Kage Studio", parent, true),
		m_VBox(Gtk::Orientation::VERTICAL, 8),
		m_HBox(Gtk::Orientation::HORIZONTAL, 8),
		m_Table(),//2, 2, false),
		m_Label1("", true),
		m_Label2("", true) {
	set_modal(true);
	string l_license = string("") +
	"This program is free software: you can redistribute it and/or modify\n" +
	"it under the terms of the GNU General Public License as published by\n" +
	"the Free Software Foundation, either version 3 of the License, or\n" +
	"(at your option) any later version.\n" +
	"\n" +
	"This program is distributed in the hope that it will be useful,\n" +
	"but WITHOUT ANY WARRANTY; without even the implied warranty of\n" +
	"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n" +
	"GNU General Public License for more details.";
	m_Label1.set_label(KageAbout::app_title + " v" + KageAbout::app_version + "\n\n" + KageAbout::app_comment + "\n\n" + KageAbout::app_website + "\n" + KageAbout::app_copyright + " " + KageAbout::app_author + " <" + KageAbout::app_contact + ">\nAll Rights Reversed\n\n" + l_license);
	
	set_child(m_VBox);
	m_VBox.append(m_HBox);
		Gtk::Picture l_picture;
			m_HBox.append(l_picture);
				l_picture.set_resource("/kage/share/icons/about.png");
				l_picture.set_can_shrink(false);
			m_HBox.append(m_Table);
				m_Table.attach(m_Label1, 0, 0, 400, 50);
	//			m_Table.attach(m_Label2, 0, 10, 400, 330);
	
	set_resizable(false);
	
	//m_Table.set_row_spacings(4);
	//m_Table.set_col_spacings(4);
	
	//add_button("Okay", Gtk::ResponseType::OK);
	
	show();
}

KageAbout::~KageAbout() {
	//
}

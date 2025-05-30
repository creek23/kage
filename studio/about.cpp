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

#include "about.h"

std::string KageAbout::app_title = "Kage Studio";
std::string KageAbout::app_version = "0.8.240824";
std::string KageAbout::app_author = "Mj Mendoza IV";
std::string KageAbout::app_contact = "mj.mendoza.iv@gmail.com";
std::string KageAbout::app_website = "https://kage.sourceforge.io";
std::string KageAbout::app_copyright = "Copyleft 2011~2024";
std::string KageAbout::app_comment = "A Simple Free and Open Source vector-based 2D Animation Software.";
Glib::RefPtr<Gdk::Pixbuf> KageAbout::imageABOUT;

KageAbout::KageAbout(Gtk::Window& parent)
		: Gtk::Dialog("About Kage Studio", parent, true),
		m_HBox(false, 8),
		m_Table(2, 2, false),
		m_Label1("", true),
		m_Label2("", true),
		m_Image(Gtk::Stock::DIALOG_INFO, Gtk::ICON_SIZE_DIALOG) {
	std::string l_license = std::string("") +
	"This program is free software: you can redistribute it and/or modify\n" +
	"it under the terms of the GNU General Public License as published by\n" +
	"the Free Software Foundation, either version 3 of the License, or\n" +
	"(at your option) any later version.\n" +
	"\n" +
	"This program is distributed in the hope that it will be useful,\n" +
	"but WITHOUT ANY WARRANTY; without even the implied warranty of\n" +
	"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n" +
	"GNU General Public License for more details.";
	m_Label1.set_label(KageAbout::app_title + " v" + KageAbout::app_version + " (Emcee)");
	m_Label2.set_label(KageAbout::app_comment + "\n\n" + KageAbout::app_website + "\n" + KageAbout::app_copyright + " " + KageAbout::app_author + " <" + KageAbout::app_contact + ">\nAll Rights Reversed\n\n" + l_license);
	
	add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
	
	m_HBox.set_border_width(8);
	get_vbox()->pack_start(m_HBox, Gtk::PACK_SHRINK);
	m_HBox.pack_start(m_Image, Gtk::PACK_SHRINK);
	
	m_Table.set_row_spacings(4);
	m_Table.set_col_spacings(4);
	m_HBox.pack_start(m_Table);
	
	KageAbout::imageABOUT = Gdk::Pixbuf::create_from_resource("/kage/share/icons/about.png");
		m_Image.set(KageAbout::imageABOUT);
	//
	
	m_Table.attach(m_Label1, 0, 1, 0, 1);
	m_Table.attach(m_Label2, 0, 1, 1, 2);
	
	show_all();
}

KageAbout::~KageAbout() {
	//
}

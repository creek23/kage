
#include "about.h"

AboutDialog::AboutDialog(Gtk::Window& parent)
		: Gtk::Dialog("About Kage Studio", parent, true),
		m_HBox(false, 8),
		m_Table(2, 2, false),
		m_Label1("Kage Studio v0.2.191029", true),
		m_Label2("Create interactive animations with Kage platform\n\nhttp://konsolscript.sourceforge.net\nCopyright 2019 Mj Mendoza IV\nAll Rights Reversed", true),
		m_Image(Gtk::Stock::DIALOG_INFO, Gtk::ICON_SIZE_DIALOG) {
	add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
	
	m_HBox.set_border_width(8);
	get_vbox()->pack_start(m_HBox, Gtk::PACK_SHRINK);
	m_HBox.pack_start(m_Image, Gtk::PACK_SHRINK);
	
	m_Table.set_row_spacings(4);
	m_Table.set_col_spacings(4);
	m_HBox.pack_start(m_Table);
	
	m_Table.attach(m_Label1, 0, 1, 0, 1);
	m_Table.attach(m_Label2, 0, 1, 1, 2);
	
	show_all();
}

AboutDialog::~AboutDialog() {
	//
}

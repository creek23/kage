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

#include "layer_rename.h"

LayerRenameDialog::LayerRenameDialog(Gtk::Window& parent, KageLayer *p_layer)
		: Gtk::Dialog("Rename Layer", parent, true),
		m_HBox(false, 8),
		_label("Label:", true) {
//	add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
	set_resizable(false);
	
	m_HBox.set_border_width(8);
	get_vbox()->pack_start(m_HBox, Gtk::PACK_SHRINK);
		m_HBox.pack_start(_label);
		m_HBox.pack_start(_txtLabel, Gtk::PACK_SHRINK);
			_txtLabel.set_text(p_layer->getLabel());
			_layer = p_layer;
			_txtLabel.signal_activate().connect( sigc::mem_fun(*this, &LayerRenameDialog::EntryLayerLabel_onEnter) );
	
	show_all();
}

LayerRenameDialog::~LayerRenameDialog() {
	//
}

void LayerRenameDialog::EntryLayerLabel_onEnter() {
	string l_newLabel = _txtLabel.get_text();
	//TODO: sanitize input
	string l_satinized = "";
	char l_char;
		for (unsigned int i = 0; i < l_newLabel.length(); ++i) {
			if (l_newLabel[i] >= 35 && l_newLabel[i] <= 126             //#$%&'()*+,-./ and 0~9 and :;<=>?@ and A~Z and []^_` and a~z and {|}~
					|| l_newLabel[i] == 32                              //[space]
					|| l_newLabel[i] == 33) {                           // !
				l_satinized += l_newLabel[i];
			} else if (l_newLabel[i] == 34) {                           // double quotes
				l_satinized += "'"; //use single quote instead
			} else {
				cout << l_newLabel[i] << "\t"<< int(l_newLabel[i]) << endl;
				l_satinized += "_";
			}
		}
		_layer->setLabel(l_satinized);
	hide();
}

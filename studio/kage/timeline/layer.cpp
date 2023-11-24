/*
 * Kage Studio - a simple free and open source vector-based 2D animation software
 * Copyright (C) 2011~2023  Mj Mendoza IV
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

#include <cairomm/context.h>
#include <cairomm/cairomm.h>
#include <gdkmm/general.h> // set_source_pixbuf()
#include <iostream>

#include "../../kage.h"

#include <giomm/resource.h>

bool KageLayerUI::mouseIsDown = false;
Glib::RefPtr<Gdk::Pixbuf> KageLayerUI::imageVISIBLE_TRUE;
Glib::RefPtr<Gdk::Pixbuf> KageLayerUI::imageVISIBLE_FALSE;
Glib::RefPtr<Gdk::Pixbuf> KageLayerUI::imageLOCKED_TRUE;
Glib::RefPtr<Gdk::Pixbuf> KageLayerUI::imageLOCKED_FALSE;

KageLayerUI::KageLayerUI(KageLayerManager *p_layerManager, unsigned p_layerID) {
	set_state_flags(Gtk::STATE_FLAG_NORMAL);
	set_can_focus(true); //to accept key_press
	_layerManager = p_layerManager;
	layerID = p_layerID;
	set_size_request(8, 24);
	_selected = false;
	_visible = true;
	_lock = false;
	_label = "Layer " + StringHelper::unsignedIntegerToString(p_layerID);
	
	addEventsListener();
	
	//add(_txtLabel);//, Gtk::PACK_EXPAND_WIDGET);
	_txtLabel.set_size_request(20, 24);
//	_txtLabel.set_width_chars(9);
	_txtLabel.set_max_length(50);
	_txtLabel.set_text(_label);
	_txtLabel.signal_activate().connect(
							sigc::mem_fun(*this, &KageLayerUI::txtLabel_onEnter));
	_txtLabel.show();
}

KageLayerUI::~KageLayerUI() {
	//
}

void KageLayerUI::addEventsListener() {
//	add_events(Gdk::KEY_PRESS_MASK    | Gdk::KEY_RELEASE_MASK);
	add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
	add_events(Gdk::ENTER_NOTIFY_MASK | Gdk::LEAVE_NOTIFY_MASK);
	add_events(Gdk::FOCUS_CHANGE_MASK);
	
	//*window = NULL;
}

void KageLayerUI::txtLabel_onEnter() {
	_label = _txtLabel.get_text();
	_txtLabel.hide();
}

bool KageLayerUI::on_key_press_event(GdkEventKey *e) {
	Kage::timestamp_IN();
	std::cout << " KageFrame(L " << layerID << ") on_key_press_event" << std::endl;
	if (e->keyval == GDK_KEY_period) {
	} else if (e->keyval == GDK_KEY_comma) {
	}
	Kage::timestamp_OUT();
	return true;
}
bool KageLayerUI::on_expose_event(GdkEventExpose* e) {
	if (!window) {
		window = get_window();
	}
	if (window) {
		invalidateToRender();
	}
	
	return true;
}

bool KageLayerUI::on_event(GdkEvent *e) {
	if (e->type == GDK_ENTER_NOTIFY) {
		Kage::timestamp_IN();
		std::cout << " KageLayerUI(L " << layerID << ") on_event enter" << std::endl;
		invalidateToRender();
		Kage::timestamp_OUT();
	} else if (e->type == GDK_LEAVE_NOTIFY) {
		Kage::timestamp_IN();
		std::cout << " KageLayerUI(L " << layerID << ") on_event leave" << std::endl;
		invalidateToRender();
		Kage::timestamp_OUT();
	} else if (e->type == GDK_DOUBLE_BUTTON_PRESS) {
		if (e->button.x > 36) {
			_layerManager->renameLayer(this);
		}
		//_txtLabel.show();
	} else if (e->type == GDK_BUTTON_RELEASE) {
		KageLayerUI::mouseIsDown = false;
		grab_focus();
		invalidateToRender();
	} else if (e->type == GDK_BUTTON_PRESS) {
		KageLayerUI::mouseIsDown = true;
		if (e->button.x < 18) {
			toggleVisibility();
		} else if (e->button.x < 36) {
			toggleLock();
		} else {
			_layerManager->setSelected(this);
		}
		_layerManager->renderStage();
		invalidateToRender();
	} else if (e->type == GDK_EXPOSE) {
		on_expose_event((GdkEventExpose*) e);
	} else if (e->type == GDK_FOCUS_CHANGE) {
		//filter out from echos
	} else if (e->type == GDK_KEY_PRESS) {
		on_key_press_event((GdkEventKey*) e);
	} else if (e->type == GDK_KEY_RELEASE) {
		//filter out from echos
	} else if (e->type == GDK_CONFIGURE) {
		//filter out from echos 
	}
	return true;
}


void KageLayerUI::forceRender() {
	if (KageScene::LOADING_MODE == true) {
		return;
	}
	Kage::timestamp_IN(); cout << " KageLayerUI::forceRender()" << endl;
	invalidateToRender();
	Kage::timestamp_OUT();
}
bool KageLayerUI::invalidateToRender() {
	if (KageScene::LOADING_MODE == true) {
		return true;
	}
	//Kage::timestamp_IN(); cout << " KageLayerUI::invalidateToRender()" << endl;
	if (!window) {
		window = get_window();
	}
	try {
		// force our program to redraw the entire stage
		Gdk::Rectangle r(0, 0, get_allocation().get_width(),
				get_allocation().get_height());
		if (window)
			window->invalidate_rect(r, false);
	} catch (std::exception& e) {
		std::cerr << "Exception caught : " << e.what() << std::endl;
	}
	
	//Kage::timestamp_OUT();
	
	return true;
}

bool KageLayerUI::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
	if (!window) {
		window = get_window();
	}
	if (window) {
		if (!KageLayerUI::imageVISIBLE_TRUE) {
			try {
				KageLayerUI::imageVISIBLE_TRUE  = Gdk::Pixbuf::create_from_resource("/kage/share/layer/visible_true.png");
				KageLayerUI::imageVISIBLE_FALSE = Gdk::Pixbuf::create_from_resource("/kage/share/layer/visible_false.png");
				KageLayerUI::imageLOCKED_TRUE     = Gdk::Pixbuf::create_from_resource("/kage/share/layer/locked_true.png");
				KageLayerUI::imageLOCKED_FALSE    = Gdk::Pixbuf::create_from_resource("/kage/share/layer/locked_false.png");
			} catch(const Gio::ResourceError &ex) {
				std::cerr << "ResourceError: " << ex.what() << std::endl;
			} catch(const Gdk::PixbufError &ex) {
				std::cerr << "PixbufError: " << ex.what() << std::endl;
			}
		}
		
		//draw background
		if (isSelected() == true) {
			cr->move_to(0, 0);
				cr->line_to(get_width(),            0);
				cr->line_to(get_width(), get_height());
				cr->line_to(          0, get_height());
			cr->close_path();
				cr->set_source_rgb(0.0, 0.47, 0.84);
				cr->fill_preserve();
		}
		
		//draw label
		cr->select_font_face ("Verdana", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_NORMAL);
		cr->set_font_size(12);
		if (isSelected() == true) {
			cr->set_source_rgb(1, 1, 1);
		} else {
			if (Kage::NotDarkMode) {
				cr->set_source_rgb(0, 0, 0);
			} else {
				cr->set_source_rgb(1, 1, 1);
			}
		}
		cr->move_to(50, 15);
		cr->show_text(_label);
		
		if (_visible == true) {
			Gdk::Cairo::set_source_pixbuf(cr, KageLayerUI::imageVISIBLE_TRUE, 2, 0);
		} else {
			Gdk::Cairo::set_source_pixbuf(cr, KageLayerUI::imageVISIBLE_FALSE, 2, 0);
		}
		cr->paint();
		
		if (_lock == true) {
			Gdk::Cairo::set_source_pixbuf(cr, KageLayerUI::imageLOCKED_TRUE, 18, 0);
		} else {
			Gdk::Cairo::set_source_pixbuf(cr, KageLayerUI::imageLOCKED_FALSE, 18, 0);
		}
		cr->paint();
		
		return true;
	}
	return false;
}

void KageLayerUI::setLabel(string p_label) {
	_label = p_label;
	invalidateToRender();
}
string KageLayerUI::getLabel() {
	return _label;
}
void KageLayerUI::setSelected(bool p_selected) {
	_selected = p_selected;
	invalidateToRender();
}
bool KageLayerUI::isSelected() {
	return _selected;
}

bool KageLayerUI::isVisible() {
	return _visible;
}

bool KageLayerUI::isLocked() {
	return _lock;
}

void KageLayerUI::setFocus() {
	grab_focus();
}

void KageLayerUI::toggleVisibility() {
	if (_visible == true) {
		_visible = false;
	} else {
		_visible = true;
	}
	invalidateToRender();
}

void KageLayerUI::toggleLock() {
	if (_lock == true) {
		_lock = false;
	} else {
		_lock = true;
	}
	invalidateToRender();
}

void KageLayerUI::setVisible(bool p_visible) {
	_visible = p_visible;
	invalidateToRender();
}

void KageLayerUI::setLock(bool p_lock) {
	_lock = p_lock;
	invalidateToRender();
}

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

#include <cairomm/context.h>
#include <cairomm/cairomm.h>
#include <gdkmm/general.h> // set_source_pixbuf()
#include <iostream>

#include "frame.h"
#include "framesmanager.h"
#include "../../kage.h"

#include <giomm/resource.h>

KageLayer::KageLayer(KageLayerManager *p_layerManager, unsigned p_layerID) {
	//set_state_flags(Gtk::STATE_FLAG_NORMAL);
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
	_txtLabel.signal_editing_done().connect(
			sigc::mem_fun(*this, &KageLayer::txtLabel_onEnter));
	_txtLabel.show();

	set_draw_func(sigc::mem_fun(*this, &KageLayer::on_draw));
}

KageLayer::~KageLayer() {
	//
}

void KageLayer::addEventsListener() {
//	add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
//	add_events(Gdk::ENTER_NOTIFY_MASK | Gdk::LEAVE_NOTIFY_MASK);
//	add_events(Gdk::FOCUS_CHANGE_MASK);
	
	//*window = NULL;
	// Events.
	auto controller = Gtk::EventControllerKey::create();
		controller->signal_key_pressed().connect(
			sigc::mem_fun(*this, &KageLayer::on_key_pressed), false);
	add_controller(controller);

	controllerDrag  = Gtk::GestureDrag::create();
		controllerDrag->set_button(GDK_BUTTON_PRIMARY);
			controllerDrag->signal_drag_begin().connect(sigc::mem_fun(*this, &KageLayer::onDragBegin));
			controllerDrag->signal_drag_update().connect(sigc::mem_fun(*this, &KageLayer::onDragUpdate));
			controllerDrag->signal_drag_end().connect(sigc::mem_fun(*this, &KageLayer::onDragEnd));
	add_controller(controllerDrag);
	
	controllerStylus = Gtk::GestureStylus::create();
		controllerStylus->signal_down().connect(sigc::mem_fun(*this, &KageLayer::onStylusDown));
        controllerStylus->signal_up().connect(sigc::mem_fun(*this, &KageLayer::onStylusUp));
        controllerStylus->signal_motion().connect(sigc::mem_fun(*this, &KageLayer::onStylusMotion));
	add_controller(controllerStylus);
	
	controllerClick = Gtk::GestureClick::create();
		controllerClick->set_button(GDK_BUTTON_SECONDARY);
        	controllerClick->signal_pressed().connect(sigc::mem_fun(*this, &KageLayer::onClick));
	add_controller(controllerClick);
}

void KageLayer::onDragBegin(double p_x, double p_y) {
	// First check if the event sequence is already handled by the stylus gesture; if not
	// we will handle is here; if it is deny it
	auto sequence = controllerDrag->get_last_updated_sequence();
	if (controllerStylus->get_sequence_state(sequence) == Gtk::EventSequenceState::CLAIMED) {
		controllerDrag->set_sequence_state(sequence, Gtk::EventSequenceState::DENIED);
	} else {
		start_x = p_x;
		start_y = p_y;
		draw_brush(p_x, p_y);
	}
	cout << "layer onDragBegin p_x " << p_x << endl;
	KageLayer::mouseIsDown = true;
	if (p_x < 18) {
		toggleVisibility();
	} else if (p_x < 36) {
		toggleLock();
	} else {
		_layerManager->setSelected(this);
	}
	_layerManager->renderStage();
	//render(); //not needed as toggleVisibility(), toggleLock(), and layerManager->setSelected() will call render()
}

void KageLayer::onDragUpdate(double p_x, double p_y) {
	// First check if the event sequence is already handled by the stylus gesture; if not
	// we will handle is here; if it is deny it
	auto sequence = controllerDrag->get_last_updated_sequence();
	if (controllerStylus->get_sequence_state(sequence) == Gtk::EventSequenceState::CLAIMED) {
		controllerDrag->set_sequence_state(sequence, Gtk::EventSequenceState::DENIED);
	} else {
		draw_brush(start_x + p_x, start_y + p_y);
	}
}

void KageLayer::onDragEnd(double p_x, double p_y) {
	// First check if the event sequence is already handled by the stylus gesture; if not
	// we will handle is here; if it is deny it
	auto sequence = controllerDrag->get_last_updated_sequence();
	if (controllerStylus->get_sequence_state(sequence) == Gtk::EventSequenceState::CLAIMED) {
		controllerDrag->set_sequence_state(sequence, Gtk::EventSequenceState::DENIED);
	} else {
		draw_brush(start_x + p_x, start_y + p_y);
	}

	KageLayer::mouseIsDown = false;
	grab_focus();
	render();
}
void KageLayer::onStylusDown(double p_x, double p_y) {
	auto pressure = controllerStylus->get_axis(Gdk::AxisUse::PRESSURE);
	draw_brush(p_x, p_y, pressure.value_or(0.5));
	// Claim the event sequence; the drag gesture should not handle it
	auto sequence = controllerStylus->get_last_updated_sequence();
	controllerStylus->set_sequence_state(sequence, Gtk::EventSequenceState::CLAIMED);
}

void KageLayer::onStylusMotion(double p_x, double p_y) {
	auto pressure = controllerStylus->get_axis(Gdk::AxisUse::PRESSURE);
	draw_brush(p_x, p_y, pressure.value_or(0.5));
	// Claim the event sequence; the drag gesture should not handle it
	auto sequence = controllerStylus->get_last_updated_sequence();
	controllerStylus->set_sequence_state(sequence, Gtk::EventSequenceState::CLAIMED);
}

void KageLayer::onStylusUp(double p_x, double p_y) {
	auto pressure = controllerStylus->get_axis(Gdk::AxisUse::PRESSURE);
	draw_brush(p_x, p_y, pressure.value_or(0.5));
	// Claim the event sequence; the drag gesture should not handle it
	auto sequence = controllerStylus->get_last_updated_sequence();
	controllerStylus->set_sequence_state(sequence, Gtk::EventSequenceState::CLAIMED);
}

void KageLayer::onClick (int n_press, double p_x, double p_y) {
	cout << "Layer onClick" << endl;
//	clear_surface();
//	queue_draw();
}
/* Draw a rectangle on the surface at the given position */
void KageLayer::draw_brush(double p_x, double p_y, double size) {
	/* Paint to the surface, where we store our state */
	//auto cr = Cairo::Context::create(get_native()->get_surface());
/*	if (cr) {
		double w = 12 * size;
		cr->rectangle(p_x - w*0.5, p_y - w*0.5, w, w);
		cr->fill();
		/* Now invalidate the drawing area. * /
		queue_draw();
	}*/
}

void KageLayer::txtLabel_onEnter() {
	_label = _txtLabel.get_text();
	_txtLabel.hide();
}

bool KageLayer::on_key_pressed(guint keyval, guint keycode, Gdk::ModifierType state) {
//bool KageLayer::on_key_press_event(GdkEventKey* e) {
	//why this function here?
	Kage::timestamp_IN();
	std::cout << " KageFrame(L " << layerID << ") on_key_press_event" << std::endl;
	if (keyval == GDK_KEY_period) {
		
	} else if (keyval == GDK_KEY_comma) {
		
	}
	Kage::timestamp_OUT();
	return true;
}

bool KageLayer::on_event(GdkEvent *e) {
	/*if (e->type == GDK_ENTER_NOTIFY) {
		Kage::timestamp_IN();
		std::cout << " KageLayer(L " << layerID << ") on_event enter" << std::endl;
		render();
		Kage::timestamp_OUT();
	} else if (e->type == GDK_LEAVE_NOTIFY) {
		Kage::timestamp_IN();
		std::cout << " KageLayer(L " << layerID << ") on_event leave" << std::endl;
		render();
		Kage::timestamp_OUT();
	} else if (e->type == GDK_DOUBLE_BUTTON_PRESS) {
		if (e->button.x > 36) {
			_layerManager->renameLayer(this);
		}
		//_txtLabel.show();
	} else if (e->type == GDK_BUTTON_RELEASE) {
		KageLayer::mouseIsDown = false;
		grab_focus();
		render();
	} else if (e->type == GDK_BUTTON_PRESS) {
		KageLayer::mouseIsDown = true;
		if (e->button.x < 18) {
			toggleVisibility();
		} else if (e->button.x < 36) {
			toggleLock();
		} else {
			_layerManager->setSelected(this);
		}
		_layerManager->renderStage();
		render();
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
	}*/
	cout << "Layer on_event " << endl;
	return true;
}


void KageLayer::forceRender() {
	Kage::timestamp_IN(); cout << " KageLayer::forceRender()" << endl;
	render();
	Kage::timestamp_OUT();
}
bool KageLayer::render() {
	//there's NEW queue function similar to invalidating drawing
	/*Kage::timestamp_IN(); cout << " KageLayer::render()" << endl;
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
	
	Kage::timestamp_OUT();
	*/
	queue_draw();
	return true;
}

void KageLayer::on_draw(const Cairo::RefPtr<Cairo::Context>& p_cr, int p_width, int p_height) {
	/*if (!window) {
		window = get_window();
	}*/
	//if (window) {
		//if (!cr) {
			cr = p_cr;
		//}
		if (!KageLayer::imageVISIBLE_TRUE) {
			try {
				KageLayer::imageVISIBLE_TRUE  = Gdk::Pixbuf::create_from_resource("/kage/share/layer/visible_true.png");
				KageLayer::imageVISIBLE_FALSE = Gdk::Pixbuf::create_from_resource("/kage/share/layer/visible_false.png");
				KageLayer::imageLOCKED_TRUE     = Gdk::Pixbuf::create_from_resource("/kage/share/layer/locked_true.png");
				KageLayer::imageLOCKED_FALSE    = Gdk::Pixbuf::create_from_resource("/kage/share/layer/locked_false.png");
			} catch(const Gio::ResourceError &ex) {
				std::cerr << "ResourceError: " << ex.what() << std::endl;
			} catch(const Gdk::PixbufError &ex) {
				std::cerr << "PixbufError: " << ex.what() << std::endl;
			}
		}
		
		cout << "layer isSelected() " << isSelected() << endl;
		cout << "layer get_width() " << get_width() << endl;
		cout << "layer get_height() " << get_height() << endl;
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
		Pango::FontDescription font;
		font.set_family("Verdana");
  		font.set_weight(Pango::Weight::NORMAL);
		auto layout = create_pango_layout(_label);
			layout->set_font_description(font);
		//layout->show_in_cairo_context(cr);
		//cr->select_font_face ("Verdana", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_NORMAL);
		cr->set_font_size(12);
		
		cout << "layer _label " << _label << endl;
		if (isSelected() == true) {
			cr->set_source_rgb(1, 1, 1);
		} else {
			cr->set_source_rgb(0, 0, 0);
		}
		cr->move_to(50, 15);
		cr->show_text(_label);
		
		cout << "layer _visible " << _visible << endl;
		if (_visible == true) {
			Gdk::Cairo::set_source_pixbuf(cr, KageLayer::imageVISIBLE_TRUE, 2, 0);
		} else {
			Gdk::Cairo::set_source_pixbuf(cr, KageLayer::imageVISIBLE_FALSE, 2, 0);
		}
		cr->paint();
		
		cout << "layer _lock " << _lock << endl;
		if (_lock == true) {
			Gdk::Cairo::set_source_pixbuf(cr, KageLayer::imageLOCKED_TRUE, 18, 0);
		} else {
			Gdk::Cairo::set_source_pixbuf(cr, KageLayer::imageLOCKED_FALSE, 18, 0);
		}
		cr->paint();
		
		return;// true;
	//}
	//return false;
}

void KageLayer::setLabel(string p_label) {
	_label = p_label;
	render();
}
string KageLayer::getLabel() {
	return _label;
}
void KageLayer::setSelected(bool p_selected) {
	_selected = p_selected;
	render();
}
bool KageLayer::isSelected() {
	return _selected;
}

bool KageLayer::isVisible() {
	return _visible;
}

bool KageLayer::isLocked() {
	return _lock;
}

void KageLayer::setFocus() {
	grab_focus();
}

void KageLayer::toggleVisibility() {
	if (_visible == true) {
		_visible = false;
	} else {
		_visible = true;
	}
	render();
}

void KageLayer::toggleLock() {
	if (_lock == true) {
		_lock = false;
	} else {
		_lock = true;
	}
	render();
}

void KageLayer::setVisible(bool p_visible) {
	_visible = p_visible;
	render();
}

void KageLayer::setLock(bool p_lock) {
	_lock = p_lock;
	render();
}

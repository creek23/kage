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

#include "frame.h"
#include "framesmanager.h"
#include "../../kage.h"

#include <giomm/resource.h>

bool KageFrame::mouseIsDown = false;

Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageNULL;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageNULL_CUR;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageNULL_X;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageNULL_X_CUR;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageBLANK;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageBLANK_CUR;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageBLANK_CUR_SEL;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageBLANK_CUR_TWN;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageBLANK_CUR_SEL_TWN;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageBLANK_SEL;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageBLANK_SEL_TWN;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageBLANK_TWN;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageBLANK_X;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageBLANK_X_CUR;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageBLANK_X_CUR_SEL;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageBLANK_X_CUR_TWN;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageBLANK_X_CUR_SEL_TWN;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageBLANK_X_SEL;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageBLANK_X_SEL_TWN;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageBLANK_X_TWN;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageDRAWN;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageDRAWN_CUR;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageDRAWN_CUR_SEL;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageDRAWN_CUR_TWN;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageDRAWN_CUR_SEL_TWN;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageDRAWN_SEL;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageDRAWN_SEL_TWN;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageDRAWN_TWN;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageDRAWN_X;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageDRAWN_X_CUR;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageDRAWN_X_CUR_SEL;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageDRAWN_X_CUR_TWN;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageDRAWN_X_CUR_SEL_TWN;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageDRAWN_X_SEL;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageDRAWN_X_SEL_TWN;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageDRAWN_X_TWN;

KageFrame::KageFrame(KageFrameset *p_frameset, unsigned p_layerID, unsigned int p_frameID) :
		vectorsData() {
//	set_state_flags(Gtk::STATE_FLAG_NORMAL); -- fail on gtkmm4
	set_can_focus(true); //to accept key_press
	_frameset = p_frameset;
	layerID = p_layerID;
	frameID = p_frameID;
	set_size_request(8, 23);
	setSelected(false);
	setCurrent(false);
	forceSetTween(false);
	setNull(false);
	setExtension(KageFrame::EXTENSION_NOT);
//		add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
//		add_events(Gdk::ENTER_NOTIFY_MASK | Gdk::LEAVE_NOTIFY_MASK);
//		add_events(Gdk::FOCUS_CHANGE_MASK);
	//	add_events(Gdk::POINTER_MOTION_MASK);
	// Events.
	auto controller = Gtk::EventControllerKey::create();
		controller->signal_key_pressed().connect(
			sigc::mem_fun(*this, &KageFrame::on_key_pressed), false);
	add_controller(controller);

	set_draw_func(sigc::mem_fun(*this, &KageFrame::on_draw));

	controllerDrag  = Gtk::GestureDrag::create();
		controllerDrag->set_button(GDK_BUTTON_PRIMARY);
			controllerDrag->signal_drag_begin().connect(sigc::mem_fun(*this, &KageFrame::onDragBegin));
			controllerDrag->signal_drag_update().connect(sigc::mem_fun(*this, &KageFrame::onDragUpdate));
			controllerDrag->signal_drag_end().connect(sigc::mem_fun(*this, &KageFrame::onDragEnd));
	add_controller(controllerDrag);
	
	controllerStylus = Gtk::GestureStylus::create();
		controllerStylus->signal_down().connect(sigc::mem_fun(*this, &KageFrame::onStylusDown));
        controllerStylus->signal_up().connect(sigc::mem_fun(*this, &KageFrame::onStylusUp));
        controllerStylus->signal_motion().connect(sigc::mem_fun(*this, &KageFrame::onStylusMotion));
	add_controller(controllerStylus);
	
	controllerClick = Gtk::GestureClick::create();
		controllerClick->set_button(GDK_BUTTON_SECONDARY);
        	controllerClick->signal_pressed().connect(sigc::mem_fun(*this, &KageFrame::onClick));
	add_controller(controllerClick);
}

void KageFrame::onDragBegin(double p_x, double p_y) {
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

	KageFrame::mouseIsDown = true;
	_frameset->setSelected(this);
	cout << p_x << " " << p_y << endl;
	
	_frameset->getFsm()->renderStage();
}

void KageFrame::onDragUpdate(double p_x, double p_y) {
	// First check if the event sequence is already handled by the stylus gesture; if not
	// we will handle is here; if it is deny it
	auto sequence = controllerDrag->get_last_updated_sequence();
	if (controllerStylus->get_sequence_state(sequence) == Gtk::EventSequenceState::CLAIMED) {
		controllerDrag->set_sequence_state(sequence, Gtk::EventSequenceState::DENIED);
	} else {
		draw_brush(start_x + p_x, start_y + p_y);
	}
}

void KageFrame::onDragEnd(double p_x, double p_y) {
	// First check if the event sequence is already handled by the stylus gesture; if not
	// we will handle is here; if it is deny it
	auto sequence = controllerDrag->get_last_updated_sequence();
	if (controllerStylus->get_sequence_state(sequence) == Gtk::EventSequenceState::CLAIMED) {
		controllerDrag->set_sequence_state(sequence, Gtk::EventSequenceState::DENIED);
	} else {
		draw_brush(start_x + p_x, start_y + p_y);
	}

	KageFrame::mouseIsDown = false;
	grab_focus();
//	render();

}

KageFrame::~KageFrame() {
	//
}

bool KageFrame::on_key_pressed(guint keyval, guint keycode, Gdk::ModifierType state) {
	Kage::timestamp_IN();
	std::cout << " KageFrame(F " << frameID << " L " << layerID << ") on_key_press_event" << std::endl;
	if (keyval == GDK_KEY_period) {
		_frameset->getFsm()->switchToNextFrame(frameID);
		_frameset->getFsm()->renderStage();
	} else if (keyval == GDK_KEY_comma) {
		_frameset->getFsm()->switchToPreviousFrame(frameID);
		_frameset->getFsm()->renderStage();
	}
	Kage::timestamp_OUT();
	return true;
}

void KageFrame::onStylusDown(double p_x, double p_y) {
	auto pressure = controllerStylus->get_axis(Gdk::AxisUse::PRESSURE);
	draw_brush(p_x, p_y, pressure.value_or(0.5));
	// Claim the event sequence; the drag gesture should not handle it
	auto sequence = controllerStylus->get_last_updated_sequence();
	controllerStylus->set_sequence_state(sequence, Gtk::EventSequenceState::CLAIMED);
}

void KageFrame::onStylusMotion(double p_x, double p_y) {
	auto pressure = controllerStylus->get_axis(Gdk::AxisUse::PRESSURE);
	draw_brush(p_x, p_y, pressure.value_or(0.5));
	// Claim the event sequence; the drag gesture should not handle it
	auto sequence = controllerStylus->get_last_updated_sequence();
	controllerStylus->set_sequence_state(sequence, Gtk::EventSequenceState::CLAIMED);
}

void KageFrame::onStylusUp(double p_x, double p_y) {
	auto pressure = controllerStylus->get_axis(Gdk::AxisUse::PRESSURE);
	draw_brush(p_x, p_y, pressure.value_or(0.5));
	// Claim the event sequence; the drag gesture should not handle it
	auto sequence = controllerStylus->get_last_updated_sequence();
	controllerStylus->set_sequence_state(sequence, Gtk::EventSequenceState::CLAIMED);
}

void KageFrame::onClick (int n_press, double p_x, double p_y) {
//	clear_surface();
	queue_draw();
}
/* Draw a rectangle on the surface at the given position */
void KageFrame::draw_brush(double p_x, double p_y, double size) {
	/* Paint to the surface, where we store our state */
	//auto cr = Cairo::Context::create(get_native()->get_surface());
	if (cr) {
		double w = 12 * size;
		cr->rectangle(p_x - w*0.5, p_y - w*0.5, w, w);
		cr->fill();
		/* Now invalidate the drawing area. */
		queue_draw();
	}
}
/*bool KageFrame::on_expose_event(GdkEventExpose* e) {
	if (!window) {
		window = get_window();
	}
	if (window) {
		render();
	}
	
	return true;
}

bool KageFrame::on_mouse_down(asdm, double p_x, double p_y) {}
bool KageFrame::on_mouse_move(asdm, double p_x, double p_y) {}
bool KageFrame::on_mouse_up(asdm, double p_x, double p_y) {}*/
bool KageFrame_on_event(GdkEvent *e) {/*
	if (e->type == GDK_ENTER_NOTIFY) {
		Kage::timestamp_IN();
		std::cout << " KageFrame(F " << frameID << " L " << layerID << ") on_event enter" << std::endl;
		render();
		Kage::timestamp_OUT();
	} else if (e->type == GDK_LEAVE_NOTIFY) {
		Kage::timestamp_IN();
		std::cout << " KageFrame(F " << frameID << " L " << layerID << ") on_event leave" << std::endl;
		render();
		Kage::timestamp_OUT();
	} else if (e->type == GDK_BUTTON_RELEASE) {
		KageFrame::mouseIsDown = false;
		grab_focus();
//		render();
	} else if (e->type == GDK_BUTTON_PRESS) {
		KageFrame::mouseIsDown = true;
		_frameset->setSelected(this);
		cout << e->button.x << " " << e->button.y << endl;
		
		_frameset->getFsm()->renderStage();
	} else if (e->type == GDK_EXPOSE) {
		on_expose_event((GdkEventExpose*) e);
	} else if (e->type == GDK_FOCUS_CHANGE) {
		//filter out from echos
		cout << "GDK_FOCUS_CHANGE e->send_event " << ((GdkEventFocus*)e)->send_event << " e->in " << ((GdkEventFocus*)e)->in << endl;
		if (((GdkEventFocus*)e)->in) {
			KageFrame::_gotFocus = true;
		} else {
			KageFrame::_gotFocus = false;
		}
	} else if (e->type == GDK_KEY_PRESS) {
		on_key_press_event((GdkEventKey*) e);
	} else if (e->type == GDK_KEY_RELEASE) {
//		_frameset->getFsm()->setCurrentFrame(getCurrentFrame());
		//filter out from echos
	} else if (e->type == GDK_CONFIGURE) {
		//filter out from echos 
	} else {
		std::cout << " KageFrame(F " << frameID << " L " << layerID << ") on_event type? " << e->type << std::endl;
	}*/
	return true;
}


void KageFrame::forceRender() {
	render();
}
bool KageFrame::render() {
	/*Kage::timestamp_IN(); cout << " KageFrame::render()" << endl;
	if (!window) {
		window = get_window();
	}
	
	if (window) {
		// force our program to redraw the entire stage
		Gdk::Rectangle r(0, 0, get_allocation().get_width(),
				get_allocation().get_height());
		window->invalidate_rect(r, false);
	}
	
	Kage::timestamp_OUT();
	*/
	return true;
}

void KageFrame::on_draw(const Cairo::RefPtr<Cairo::Context>& p_cr, int p_width, int p_height) {
	/*if (!window) {
		window = get_window();
	}
	if (window) {*/
		if (!cr) {
			cr = p_cr;
		}	
		if (!KageFrame::imageNULL) {
			try {
				KageFrame::imageNULL           = Gdk::Pixbuf::create_from_resource("/kage/share/frame/null.png");
				KageFrame::imageNULL_CUR       = Gdk::Pixbuf::create_from_resource("/kage/share/frame/null_cur.png");
				KageFrame::imageNULL_X         = Gdk::Pixbuf::create_from_resource("/kage/share/frame/null_x.png");
				KageFrame::imageNULL_X_CUR     = Gdk::Pixbuf::create_from_resource("/kage/share/frame/null_x_cur.png");
				KageFrame::imageBLANK             = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank.png");
				KageFrame::imageBLANK_CUR         = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_cur.png");
				KageFrame::imageBLANK_CUR_SEL     = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_cur_sel.png");
				KageFrame::imageBLANK_CUR_SEL_TWN = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_cur_sel_twn.png");
				KageFrame::imageBLANK_CUR_TWN     = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_cur_twn.png");
				KageFrame::imageBLANK_SEL         = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_sel.png");
				KageFrame::imageBLANK_SEL_TWN     = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_sel_twn.png");
				KageFrame::imageBLANK_TWN         = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_twn.png");
				KageFrame::imageBLANK_X             = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_x.png");
				KageFrame::imageBLANK_X_CUR         = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_x_cur.png");
				KageFrame::imageBLANK_X_CUR_SEL     = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_x_cur_sel.png");
				KageFrame::imageBLANK_X_CUR_SEL_TWN = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_x_cur_sel_twn.png");
				KageFrame::imageBLANK_X_CUR_TWN     = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_x_cur_twn.png");
				KageFrame::imageBLANK_X_SEL         = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_x_sel.png");
				KageFrame::imageBLANK_X_SEL_TWN     = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_x_sel_twn.png");
				KageFrame::imageBLANK_X_TWN         = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_x_twn.png");
				KageFrame::imageDRAWN             = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn.png");
				KageFrame::imageDRAWN_CUR         = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_cur.png");
				KageFrame::imageDRAWN_CUR_SEL     = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_cur_sel.png");
				KageFrame::imageDRAWN_CUR_SEL_TWN = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_cur_sel_twn.png");
				KageFrame::imageDRAWN_CUR_TWN     = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_cur_twn.png");
				KageFrame::imageDRAWN_SEL         = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_sel.png");
				KageFrame::imageDRAWN_SEL_TWN     = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_sel_twn.png");
				KageFrame::imageDRAWN_TWN         = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_twn.png");
				KageFrame::imageDRAWN_X             = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_x.png");
				KageFrame::imageDRAWN_X_CUR         = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_x_cur.png");
				KageFrame::imageDRAWN_X_CUR_SEL     = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_x_cur_sel.png");
				KageFrame::imageDRAWN_X_CUR_SEL_TWN = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_x_cur_sel_twn.png");
				KageFrame::imageDRAWN_X_CUR_TWN     = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_x_cur_twn.png");
				KageFrame::imageDRAWN_X_SEL         = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_x_sel.png");
				KageFrame::imageDRAWN_X_SEL_TWN     = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_x_sel_twn.png");
				KageFrame::imageDRAWN_X_TWN         = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_x_twn.png");
			} catch (const Gio::ResourceError &ex) {
				std::cerr << "KageFrame::ResourceError: " << ex.what() << std::endl;
			} catch (const Gdk::PixbufError &ex) {
				std::cerr << "KageFrame::PixbufError: " << ex.what() << std::endl;
			}
		}
		bool l_current = _frameset->isCurrentFrame(frameID);
		bool l_selected = isSelected();
		bool l_tween = (getTween() > 0);
		switch (_extension) {
			case KageFrame::EXTENSION_NOT:
			case KageFrame::EXTENSION_END:
				if (isEmpty() == true) {
					if (       l_current == true  && l_selected == false && l_tween == false) {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageBLANK_CUR, 0, -1);
					} else if (l_current == false && l_selected == true  && l_tween == false) {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageBLANK_SEL, 0, -1);
					} else if (l_current == false && l_selected == false && l_tween == true ) {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageBLANK_TWN, 0, -1);
					} else if (l_current == true  && l_selected == true  && l_tween == false) {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageBLANK_CUR_SEL, 0, -1);
					} else if (l_current == true  && l_selected == true  && l_tween == true ) {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageBLANK_CUR_SEL_TWN, 0, -1);
					} else if (l_current == true  && l_selected == false && l_tween == true ) {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageBLANK_CUR_TWN, 0, -1);
					} else if (l_current == false && l_selected == true  && l_tween == true ) {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageBLANK_SEL_TWN, 0, -1);
					} else {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageBLANK, 0, -1);
					}
				} else if (isNull() == true) {
					if (_frameset->isCurrentFrame(frameID) == true) {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageNULL_CUR, 0, -1);
					} else {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageNULL, 0, -1);
					}
				} else {
					if (       l_current == true  && l_selected == false && l_tween == false) {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageDRAWN_CUR, 0, -1);
					} else if (l_current == false && l_selected == true  && l_tween == false) {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageDRAWN_SEL, 0, -1);
					} else if (l_current == false && l_selected == false && l_tween == true ) {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageDRAWN_TWN, 0, -1);
					} else if (l_current == true  && l_selected == true  && l_tween == false) {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageDRAWN_CUR_SEL, 0, -1);
					} else if (l_current == true  && l_selected == true  && l_tween == true ) {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageDRAWN_CUR_SEL_TWN, 0, -1);
					} else if (l_current == true  && l_selected == false && l_tween == true ) {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageDRAWN_CUR_TWN, 0, -1);
					} else if (l_current == false && l_selected == true  && l_tween == true ) {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageDRAWN_SEL_TWN, 0, -1);
					} else {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageDRAWN, 0, -1);
					}
				}
				break;
			case KageFrame::EXTENSION_START:
			case KageFrame::EXTENSION_MID:
				if (isEmpty() == true) {
					if (       l_current == true  && l_selected == false && l_tween == false) {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageBLANK_X_CUR, 0, -1);
					} else if (l_current == false && l_selected == true  && l_tween == false) {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageBLANK_X_SEL, 0, -1);
					} else if (l_current == false && l_selected == false && l_tween == true ) {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageBLANK_X_TWN, 0, -1);
					} else if (l_current == true  && l_selected == true  && l_tween == false) {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageBLANK_X_CUR_SEL, 0, -1);
					} else if (l_current == true  && l_selected == true  && l_tween == true ) {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageBLANK_X_CUR_SEL_TWN, 0, -1);
					} else if (l_current == true  && l_selected == false && l_tween == true ) {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageBLANK_X_CUR_TWN, 0, -1);
					} else if (l_current == false && l_selected == true  && l_tween == true ) {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageBLANK_X_SEL_TWN, 0, -1);
					} else {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageBLANK_X, 0, -1);
					}
				} else if (isNull() == true) {
					if (_frameset->isCurrentFrame(frameID) == true) {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageNULL_X_CUR, 0, -1);
					} else {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageNULL_X, 0, -1);
					}
				} else {
					if (       l_current == true  && l_selected == false && l_tween == false) {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageDRAWN_X_CUR, 0, -1);
					} else if (l_current == false && l_selected == true  && l_tween == false) {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageDRAWN_X_SEL, 0, -1);
					} else if (l_current == false && l_selected == false && l_tween == true ) {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageDRAWN_X_TWN, 0, -1);
					} else if (l_current == true  && l_selected == true  && l_tween == false) {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageDRAWN_X_CUR_SEL, 0, -1);
					} else if (l_current == true  && l_selected == true  && l_tween == true ) {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageDRAWN_X_CUR_SEL_TWN, 0, -1);
					} else if (l_current == true  && l_selected == false && l_tween == true ) {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageDRAWN_X_CUR_TWN, 0, -1);
					} else if (l_current == false && l_selected == true  && l_tween == true ) {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageDRAWN_X_SEL_TWN, 0, -1);
					} else {
						Gdk::Cairo::set_source_pixbuf(p_cr, KageFrame::imageDRAWN_X, 0, -1);
					}
				}
				break;
		}
		p_cr->paint();
		
		return;// true;
	//}
	return;// false;
}

void KageFrame::setSelected(bool p_selected) {
	_selected = p_selected;
	render();
}
bool KageFrame::isSelected() {
	return _selected;
}

void KageFrame::setExtension(KageFrame::extension p_extension) {
	_extension = p_extension;
	render();
}
KageFrame::extension KageFrame::getExtension() {
	return _extension;
}

void KageFrame::setNull(bool p_null) {
	_null = p_null;
	render();
}
bool KageFrame::isNull() {
	return _null;
}
void KageFrame::forceSetTween(unsigned int p_tween) {
	_tweenX = p_tween/10;
	_tweenY = p_tween - (_tweenX*10);
	cout << " KageFrame::setTween() " << _tweenX << " " << _tweenY << endl;
	
	render();
}
void KageFrame::setTween(unsigned int p_tween) {
	if (_null == true) {
		return;
	}
	cout << " KageFrame::setTween() " << frameID << " " << p_tween << endl;
	if (       _extension == KageFrame::EXTENSION_NOT) {
		forceSetTween(p_tween);
	} else if (_extension == KageFrame::EXTENSION_START) {
		forceSetTween(p_tween);
		_frameset->setExtendedFrameTween(frameID, p_tween);
	} else {
		_frameset->setPreviousFrameTween(frameID, p_tween);
	}
}
unsigned int KageFrame::getTween() {
//	cout << " KageFrame::getTween() " << _tweenX << " " << _tweenY << " | " << ((_tweenX * 10) + _tweenY) << endl;
	return (_tweenX * 10) + _tweenY;
}

void KageFrame::setCurrent(bool p_current) {
	if (p_current == true) {
		grab_focus();
	}
	_current = p_current;
	render();
}
bool KageFrame::isCurrent() {
	return _current;
}

VectorDataManager KageFrame::getFrameData() {
	if (_null == true) {
		cout << "KageFrame::getFrameData is returning empty" << endl;
		VectorDataManager l_nullReturn;
		return l_nullReturn;
	}
	if (       _extension == KageFrame::EXTENSION_NOT
			|| _extension == KageFrame::EXTENSION_START) {
		return vectorsData;
	} else {
		return _frameset->getPreviousFrameData(frameID);
	}
}
void KageFrame::setFrameData(VectorDataManager p_vectorsData) {
	if (_null == true) {
		setNull(false);
	}
	if (       _extension == KageFrame::EXTENSION_NOT
			|| _extension == KageFrame::EXTENSION_START) {
		vectorsData = p_vectorsData;
		render();
	} else {
		_frameset->setFrameDataToPreviousFrame(p_vectorsData, frameID);
	}
}

bool KageFrame::isEmpty() {
	if (       _extension == KageFrame::EXTENSION_NOT
			|| _extension == KageFrame::EXTENSION_START) {
		return (vectorsData.getVectorData().size() == 0);
	} else {
		return (_frameset->getPreviousFrameData(frameID).getVectorData().size() <= 1);
	}
}

void KageFrame::setFocus() {
	grab_focus();
}

vector<unsigned int> KageFrame::raiseSelectedShape(vector<unsigned int> p_selectedShapes) {
	if (       _extension == KageFrame::EXTENSION_NOT
			|| _extension == KageFrame::EXTENSION_START) {
		return vectorsData.raiseSelectedShape(p_selectedShapes);
	} else {
		VectorDataManager l_vectorData = _frameset->getPreviousFrameData(frameID);
		vector<unsigned int> l_selectedShapes = l_vectorData.raiseSelectedShape(p_selectedShapes);
		setFrameData(l_vectorData);
		return l_selectedShapes;
	}
}
vector<unsigned int> KageFrame::lowerSelectedShape(vector<unsigned int> p_selectedShapes) {
	if (       _extension == KageFrame::EXTENSION_NOT
			|| _extension == KageFrame::EXTENSION_START) {
		return vectorsData.lowerSelectedShape(p_selectedShapes);
	} else {
		VectorDataManager l_vectorData = _frameset->getPreviousFrameData(frameID);
		vector<unsigned int> l_selectedShapes = l_vectorData.lowerSelectedShape(p_selectedShapes);
		setFrameData(l_vectorData);
		return l_selectedShapes;
	}
}
vector<unsigned int> KageFrame::raiseToTopSelectedShape(vector<unsigned int> p_selectedShapes) {
	if (       _extension == KageFrame::EXTENSION_NOT
			|| _extension == KageFrame::EXTENSION_START) {
		return vectorsData.raiseToTopSelectedShape(p_selectedShapes);
	} else {
		VectorDataManager l_vectorData = _frameset->getPreviousFrameData(frameID);
		vector<unsigned int> l_selectedShapes = l_vectorData.raiseToTopSelectedShape(p_selectedShapes);
		setFrameData(l_vectorData);
		return l_selectedShapes;
	}
}
vector<unsigned int> KageFrame::lowerToBottomSelectedShape(vector<unsigned int> p_selectedShapes) {
	if (       _extension == KageFrame::EXTENSION_NOT
			|| _extension == KageFrame::EXTENSION_START) {
		return vectorsData.lowerToBottomSelectedShape(p_selectedShapes);
	} else {
		VectorDataManager l_vectorData = _frameset->getPreviousFrameData(frameID);
		vector<unsigned int> l_selectedShapes = l_vectorData.lowerToBottomSelectedShape(p_selectedShapes);
		setFrameData(l_vectorData);
		return l_selectedShapes;
	}
}

vector<unsigned int> KageFrame::groupSelectedShapes(vector<unsigned int> p_selectedShapes) {
	if (       _extension == KageFrame::EXTENSION_NOT
			|| _extension == KageFrame::EXTENSION_START) {
		return vectorsData.groupSelectedShapes(p_selectedShapes);
	} else {
		VectorDataManager l_vectorData = _frameset->getPreviousFrameData(frameID);
		vector<unsigned int> l_selectedShapes = l_vectorData.groupSelectedShapes(p_selectedShapes);
		setFrameData(l_vectorData);
		return l_selectedShapes;
	}
}
vector<unsigned int> KageFrame::ungroupSelectedShapes(vector<unsigned int> p_selectedShapes) {
	if (       _extension == KageFrame::EXTENSION_NOT
			|| _extension == KageFrame::EXTENSION_START) {
		return vectorsData.ungroupSelectedShapes(p_selectedShapes);
	} else {
		VectorDataManager l_vectorData = _frameset->getPreviousFrameData(frameID);
		vector<unsigned int> l_selectedShapes = l_vectorData.ungroupSelectedShapes(p_selectedShapes);
		setFrameData(l_vectorData);
		return l_selectedShapes;
	}
}

vector<unsigned int> KageFrame::duplicateShapes(vector<unsigned int> p_selectedShapes) {
	if (       _extension == KageFrame::EXTENSION_NOT
			|| _extension == KageFrame::EXTENSION_START) {
		return vectorsData.duplicateShapes(p_selectedShapes);
	} else {
		VectorDataManager l_vectorData = _frameset->getPreviousFrameData(frameID);
		vector<unsigned int> l_selectedShapes = l_vectorData.duplicateShapes(p_selectedShapes);
		setFrameData(l_vectorData);
		return l_selectedShapes;
	}
}

bool KageFrame::flipHorizontalSelectedShape(vector<unsigned int> p_selectedShapes) {
	if (       _extension == KageFrame::EXTENSION_NOT
			|| _extension == KageFrame::EXTENSION_START) {
		return vectorsData.flipHorizontalSelectedShape(p_selectedShapes);
	} else {
		VectorDataManager l_vectorData = _frameset->getPreviousFrameData(frameID);
		bool l_selectedShapes = l_vectorData.flipHorizontalSelectedShape(p_selectedShapes);
		setFrameData(l_vectorData);
		return l_selectedShapes;
	}
}
bool KageFrame::flipVerticalSelectedShape(vector<unsigned int> p_selectedShapes) {
	if (       _extension == KageFrame::EXTENSION_NOT
			|| _extension == KageFrame::EXTENSION_START) {
		return vectorsData.flipVerticalSelectedShape(p_selectedShapes);
	} else {
		VectorDataManager l_vectorData = _frameset->getPreviousFrameData(frameID);
		bool l_selectedShapes = l_vectorData.flipVerticalSelectedShape(p_selectedShapes);
		setFrameData(l_vectorData);
		return l_selectedShapes;
	}
}
bool KageFrame::recenterRotationPoint(vector<unsigned int> p_selectedShapes) {
	if (       _extension == KageFrame::EXTENSION_NOT
			|| _extension == KageFrame::EXTENSION_START) {
		return vectorsData.recenterRotationPoint(p_selectedShapes);
	} else {
		VectorDataManager l_vectorData = _frameset->getPreviousFrameData(frameID);
		bool l_selectedShapes = l_vectorData.recenterRotationPoint(p_selectedShapes);
		setFrameData(l_vectorData);
		return l_selectedShapes;
	}
}
void KageFrame::addDataToFrame(VectorDataManager v) {
	if (       _extension == KageFrame::EXTENSION_NOT
			|| _extension == KageFrame::EXTENSION_START) {
		if (_null == true) {
			setNull(false);
		}
		vectorsData.push(v);
		forceRender();
	} else {
		_frameset->addDataToPreviousFrame(v, frameID);
	}
}

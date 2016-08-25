
#include "frame.h"
#include <cairomm/context.h>
#include <cairomm/cairomm.h>
#include <iostream>
#include "kage/data/vectordatamanager.cpp"

KageFrame::KageFrame(KageFrameManager *p_fm, unsigned p_layerID, unsigned int p_frameID) :
		vectorsData() {
	set_flags(Gtk::CAN_FOCUS);
	set_can_focus(true); //to accept key_press
	current = false;
	fm = p_fm;
	layerID = p_layerID;
	frameID = p_frameID;
	set_size_request(8, 20);
	selected = false;
//	add_events(Gdk::KEY_PRESS_MASK    | Gdk::KEY_RELEASE_MASK);
	add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
	add_events(Gdk::ENTER_NOTIFY_MASK | Gdk::LEAVE_NOTIFY_MASK);
	add_events(Gdk::FOCUS_CHANGE_MASK);
//	add_events(Gdk::POINTER_MOTION_MASK);
//	signal_event().connect(sigc::mem_fun(*this, &KageFrame::on_event));
}

KageFrame::~KageFrame() {
	//
}

bool KageFrame::on_key_press_event(GdkEventKey *e) {
	if (e->keyval == 46) { //. dot
		fm->getFsm()->setCurrentFrame(frameID+1);
		fm->focusFrame(frameID+1);
	} else if (e->keyval == 44) { //, comma
		fm->getFsm()->setCurrentFrame(frameID-1);
		fm->focusFrame(frameID-1);
	} else {
		cout << "f" << frameID << " " << e->keyval << "_" << e->string << endl;
	}
}
bool KageFrame::on_expose_event(GdkEventExpose* e) {
	if (!window) {
		window = get_window();
	}
	if (window) {
		Gtk::Allocation allocation = get_allocation();
		const int width = allocation.get_width();
		const int height = allocation.get_height();
		Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
		cr->rectangle(e->area.x, e->area.y, e->area.width, e->area.height);
		cr->clip();
		render();
	}
	
	return true;
}

bool KageFrame::on_event(GdkEvent *e) {
	if (e->type == GDK_ENTER_NOTIFY) {
		cout << "f" << frameID << " enter notify... " << e->type << endl;
		render();
	} else if (e->type == GDK_LEAVE_NOTIFY) {
		cout << "f" << frameID << " leave notify... " << e->type << endl;
		render();
	} else if (e->type == GDK_BUTTON_RELEASE) {
		KageFrame::mouseIsDown = false;
		fm->getFsm()->setCurrentFrame(KageFramesManager::currentFrame);
		grab_focus();
//		render();
	} else if (e->type == GDK_BUTTON_PRESS) {
		KageFrame::mouseIsDown = true;
		//KageFramesManager::currentFrame = frameID;
		cout << "layerID " << layerID << endl;
		fm->getFsm()->setCurrentLayer(layerID);
		fm->getFsm()->setCurrentFrame(frameID);
		fm->getFsm()->selectAll(false);
		setSelected(true);
		cout << e->button.x << " " << e->button.y << endl;
	} else if (e->type == GDK_EXPOSE) {
		on_expose_event((GdkEventExpose*) e);
	} else if (e->type == GDK_FOCUS_CHANGE) {
		//filter out from echos
	} else if (e->type == GDK_KEY_PRESS) {
		on_key_press_event((GdkEventKey*) e);
	} else if (e->type == GDK_KEY_RELEASE) {
		fm->getFsm()->setCurrentFrame(KageFramesManager::currentFrame);
		//filter out from echos
	} else if (e->type == GDK_CONFIGURE) {
		//filter out from echos 
	} else {
		cout << "f" << frameID << "?" << e->type << endl;
	}
	return true;
}


bool KageFrame::render() {
	if (!window) {
		window = get_window();
	}
	if (window) {
		if (!KageFrame::imageNULL) {
			KageFrame::imageNULL     = Gdk::Pixbuf::create_from_file("shared/frame/null.png");
		}
		if (!KageFrame::imageNULL_CUR) {
			KageFrame::imageNULL_CUR = Gdk::Pixbuf::create_from_file("shared/frame/null_cur.png");
		}
		if (!KageFrame::imageBLANK) {
			KageFrame::imageBLANK     = Gdk::Pixbuf::create_from_file("shared/frame/blank.png");
		}
		if (!KageFrame::imageBLANK_CUR) {
			KageFrame::imageBLANK_CUR = Gdk::Pixbuf::create_from_file("shared/frame/blank_cur.png");
		}
		if (!KageFrame::imageDRAWN) {
			KageFrame::imageDRAWN     = Gdk::Pixbuf::create_from_file("shared/frame/drawn.png");
		}
		if (!KageFrame::imageDRAWN_CUR) {
			KageFrame::imageDRAWN_CUR = Gdk::Pixbuf::create_from_file("shared/frame/drawn_cur.png");
		}
		if (!KageFrame::imageSELECTED) {
			KageFrame::imageSELECTED     = Gdk::Pixbuf::create_from_file("shared/frame/selected.png");
		}
		if (!KageFrame::imageSELECTED_CUR) {
			KageFrame::imageSELECTED_CUR = Gdk::Pixbuf::create_from_file("shared/frame/selected_cur.png");
		}
		if (isSelected() == true) {
			//if (isCurrent() == true) {
			if (KageFramesManager::currentFrame == frameID) {
				KageFrame::imageSELECTED_CUR->render_to_drawable(window, get_style()->get_black_gc(), 0, 0, 0, 0, 8, 20, Gdk::RGB_DITHER_NONE, 0, 0);
			} else {
				KageFrame::imageSELECTED->render_to_drawable(window, get_style()->get_black_gc(), 0, 0, 0, 0, 8, 20, Gdk::RGB_DITHER_NONE, 0, 0);
			}
		} else {
			if (isEmpty() == true) {
				//if (isCurrent() == true) {
				if (KageFramesManager::currentFrame == frameID) {
					KageFrame::imageBLANK_CUR->render_to_drawable(
							window, get_style()->get_black_gc(),
							0, 0, 0, 0, 8, 20, // draw the whole imageNULL (from 0,0 to the full width,height) at 100,80 in the window
						//	0, 0, 0, 0, imageNULL->get_width(), imageNULL->get_height(),
							Gdk::RGB_DITHER_NONE, 0, 0
						);
				} else {
					KageFrame::imageBLANK->render_to_drawable(window, get_style()->get_black_gc(), 0, 0, 0, 0, 8, 20, Gdk::RGB_DITHER_NONE, 0, 0);
				}
			} else {
				//if (isCurrent() == true) {
				if (KageFramesManager::currentFrame == frameID) {
					KageFrame::imageDRAWN_CUR->render_to_drawable(window, get_style()->get_black_gc(), 0, 0, 0, 0, 8, 20, Gdk::RGB_DITHER_NONE, 0, 0);
				} else {
					KageFrame::imageDRAWN->render_to_drawable(window, get_style()->get_black_gc(), 0, 0, 0, 0, 8, 20, Gdk::RGB_DITHER_NONE, 0, 0);
				}
			}
		}
		
		return true;
		Gtk::Allocation allocation = get_allocation();
		const int width = allocation.get_width();
		const int height = allocation.get_height();
		Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
//		cr->rectangle(e->area.x, e->area.y, e->area.width, e->area.height);
//		cr->clip();
		//draw frame-background
		cr->set_line_width(0.0);
		if (isSelected() == true) {
			cr->set_source_rgb(0.0, 0.0, 0.44);
		} else {
			if (isEmpty() == true) {
				cr->set_source_rgb(1.0, 1.0, 1.0);
			} else {
				cr->set_source_rgb(0.9, 0.9, 0.9);
			}
		}
			cr->move_to(0, 0);
			cr->line_to(width, 0);
			cr->line_to(width, height);
			cr->line_to(0, height);
			cr->line_to(0, 0);
		cr->fill_preserve();

		//draw frame-border
		cr->set_line_width(1.0);
		cr->set_source_rgb(0.87, 0.87, 0.87);
		cr->move_to(width, 0);
		cr->line_to(width, height);
		cr->line_to(0, height);
		cr->stroke();
	}
}

void KageFrame::setSelected(bool p_selected) {
	selected = p_selected;
	render();
}
bool KageFrame::isSelected() {
	return selected;
}

void KageFrame::setCurrent(bool p_current) {
	current = p_current;
	render();
}
bool KageFrame::isCurrent() {
	return current;
}

bool KageFrame::isEmpty() {
	return (vectorsData.getVectorData().size() <= 1);
}

unsigned int KageFrame::getFrameSource() {
	return frameSource;
}

void KageFrame::setFocus() {
	grab_focus();
}

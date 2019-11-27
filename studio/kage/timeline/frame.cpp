
#include <cairomm/context.h>
#include <cairomm/cairomm.h>
#include <gdkmm/general.h> // set_source_pixbuf()
#include <iostream>

#include "frame.h"
#include "framesmanager.h"
#include "../../kage.h"

KageFrame::KageFrame(KageFrameManager *p_fm, unsigned p_layerID, unsigned int p_frameID) :
		vectorsData() {
	set_state_flags(Gtk::STATE_FLAG_NORMAL);
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
	Kage::timestamp();
	std::cout << " KageFrame(F " << frameID << " L " << layerID << ") on_key_press_event" << std::endl;
	if (e->keyval == 46) { //. dot
		fm->getFsm()->setCurrentFrame(frameID+1);
		fm->focusFrame(frameID+1);
	} else if (e->keyval == 44) { //, comma
		fm->getFsm()->setCurrentFrame(frameID-1);
		fm->focusFrame(frameID-1);
	}
	return true;
}
bool KageFrame::on_expose_event(GdkEventExpose* e) {
	if (!window) {
		window = get_window();
	}
	if (window) {
		render();
	}
	
	return true;
}

bool KageFrame::on_event(GdkEvent *e) {
	if (e->type == GDK_ENTER_NOTIFY) {
		Kage::timestamp();
		std::cout << " KageFrame(F " << frameID << " L " << layerID << ") on_event enter" << std::endl;
		render();
	} else if (e->type == GDK_LEAVE_NOTIFY) {
		Kage::timestamp();
		std::cout << " KageFrame(F " << frameID << " L " << layerID << ") on_event leave" << std::endl;
		render();
	} else if (e->type == GDK_BUTTON_RELEASE) {
		KageFrame::mouseIsDown = false;
		fm->getFsm()->setCurrentFrame(frameID);
		fm->getFsm()->setCurrentLayer(layerID);
		grab_focus();
//		render();
	} else if (e->type == GDK_BUTTON_PRESS) {
		KageFrame::mouseIsDown = true;
		//KageFramesManager::currentFrame = frameID; ///?!?
		fm->getFsm()->setCurrentLayer(layerID);
		fm->getFsm()->setCurrentFrame(frameID);
		fm->getFsm()->selectAll(false);
		setSelected(true);
		cout << e->button.x << " " << e->button.y << endl;
		
		fm->getFsm()->renderStage();
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
	}
	return true;
}


void KageFrame::forceRender() {
	render();
}
bool KageFrame::render() {
	if (!window) {
		window = get_window();
	}
	
	// force our program to redraw the entire stage
	Gdk::Rectangle r(0, 0, get_allocation().get_width(),
			get_allocation().get_height());
	window->invalidate_rect(r, false);
	
	return true;
}

bool KageFrame::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
	if (!window) {
		window = get_window();
	}
	if (window) {
		if (!KageFrame::imageNULL) {
			#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
				string _SLASH_ = "\\";
			#else
				string _SLASH_ = "/";
			#endif
			
			KageFrame::imageNULL         = Gdk::Pixbuf::create_from_file("shared" + _SLASH_ + "frame" + _SLASH_ + "null.png");
			KageFrame::imageNULL_CUR     = Gdk::Pixbuf::create_from_file("shared" + _SLASH_ + "frame" + _SLASH_ + "null_cur.png");
			KageFrame::imageBLANK        = Gdk::Pixbuf::create_from_file("shared" + _SLASH_ + "frame" + _SLASH_ + "blank.png");
			KageFrame::imageBLANK_CUR    = Gdk::Pixbuf::create_from_file("shared" + _SLASH_ + "frame" + _SLASH_ + "blank_cur.png");
			KageFrame::imageDRAWN        = Gdk::Pixbuf::create_from_file("shared" + _SLASH_ + "frame" + _SLASH_ + "drawn.png");
			KageFrame::imageDRAWN_CUR    = Gdk::Pixbuf::create_from_file("shared" + _SLASH_ + "frame" + _SLASH_ + "drawn_cur.png");
			KageFrame::imageSELECTED     = Gdk::Pixbuf::create_from_file("shared" + _SLASH_ + "frame" + _SLASH_ + "selected.png");
			KageFrame::imageSELECTED_CUR = Gdk::Pixbuf::create_from_file("shared" + _SLASH_ + "frame" + _SLASH_ + "selected_cur.png");
		}
		if (isSelected() == true) {
			if (KageFramesManager::currentFrame == frameID) {
				Gdk::Cairo::set_source_pixbuf(cr, KageFrame::imageSELECTED_CUR, 0, 0);
			} else {
				Gdk::Cairo::set_source_pixbuf(cr, KageFrame::imageSELECTED, 0, 0);
			}
		} else if (isEmpty() == true) {
			if (KageFramesManager::currentFrame == frameID) {
				Gdk::Cairo::set_source_pixbuf(cr, KageFrame::imageBLANK_CUR, 0, 0);
			} else {
				Gdk::Cairo::set_source_pixbuf(cr, KageFrame::imageBLANK, 0, 0);
			}
		} else {
			if (KageFramesManager::currentFrame == frameID) {
				Gdk::Cairo::set_source_pixbuf(cr, KageFrame::imageDRAWN_CUR, 0, 0);
			} else {
				Gdk::Cairo::set_source_pixbuf(cr, KageFrame::imageDRAWN, 0, 0);
			}
		}
		cr->paint();
		
		return true;
	}
	return false;
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

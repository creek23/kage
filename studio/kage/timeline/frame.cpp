
#include <cairomm/context.h>
#include <cairomm/cairomm.h>
#include <gdkmm/general.h> // set_source_pixbuf()
#include <iostream>

#include "frame.h"
#include "framesmanager.h"
#include "../../kage.h"

#include <giomm/resource.h>

KageFrame::KageFrame(KageFrameManager *p_fm, unsigned p_layerID, unsigned int p_frameID) :
		vectorsData() {
	set_state_flags(Gtk::STATE_FLAG_NORMAL);
	set_can_focus(true); //to accept key_press
	current = false;
	fm = p_fm;
	layerID = p_layerID;
	frameID = p_frameID;
	set_size_request(8, 23);
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
	if (e->keyval == GDK_KEY_period) {
		fm->getFsm()->setCurrentFrame(frameID+1);
		fm->focusFrame(frameID+1);
	} else if (e->keyval == GDK_KEY_comma) {
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
		std::cout << " KageFrame(F " << frameID << " L " << layerID << ") on_event ANY" << std::endl;
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
		fm->setSelected(this); //a bit redundant?
		grab_focus();
//		render();
	} else if (e->type == GDK_BUTTON_PRESS) {
		KageFrame::mouseIsDown = true;
		//KageFramesManager::currentFrame = frameID; ///?!?
		fm->setSelected(this);
		cout << e->button.x << " " << e->button.y << endl;
		
		fm->getFsm()->renderStage();
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
	
	if (window) {
		// force our program to redraw the entire stage
		Gdk::Rectangle r(0, 0, get_allocation().get_width(),
				get_allocation().get_height());
		window->invalidate_rect(r, false);
	}
	return true;
}

bool KageFrame::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
	if (!window) {
		window = get_window();
	}
	if (window) {
		if (!KageFrame::imageNULL) {
			try {
				KageFrame::imageNULL         = Gdk::Pixbuf::create_from_resource("/kage/share/frame/null.png");
				KageFrame::imageNULL_CUR     = Gdk::Pixbuf::create_from_resource("/kage/share/frame/null_cur.png");
				KageFrame::imageBLANK        = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank.png");
				KageFrame::imageBLANK_CUR    = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_cur.png");
				KageFrame::imageDRAWN        = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn.png");
				KageFrame::imageDRAWN_CUR    = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_cur.png");
				KageFrame::imageSELECTED     = Gdk::Pixbuf::create_from_resource("/kage/share/frame/selected.png");
				KageFrame::imageSELECTED_CUR = Gdk::Pixbuf::create_from_resource("/kage/share/frame/selected_cur.png");
			} catch(const Gio::ResourceError &ex) {
				std::cerr << "KageFrame::ResourceError: " << ex.what() << std::endl;
			} catch(const Gdk::PixbufError &ex) {
				std::cerr << "KageFrame::PixbufError: " << ex.what() << std::endl;
			}
		}
		if (isSelected() == true) {
			if (KageFramesManager::currentFrame == frameID) {
				Gdk::Cairo::set_source_pixbuf(cr, KageFrame::imageSELECTED_CUR, 0, -1);
			} else {
				Gdk::Cairo::set_source_pixbuf(cr, KageFrame::imageSELECTED, 0, -1);
			}
		} else if (isEmpty() == true) {
			if (KageFramesManager::currentFrame == frameID) {
				Gdk::Cairo::set_source_pixbuf(cr, KageFrame::imageBLANK_CUR, 0, -1);
			} else {
				Gdk::Cairo::set_source_pixbuf(cr, KageFrame::imageBLANK, 0, -1);
			}
		} else {
			if (KageFramesManager::currentFrame == frameID) {
				Gdk::Cairo::set_source_pixbuf(cr, KageFrame::imageDRAWN_CUR, 0, -1);
			} else {
				Gdk::Cairo::set_source_pixbuf(cr, KageFrame::imageDRAWN, 0, -1);
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

vector<unsigned int> KageFrame::raiseSelectedShape(vector<unsigned int> p_selectedShapes) {
	return vectorsData.raiseSelectedShape(p_selectedShapes);
}
vector<unsigned int> KageFrame::lowerSelectedShape(vector<unsigned int> p_selectedShapes) {
	return vectorsData.lowerSelectedShape(p_selectedShapes);
}
vector<unsigned int> KageFrame::raiseToTopSelectedShape(vector<unsigned int> p_selectedShapes) {
	return vectorsData.raiseToTopSelectedShape(p_selectedShapes);
}
vector<unsigned int> KageFrame::lowerToBottomSelectedShape(vector<unsigned int> p_selectedShapes) {
	return vectorsData.lowerToBottomSelectedShape(p_selectedShapes);
}

vector<unsigned int> KageFrame::groupSelectedShapes(vector<unsigned int> p_selectedShapes) {
	return vectorsData.groupSelectedShapes(p_selectedShapes);
}
vector<unsigned int> KageFrame::ungroupSelectedShapes(vector<unsigned int> p_selectedShapes) {
	return vectorsData.ungroupSelectedShapes(p_selectedShapes);
}

vector<unsigned int> KageFrame::duplicateShapes(vector<unsigned int> p_selectedShapes) {
	return vectorsData.duplicateShapes(p_selectedShapes);
}

bool KageFrame::flipHorizontalSelectedShape(vector<unsigned int> p_selectedShapes) {
	return vectorsData.flipHorizontalSelectedShape(p_selectedShapes);
}
bool KageFrame::flipVerticalSelectedShape(vector<unsigned int> p_selectedShapes) {
	return vectorsData.flipVerticalSelectedShape(p_selectedShapes);
}

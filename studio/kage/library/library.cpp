/*
 * Kage Studio - a simple free and open source vector-based 2D animation software
 * Copyright (C) 2023  Mj Mendoza IV
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

#include "library.h"
#include "../../kage.h"

#include <giomm/resource.h>

bool KageLibrary::mouseIsDown = false;
bool KageLibrary::_gotFocus = false;

Glib::RefPtr<Gdk::Pixbuf> KageLibrary::imageNULL;

KageLibrary::KageLibrary(Kage* p_kage) :
		vectorsData() {
    _kage = p_kage;
	set_state_flags(Gtk::STATE_FLAG_NORMAL);
	set_can_focus(true); //to accept key_press
	set_size_request(120, 120);
	setSelected(false);
	setExtension(KageLibrary::EXTENSION_NOT);
//	add_events(Gdk::KEY_PRESS_MASK    | Gdk::KEY_RELEASE_MASK);
	add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
	add_events(Gdk::ENTER_NOTIFY_MASK | Gdk::LEAVE_NOTIFY_MASK);
	add_events(Gdk::FOCUS_CHANGE_MASK);
//	add_events(Gdk::POINTER_MOTION_MASK);

	resetAssetID();
}

KageLibrary::~KageLibrary() {
	//
}

bool KageLibrary::on_key_press_event(GdkEventKey *e) {
	Kage::timestamp_IN();
	//arrow key presses could mean user is trying to navigate to other assets
	Kage::timestamp_OUT();
	return true;
}
bool KageLibrary::on_expose_event(GdkEventExpose* e) {
	if (!window) {
		window = get_window();
	}
	if (window) {
		invalidateToRender();
	}
	
	return true;
}

bool KageLibrary::on_event(GdkEvent *e) {
	if (e->type == GDK_ENTER_NOTIFY) {
		Kage::timestamp_IN();
		invalidateToRender();
		Kage::timestamp_OUT();
	} else if (e->type == GDK_LEAVE_NOTIFY) {
		Kage::timestamp_IN();
		invalidateToRender();
		Kage::timestamp_OUT();
	} else if (e->type == GDK_BUTTON_RELEASE) {
		KageLibrary::mouseIsDown = false;
		grab_focus();
//		invalidateToRender();
	} else if (e->type == GDK_BUTTON_PRESS) {
		KageLibrary::mouseIsDown = true;
		//
	} else if (e->type == GDK_EXPOSE) {
		on_expose_event((GdkEventExpose*) e);
	} else if (e->type == GDK_FOCUS_CHANGE) {
		//filter out from echos
		cout << "GDK_FOCUS_CHANGE e->send_event " << ((GdkEventFocus*)e)->send_event << " e->in " << ((GdkEventFocus*)e)->in << endl;
		if (((GdkEventFocus*)e)->in) {
			KageLibrary::_gotFocus = true;
		} else {
			KageLibrary::_gotFocus = false;
		}
	} else if (e->type == GDK_KEY_PRESS) {
		on_key_press_event((GdkEventKey*) e);
	} else if (e->type == GDK_KEY_RELEASE) {
//		_frameset->getFsm()->setCurrentFrame(getCurrentFrame());
		//filter out from echos
	} else if (e->type == GDK_CONFIGURE) {
		//filter out from echos 
	}
	return true;
}


void KageLibrary::forceRender() {
	if (KageFramesetManager::LOADING_MODE == true) {
		return;
	}
		invalidateToRender();
	Kage::timestamp_OUT();
}
bool KageLibrary::invalidateToRender() {
	if (KageFramesetManager::LOADING_MODE == true) {
		return true;
	}
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

bool KageLibrary::on_draw(const Cairo::RefPtr<Cairo::Context>& p_context) {
	if (KageFramesetManager::LOADING_MODE == true) {
		return true;
	}
	if (!window) {
		window = get_window();
	}
	if (window) {
		if (!KageLibrary::imageNULL) {
			try {
				KageLibrary::imageNULL           = Gdk::Pixbuf::create_from_resource("/kage/share/frame/null.png");
			} catch (const Gio::ResourceError &ex) {
				std::cerr << "KageLibrary::ResourceError: " << ex.what() << std::endl;
			} catch (const Gdk::PixbufError &ex) {
				std::cerr << "KageLibrary::PixbufError: " << ex.what() << std::endl;
			}
		}
		bool l_current = false;
		bool l_selected = isSelected();
		bool l_tween = false;

		//TODO: preview asset based on width and height of this Asset Preview Widget
		p_context->move_to(  0,   0);
		p_context->line_to(get_width(),   0);
		p_context->line_to(get_width(), 120);
		p_context->line_to(  0, 120);
		p_context->line_to(  0,   0);
		p_context->set_source_rgba(0.85f, 0.85f, 0.85f, 1.0f);
		p_context->fill_preserve();

		//TODO: check if _renderAssetID is IMAGE
		if (_renderAssetID != UINT_MAX && _kage->m_KageStage.cairoPNG[_renderAssetID]) {
			try {
				KageStage::cairoImageSurface = _kage->m_KageStage.cairoPNG[_renderAssetID];
				
				double imageWidth = KageStage::cairoImageSurface->get_width();
				double imageHeight = KageStage::cairoImageSurface->get_height();
				
				double l_scale = 1.0f;
				if (imageWidth > imageHeight) {
					l_scale = ((double)get_width() / imageWidth);
				} else {
					l_scale = ((double)get_height() / imageHeight);
				}
				
				double l_xPos = ((double)get_width()  / 2.0f) - (imageWidth  * l_scale / 2.0f);
				double l_yPos = ((double)get_height() / 2.0f) - (imageHeight * l_scale / 2.0f);
				
				p_context->save();
					p_context->scale(l_scale, l_scale);
					p_context->set_source(KageStage::cairoImageSurface, l_xPos/l_scale, l_yPos/l_scale);
					p_context->paint();
				p_context->restore();
			} catch (std::bad_alloc const &ex) {
				cout << "handling zoom error: bad_alloc" << endl;
			} catch (std::exception ex) {
				//unhandled exception (type ) in signal handler:
				//what: std::bad_alloc
			}
		} else {
			p_context->select_font_face ("Verdana", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD);
			p_context->set_font_size(12);
			p_context->set_source_rgba(0.3f, 0.3f, 0.3f, 1.0f);
			p_context->move_to(10, 40);
			p_context->show_text("Under Construction");
			p_context->move_to(10, 80);
			p_context->show_text("Preview Asset");
		}
		
		return true;
	}
	return false;
}

void KageLibrary::render(unsigned int p_assetID) {
	_renderAssetID = p_assetID;

	invalidateToRender();
}

void KageLibrary::setSelected(bool p_selected) {
	_selected = p_selected;
	invalidateToRender();
}
bool KageLibrary::isSelected() {
	return _selected;
}

void KageLibrary::setExtension(KageLibrary::extension p_extension) {
	//_extension = p_extension;
	invalidateToRender();
}
KageLibrary::extension KageLibrary::getExtension() {
	KageLibrary::extension foo; 
	return foo;
}

VectorDataManager KageLibrary::getFrameData() {
	if (_null == true) {
		cout << "KageLibrary::getFrameData is returning empty" << endl;
		VectorDataManager l_nullReturn;
		return l_nullReturn;
	}
	VectorDataManager l_nullReturn;
		return l_nullReturn;
}
void KageLibrary::setFrameData(VectorDataManager p_vectorsData) {
	//
}

bool KageLibrary::isEmpty() {
	return false;
}

void KageLibrary::setFocus() {
	grab_focus();
}

vector<unsigned int> KageLibrary::raiseSelectedShape(vector<unsigned int> p_selectedShapes) {
	vector<unsigned int> foo;
	return foo;
}
void KageLibrary::addDataToFrame(VectorDataManager v) {
	//
}
void KageLibrary::resetAssetID() {
	_renderAssetID = UINT_MAX;
}

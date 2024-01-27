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

#include <cairomm/context.h>
#include <cairomm/cairomm.h>
#include <gdkmm/general.h> // set_source_pixbuf()
#include <iostream>

#include "../../kage.h"

#include <giomm/resource.h>

bool KageScenesUI::mouseIsDown = false;
bool KageScenesUI::_gotFocus = false;
Glib::RefPtr<Gdk::Pixbuf> KageScenesUI::iconSCENE_KAGE;

KageScenesUI::KageScenesUI(Kage* p_kage) {
	_kage = p_kage;
	
	set_state_flags(Gtk::STATE_FLAG_NORMAL);
	set_can_focus(true); //to accept key_press
	set_size_request(320, 24);
	
	addEventsListener();

	//if (!KageScenesUI::iconSCENE_KAGE) {
		try {
			//TODO: use icons from here -> https://www.iconfinder.com/iconsets/ionicons
			KageScenesUI::iconSCENE_KAGE  = Gdk::Pixbuf::create_from_resource("/kage/share/library/asset_kage.png");
		} catch(const Gio::ResourceError &ex) {
			std::cerr << "ResourceError: " << ex.what() << std::endl;
		} catch(const Gdk::PixbufError &ex) {
			std::cerr << "PixbufError: " << ex.what() << std::endl;
		}
	//}
}

KageScenesUI::~KageScenesUI() {
	_kage = NULL;
}

void KageScenesUI::addEventsListener() {
//	add_events(Gdk::KEY_PRESS_MASK    | Gdk::KEY_RELEASE_MASK);
	add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
	add_events(Gdk::ENTER_NOTIFY_MASK | Gdk::LEAVE_NOTIFY_MASK);
	add_events(Gdk::FOCUS_CHANGE_MASK);
	
	//*window = NULL;
}

bool KageScenesUI::on_key_press_event(GdkEventKey *e) {
	Kage::timestamp_IN();
	if (e->keyval == GDK_KEY_Control_L || e->keyval == GDK_KEY_Control_R) {
		//
	} else if (e->keyval == GDK_KEY_Up || e->keyval == GDK_KEY_KP_Up) {
		//_kage->_document->setAboveSceneUISelected(this);
	} else if (e->keyval == GDK_KEY_Down || e->keyval == GDK_KEY_KP_Down) {
		//_kage->_document->setBelowSceneUISelected(this);
	}
	Kage::timestamp_OUT();
	return true;
}
bool KageScenesUI::on_key_release_event(GdkEventKey *e) {
/*	std::cout << " KageScenesUI(L " << sceneUIID << ") on_key_release_event" << std::endl;
	if (e->keyval == GDK_KEY_Control_L || e->keyval == GDK_KEY_Control_R) {
		//
	}*/
	
	return true;
}

bool KageScenesUI::on_expose_event(GdkEventExpose* e) {
	if (!window) {
		window = get_window();
	}
	if (window) {
		invalidateToRender();
	}
	
	return true;
}

bool KageScenesUI::on_event(GdkEvent *e) {
	std::cout << "KageScenesUI::on_event" << std::endl;
	if (e->type == GDK_ENTER_NOTIFY) {
		invalidateToRender();
	} else if (e->type == GDK_MOTION_NOTIFY) {
		draw2.x = e->button.x;
		draw2.y = e->button.y;
		invalidateToRender();
	} else if (e->type == GDK_LEAVE_NOTIFY) {
		KageScenesUI::mouseIsDown = false;
		invalidateToRender();
	} else if (e->type == GDK_DOUBLE_BUTTON_PRESS) {
		//determine Y location to identify which scene to rename
		int l_scenes= _kage->_document.Scenes.size();
		double l_y = get_height() - (l_scenes * FRAME_HEIGHT);
		int l_index = l_scenes - ((e->button.y - l_y) / FRAME_HEIGHT);
		std::cout << "\n double clicked on l_index " << l_index << " / " << _kage->_document.Scenes.size() << std::endl;
		if (l_index < _kage->_document.Scenes.size()) {
			KageScene *l_scene = _kage->_document.Scenes[l_index];
			std::cout << "\te->button.x " << e->button.x << std::endl;
			try {
				if (e->button.x > 36) {
					string l_sceneLabel = _kage->_document.getScene()->getLabel();
					LabelRenameDialog* pDialog = new LabelRenameDialog(*_kage, l_sceneLabel);
						pDialog->run();
					_kage->_document.getScene()->setLabel(pDialog->getLabel());
					delete pDialog;
				}
			} catch (std::exception& e) {
				std::cout << "KageScenesUI::on_event Exception : " << e.what() << std::endl;
			}
		}
		//_txtLabel.show();
	} else if (e->type == GDK_BUTTON_RELEASE) {
		KageScenesUI::mouseIsDown = false;
		grab_focus();
		invalidateToRender();
	} else if (e->type == GDK_BUTTON_PRESS) {
		KageScenesUI::mouseIsDown = true;
		//determine Y location to identify which scene to highlight
		int l_scenes = _kage->_document.Scenes.size();
		//double l_y = get_height() - (l_scenes * FRAME_HEIGHT);
		//int l_index = l_scenes - ((e->button.y - l_y) / FRAME_HEIGHT);
		int l_index = (e->button.y / FRAME_HEIGHT);
		std::cout << "KageScenesUI::on_event clicked on l_index " << l_index << " / " << l_scenes << std::endl;
		try {
			if (l_index < l_scenes) {
				_kage->setDocumentCurrentScene(l_index+1);
			} else {
				std::cout << "KageScenesUI::on_event\n\t_kage->_document.getActiveSceneID() " << _kage->_document.getActiveSceneID() << " ??? " << _kage->_document.getScene()->Layers.size() << std::endl << std::endl;
			}
		} catch (std::exception& e) {
			std::cout << "KageScenesUI::on_event Exception : " << e.what() << std::endl;
		}
	} else if (e->type == GDK_EXPOSE) {
		on_expose_event((GdkEventExpose*) e);
	} else if (e->type == GDK_FOCUS_CHANGE) {
		//filter out from echos
	} else if (e->type == GDK_KEY_PRESS) {
		on_key_press_event((GdkEventKey*) e);
	} else if (e->type == GDK_KEY_RELEASE) {
		on_key_release_event((GdkEventKey*) e);
	} else if (e->type == GDK_CONFIGURE) {
		//filter out from echos 
	}
	return true;
}


void KageScenesUI::forceRender() {
	if (KageScene::LOADING_MODE == true) {
		return;
	}
	Kage::timestamp_IN(); std::cout << " KageScenesUI::forceRender()" << std::endl;
	invalidateToRender();
	Kage::timestamp_OUT();
}
bool KageScenesUI::invalidateToRender() {
	if (KageScene::LOADING_MODE == true) {
		return true;
	}
	Kage::timestamp_IN(); std::cout << " KageScenesUI::invalidateToRender()" << std::endl;
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
		std::cerr << "KageScenesUI::invalidateToRender Exception : " << e.what() << std::endl;
	}
	
	Kage::timestamp_OUT();
	
	return true;
}

bool KageScenesUI::on_draw(const Cairo::RefPtr<Cairo::Context>& p_context) {
	if (KageScene::LOADING_MODE == true) {
		return true;
	}
	if (!window) {
		window = get_window();
	}
	if (window) {
		//draw background
		p_context->move_to(          0,            0);
		p_context->line_to(get_width(),            0);
		p_context->line_to(get_width(), get_height());
		p_context->line_to(          0, get_height());
		p_context->line_to(          0,            0);
		if (Kage::NotDarkMode) {
			p_context->set_source_rgb(0.75f, 0.75f, 0.75f);
		} else {
			p_context->set_source_rgb(0.38f, 0.38f, 0.38f);
		}
		p_context->fill();
		
		unsigned int l_scenes = _kage->_document.Scenes.size();

		int l_width, l_height;
		get_size_request(l_width, l_height);
		if (l_height/FRAME_HEIGHT_OFFSET < l_scenes) {
			l_width = FRAME_WIDTH;
			l_height = l_scenes * FRAME_HEIGHT_OFFSET;
			set_size_request(l_width, l_height);
		}
		
		double l_y = 0;
		KageScene *l_scene;
		for (unsigned int i = 0; i < l_scenes; ++i) {
			l_scene = _kage->_document.Scenes[i];
	        //draw background
	        
				p_context->move_to(              0, l_y               );
					p_context->line_to(get_width(), l_y               );
					p_context->line_to(get_width(), l_y + FRAME_HEIGHT);
					p_context->line_to(          0, l_y + FRAME_HEIGHT);
				p_context->close_path();
			if (l_scene->isSelected() == true) {
				if (Kage::NotDarkMode) {
					p_context->set_source_rgb(0.00f, 0.47f, 0.84f);
				} else {
					p_context->set_source_rgb(0.58f, 0.07f, 0.00f);
				}
			} else {
				if (Kage::NotDarkMode) {
					p_context->set_source_rgb( 0.965f, 0.961f, 0.957f);
				} else {
					p_context->set_source_rgb(0.21f, 0.21f, 0.21f);
				}
			}
				p_context->fill();
			//draw label
			p_context->select_font_face ("Verdana", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_NORMAL);
			p_context->set_font_size(12);
			if (l_scene->isSelected() == true) {
				p_context->set_source_rgb(1, 1, 1);
			} else {
				if (Kage::NotDarkMode) {
					p_context->set_source_rgb(0, 0, 0);
				} else {
					p_context->set_source_rgb(1, 1, 1);
				}
			}
			p_context->move_to(24, 15 + l_y);
			p_context->show_text(l_scene->getLabel());
			
			Gdk::Cairo::set_source_pixbuf(p_context, KageScenesUI::iconSCENE_KAGE, 2, l_y);
			p_context->paint();
			
			l_y += FRAME_HEIGHT;
		}
		
		return true;
	}
	return false;
}

void KageScenesUI::setFocus() {
	grab_focus();
}

/** For use of KageLayer.  When a KageLayer's visibility is clicked,
 * this function will be called to reflect Layer's visibility on stage.
 */
void KageScenesUI::renderStage() { //who is calling ScenesUI renderStage?
	if (KageScene::LOADING_MODE == true) {
		return;
	}
	_kage->forceRenderFrames();
	_kage->refreshUI();
}
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

bool KageLayersUI::mouseIsDown = false;
bool KageLayersUI::_gotFocus = false;
Glib::RefPtr<Gdk::Pixbuf> KageLayersUI::imageVISIBLE_TRUE;
Glib::RefPtr<Gdk::Pixbuf> KageLayersUI::imageVISIBLE_FALSE;
Glib::RefPtr<Gdk::Pixbuf> KageLayersUI::imageLOCKED_TRUE;
Glib::RefPtr<Gdk::Pixbuf> KageLayersUI::imageLOCKED_FALSE;

KageLayersUI::KageLayersUI(Kage* p_kage) {
	_kage = p_kage;
	
	set_state_flags(Gtk::STATE_FLAG_NORMAL);
	set_can_focus(true); //to accept key_press
	set_size_request(FRAME_WIDTH, FRAME_HEIGHT);
	
	addEventsListener();

	try {
		KageLayersUI::imageVISIBLE_TRUE  = Gdk::Pixbuf::create_from_resource("/kage/share/layer/visible_true.png");
		KageLayersUI::imageVISIBLE_FALSE = Gdk::Pixbuf::create_from_resource("/kage/share/layer/visible_false.png");
		KageLayersUI::imageLOCKED_TRUE     = Gdk::Pixbuf::create_from_resource("/kage/share/layer/locked_true.png");
		KageLayersUI::imageLOCKED_FALSE    = Gdk::Pixbuf::create_from_resource("/kage/share/layer/locked_false.png");
	} catch(const Gio::ResourceError &ex) {
		std::cerr << "ResourceError: " << ex.what() << std::endl;
	} catch(const Gdk::PixbufError &ex) {
		std::cerr << "PixbufError: " << ex.what() << std::endl;
	}
}

KageLayersUI::~KageLayersUI() {
	_kage = NULL;
}

void KageLayersUI::addEventsListener() {
//	add_events(Gdk::KEY_PRESS_MASK    | Gdk::KEY_RELEASE_MASK);
	add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
	add_events(Gdk::ENTER_NOTIFY_MASK | Gdk::LEAVE_NOTIFY_MASK);
	add_events(Gdk::FOCUS_CHANGE_MASK);
	
	//*window = NULL;
}

bool KageLayersUI::on_key_press_event(GdkEventKey *e) {
	Kage::timestamp_IN();
	if (e->keyval == GDK_KEY_period) {
	} else if (e->keyval == GDK_KEY_comma) {
	}
	Kage::timestamp_OUT();
	return true;
}
bool KageLayersUI::on_key_release_event(GdkEventKey *e) {
/*	if (e->keyval == GDK_KEY_Control_L || e->keyval == GDK_KEY_Control_R) {
		g_FrameModifier = false; //toggle-off Frame-move/resize listeners
		g_FrameModify = FrameModifier::MODIFY_NOT;
		invalidateToRender();
	}*/
	
	return true;
}

bool KageLayersUI::on_expose_event(GdkEventExpose* e) {
	if (!window) {
		window = get_window();
	}
	if (window) {
		invalidateToRender();
	}
	
	return true;
}

bool KageLayersUI::on_event(GdkEvent *e) {
	if (e->type == GDK_ENTER_NOTIFY) {
		invalidateToRender();
	} else if (e->type == GDK_MOTION_NOTIFY) {
		draw2.x = e->button.x;
		draw2.y = e->button.y;
		invalidateToRender();
	} else if (e->type == GDK_LEAVE_NOTIFY) {
		KageLayersUI::mouseIsDown = false;
		invalidateToRender();
	} else if (e->type == GDK_DOUBLE_BUTTON_PRESS) {
		//determine Y location to identify which layer to rename
		try {
			int l_layers = _kage->_document.getScene()->Layers.size();
			double l_y = get_height() - (l_layers * FRAME_HEIGHT);
			int l_index = l_layers - ((e->button.y - l_y) / FRAME_HEIGHT);
			std::cout << "\n double clicked on l_index " << l_index << " / " << _kage->_document.getScene()->Layers.size() << std::endl;
			if (l_index < _kage->_document.getScene()->Layers.size()) {
				KageLayer *l_layer = _kage->_document.getScene()->Layers[l_index];
				std::cout << "\te->button.x " << e->button.x << std::endl;
				if (e->button.x > 36) {
					std::string l_layerLabel = _kage->_document.getScene()->getLayerLabel();
					LabelRenameDialog* pDialog = new LabelRenameDialog(*_kage, l_layerLabel);
						pDialog->run();
					_kage->_document.getScene()->setLayerLabel(pDialog->getLabel());
					delete pDialog;
				}
			} else {
				std::cout << "_kage->_document.getActiveSceneID() " << _kage->_document.getActiveSceneID() << std::endl << std::endl;
			}
		} catch (std::exception& e) {
			std::cout << "KageLayersUI::on_event Exception : " << e.what() << std::endl;
		}
	} else if (e->type == GDK_BUTTON_RELEASE) {
		KageLayersUI::mouseIsDown = false;
		grab_focus();
		invalidateToRender();
	} else if (e->type == GDK_BUTTON_PRESS) {
		KageLayersUI::mouseIsDown = true;
		//determine Y location to identify which layer to toggleVisibility/Lock
		try {
			int l_layers = _kage->_document.getScene()->Layers.size();
			double l_y = get_height() - (l_layers * FRAME_HEIGHT);
			int l_index = l_layers - ((e->button.y - l_y) / FRAME_HEIGHT);
			std::cout << "\n clicked on l_index " << l_index << " / " << _kage->_document.getScene()->Layers.size() << std::endl;
			if (l_index < _kage->_document.getScene()->Layers.size()) {
				KageLayer *l_layer = _kage->_document.getScene()->Layers[l_index];
				std::cout << "\te->button.x " << e->button.x << std::endl;
				if (e->button.x < 18) {
					l_layer->toggleVisibility();
					invalidateToRender();
					renderStage();
					_kage->stackDo();
				} else if (e->button.x < 36) {
					l_layer->toggleLock();
					invalidateToRender();
					_kage->stackDo();
				} else {
					_kage->setDocumentSceneCurrentLayer(l_index+1, false);
					_kage->setDocumentSceneLayerCurrentFrame(_kage->getDocumentSceneLayerCurrentFrame(), false);
					_kage->stackDo();
				}
			} else {
				std::cout << "_kage->_document.getActiveSceneID() " << _kage->_document.getActiveSceneID() << std::endl << std::endl;
			}
		} catch (std::exception& e) {
			std::cout << "KageLayersUI::on_event Exception : " << e.what() << std::endl;
		}
	} else if (e->type == GDK_EXPOSE) {
		on_expose_event((GdkEventExpose*) e);
	} else if (e->type == GDK_FOCUS_CHANGE) {
		//filter out from echos
		std::cout << "KageLayersUI GDK_FOCUS_CHANGE e->send_event " << ((GdkEventFocus*)e)->send_event << " e->in " << ((GdkEventFocus*)e)->in << std::endl;
		if (((GdkEventFocus*)e)->in) {
			KageTimeline::_gotFocus = true;
		} else {
			KageTimeline::_gotFocus = false;
		}
	} else if (e->type == GDK_KEY_PRESS) {
		on_key_press_event((GdkEventKey*) e);
	} else if (e->type == GDK_KEY_RELEASE) {
		on_key_release_event((GdkEventKey*) e);
	} else if (e->type == GDK_CONFIGURE) {
		//filter out from echos 
	}
	return true;
}


void KageLayersUI::forceRender() {
	if (KageScene::LOADING_MODE == true) {
		return;
	}
	invalidateToRender();
}
bool KageLayersUI::invalidateToRender() {
	if (KageScene::LOADING_MODE == true) {
		return true;
	}
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
		std::cerr << "KageLayersUI::invalidateToRender Exception : " << e.what() << std::endl;
	}
	
	Kage::timestamp_OUT();
	
	return true;
}

bool KageLayersUI::on_draw(const Cairo::RefPtr<Cairo::Context>& p_context) {
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
		unsigned int l_layers = 0;
		try {
			l_layers = _kage->_document.getScene()->Layers.size();
		} catch (std::exception& e) {
			std::cout << "KageLayersUI::on_draw Exception : " << e.what() << std::endl;
		}

		int l_width, l_height;
		get_size_request(l_width, l_height);
		if (l_height/FRAME_HEIGHT_OFFSET < l_layers) {
			l_width = FRAME_WIDTH;
			l_height = l_layers * FRAME_HEIGHT_OFFSET;
			set_size_request(l_width, l_height);
		}
		
		double l_y = get_height() - FRAME_HEIGHT;
		KageLayer *l_layer;
		for (unsigned int i = 0; i < l_layers; ++i) {
			l_layer = NULL;
			try {
				l_layer = _kage->_document.getScene()->Layers[i];
				//draw background
				p_context->move_to(              0, l_y               );
					p_context->line_to(get_width(), l_y               );
					p_context->line_to(get_width(), l_y + FRAME_HEIGHT);
					p_context->line_to(          0, l_y + FRAME_HEIGHT);
				p_context->close_path();

				if (l_layer->isSelected() == true) {
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
				//TODO: read up about fill_preserve vs fill
				//p_context->fill_preserve(); <-- not working as assumed/expected behavior
				
				//draw label
				p_context->select_font_face ("Verdana", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_NORMAL);
				p_context->set_font_size(12);
				if (l_layer->isSelected() == true) {
					p_context->set_source_rgb(1, 1, 1);
				} else {
					if (Kage::NotDarkMode) {
						p_context->set_source_rgb(0, 0, 0);
					} else {
						p_context->set_source_rgb(1, 1, 1);
					}
				}
				p_context->move_to(50, 15 + l_y);
				p_context->show_text(l_layer->getLabel());
				
				if (l_layer->isVisible() == true) {
					Gdk::Cairo::set_source_pixbuf(p_context, KageLayersUI::imageVISIBLE_TRUE, 2, l_y);
				} else {
					Gdk::Cairo::set_source_pixbuf(p_context, KageLayersUI::imageVISIBLE_FALSE, 2, l_y);
				}
				p_context->paint();
				
				if (l_layer->isLocked() == true) {
					Gdk::Cairo::set_source_pixbuf(p_context, KageLayersUI::imageLOCKED_TRUE, 18, l_y);
				} else {
					Gdk::Cairo::set_source_pixbuf(p_context, KageLayersUI::imageLOCKED_FALSE, 18, l_y);
				}
				p_context->paint();
			} catch (std::exception& e) {
				std::cout << "KageLayersUI::on_draw Exception : " << e.what() << std::endl;
			}
			
			l_y -= FRAME_HEIGHT;
		}
		
		return true;
	}
	return false;
}

void KageLayersUI::setFocus() {
	grab_focus();
}

/** For use of KageLayer.  When a KageLayer's visibility is clicked,
 * this function will be called to reflect Layer's visibility on stage.
 */
void KageLayersUI::renderStage() {
	if (KageScene::LOADING_MODE == true) {
		return;
	}
	_kage->forceRenderFrames();
//	_kage->_layers.forceRender(); redundant?
	_kage->refreshUI();
}
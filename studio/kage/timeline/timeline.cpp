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

#include "timeline.h"
#include "../data/frame.h"
#include "../data/layer.h"
#include "../../kage.h"

#include <giomm/resource.h>

bool KageTimeline::mouseIsDown = false;
bool KageTimeline::_gotFocus = false;

Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageNULL;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageNULL_CUR;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageNULL_X;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageNULL_X_CUR;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageBLANK;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageBLANK_CUR;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageBLANK_CUR_SEL;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageBLANK_CUR_TWN;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageBLANK_CUR_SEL_TWN;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageBLANK_SEL;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageBLANK_SEL_TWN;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageBLANK_TWN;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageBLANK_X;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageBLANK_X_CUR;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageBLANK_X_CUR_SEL;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageBLANK_X_CUR_TWN;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageBLANK_X_CUR_SEL_TWN;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageBLANK_X_SEL;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageBLANK_X_SEL_TWN;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageBLANK_X_TWN;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageDRAWN;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageDRAWN_CUR;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageDRAWN_CUR_SEL;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageDRAWN_CUR_TWN;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageDRAWN_CUR_SEL_TWN;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageDRAWN_SEL;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageDRAWN_SEL_TWN;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageDRAWN_TWN;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageDRAWN_X;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageDRAWN_X_CUR;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageDRAWN_X_CUR_SEL;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageDRAWN_X_CUR_TWN;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageDRAWN_X_CUR_SEL_TWN;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageDRAWN_X_SEL;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageDRAWN_X_SEL_TWN;
Glib::RefPtr<Gdk::Pixbuf> KageTimeline::imageDRAWN_X_TWN;

KageTimeline::KageTimeline(Kage* p_kage) :
		vectorsData() {
    _kage = p_kage;
	set_state_flags(Gtk::STATE_FLAG_NORMAL);
	set_can_focus(true); //to accept key_press
	set_size_request(120, 23);
	setSelected(false);
	add_events(Gdk::KEY_PRESS_MASK    | Gdk::KEY_RELEASE_MASK);
	add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
	add_events(Gdk::ENTER_NOTIFY_MASK | Gdk::LEAVE_NOTIFY_MASK);
	add_events(Gdk::FOCUS_CHANGE_MASK);
	add_events(Gdk::POINTER_MOTION_MASK);
}

KageTimeline::~KageTimeline() {
	//
}

unsigned int g_frameSelectIndex;
bool KageTimeline::on_key_press_event(GdkEventKey *e) {
	if (e->keyval == GDK_KEY_period) {
		if (_kage->switchToNextFrame()) {
			++g_frameSelectIndex;
		}
	} else if (e->keyval == GDK_KEY_comma) {
		if (_kage->switchToPreviousFrame()) {
			--g_frameSelectIndex;
		}
	}

	return true;
}
bool KageTimeline::on_expose_event(GdkEventExpose* e) {
	if (!window) {
		window = get_window();
	}
	if (window) {
		invalidateToRender();
	}
	
	return true;
}

bool KageTimeline::on_event(GdkEvent *e) {
	if (e->type == GDK_ENTER_NOTIFY) {
		invalidateToRender();
	} else if (e->type == GDK_MOTION_NOTIFY) {
		draw2.x = e->button.x;
		draw2.y = e->button.y;
		invalidateToRender();
	} else if (e->type == GDK_LEAVE_NOTIFY) {
		KageTimeline::mouseIsDown = false;
		invalidateToRender();
	} else if (e->type == GDK_BUTTON_RELEASE) {
		KageTimeline::mouseIsDown = false;
		grab_focus();
		if (draw1.x == draw2.x && draw1.y == draw2.y) {
			//single SELECT frame
			const unsigned int FRAME_WIDTH_OFFSET = 8;
			const unsigned int FRAME_HEIGHT = 23;
			unsigned int p_frameIndex = ((draw1.x-FRAME_WIDTH_OFFSET) - ((unsigned int)(draw1.x-FRAME_WIDTH_OFFSET) % 8)) / 8;
			unsigned int p_layerCount = _kage->_document.Scenes[_kage->_document.getActiveSceneID()].Layers.size();
			unsigned int p_layerHeight = p_layerCount * FRAME_HEIGHT;
			if (get_height() >= p_layerHeight) {
				draw1.y -= (get_height() - p_layerHeight); //remove excess
			} else {
				draw1.y += (p_layerHeight-get_height()); //pad hidden layers
			}
			unsigned int l_layerSelectIndex = (draw1.y - ((unsigned int)draw1.y % FRAME_HEIGHT)) / FRAME_HEIGHT;
			g_frameSelectIndex = p_frameIndex;
			_kage->setCurrentLayer(p_layerCount-l_layerSelectIndex);
			_kage->setCurrentFrame(p_frameIndex+1);
		} else {
			//TODO: multi-SELECT frames
		}
		invalidateToRender();
	} else if (e->type == GDK_BUTTON_PRESS) {
		KageTimeline::mouseIsDown = true;
		
		draw1.x = e->button.x;
		draw1.y = e->button.y;
		//
	} else if (e->type == GDK_EXPOSE) {
		on_expose_event((GdkEventExpose*) e);
	} else if (e->type == GDK_FOCUS_CHANGE) {
		//filter out from echos
		cout << "GDK_FOCUS_CHANGE e->send_event " << ((GdkEventFocus*)e)->send_event << " e->in " << ((GdkEventFocus*)e)->in << endl;
		if (((GdkEventFocus*)e)->in) {
			KageTimeline::_gotFocus = true;
		} else {
			KageTimeline::_gotFocus = false;
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


void KageTimeline::forceRender() {
	if (KageScene::LOADING_MODE == true) {
		return;
	}
		invalidateToRender();
	Kage::timestamp_OUT();
}
bool KageTimeline::invalidateToRender() {
	if (KageScene::LOADING_MODE == true) {
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

bool KageTimeline::on_draw(const Cairo::RefPtr<Cairo::Context>& p_context) {
	if (KageScene::LOADING_MODE == true) {
		return true;
	}
	if (!window) {
		window = get_window();
	}
	if (window) {
		if (!KageTimeline::imageNULL) {
			try {
				KageTimeline::imageNULL           = Gdk::Pixbuf::create_from_resource("/kage/share/frame/null.png");
				KageTimeline::imageNULL_CUR       = Gdk::Pixbuf::create_from_resource("/kage/share/frame/null_cur.png");
				KageTimeline::imageNULL_X         = Gdk::Pixbuf::create_from_resource("/kage/share/frame/null_x.png");
				KageTimeline::imageNULL_X_CUR     = Gdk::Pixbuf::create_from_resource("/kage/share/frame/null_x_cur.png");
				KageTimeline::imageBLANK             = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank.png");
				KageTimeline::imageBLANK_CUR         = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_cur.png");
				KageTimeline::imageBLANK_CUR_SEL     = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_cur_sel.png");
				KageTimeline::imageBLANK_CUR_SEL_TWN = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_cur_sel_twn.png");
				KageTimeline::imageBLANK_CUR_TWN     = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_cur_twn.png");
				KageTimeline::imageBLANK_SEL         = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_sel.png");
				KageTimeline::imageBLANK_SEL_TWN     = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_sel_twn.png");
				KageTimeline::imageBLANK_TWN         = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_twn.png");
				KageTimeline::imageBLANK_X             = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_x.png");
				KageTimeline::imageBLANK_X_CUR         = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_x_cur.png");
				KageTimeline::imageBLANK_X_CUR_SEL     = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_x_cur_sel.png");
				KageTimeline::imageBLANK_X_CUR_SEL_TWN = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_x_cur_sel_twn.png");
				KageTimeline::imageBLANK_X_CUR_TWN     = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_x_cur_twn.png");
				KageTimeline::imageBLANK_X_SEL         = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_x_sel.png");
				KageTimeline::imageBLANK_X_SEL_TWN     = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_x_sel_twn.png");
				KageTimeline::imageBLANK_X_TWN         = Gdk::Pixbuf::create_from_resource("/kage/share/frame/blank_x_twn.png");
				KageTimeline::imageDRAWN             = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn.png");
				KageTimeline::imageDRAWN_CUR         = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_cur.png");
				KageTimeline::imageDRAWN_CUR_SEL     = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_cur_sel.png");
				KageTimeline::imageDRAWN_CUR_SEL_TWN = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_cur_sel_twn.png");
				KageTimeline::imageDRAWN_CUR_TWN     = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_cur_twn.png");
				KageTimeline::imageDRAWN_SEL         = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_sel.png");
				KageTimeline::imageDRAWN_SEL_TWN     = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_sel_twn.png");
				KageTimeline::imageDRAWN_TWN         = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_twn.png");
				KageTimeline::imageDRAWN_X             = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_x.png");
				KageTimeline::imageDRAWN_X_CUR         = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_x_cur.png");
				KageTimeline::imageDRAWN_X_CUR_SEL     = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_x_cur_sel.png");
				KageTimeline::imageDRAWN_X_CUR_SEL_TWN = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_x_cur_sel_twn.png");
				KageTimeline::imageDRAWN_X_CUR_TWN     = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_x_cur_twn.png");
				KageTimeline::imageDRAWN_X_SEL         = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_x_sel.png");
				KageTimeline::imageDRAWN_X_SEL_TWN     = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_x_sel_twn.png");
				KageTimeline::imageDRAWN_X_TWN         = Gdk::Pixbuf::create_from_resource("/kage/share/frame/drawn_x_twn.png");
			} catch (const Gio::ResourceError &ex) {
				std::cerr << "KageTimeline::ResourceError: " << ex.what() << std::endl;
			} catch (const Gdk::PixbufError &ex) {
				std::cerr << "KageTimeline::PixbufError: " << ex.what() << std::endl;
			}
		}
		bool l_current = false;
		bool l_selected = isSelected();
		bool l_tween = false;
		//draw background
		const unsigned int FRAME_WIDTH_OFFSET = 8;
		const unsigned int FRAME_HEIGHT_OFFSET = 23;
		p_context->move_to(FRAME_WIDTH_OFFSET,            0);
		p_context->line_to(       get_width(),            0);
		p_context->line_to(       get_width(), get_height());
		p_context->line_to(FRAME_WIDTH_OFFSET, get_height());
		p_context->line_to(FRAME_WIDTH_OFFSET,            0);
		p_context->set_source_rgb(0.75f, 0.75f, 0.75f);
		p_context->fill();
		p_context->set_source_rgb(0.75f, 0.75f, 0.75f);
		p_context->stroke();

		unsigned int l_layers = _kage->_document.Scenes[_kage->_document.getActiveSceneID()].Layers.size();
		double l_y = get_height();
		//draw timeline grid
		while (l_y + (FRAME_HEIGHT_OFFSET*2) > 0) {
			p_context->rectangle(FRAME_WIDTH_OFFSET, l_y, get_width(), FRAME_HEIGHT_OFFSET);
			p_context->set_source_rgb(0.9f, 0.9f, 0.9f);
			p_context->fill();
			
			l_y -= (FRAME_HEIGHT_OFFSET*2);
		}
		double l_x = FRAME_WIDTH_OFFSET;
		while (l_x < get_width()) {
			p_context->move_to( l_x,            0);
			p_context->line_to( l_x, get_height());
			p_context->set_source_rgb(0.80f, 0.80f, 0.80f);
			p_context->set_line_width(1.0f);
			p_context->stroke();
			l_x += (FRAME_WIDTH_OFFSET * _kage->_document.Project._fps);
		}
		
		//draw selected frame index
		p_context->move_to(g_frameSelectIndex * FRAME_WIDTH_OFFSET + (FRAME_WIDTH_OFFSET/2) + FRAME_WIDTH_OFFSET,            0);
		p_context->line_to(g_frameSelectIndex * FRAME_WIDTH_OFFSET + (FRAME_WIDTH_OFFSET/2) + FRAME_WIDTH_OFFSET, get_height());
		p_context->set_source_rgb(0.75f, 0.0f, 0.0f);
		p_context->stroke();


		l_y = get_height() - FRAME_HEIGHT_OFFSET;
		unsigned int l_frames = 0;
		for (unsigned int i = 0; i < l_layers; ++i) {
			double l_layerSelectIndex = UINT_MAX;
			KageLayer *l_layer = _kage->_document.Scenes[_kage->_document.getActiveSceneID()].Layers[i];
			l_frames = l_layer->Frames.size();
			double l_x = FRAME_WIDTH_OFFSET;//offset due to VBox divider mouse-handling
			for (unsigned int j = 0; j < l_frames; ++j) {
				KageFrame *l_frame = l_layer->Frames[j];
				bool l_current = l_layer->isCurrentFrame(l_frame->frameID);
				bool l_selected = l_frame->isSelected();
				bool l_tween = (l_frame->getTween() > 0);
				if (l_current && l_selected) {
					l_layerSelectIndex = i;
				}
				switch (l_frame->getExtension()) {
					case KageFrame::EXTENSION_NOT:
					case KageFrame::EXTENSION_END:
						if (l_frame->isEmpty() == true) {
							if (       l_current == true  && l_selected == false && l_tween == false) {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageBLANK_CUR, l_x, l_y);
							} else if (l_current == false && l_selected == true  && l_tween == false) {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageBLANK_SEL, l_x, l_y);
							} else if (l_current == false && l_selected == false && l_tween == true ) {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageBLANK_TWN, l_x, l_y);
							} else if (l_current == true  && l_selected == true  && l_tween == false) {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageBLANK_CUR_SEL, l_x, l_y);
							} else if (l_current == true  && l_selected == true  && l_tween == true ) {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageBLANK_CUR_SEL_TWN, l_x, l_y);
							} else if (l_current == true  && l_selected == false && l_tween == true ) {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageBLANK_CUR_TWN, l_x, l_y);
							} else if (l_current == false && l_selected == true  && l_tween == true ) {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageBLANK_SEL_TWN, l_x, l_y);
							} else {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageBLANK, l_x, l_y);
							}
						} else if (l_frame->isNull() == true) {
							if (l_layer->isCurrentFrame(l_frame->frameID) == true) {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageNULL_CUR, l_x, l_y);
							} else {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageNULL, l_x, l_y);
							}
						} else {
							if (       l_current == true  && l_selected == false && l_tween == false) {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageDRAWN_CUR, l_x, l_y);
							} else if (l_current == false && l_selected == true  && l_tween == false) {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageDRAWN_SEL, l_x, l_y);
							} else if (l_current == false && l_selected == false && l_tween == true ) {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageDRAWN_TWN, l_x, l_y);
							} else if (l_current == true  && l_selected == true  && l_tween == false) {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageDRAWN_CUR_SEL, l_x, l_y);
							} else if (l_current == true  && l_selected == true  && l_tween == true ) {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageDRAWN_CUR_SEL_TWN, l_x, l_y);
							} else if (l_current == true  && l_selected == false && l_tween == true ) {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageDRAWN_CUR_TWN, l_x, l_y);
							} else if (l_current == false && l_selected == true  && l_tween == true ) {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageDRAWN_SEL_TWN, l_x, l_y);
							} else {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageDRAWN, l_x, l_y);
							}
						}
						break;
					case KageFrame::EXTENSION_START:
					case KageFrame::EXTENSION_MID:
						if (l_frame->isEmpty() == true) {
							if (       l_current == true  && l_selected == false && l_tween == false) {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageBLANK_X_CUR, l_x, l_y);
							} else if (l_current == false && l_selected == true  && l_tween == false) {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageBLANK_X_SEL, l_x, l_y);
							} else if (l_current == false && l_selected == false && l_tween == true ) {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageBLANK_X_TWN, l_x, l_y);
							} else if (l_current == true  && l_selected == true  && l_tween == false) {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageBLANK_X_CUR_SEL, l_x, l_y);
							} else if (l_current == true  && l_selected == true  && l_tween == true ) {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageBLANK_X_CUR_SEL_TWN, l_x, l_y);
							} else if (l_current == true  && l_selected == false && l_tween == true ) {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageBLANK_X_CUR_TWN, l_x, l_y);
							} else if (l_current == false && l_selected == true  && l_tween == true ) {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageBLANK_X_SEL_TWN, l_x, l_y);
							} else {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageBLANK_X, l_x, l_y);
							}
						} else if (l_frame->isNull() == true) {
							if (l_layer->isCurrentFrame(l_frame->frameID) == true) {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageNULL_X_CUR, l_x, l_y);
							} else {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageNULL_X, l_x, l_y);
							}
						} else {
							if (       l_current == true  && l_selected == false && l_tween == false) {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageDRAWN_X_CUR, l_x, l_y);
							} else if (l_current == false && l_selected == true  && l_tween == false) {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageDRAWN_X_SEL, l_x, l_y);
							} else if (l_current == false && l_selected == false && l_tween == true ) {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageDRAWN_X_TWN, l_x, l_y);
							} else if (l_current == true  && l_selected == true  && l_tween == false) {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageDRAWN_X_CUR_SEL, l_x, l_y);
							} else if (l_current == true  && l_selected == true  && l_tween == true ) {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageDRAWN_X_CUR_SEL_TWN, l_x, l_y);
							} else if (l_current == true  && l_selected == false && l_tween == true ) {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageDRAWN_X_CUR_TWN, l_x, l_y);
							} else if (l_current == false && l_selected == true  && l_tween == true ) {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageDRAWN_X_SEL_TWN, l_x, l_y);
							} else {
								Gdk::Cairo::set_source_pixbuf(p_context, KageTimeline::imageDRAWN_X, l_x, l_y);
							}
						}
						break;
				}
				p_context->paint();
				l_x += FRAME_WIDTH_OFFSET;
			}
			
			//dont highlight non-selected layer
			if (l_layerSelectIndex != i) {
				p_context->rectangle(FRAME_WIDTH_OFFSET, l_y, get_width(), FRAME_HEIGHT_OFFSET);
				p_context->set_source_rgba(0.85f, 0.85f, 0.85f, 0.25f);
				p_context->fill();
			}
			
			l_y -= FRAME_HEIGHT_OFFSET;
		}

		//resize widget as per current frame/layer count
		int l_width, l_height;
		get_size_request(l_width, l_height);
		if (l_width/FRAME_WIDTH_OFFSET != l_frames+1 /* +1 is for offset due to VBox divider mouse-handling*/
				|| l_height/FRAME_HEIGHT_OFFSET != l_layers) {
			l_width = (l_frames+1) * FRAME_WIDTH_OFFSET;
			l_height = l_layers * FRAME_HEIGHT_OFFSET;
			set_size_request(l_width, l_height);
		}

		Cairo::TextExtents extents;
		string l_text;
		//draw currently selected Layer/Frame
		p_context->select_font_face ("Arial", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD);
		p_context->set_font_size(16);
		p_context->set_source_rgb(0.25f, 0.5f, 0.25f);
		l_text = "Layer " + StringHelper::unsignedIntegerToString(_kage->getCurrentLayer()) + " Frame " + StringHelper::unsignedIntegerToString(_kage->getCurrentFrame());// + " ? " + StringHelper::unsignedIntegerToString(g_frameSelectIndex);
		p_context->get_text_extents(l_text, extents);
			p_context->move_to( get_width() - (extents.width + 24), get_height() - 24);

		p_context->show_text(l_text);
		
		//draw frame Number
		l_x = FRAME_WIDTH_OFFSET;
		unsigned int l_ctr = 0;
		
		p_context->select_font_face ("Arial", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD);
		p_context->set_font_size(16);
		//p_context->set_source_rgb(0.34f, 0.34f, 0.34f);
		p_context->set_source_rgb(0.5f, 0.5f, 0.5f);
		while (l_x < get_width()) {
			l_text = StringHelper::unsignedIntegerToString(_kage->_document.Project._fps * l_ctr);
			p_context->get_text_extents(l_text, extents);
				p_context->move_to( l_x-(extents.width/2), 16);
				p_context->show_text(l_text);

			l_text = StringHelper::unsignedIntegerToString(l_ctr) + "s";
			p_context->get_text_extents(l_text, extents);
				p_context->move_to( l_x-(extents.width/2), get_height()-2);
				p_context->show_text(l_text);
			l_x += (FRAME_WIDTH_OFFSET * _kage->_document.Project._fps);
			++l_ctr;
		}
		
		//render mouse-multiselect
		if (mouseIsDown == true) {
			vector<double> dashes;
				dashes.push_back(4.0); /* ink */
				dashes.push_back(4.0); /* skip */
			
			p_context->set_dash(dashes, 0.0);
			p_context->move_to(draw1.x, draw1.y);
			p_context->line_to(draw2.x, draw1.y);
			p_context->line_to(draw2.x, draw2.y);
			p_context->line_to(draw1.x, draw2.y);
			p_context->line_to(draw1.x, draw1.y);
			p_context->set_source_rgb(0.85f, 0.0f, 0.0f);
			p_context->stroke();
		}
		return true;
	}
	return false;
}

void KageTimeline::setSelected(bool p_selected) {
	_selected = p_selected;
	invalidateToRender();
}
bool KageTimeline::isSelected() {
	return _selected;
}

VectorDataManager KageTimeline::getFrameData() {
	if (_null == true) {
		cout << "KageTimeline::getFrameData is returning empty" << endl;
		VectorDataManager l_nullReturn;
		return l_nullReturn;
	}
	VectorDataManager l_nullReturn;
		return l_nullReturn;
}
void KageTimeline::setFrameData(VectorDataManager p_vectorsData) {
	//
}

void KageTimeline::setFocus() {
	grab_focus();
}

vector<unsigned int> KageTimeline::raiseSelectedShape(vector<unsigned int> p_selectedShapes) {
	vector<unsigned int> foo;
	return foo;
}
void KageTimeline::addDataToFrame(VectorDataManager v) {
	//
}
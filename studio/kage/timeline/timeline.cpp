/*
 * Kage Studio - a simple free and open source vector-based 2D animation software
 * Copyright (C) 2023-2024  Mj Mendoza IV
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

	//if (!KageTimeline::imageNULL) {
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
	//}
}

KageTimeline::~KageTimeline() {
	//
}

bool g_FrameModifier = false;
bool g_FrameModifierOngoing = false;
bool g_multipleSelect = false;
enum FrameModifier {
	MODIFY_NOT,
	MODIFY_MOVE,
	MODIFY_EXTEND
};
FrameModifier g_FrameModify = FrameModifier::MODIFY_NOT;
int g_FrameModifyLayerIndex;
int g_FrameModifyFrameIndexStart;
int g_FrameModifyFrameIndexStop;
int g_FrameModifyFrameIndex;

bool KageTimeline::on_key_press_event(GdkEventKey *e) {
	if (e->keyval == GDK_KEY_period) {
		if (_kage->sceneLayerSwitchToNextFrame()) {
			++g_frameSelectIndex;
		}
	} else if (e->keyval == GDK_KEY_comma) {
		if (_kage->sceneLayerSwitchToPreviousFrame()) {
			--g_frameSelectIndex;
		}
	} else if (e->keyval == GDK_KEY_Escape) {
		_kage->Stop_onClick();
	} else if (e->keyval == GDK_KEY_Control_L || e->keyval == GDK_KEY_Control_R) {
		g_FrameModifier = true; //toggle Frame-move/resize listeners
		invalidateToRender();
	} else if (e->keyval == GDK_KEY_Shift_L || e->keyval == GDK_KEY_Shift_R) {
		g_multipleSelect = true;
	}

	return true;
}
bool KageTimeline::on_key_release_event(GdkEventKey *e) {
	if (e->keyval == GDK_KEY_Control_L || e->keyval == GDK_KEY_Control_R) {
		g_FrameModifier = false; //toggle-off Frame-move/resize listeners
		g_FrameModify = FrameModifier::MODIFY_NOT;
		invalidateToRender();
	} else if (e->keyval == GDK_KEY_Shift_L || e->keyval == GDK_KEY_Shift_R) {
		g_multipleSelect = false;
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
		unsigned int l_realDrawX = 0;
		unsigned int l_layerCount = 0;
		try {
			l_layerCount = _kage->_document.getScene()->Layers.size();
		} catch (std::exception& e) {
			std::cout << "KageTime::on_event Exception : " << e.what() << std::endl;
		}
		unsigned int l_layerHeight = l_layerCount * FRAME_HEIGHT;
		if (draw1.x == draw2.x && draw1.y == draw2.y) {
			//single SELECT frame
			l_realDrawX = draw1.x - FRAME_WIDTH_OFFSET;
			unsigned int p_frameIndex = ((l_realDrawX) - ((l_realDrawX) % FRAME_WIDTH)) / FRAME_WIDTH;
			if (get_height() >= l_layerHeight) {
				draw1.y -= (get_height() - l_layerHeight); //remove excess
			} else {
				draw1.y += (l_layerHeight-get_height()); //pad hidden layers
			}
			g_layerSelectIndex = (draw1.y - ((unsigned int)draw1.y % FRAME_HEIGHT)) / FRAME_HEIGHT;
			g_frameSelectIndex = p_frameIndex;
			_kage->setDocumentSceneCurrentLayer(l_layerCount-g_layerSelectIndex, g_multipleSelect);
			_kage->setDocumentSceneLayerCurrentFrame(p_frameIndex+1, g_multipleSelect);
		} else {
			//multi-SELECT frames
			//identify all frames within draw1.x to draw2.x
			l_realDrawX = draw1.x - FRAME_WIDTH_OFFSET;
			unsigned int l_frameStartIndex = ((l_realDrawX) - (l_realDrawX % FRAME_WIDTH)) / FRAME_WIDTH;
			l_realDrawX = draw2.x - FRAME_WIDTH_OFFSET;
			unsigned int l_frameStopIndex  = ((l_realDrawX) - (l_realDrawX % FRAME_WIDTH)) / FRAME_WIDTH;
			if (l_frameStopIndex < l_frameStartIndex) {
				unsigned int l_tmpFrameIndex = l_frameStopIndex;
				l_frameStopIndex = l_frameStartIndex;
				l_frameStartIndex = l_tmpFrameIndex;
			}
			if (l_realDrawX < (FRAME_WIDTH/2)) {
				//do nothing
			} else {
				l_frameStartIndex += 1;
			}
			l_frameStopIndex  -= 1;
			
			//identify all layers within draw1.y to draw2.y
			if (get_height() >= l_layerHeight) {
				draw1.y -= (get_height() - l_layerHeight); //remove excess
				draw2.y -= (get_height() - l_layerHeight); //remove excess
			} else {
				draw1.y += (l_layerHeight-get_height()); //pad hidden layers
				draw2.y += (l_layerHeight-get_height()); //pad hidden layers
			}
			unsigned int l_layerStartIndex = ((draw1.y - ((unsigned int)draw1.y % FRAME_HEIGHT)) / FRAME_HEIGHT) + 1;
			unsigned int l_layerStopIndex  = ((draw2.y - ((unsigned int)draw2.y % FRAME_HEIGHT)) / FRAME_HEIGHT);
			l_layerStartIndex = l_layerCount - l_layerStartIndex;
			l_layerStopIndex  = l_layerCount - l_layerStopIndex;
			if (l_layerStopIndex < l_layerStartIndex) {
				unsigned int l_tmpLayerIndex = l_layerStopIndex;
				l_layerStopIndex = l_layerStartIndex;
				l_layerStartIndex = l_tmpLayerIndex;
			}
			
			//loop from start to stop to select frames in between
			for (unsigned int l_layerIndex = 0; l_layerIndex < _kage->_document.getScene()->Layers.size(); ++l_layerIndex) {
				if (l_layerIndex >= l_layerStartIndex && l_layerIndex < l_layerStopIndex) {
					for (unsigned int l_frameIndex = 0; l_frameIndex < _kage->_document.getScene()->Layers[l_layerIndex]->Frames.size(); ++l_frameIndex) {
						if (l_frameIndex >= l_frameStartIndex && l_frameIndex <= l_frameStopIndex) {
							_kage->_document.getScene()->Layers[l_layerIndex]->Frames[l_frameIndex]->setSelected(true);
						} else {
							_kage->_document.getScene()->Layers[l_layerIndex]->Frames[l_frameIndex]->setSelected(false);
						}
					}
				} else {
					for (unsigned int l_frameIndex = 0; l_frameIndex < _kage->_document.getScene()->Layers[l_layerIndex]->Frames.size(); ++l_frameIndex) {
						_kage->_document.getScene()->Layers[l_layerIndex]->Frames[l_frameIndex]->setSelected(false);
					}
				}
			}
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
		std::cout << "KageTimeline GDK_FOCUS_CHANGE e->send_event " << ((GdkEventFocus*)e)->send_event << " e->in " << ((GdkEventFocus*)e)->in << std::endl;
		if (((GdkEventFocus*)e)->in) {
			KageTimeline::_gotFocus = true;
		} else {
			KageTimeline::_gotFocus = false;
		}
	} else if (e->type == GDK_KEY_PRESS) {
		on_key_press_event((GdkEventKey*) e);
	} else if (e->type == GDK_KEY_RELEASE) {
		on_key_release_event((GdkEventKey*) e);
//		_frameset->getFsm()->setCurrentFrame(getDocumentSceneLayerCurrentFrame(), false);
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
		bool l_current = false;
		bool l_selected = isSelected();
		bool l_tween = false;
		//draw background
		p_context->move_to(FRAME_WIDTH_OFFSET,            0);
		p_context->line_to(       get_width(),            0);
		p_context->line_to(       get_width(), get_height());
		p_context->line_to(FRAME_WIDTH_OFFSET, get_height());
		p_context->line_to(FRAME_WIDTH_OFFSET,            0);
		if (Kage::NotDarkMode) {
			p_context->set_source_rgb(0.75f, 0.75f, 0.75f);
		} else {
			p_context->set_source_rgb(0.38f, 0.38f, 0.38f);
		}
		p_context->fill();
		
		double l_y = get_height();
		//draw timeline grid
		while (l_y + (FRAME_HEIGHT_OFFSET*2) > 0) {
			p_context->rectangle(FRAME_WIDTH_OFFSET, l_y, get_width(), FRAME_HEIGHT_OFFSET);
			if (Kage::NotDarkMode) {
				p_context->set_source_rgb(0.9f, 0.9f, 0.9f);
			} else {
				p_context->set_source_rgb(0.75f, 0.75f, 0.75f);
			}
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
			l_x += (FRAME_WIDTH * _kage->_document._fps);
		}
		
		unsigned int l_frames;
		double l_layerSelectIndex = UINT_MAX;
		KageLayer *l_layer;
		KageFrame *l_frame;
		unsigned int l_layers = 0;
		try {
			l_layers = _kage->_document.getScene()->Layers.size();
		} catch (std::exception& e) {
			std::cout << "KageTimeline::on_draw Exception : " << e.what() << std::endl;
		}
		for (unsigned int i = 0; i < l_layers; ++i) {
			l_layer = NULL;
			l_frames = 0;
			try {
				l_layer = _kage->_document.getScene()->Layers[i];
				l_frames = l_layer->Frames.size();
			} catch (std::exception& e) {
				std::cout << "KageTimeline::on_draw Exception : " << e.what() << std::endl;
			}
			for (unsigned int j = 0; j < l_frames; ++j) {
				l_frame = l_layer->Frames[j];
				l_current = l_layer->isCurrentFrame(l_frame->frameID);
				l_selected = l_frame->isSelected();
				if (l_current && l_selected) {
					l_layerSelectIndex = i;
					g_frameSelectIndex = j;
					i = l_layers;
					break;
				}
			}
		}
		//draw selected frame index
		p_context->move_to(g_frameSelectIndex * FRAME_WIDTH + (FRAME_WIDTH/2) + FRAME_WIDTH_OFFSET,            0);
		p_context->line_to(g_frameSelectIndex * FRAME_WIDTH + (FRAME_WIDTH/2) + FRAME_WIDTH_OFFSET, get_height());
		p_context->set_source_rgb(0.75f, 0.0f, 0.0f);
		p_context->stroke();

		l_y = get_height() - FRAME_HEIGHT_OFFSET;
		for (unsigned int i = 0; i < l_layers; ++i) {
			l_layer = NULL;
			l_frames = 0;
			try {
				l_layer = _kage->_document.getScene()->Layers[i];
				l_frames = l_layer->Frames.size();
			} catch (std::exception& e) {
				std::cout << "KageTimeline::on_draw Exception : " << e.what() << std::endl;
			}
			l_x = FRAME_WIDTH_OFFSET;//offset due to VBox divider mouse-handling
			for (unsigned int j = 0; j < l_frames; ++j) {
				l_frame = l_layer->Frames[j];
				l_current = l_layer->isCurrentFrame(l_frame->frameID);
				l_selected = l_frame->isSelected();
				bool l_tween = (l_frame->getTween() > 0);
				switch (l_frame->getExtension()) {
					case KageFrame::extension::EXTENSION_NOT:
					case KageFrame::extension::EXTENSION_END:
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
					case KageFrame::extension::EXTENSION_START:
					case KageFrame::extension::EXTENSION_MID:
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
				l_x += FRAME_WIDTH;
			}
			
			//dont highlight non-selected layer
			if (l_layerSelectIndex != i) {
				if (_kage->_toggleOnion.get_active() == true) {
					if (g_frameSelectIndex-2 > 0) {
						//draw left side
						p_context->rectangle(FRAME_WIDTH_OFFSET, l_y, (g_frameSelectIndex-2) * FRAME_WIDTH, FRAME_HEIGHT_OFFSET);
						p_context->set_source_rgba(0.85f, 0.85f, 0.85f, 0.25f);
						p_context->fill();
					}
					if (g_frameSelectIndex+2 < l_frames-1) {
						//draw left side
						p_context->rectangle((g_frameSelectIndex+2) * FRAME_WIDTH + FRAME_WIDTH + FRAME_WIDTH_OFFSET, l_y, get_width(), FRAME_HEIGHT_OFFSET);
						p_context->set_source_rgba(0.85f, 0.85f, 0.85f, 0.25f);
						p_context->fill();
					}
				} else {
					p_context->rectangle(FRAME_WIDTH_OFFSET, l_y, get_width(), FRAME_HEIGHT_OFFSET);
					p_context->set_source_rgba(0.85f, 0.85f, 0.85f, 0.25f);
					p_context->fill();
				}
			}
			
			l_y -= FRAME_HEIGHT;
		}

		//resize widget as per current frame/layer count
		int l_width, l_height;
		get_size_request(l_width, l_height);
		if (l_width/FRAME_WIDTH < l_frames+1 /* +1 is for offset due to VBox divider mouse-handling*/
				|| l_height/FRAME_HEIGHT_OFFSET < l_layers) {
			l_width = (l_frames+1) * FRAME_WIDTH;
			l_height = l_layers * FRAME_HEIGHT_OFFSET;
			set_size_request(l_width, l_height);
		}

		Cairo::TextExtents extents;
		std::string l_text;
		//draw currently selected Layer/Frame
		p_context->select_font_face ("Arial", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD);
		p_context->set_font_size(16);
		if (Kage::NotDarkMode) {
			p_context->set_source_rgb(0.00f, 0.47f, 0.84f);
		} else {
			p_context->set_source_rgb(0.58f, 0.07f, 0.00f);
		}
		l_text = "Layer " + StringHelper::unsignedIntegerToString(_kage->getDocumentSceneCurrentLayer()) + " Frame " + StringHelper::unsignedIntegerToString(_kage->getDocumentSceneLayerCurrentFrame());// + " ? " + StringHelper::unsignedIntegerToString(g_frameSelectIndex);
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
			l_text = StringHelper::unsignedIntegerToString(_kage->_document._fps * l_ctr);
			p_context->get_text_extents(l_text, extents);
				p_context->move_to( l_x-(extents.width/2), 16);
				p_context->show_text(l_text);

			l_text = StringHelper::unsignedIntegerToString(l_ctr) + "s";
			p_context->get_text_extents(l_text, extents);
				p_context->move_to( l_x-(extents.width/2), get_height()-2);
				p_context->show_text(l_text);
			l_x += (FRAME_WIDTH * _kage->_document._fps);
			++l_ctr;
		}
		
		//render mouse-multiselect
		if (mouseIsDown == true) {
			std::vector<double> dashes;
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
		std::cout << "KageTimeline::getFrameData is returning empty" << std::endl;
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

std::vector<unsigned int> KageTimeline::raiseSelectedShape(std::vector<unsigned int> p_selectedShapes) {
	std::vector<unsigned int> foo;
	return foo;
}
void KageTimeline::addDataToFrame(VectorDataManager v) {
	//
}
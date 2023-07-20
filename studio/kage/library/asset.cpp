/*
 * Kage Studio - a simple free and open source vector-based 2D animation software
 * Copyright (C) 2011~2023  Mj Mendoza IV
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

#include "asset.h"
#include "../timeline/framesmanager.h"
#include "../../kage.h"

#include <giomm/resource.h>

bool KageAsset::mouseIsDown = false;
Glib::RefPtr<Gdk::Pixbuf> KageAsset::iconASSET_IMAGE;
Glib::RefPtr<Gdk::Pixbuf> KageAsset::iconASSET_KAGE;
Glib::RefPtr<Gdk::Pixbuf> KageAsset::iconASSET_VIDEO;

KageAsset::KageAsset(KageAssetManager *p_assetManager, unsigned p_assetID) {
	set_state_flags(Gtk::STATE_FLAG_NORMAL);
	set_can_focus(true); //to accept key_press
	_assetManager = p_assetManager;
	assetID = p_assetID;
	set_size_request(180, 24);
    cout << "wxh "<< get_width() << " " << get_height() << endl;
	_selected = false;
	_assetType = KageAsset::AssetType::ASSET_IMAGE;
	_label = "Asset " + StringHelper::unsignedIntegerToString(p_assetID);
	
	addEventsListener();
	
	//add(_txtLabel);//, Gtk::PACK_EXPAND_WIDGET);
	_txtLabel.set_size_request(20, 24);
//	_txtLabel.set_width_chars(9);
	_txtLabel.set_max_length(50);
	_txtLabel.set_text(_label);
	_txtLabel.signal_activate().connect(
							sigc::mem_fun(*this, &KageAsset::txtLabel_onEnter));
	_txtLabel.show();
	show();
    cout << "wxh "<< get_width() << " " << get_height() << endl;

	_CTRL = false;
}

KageAsset::~KageAsset() {
	//
}

void KageAsset::addEventsListener() {
//	add_events(Gdk::KEY_PRESS_MASK    | Gdk::KEY_RELEASE_MASK);
	add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
	add_events(Gdk::ENTER_NOTIFY_MASK | Gdk::LEAVE_NOTIFY_MASK);
	add_events(Gdk::FOCUS_CHANGE_MASK);
	
	//*window = NULL;
}

void KageAsset::txtLabel_onEnter() {
	_label = _txtLabel.get_text();
	_txtLabel.hide();
}

bool KageAsset::on_key_press_event(GdkEventKey *e) {
	Kage::timestamp_IN();
	std::cout << " KageFrame(L " << assetID << ") on_key_press_event" << std::endl;
	if (e->keyval == GDK_KEY_Control_L || e->keyval == GDK_KEY_Control_R) {
		_CTRL = true;
	}
	Kage::timestamp_OUT();
	return true;
}
bool KageAsset::on_key_release_event(GdkEventKey *e) {
	Kage::timestamp_IN();
	std::cout << " KageFrame(L " << assetID << ") on_key_release_event" << std::endl;
	if (e->keyval == GDK_KEY_Control_L || e->keyval == GDK_KEY_Control_R) {
		_CTRL = false;
	}
	Kage::timestamp_OUT();
	return true;
}
bool KageAsset::on_expose_event(GdkEventExpose* e) {
	if (!window) {
		window = get_window();
	}
	if (window) {
		invalidateToRender();
	}
	
	return true;
}

bool KageAsset::on_event(GdkEvent *e) {
	if (e->type == GDK_ENTER_NOTIFY) {
		Kage::timestamp_IN();
		std::cout << " KageAsset(L " << assetID << ") on_event enter" << std::endl;
		invalidateToRender();
		Kage::timestamp_OUT();
	} else if (e->type == GDK_LEAVE_NOTIFY) {
		Kage::timestamp_IN();
		std::cout << " KageAsset(L " << assetID << ") on_event leave" << std::endl;
		invalidateToRender();
		Kage::timestamp_OUT();
	} else if (e->type == GDK_DOUBLE_BUTTON_PRESS) {
		if (e->button.x > 36) {
			if (_CTRL) {
				_assetManager->_kage->m_KageStage.addImage(assetID);
			} else {
				_assetManager->renameAsset(this);
			}
		}
		//_txtLabel.show();
	} else if (e->type == GDK_BUTTON_RELEASE) {
		KageAsset::mouseIsDown = false;
		grab_focus();

		_assetManager->render(_renderAssetID);

		invalidateToRender();
	} else if (e->type == GDK_BUTTON_PRESS) {
		KageAsset::mouseIsDown = true;
		//if (e->button.x < 18) {
		//}
		_assetManager->setSelected(this);
		_assetManager->renderStage();
		invalidateToRender();
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


void KageAsset::forceRender() {
	if (KageFramesetManager::LOADING_MODE == true) {
		return;
	}
	Kage::timestamp_IN(); cout << " KageAsset::forceRender()" << endl;
	invalidateToRender();
	Kage::timestamp_OUT();
}
bool KageAsset::invalidateToRender() {
	if (KageFramesetManager::LOADING_MODE == true) {
		return true;
	}
	Kage::timestamp_IN(); cout << " KageAsset::invalidateToRender()" << endl;
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
    std::cout << "KageAsset:invalidateToRender E..." << std::endl;
		
	return true;
}

bool KageAsset::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
	if (!window) {
		window = get_window();
	}
	if (window) {
		if (!KageAsset::iconASSET_IMAGE) {
			try {
                //TODO: use icons from here -> https://www.iconfinder.com/iconsets/ionicons
				KageAsset::iconASSET_IMAGE = Gdk::Pixbuf::create_from_resource("/kage/share/library/asset_image.png");
				KageAsset::iconASSET_KAGE  = Gdk::Pixbuf::create_from_resource("/kage/share/library/asset_kage.png");
				KageAsset::iconASSET_VIDEO = Gdk::Pixbuf::create_from_resource("/kage/share/library/asset_video.png");
				std::cout << "KageAsset: images loaded " << std::endl;
			} catch(const Gio::ResourceError &ex) {
				std::cerr << "ResourceError: " << ex.what() << std::endl;
			} catch(const Gdk::PixbufError &ex) {
				std::cerr << "PixbufError: " << ex.what() << std::endl;
			}
		}
		
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
		cr->select_font_face ("Verdana", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_NORMAL);
		cr->set_font_size(12);
		if (isSelected() == true) {
			cr->set_source_rgb(1, 1, 1);
		} else {
			cr->set_source_rgb(0, 0, 0);
		}
		cr->move_to(24, 15);
		cr->show_text(_label);
		
		if (       _assetType == KageAsset::AssetType::ASSET_IMAGE) {
			Gdk::Cairo::set_source_pixbuf(cr, KageAsset::iconASSET_IMAGE, 2, 0);
		} else if (_assetType == KageAsset::AssetType::ASSET_KAGE) {
			Gdk::Cairo::set_source_pixbuf(cr, KageAsset::iconASSET_KAGE, 2, 0);
		} else if (_assetType == KageAsset::AssetType::ASSET_VIDEO) {
			Gdk::Cairo::set_source_pixbuf(cr, KageAsset::iconASSET_VIDEO, 2, 0);
		}
		cr->paint();
		
		return true;
	}
	return false;
}

/**
 * @brief When User imports a file, the original file is copied inside project file; the copied file name will be the Asset Hash
 * \sa setFileName
 * @param p_assetHash 
 */
void KageAsset::setAssetHash(string p_assetHash) {
	_assetHash = p_assetHash;
}
string KageAsset::getAssetHash() {
	return _assetHash;
}

void KageAsset::setFilePath(string p_filePath) {
	_filePath = p_filePath;
}
string KageAsset::getFilePath() {
	return _filePath;
}
/**
 * @brief Set the original File Name where this ASSET was loaded
 * \sa setAssetHash
 * @param p_fileName the original File Name
 */
void KageAsset::setFileName(string p_fileName) {
	_fileName = p_fileName;
}
string KageAsset::getFileName() {
	return _fileName;
}

void KageAsset::setLabel(string p_label) {
	_label = p_label;
	invalidateToRender();
}
string KageAsset::getLabel() {
	return _label;
}
void KageAsset::setSelected(bool p_selected) {
	_selected = p_selected;
	invalidateToRender();
}
bool KageAsset::isSelected() {
	return _selected;
}

void KageAsset::setFocus() {
	grab_focus();
}

void KageAsset::setAssetType(KageAsset::AssetType p_assetType) {
	_assetType = p_assetType;
	invalidateToRender();
}
KageAsset::AssetType KageAsset::getAssetType() {
	return _assetType;
}

void KageAsset::render(unsigned int p_assetID) {
	_renderAssetID = p_assetID;

	_assetManager->renderLibrary(p_assetID);

//	invalidateToRender();
}
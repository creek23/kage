/*
 * layermanager.cpp
 * 
 * Copyright 2011-2020 Mj Mendoza IV <mj.mendoza.iv@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
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
 * MA 02110-1301, USA.
 * 
 * 
 */

#include "layermanager.h"
#include "../../kage.h"

KageLayerManager::KageLayerManager(Kage *p_win) {
	win = p_win;
	layerCtr = 0;
}

KageLayerManager::~KageLayerManager() {
	layers.clear();
}

void KageLayerManager::addLayer(Glib::ustring p_name) {
	++layerCtr;
	if (p_name == "") {
		p_name = Glib::ustring::compose("Layer %1", layerCtr);
	}
	
	layers.push_back(Gtk::manage(new KageLayer(this, layerCtr)));
		pack_end(*layers.back(), Gtk::PACK_SHRINK);
			(*layers.back()).setLabel(p_name);
			(*layers.back()).set_size_request(100, 23);
			(*layers.back()).set_focus_on_click(false);
	_currentLayerID = (*layers.back()).layerID;
	_currentLayerIndex = layers.size()-1;
}

bool KageLayerManager::removeAllLayers() {
	for (unsigned int i = 0; i < layerCount(); ++i) {
		remove(*layers[i]);
	}
	layerCtr = 0;
	layers.clear();
	return true;
}

/** For use of KageLayer.  When a KageLayer is clicked, previously clicked
 * KageLayer should be un-selected.
 * \param p_layer is pointer to KageLayer who called this function
*/
void KageLayerManager::setSelected(KageLayer *p_layer) {
	if (_currentLayerIndex < layerCount() && layers[_currentLayerIndex]->layerID == _currentLayerID) {
		layers[_currentLayerIndex]->setSelected(false);
	} else {
		for (unsigned int i = 0; i < layerCount(); ++i) {
			if (layers[i]->layerID == _currentLayerID) {
				layers[i]->setSelected(false);
				break;
			}
		}
	}
	p_layer->setSelected(true);
	_currentLayerID = p_layer->layerID;
	
	win->setCurrentFrame(win->getCurrentFrame());
}

/** Not to be confused of getCurrentLayer().  This function returns the
 * pointer to active KageLayer Object to caller
 * \return pointer to registered active layer
*/
KageLayer *KageLayerManager::getLayer() {
	if (_currentLayerIndex < layerCount() && layers[_currentLayerIndex]->layerID == _currentLayerID) {
		return layers[_currentLayerIndex];
	} else {
		for (unsigned int i = 0; i < layerCount(); ++i) {
			if (layers[i]->layerID == _currentLayerID) {
				_currentLayerIndex = i;
				return layers[i];
			}
		}
	}
	return NULL;
}

bool KageLayerManager::isLayerLocked() {
	if (_currentLayerIndex < layerCount() && layers[_currentLayerIndex]->layerID == _currentLayerID) {
		return layers[_currentLayerIndex]->isLocked();
	} else {
		for (unsigned int i = 0; i < layerCount(); ++i) {
			if (layers[i]->layerID == _currentLayerID) {
				_currentLayerIndex = i;
				return layers[i]->isLocked();
			}
		}
	}
	return true;
}

/** For use of Kage.  To keep track of active Layer in-check of all
 * layers in KageLayerManager.
 * \return index of registered active layer
 * \sa setCurrentLayer()
*/
unsigned int KageLayerManager::getCurrentLayer() {
	if (_currentLayerIndex < layerCount() && layers[_currentLayerIndex]->layerID == _currentLayerID) {
		return _currentLayerIndex+1;
	} else {
		for (unsigned int i = 0; i < layerCount(); ++i) {
			if (layers[i]->layerID == _currentLayerID) {
				_currentLayerIndex = i;
				return i+1; //temporarily add +1 as we integrate code to previous implementation
			}
		}
	}
	return layers.size(); //temporarily DONT -1 as we integrate code to previous implementation
}
/** For use of Kage.  When a KageFrame is clicked, KageFramesManager will
 * call this function via Kage then sets currently active Layer
 * \param p_layer is index of Layer known to KageFrame
 * \sa getCurrentLayer()
*/
void KageLayerManager::setCurrentLayer(unsigned int p_layer) {
	if (_currentLayerIndex < layerCount() && layers[_currentLayerIndex]->layerID == _currentLayerID) {
		layers[_currentLayerIndex]->setSelected(false);
	} else {
		for (unsigned int i = 0; i < layerCount(); ++i) {
			if (layers[i]->layerID == _currentLayerID) {
				layers[i]->setSelected(false);
				break;
			}
		}
	}
	
	unsigned int l_count = layerCount();
	if (p_layer > l_count) {
		p_layer = l_count;
	}
	if (p_layer < 1) {
		p_layer = 1;
	}
	_currentLayerIndex = p_layer-1;
	_currentLayerID = layers[_currentLayerIndex]->layerID;
	layers[_currentLayerIndex]->setSelected(true);
}

/** For use of KageLayer.  When a KageLayer's visibility is clicked,
 * this function will be called to reflect Layer's visibility on stage.
*/
void KageLayerManager::renderStage() {
	Kage::timestamp();
	cout << " KageLayerManager::renderStage <" << endl;
	win->forceRenderFrames();
	Kage::timestamp();
	cout << " KageLayerManager::renderStage >" << endl;
}

/** For use of Kage when exporting/saving to file.
 * \return size of array of KageLayer
*/
unsigned int KageLayerManager::layerCount() {
	return layers.size();
}

/*
 * Kage Studio - a simple free and open source vector-based 2D animation software
 * Copyright (C) 2011~2020  Mj Mendoza IV
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

#include "layermanager.h"
#include "../../kage.h"

KageLayerManager::KageLayerManager(Kage *p_win) {
	win = p_win;
	layerCtr = 0;
}

KageLayerManager::~KageLayerManager() {
	layers.clear();
}

unsigned int KageLayerManager::addLayer(Glib::ustring p_name) {
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
	
	return layerCtr;
}

void KageLayerManager::deleteLayer() {
	if (_currentLayerIndex < layerCount() && layers[_currentLayerIndex]->layerID == _currentLayerID) {
		if (layers[_currentLayerIndex]->isSelected()) {
			cout << " deleting; index " << _currentLayerIndex << endl;
			remove(*layers[_currentLayerIndex]);
			delete layers[_currentLayerIndex];
			layers.erase (layers.begin() + (_currentLayerIndex));
			if (_currentLayerIndex < layers.size()) {
				//do nothing
			} else if (_currentLayerIndex-1 >= 0) {
				--_currentLayerIndex;
			}
			_currentLayerID = layers[_currentLayerIndex]->layerID;
			layers[_currentLayerIndex]->setSelected(true);
		} else {
			cout << " layer not selected" << endl;
		}
	} else {
		cout << " unknown layer; identifying..." << endl;
		for (unsigned int i = 0; i < layerCount(); ++i) {
			if (layers[i]->layerID == _currentLayerID) {
				if (layers[i]->isSelected()) {
					cout << " deleting; index " << i << endl;
					remove(*layers[i]);
					delete layers[i];
					layers.erase (layers.begin() + (i-1));
					if (_currentLayerIndex < layers.size()) {
						//do nothing
					} else if (_currentLayerIndex-1 >= 0) {
						--_currentLayerIndex;
					}
					_currentLayerID = layers[_currentLayerIndex]->layerID;
					layers[_currentLayerIndex]->setSelected(true);
					return;
				}
				break;
			}
		}
	}
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
				_currentLayerIndex = i;
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

bool KageLayerManager::isLayerVisible() {
	if (_currentLayerIndex < layerCount() && layers[_currentLayerIndex]->layerID == _currentLayerID) {
		return layers[_currentLayerIndex]->isVisible();
	} else {
		for (unsigned int i = 0; i < layerCount(); ++i) {
			if (layers[i]->layerID == _currentLayerID) {
				_currentLayerIndex = i;
				return layers[i]->isVisible();
			}
		}
	}
	return true;
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
/** For use of Kage.  When a KageFrame is clicked, KageFramesetManager will
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

/** For use of Kage.  When a KageFrame is clicked, KageFramesetManager will
 * call this function via Kage then sets currently active Layer
 * \param p_layerID is layerID of Layer known to KageFrame
 * \sa getCurrentLayer()
 */
void KageLayerManager::setCurrentLayerByID(unsigned int p_layerID) {
	//unselect current layer
	if (_currentLayerIndex < layerCount() && layers[_currentLayerIndex]->layerID == _currentLayerID) {
		layers[_currentLayerIndex]->setSelected(false);
	} else {
		for (unsigned int i = 0; i < layerCount(); ++i) {
			if (layers[i]->layerID == _currentLayerID) {
				_currentLayerID = p_layerID;
				layers[i]->setSelected(false);
				break;
			}
		}
	}
	//select new layer by ID	
	for (unsigned int i = 0; i < layerCount(); ++i) {
		if (layers[i]->layerID == p_layerID) {
			_currentLayerIndex = i;
			_currentLayerID = p_layerID;
			layers[_currentLayerIndex]->setSelected(true);
			break;
		}
	}
}


/** For use of KageLayer.  When a KageLayer's visibility is clicked,
 * this function will be called to reflect Layer's visibility on stage.
 */
void KageLayerManager::renderStage() {
	Kage::timestamp_IN();
	cout << " KageLayerManager::renderStage <" << endl;
	win->forceRenderFrames();
	win->refreshUI();
	Kage::timestamp_OUT();
}

/** For use of Kage when exporting/saving to file.
 * \return size of array of KageLayer
 */
unsigned int KageLayerManager::layerCount() {
	return layers.size();
}

/** For use of KageLayer when User double-clicked Layer
 * \sa renameLayer()
 */
void KageLayerManager::renameLayer(KageLayer *p_layer) {
	setSelected(p_layer);
	
	LayerRenameDialog* pDialog = new LayerRenameDialog(*win, p_layer);
		pDialog->run();
	delete pDialog;
}

/** For use of Kage when User clicked Layer->Rename
 * \sa renameLayer(KageLayer)
 */
void KageLayerManager::renameLayer() {
	KageLayer *l_layer = NULL;
	if (_currentLayerIndex < layerCount() && layers[_currentLayerIndex]->layerID == _currentLayerID) {
		l_layer = layers[_currentLayerIndex];
	} else {
		for (unsigned int i = 0; i < layerCount(); ++i) {
			if (layers[i]->layerID == _currentLayerID) {
				_currentLayerIndex = i;
				l_layer = layers[i];
				break;
			}
		}
	}
	
	if (l_layer) {
		LayerRenameDialog* pDialog = new LayerRenameDialog(*win, l_layer);
			pDialog->run();
		delete pDialog;
	}
}
void KageLayerManager::setLabel(string p_label) {
	if (_currentLayerIndex < layerCount() && layers[_currentLayerIndex]->layerID == _currentLayerID) {
		layers[_currentLayerIndex]->setLabel(p_label);
	} else {
		for (unsigned int i = 0; i < layerCount(); ++i) {
			if (layers[i]->layerID == _currentLayerID) {
				_currentLayerIndex = i;
				layers[i]->setLabel(p_label);
				break;
			}
		}
	}
}

string KageLayerManager::getLabel() {
	if (_currentLayerIndex < layerCount() && layers[_currentLayerIndex]->layerID == _currentLayerID) {
		return layers[_currentLayerIndex]->getLabel();
	} else {
		for (unsigned int i = 0; i < layerCount(); ++i) {
			if (layers[i]->layerID == _currentLayerID) {
				return layers[i]->getLabel();
			}
		}
	}
	
	return "(blank)";
}
void KageLayerManager::toggleVisibility() {
	if (_currentLayerIndex < layerCount() && layers[_currentLayerIndex]->layerID == _currentLayerID) {
		layers[_currentLayerIndex]->toggleVisibility();
	} else {
		for (unsigned int i = 0; i < layerCount(); ++i) {
			if (layers[i]->layerID == _currentLayerID) {
				_currentLayerIndex = i;
				layers[i]->toggleVisibility();
				break;
			}
		}
	}
	renderStage();
}
void KageLayerManager::toggleLock() {
	if (_currentLayerIndex < layerCount() && layers[_currentLayerIndex]->layerID == _currentLayerID) {
		layers[_currentLayerIndex]->toggleLock();
	} else {
		for (unsigned int i = 0; i < layerCount(); ++i) {
			if (layers[i]->layerID == _currentLayerID) {
				_currentLayerIndex = i;
				layers[i]->toggleLock();
				break;
			}
		}
	}
	renderStage();
}

void KageLayerManager::setVisible(bool p_visible) {
	if (_currentLayerIndex < layerCount() && layers[_currentLayerIndex]->layerID == _currentLayerID) {
		layers[_currentLayerIndex]->setVisible(p_visible);
	} else {
		for (unsigned int i = 0; i < layerCount(); ++i) {
			if (layers[i]->layerID == _currentLayerID) {
				_currentLayerIndex = i;
				layers[i]->setVisible(p_visible);
				break;
			}
		}
	}
	renderStage();
}
void KageLayerManager::setLock(bool p_lock) {
	if (_currentLayerIndex < layerCount() && layers[_currentLayerIndex]->layerID == _currentLayerID) {
		layers[_currentLayerIndex]->setLock(p_lock);
	} else {
		for (unsigned int i = 0; i < layerCount(); ++i) {
			if (layers[i]->layerID == _currentLayerID) {
				_currentLayerIndex = i;
				layers[i]->setLock(p_lock);
				break;
			}
		}
	}
	renderStage();
}

/**
 * NOTE: layers are organized as index 0 as BOTTOM and last index is TOP
 * \sa moveDown() moveToBottom() moveUp()
 * \return True if successfully moved to bottom
 */
bool KageLayerManager::moveToTop() {
	bool l_return = false;
	if (_currentLayerIndex < layerCount() && layers[_currentLayerIndex]->layerID == _currentLayerID) {
		while (_currentLayerIndex < layerCount()-1) {
			l_return = moveUp();
		}
		return l_return;
	} else {
		for (unsigned int i = 0; i < layerCount(); ++i) {
			if (layers[i]->layerID == _currentLayerID) {
				_currentLayerIndex = i;
				while (_currentLayerIndex < layerCount()-1) {
					l_return = moveUp();
				}
				return l_return;
			}
		}
	}
	return false;
}
/**
 * NOTE: layers are organized as index 0 as BOTTOM and last index is TOP
 * \sa moveDown() moveToBottom() moveToTop()
 * \return True if successfully moved to bottom
 */
bool KageLayerManager::moveUp() {
	if (_currentLayerIndex < layerCount() && layers[_currentLayerIndex]->layerID == _currentLayerID) {
		if (_currentLayerIndex < layerCount()-1) {
			reorder_child(*layers[_currentLayerIndex], _currentLayerIndex+1);
			swap(layers[_currentLayerIndex], layers[_currentLayerIndex+1]);
			_currentLayerIndex = _currentLayerIndex+1;
			return true;
		}
	} else {
		for (unsigned int i = 0; i < layerCount(); ++i) {
			if (layers[i]->layerID == _currentLayerID) {
				_currentLayerIndex = i;
				if (_currentLayerIndex < layerCount()-1) {
					reorder_child(*layers[_currentLayerIndex], _currentLayerIndex+1);
					swap(layers[_currentLayerIndex], layers[_currentLayerIndex+1]);
					_currentLayerIndex = _currentLayerIndex+1;
					return true;
				}
				break;
			}
		}
	}
	return false;
}
/**
 * NOTE: layers are organized as index 0 as BOTTOM and last index is TOP
 * \sa moveToBottom() moveToTop() moveUp()
 * \return True if successfully moved to bottom
 */
bool KageLayerManager::moveDown() {
	if (_currentLayerIndex < layerCount() && layers[_currentLayerIndex]->layerID == _currentLayerID) {
		if (_currentLayerIndex > 0) {
			reorder_child(*layers[_currentLayerIndex], _currentLayerIndex-1);
			swap(layers[_currentLayerIndex], layers[_currentLayerIndex-1]);
			--_currentLayerIndex;
			return true;
		}
	} else {
		for (unsigned int i = 0; i < layerCount(); ++i) {
			if (layers[i]->layerID == _currentLayerID) {
				_currentLayerIndex = i;
				if (_currentLayerIndex > 0) {
					reorder_child(*layers[_currentLayerIndex], _currentLayerIndex-1);
					swap(layers[_currentLayerIndex], layers[_currentLayerIndex-1]);
					--_currentLayerIndex;
					return true;
				}
				break;
			}
		}
	}
	return false;
}
/**
 * NOTE: layers are organized as index 0 as BOTTOM and last index is TOP
 * \sa moveDown() moveToTop() moveUp()
 * \return True if successfully moved to bottom
 */
bool KageLayerManager::moveToBottom() {
	bool l_return = false;
	if (_currentLayerIndex < layerCount() && layers[_currentLayerIndex]->layerID == _currentLayerID) {
		while (_currentLayerIndex > 0) {
			l_return = moveDown();
		}
		return l_return;
	} else {
		for (unsigned int i = 0; i < layerCount(); ++i) {
			if (layers[i]->layerID == _currentLayerID) {
				_currentLayerIndex = i;
				while (_currentLayerIndex > 0) {
					l_return = moveDown();
				}
				return l_return;
			}
		}
	}
	return false;
}

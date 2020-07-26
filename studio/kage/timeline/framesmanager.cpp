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

#include "framesmanager.h"
#include "../../kage.h"

bool KageFramesetManager::LOADING_MODE = false;

KageFramesetManager::KageFramesetManager(Kage *p_win) {
	win = p_win;
	//addFrameset(1);
}

KageFramesetManager::~KageFramesetManager() {
	//
}

unsigned int KageFramesetManager::layerCount() {
	return _framesets.size();
}
unsigned int KageFramesetManager::frameCount() {
	unsigned int l_count = 1;
	if (_framesets.size() > 0) {
		l_count = (*_framesets.back()).getFrameCount();
	}
	return l_count;
}
void KageFramesetManager::addFrameset(unsigned int p_layer) {
	unsigned int l_count = 1;
	if (_framesets.size() > 0) {
		l_count = (*_framesets.back()).getFrameCount();
	}
	KageFramesetManager::LOADING_MODE = true;
		_framesets.push_back(Gtk::manage(new KageFrameset(this, p_layer, l_count)));
			pack_end(*_framesets.back(), Gtk::PACK_SHRINK);
				(*_framesets.back()).set_size_request(100, 23);
	KageFramesetManager::LOADING_MODE = false;
}

void KageFramesetManager::deleteFrameset(unsigned int p_layer) {
	_framesets[p_layer-1]->removeAllFrames();
	remove(*_framesets[p_layer-1]);
	_framesets.erase (_framesets.begin() + (p_layer-1));
}

bool KageFramesetManager::addFrame() {
	unsigned int l_currentLayer = getCurrentLayer();
	if (_framesets[l_currentLayer-1]->canReUseNextFrame() == false) {
		extendFrame();
	}
	_framesets[l_currentLayer-1]->duplicateFrame();
	switchToNextFrame();
	
	KageFrame *l_frame = _framesets[l_currentLayer-1]->getFrameAt(getCurrentFrame());
	if (l_frame) {
		_framesets[l_currentLayer-1]->setSelected(l_frame);
		win->doDeleteFrame();
	}
	
	return true;
}

bool KageFramesetManager::duplicateFrame() {
	unsigned int l_currentLayer = getCurrentLayer();
	if (_framesets[l_currentLayer-1]->canReUseNextFrame() == false) {
		extendFrame();
	}
	_framesets[l_currentLayer-1]->duplicateFrame();
	
	switchToNextFrame();
	
	KageFrame *l_frame = _framesets[l_currentLayer-1]->getFrameAt(getCurrentFrame());
	if (l_frame) {
		_framesets[l_currentLayer-1]->setSelected(l_frame);
	}
	
	return true;
}

bool KageFramesetManager::removeFrame() {
	unsigned int l_currentLayer = getCurrentLayer();
	if (_framesets[l_currentLayer-1]->removeFrame() == true) {
		unsigned int l_count = _framesets.size();
		for (unsigned int i = 0; i < l_count; ++i) {
			if (i != l_currentLayer-1) {
				_framesets[i]->removeFrame();
			}
		}
	}
	
	return true;
}

bool KageFramesetManager::extendFrame() {
	unsigned int l_count = 1;
	if (_framesets.size() > 0) {
		l_count = _framesets.size();
	}
	for (unsigned int i = 0; i < l_count; ++i) {
		_framesets[i]->extendFrame();
	}
	return true;
}

bool KageFramesetManager::removeAllFrames() {
	unsigned int l_count = _framesets.size();
	
	for (unsigned int i = 0; i < l_count; ++i) {
		_framesets[i]->removeAllFrames();
		remove(*_framesets[i]);
	}
	
	_framesets.clear();
	//addFrameset(0); //is this needed?!? -- 2020.03.14
	
	return true;
}

unsigned int KageFramesetManager::getCurrentLayer() {
	return win->getCurrentLayer();
}
void KageFramesetManager::setCurrentLayer(unsigned int p_currentLayer) {
	unsigned int l_count = _framesets.size();
	
	if (p_currentLayer > l_count) {
		p_currentLayer = l_count;
	}
	if (p_currentLayer < 1) {
		p_currentLayer = 1;
	}
	
	win->setCurrentLayer(p_currentLayer);
}
void KageFramesetManager::setCurrentLayerByID(unsigned int p_layerID) {
	win->setCurrentLayerByID(p_layerID);
}

unsigned int KageFramesetManager::getCurrentFrame() {
	//filter and make sure value is valid
	//NOTE: do NOT call setCurrentFrame to avoid recursive call to Kage::renderFrames()
	unsigned int l_count = _framesets.size();
	if (l_count > 0) {
		return _framesets[0]->getCurrentFrame();
	}
	
	return l_count;
}
void KageFramesetManager::setCurrentFrame(unsigned int p_frame) {
	unsigned int l_count = _framesets.size();
	if (l_count > 0) {
		unsigned int l_fcount = _framesets[0]->getFrameCount();
		if (p_frame > l_fcount) {
			p_frame = l_fcount;
		}
		if (p_frame < 1) {
			p_frame = 1;
		}
		
		for (unsigned int i = 0; i < l_count; ++i) {
			_framesets[i]->setCurrentFrame(p_frame);
		}
		selectAll(false);
		
		unsigned int l_currentLayer = win->getCurrentLayer();
		if (l_currentLayer < 1 || l_currentLayer > _framesets.size()) {
			//do nothing
		} else {
			KageFrame * l_frame = _framesets[l_currentLayer-1]->getFrameAt(p_frame);
			if (l_frame) {
				l_frame->setSelected(true);
			}
		}
		
		win->updateFrameLabel();
		win->forceRenderFrames();
	}
}
void KageFramesetManager::setCurrentFrameByID(unsigned int p_frameID) {
	unsigned int l_count = _framesets.size();
	if (l_count > 0) {
		unsigned int l_currentLayer = win->getCurrentLayer();
		if (l_currentLayer < 1 || l_currentLayer > _framesets.size()) {
			//do nothing
		} else {
			unsigned int l_frameByID = _framesets[l_currentLayer-1]->getFrameNumberByID(p_frameID);
			for (unsigned int i = 0; i < l_count; ++i) {
				_framesets[i]->setCurrentFrame(l_frameByID);
			}
			selectAll(false);
			
			KageFrame * l_frame = _framesets[l_currentLayer-1]->getFrameAt(l_frameByID);
			if (l_frame) {
				l_frame->setSelected(true);
			}
		}
		
		win->updateFrameLabel();
		win->forceRenderFrames();
	}
}

void KageFramesetManager::renderStage() {
	Kage::timestamp();
	cout << " KageFramesetManager::renderStage <" << endl;
	win->forceRenderFrames();
	Kage::timestamp();
	cout << " KageFramesetManager::renderStage >" << endl;
}

void KageFramesetManager::selectAll(bool p_selectAll) {
	unsigned int l_count = 1;
	if (_framesets.size() > 0) {
		l_count = _framesets.size();
	}
	for (unsigned int i = 0; i < l_count; ++i) {
		_framesets[i]->selectAll(p_selectAll);
	}
}

KageFrame *KageFramesetManager::getFrame() {
	unsigned int l_currentLayer = win->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > _framesets.size()) {
		return NULL;
	} else {
		KageFrameset *l_frameset = _framesets[l_currentLayer-1];
		if (l_frameset) {
			return l_frameset->getFrameAt(getCurrentFrame());
		} else {
			return _framesets[l_currentLayer-1]->getFrameAt(getCurrentFrame());
		}
	}
}

KageFrame *KageFramesetManager::getFrameAt(unsigned int p_frame) {
	unsigned int l_currentLayer = win->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > _framesets.size()) {
		return NULL;
	} else {
		KageFrameset *l_frameset = _framesets[l_currentLayer-1];
		if (l_frameset) {
			return l_frameset->getFrameAt(p_frame);
		} else {
			return _framesets[l_currentLayer-1]->getFrameAt(p_frame);
		}
	}
}

/**
 * NOTE: KageFrameset are organized as index 0 as BOTTOM and last index is TOP
 * \sa moveDown() moveToBottom() moveUp()
 * \return True if KageFrameset successfully moved to top
 */
bool KageFramesetManager::moveToTop() {
	unsigned int l_currentLayer = win->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > _framesets.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		if (l_currentLayer < _framesets.size()-1) {
			while (l_currentLayer < _framesets.size()-1) {
				reorder_child(*_framesets[l_currentLayer], l_currentLayer+1);
				swap(_framesets[l_currentLayer], _framesets[l_currentLayer+1]);
				l_currentLayer = l_currentLayer+1;
			}
			return true;
		}
	}
	return false;
}
/**
 * NOTE: KageFrameset are organized as index 0 as BOTTOM and last index is TOP
 * \sa moveDown() moveToBottom() moveToTop()
 * \return True if KageFrameset successfully moved up
 */
bool KageFramesetManager::moveUp() {
	unsigned int l_currentLayer = win->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > _framesets.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		if (l_currentLayer < _framesets.size()-1) {
			reorder_child(*_framesets[l_currentLayer], l_currentLayer+1);
			swap(_framesets[l_currentLayer], _framesets[l_currentLayer+1]);
			return true;
		}
	}
	return false;
}
/**
 * NOTE: KageFrameset are organized as index 0 as BOTTOM and last index is TOP
 * \sa moveToBottom() moveToTop() moveUp()
 * \return True if KageFrameset successfully moved down
 */
bool KageFramesetManager::moveDown() {
	unsigned int l_currentLayer = win->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > _framesets.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		if (l_currentLayer > 0) {
			reorder_child(*_framesets[l_currentLayer], l_currentLayer-1);
			swap(_framesets[l_currentLayer], _framesets[l_currentLayer-1]);
			return true;
		}
	}
	return false;
}
/**
 * NOTE: KageFrameset are organized as index 0 as BOTTOM and last index is TOP
 * \sa moveDown() moveToTop() moveUp()
 * \return True if KageFrameset successfully moved to bottom
 */
bool KageFramesetManager::moveToBottom() {
	unsigned int l_currentLayer = win->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > _framesets.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		if (l_currentLayer > 0) {
			while (l_currentLayer > 0) {
				reorder_child(*_framesets[l_currentLayer], l_currentLayer-1);
				swap(_framesets[l_currentLayer], _framesets[l_currentLayer-1]);
				l_currentLayer = l_currentLayer-1;
			}
			return true;
		}
	}
	return false;
}

vector<unsigned int> KageFramesetManager::raiseSelectedShape(vector<unsigned int> p_selectedShapes) {
	unsigned int l_currentLayer = win->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > _framesets.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return _framesets[l_currentLayer]->raiseSelectedShape(p_selectedShapes);
	}
	vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}
vector<unsigned int> KageFramesetManager::lowerSelectedShape(vector<unsigned int> p_selectedShapes) {
	unsigned int l_currentLayer = win->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > _framesets.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return _framesets[l_currentLayer]->lowerSelectedShape(p_selectedShapes);
	}
	vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}
vector<unsigned int> KageFramesetManager::raiseToTopSelectedShape(vector<unsigned int> p_selectedShapes) {
	unsigned int l_currentLayer = win->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > _framesets.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return _framesets[l_currentLayer]->raiseToTopSelectedShape(p_selectedShapes);
	}
	vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}
vector<unsigned int> KageFramesetManager::lowerToBottomSelectedShape(vector<unsigned int> p_selectedShapes) {
	unsigned int l_currentLayer = win->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > _framesets.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return _framesets[l_currentLayer]->lowerToBottomSelectedShape(p_selectedShapes);
	}
	vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}

vector<unsigned int> KageFramesetManager::groupSelectedShapes(vector<unsigned int> p_selectedShapes) {
	unsigned int l_currentLayer = win->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > _framesets.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return _framesets[l_currentLayer]->groupSelectedShapes(p_selectedShapes);
	}
	vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}
vector<unsigned int> KageFramesetManager::ungroupSelectedShapes(vector<unsigned int> p_selectedShapes) {
	unsigned int l_currentLayer = win->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > _framesets.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return _framesets[l_currentLayer]->ungroupSelectedShapes(p_selectedShapes);
	}
	vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}

vector<unsigned int> KageFramesetManager::duplicateShapes(vector<unsigned int> p_selectedShapes) {
	unsigned int l_currentLayer = win->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > _framesets.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return _framesets[l_currentLayer]->duplicateShapes(p_selectedShapes);
	}
	vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}

bool KageFramesetManager::flipHorizontalSelectedShape(vector<unsigned int> p_selectedShapes) {
	unsigned int l_currentLayer = win->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > _framesets.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return _framesets[l_currentLayer]->flipHorizontalSelectedShape(p_selectedShapes);
	}
	
	return false;
}
bool KageFramesetManager::flipVerticalSelectedShape(vector<unsigned int> p_selectedShapes) {
	unsigned int l_currentLayer = win->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > _framesets.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return _framesets[l_currentLayer]->flipVerticalSelectedShape(p_selectedShapes);
	}
	
	return false;
}

bool KageFramesetManager::recenterRotationPoint(vector<unsigned int> p_selectedShapes) {
	unsigned int l_currentLayer = win->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > _framesets.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return _framesets[l_currentLayer]->recenterRotationPoint(p_selectedShapes);
	}
	
	return false;
}

bool KageFramesetManager::addDataToFrame(VectorDataManager p_vectorsData) {
	unsigned int l_currentLayer = win->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > _framesets.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return _framesets[l_currentLayer]->addDataToFrame(p_vectorsData);
	}
	return false;
}

bool KageFramesetManager::setFrameData(VectorDataManager p_vectorsData) {
	unsigned int l_currentLayer = win->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > _framesets.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return _framesets[l_currentLayer]->setFrameData(p_vectorsData);
	}
	return false;
}
VectorDataManager KageFramesetManager::getFrameData() {
	unsigned int l_currentLayer = win->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > _framesets.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return _framesets[l_currentLayer]->getFrameData();
	}
	VectorDataManager l_nullReturn;
	return l_nullReturn;
}
VectorDataManager KageFramesetManager::getFrameDataAt(unsigned int p_frame) {
	unsigned int l_currentLayer = win->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > _framesets.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return _framesets[l_currentLayer]->getFrameDataAt(p_frame);
	}
	VectorDataManager l_nullReturn;
	return l_nullReturn;
}

bool KageFramesetManager::forceSetTween(unsigned int p_tween) {
	cout << " KageFramesetManager::setTween() " << endl;
	unsigned int l_currentLayer = win->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > _framesets.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return _framesets[l_currentLayer]->forceSetTween(p_tween);
	}
	
	return false;
}

bool KageFramesetManager::setTween(unsigned int p_tween) {
	cout << " KageFramesetManager::setTween() " << endl;
	unsigned int l_currentLayer = win->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > _framesets.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return _framesets[l_currentLayer]->setTween(p_tween);
	}
	
	return false;
}

unsigned int KageFramesetManager::getTween() {
	cout << " KageFramesetManager::getTween() " << endl;
	unsigned int l_currentLayer = win->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > _framesets.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return _framesets[l_currentLayer]->getTween();
	}
	
	return 0;
}

void KageFramesetManager::switchToPreviousFrame() {
	unsigned int l_count = 1;
	if (_framesets.size() > 0) {
		l_count = _framesets.size();
	}
	for (unsigned int i = 0; i < l_count; ++i) {
		_framesets[i]->switchToPreviousFrame();
	}
}
void KageFramesetManager::switchToPreviousFrame(unsigned int p_frameID) {
	unsigned int l_count = 1;
	if (_framesets.size() > 0) {
		l_count = _framesets.size();
	}
	unsigned int l_currentLayer = win->getCurrentLayer();
	
	if (l_currentLayer < 1 || l_currentLayer > _framesets.size()) {
		//do nothing
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		unsigned int l_frameByID = _framesets[l_currentLayer]->getFrameNumberByID(p_frameID);
		KageFrame * l_frame = _framesets[l_currentLayer]->getFrameAt(l_frameByID-1);
		
		switchToPreviousFrame();	
		selectAll(false);
		
		if (l_frame) {
			l_frame->setSelected(true);
			l_frame->setCurrent(true);
		}
	}
	
	win->updateFrameLabel();
	win->forceRenderFrames();
}

void KageFramesetManager::switchToNextFrame() {
	unsigned int l_count = 1;
	if (_framesets.size() > 0) {
		l_count = _framesets.size();
	}
	for (unsigned int i = 0; i < l_count; ++i) {
		_framesets[i]->switchToNextFrame();
	}
}
void KageFramesetManager::switchToNextFrame(unsigned int p_frameID) {
	unsigned int l_count = 1;
	if (_framesets.size() > 0) {
		l_count = _framesets.size();
	}
	
	unsigned int l_currentLayer = win->getCurrentLayer();
	
	if (l_currentLayer < 1 || l_currentLayer > _framesets.size()) {
		//do nothing
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		unsigned int l_frameByID = _framesets[l_currentLayer]->getFrameNumberByID(p_frameID);
		KageFrame * l_frame = _framesets[l_currentLayer]->getFrameAt(l_frameByID+1);
		
		switchToNextFrame();	
		selectAll(false);
		
		if (l_frame) {
			l_frame->setSelected(true);
			l_frame->setCurrent(true);
		}
	}
	
	win->updateFrameLabel();
	win->forceRenderFrames();
}

void KageFramesetManager::setFrameExtension(KageFrame::extension p_extension) {
	unsigned int l_currentLayer = win->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > _framesets.size()) {
		cout << "KageFramesetManager::setFrameExtension FAILED" << endl;
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		_framesets[l_currentLayer]->setFrameExtension(p_extension);
	}
}

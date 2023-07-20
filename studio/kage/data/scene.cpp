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

#include "scene.h"
#include "../../kage.h"

KageScene::KageScene(KageDocument *p_document, string p_filepath) {
	init(p_document);
	open(p_filepath);
}

void KageScene::init(KageDocument *p_document) {
	_document = p_document;

	ID = 0;
	_selected = false;

	//_activeLayer = 1;
	
	//Layers.push_back(new KageLayer (this, _activeLayer, 1));
}

bool KageScene::open(string p_filepath) {
	//TODO: open KSF -- take it from kage.cpp
	return true;
}

bool KageScene::save() {
	//TODO: save to KSF -- take it from kage.cpp
	_saved = true;
	return true;
}

bool KageScene::isSelected() {
	return _selected;
}
bool KageScene::isSaved() {
	return _saved;
}

bool KageScene::LOADING_MODE = false;

KageScene::KageScene(KageDocument *p_document) {
	init(p_document);
}
KageScene::KageScene(KageDocument *p_document, KageScene *p_scene) {
	_parent = p_scene;
	init(p_document);
}

KageScene::~KageScene() {
	Layers.clear();
	_document = NULL;
}

unsigned int KageScene::layerCount() {
	return Layers.size();
}
unsigned int KageScene::frameCount() {
	unsigned int l_count = 1;
	if (Layers.size() > 0) {
		l_count = (*Layers.back()).getFrameCount();
	}
	return l_count;
}

void KageScene::addLayer(unsigned int p_layer) {
	unsigned int l_count = 1;
	if (Layers.size() > 0) {
		l_count = (*Layers.back()).getFrameCount();
	}
	
	Layers.push_back(new KageLayer(this, p_layer, l_count));
}

//void KageScene::deleteLayer(unsigned int p_layer) {
void KageScene::deleteFrameset(unsigned int p_layer) {
	Layers[p_layer-1]->removeAllFrames();
	Layers.erase (Layers.begin() + (p_layer-1));
}

bool KageScene::addFrame() {
	unsigned int l_currentLayer = getCurrentLayer();
	if (Layers[l_currentLayer-1]->canReUseNextFrame() == false) {
		extendFrame();
	}
	Layers[l_currentLayer-1]->duplicateFrame();
	switchToNextFrame();
	
	KageFrame *l_frame = Layers[l_currentLayer-1]->getFrameAt(getCurrentFrame());
	if (l_frame) {
		Layers[l_currentLayer-1]->setSelected(l_frame);
		_document->_kage->doDeleteFrame();
	}
	
	return true;
}

bool KageScene::duplicateFrame() {
	unsigned int l_currentLayer = getCurrentLayer();
	if (Layers[l_currentLayer-1]->canReUseNextFrame() == false) {
		extendFrame();
	}
	Layers[l_currentLayer-1]->duplicateFrame();
	
	switchToNextFrame();
	
	KageFrame *l_frame = Layers[l_currentLayer-1]->getFrameAt(getCurrentFrame());
	if (l_frame) {
		Layers[l_currentLayer-1]->setSelected(l_frame);
	}
	
	return true;
}

bool KageScene::removeFrame() {
	unsigned int l_currentLayer = getCurrentLayer();
	if (Layers[l_currentLayer-1]->removeFrame() == true) {
		unsigned int l_count = Layers.size();
		for (unsigned int i = 0; i < l_count; ++i) {
			if (i != l_currentLayer-1) {
				Layers[i]->removeFrame();
			}
		}
	}
	
	return true;
}

bool KageScene::extendFrame() {
	unsigned int l_count = 1;
	if (Layers.size() > 0) {
		l_count = Layers.size();
	}
	for (unsigned int i = 0; i < l_count; ++i) {
		Layers[i]->extendFrame();
	}
	return true;
}

bool KageScene::removeAllFrames() {
	unsigned int l_count = Layers.size();
	
	for (unsigned int i = 0; i < l_count; ++i) {
		Layers[i]->removeAllFrames();
	}
	
	Layers.clear();
	//addFrameset(0); //is this needed?!? -- 2020.03.14
	
	return true;
}

void KageScene::setActiveLayerID(unsigned int p_layerID) {
	_activeLayer = p_layerID;
}

unsigned int KageScene::getCurrentLayer() {
	return _document->_kage->getCurrentLayer();
}
void KageScene::setCurrentLayer(unsigned int p_currentLayer) {
	unsigned int l_count = Layers.size();
	
	if (p_currentLayer > l_count) {
		p_currentLayer = l_count;
	}
	if (p_currentLayer < 1) {
		p_currentLayer = 1;
	}
	
	_document->_kage->setCurrentLayer(p_currentLayer);
}
void KageScene::setCurrentLayerByID(unsigned int p_layerID) {
	_document->_kage->setCurrentLayerByID(p_layerID);
}

unsigned int KageScene::getCurrentFrame() {
	//filter and make sure value is valid
	//NOTE: do NOT call setCurrentFrame to avoid recursive call to Kage::renderFrames()
	unsigned int l_count = Layers.size();
	if (l_count > 0) {
		return Layers[0]->getCurrentFrame();
	}
	
	return l_count;
}
void KageScene::setCurrentFrame(unsigned int p_frame) {
	unsigned int l_count = Layers.size();
	if (l_count > 0) {
		unsigned int l_fcount = Layers[0]->getFrameCount();
		if (p_frame > l_fcount) {
			p_frame = l_fcount;
		}
		if (p_frame < 1) {
			p_frame = 1;
		}
		
		for (unsigned int i = 0; i < l_count; ++i) {
			Layers[i]->setCurrentFrame(p_frame);
		}
		selectAll(false);
		
		unsigned int l_currentLayer = _document->_kage->getCurrentLayer();
		if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
			//do nothing
		} else {
			KageFrame * l_frame = Layers[l_currentLayer-1]->getFrameAt(p_frame);
			if (l_frame) {
				l_frame->setSelected(true);
			}
		}
		
		_document->_kage->forceRenderFrames();
		_document->_kage->refreshUI();
	}
}
void KageScene::setCurrentFrameByID(unsigned int p_frameID) {
	unsigned int l_count = Layers.size();
	if (l_count > 0) {
		unsigned int l_currentLayer = _document->_kage->getCurrentLayer();
		if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
			//do nothing
		} else {
			unsigned int l_frameByID = Layers[l_currentLayer-1]->getFrameNumberByID(p_frameID);
			for (unsigned int i = 0; i < l_count; ++i) {
				Layers[i]->setCurrentFrame(l_frameByID);
			}
			selectAll(false);
			
			KageFrame * l_frame = Layers[l_currentLayer-1]->getFrameAt(l_frameByID);
			if (l_frame) {
				l_frame->setSelected(true);
			}
		}
		
		_document->_kage->forceRenderFrames();
		_document->_kage->refreshUI();
	}
}

void KageScene::renderStage() {
	_document->_kage->forceRenderFrames();
	_document->_kage->refreshUI();
}

void KageScene::selectAll(bool p_selectAll) {
	unsigned int l_count = 1;
	if (Layers.size() > 0) {
		l_count = Layers.size();
	}
	for (unsigned int i = 0; i < l_count; ++i) {
		Layers[i]->selectAll(p_selectAll);
	}
}

KageFrame *KageScene::getFrame() {
	unsigned int l_currentLayer = _document->_kage->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		return NULL;
	} else {
		KageLayer *l_layer = Layers[l_currentLayer-1];
		if (l_layer) {
			return l_layer->getFrameAt(getCurrentFrame());
		} else {
			return Layers[l_currentLayer-1]->getFrameAt(getCurrentFrame());
		}
	}
}

KageFrame *KageScene::getFrameAt(unsigned int p_frame) {
	unsigned int l_currentLayer = _document->_kage->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		return NULL;
	} else {
		KageLayer *l_layer = Layers[l_currentLayer-1];
		if (l_layer) {
			return l_layer->getFrameAt(p_frame);
		} else {
			return Layers[l_currentLayer-1]->getFrameAt(p_frame);
		}
	}
}

KageLayer *KageScene::getLayer() {
	unsigned int l_currentLayer = _document->_kage->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		return NULL;
	} else {
		return Layers[l_currentLayer-1];
	}
}
KageLayer *KageScene::getLayerAt(unsigned int p_layer) {
	if (p_layer < 1 || p_layer > Layers.size()) {
		return NULL;
	} else {
		return Layers[p_layer-1];
	}
}
			
/**
 * NOTE: KageFrameset are organized as index 0 as BOTTOM and last index is TOP
 * \sa moveDown() moveToBottom() moveUp()
 * \return True if KageFrameset successfully moved to top
 */
bool KageScene::moveToTop() {
	unsigned int l_currentLayer = _document->_kage->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		if (l_currentLayer < Layers.size()-1) {
			while (l_currentLayer < Layers.size()-1) {
				swap(Layers[l_currentLayer], Layers[l_currentLayer+1]);
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
bool KageScene::moveUp() {
	unsigned int l_currentLayer = _document->_kage->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		if (l_currentLayer < Layers.size()-1) {
			swap(Layers[l_currentLayer], Layers[l_currentLayer+1]);
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
bool KageScene::moveDown() {
	unsigned int l_currentLayer = _document->_kage->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		if (l_currentLayer > 0) {
			swap(Layers[l_currentLayer], Layers[l_currentLayer-1]);
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
bool KageScene::moveToBottom() {
	unsigned int l_currentLayer = _document->_kage->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		if (l_currentLayer > 0) {
			while (l_currentLayer > 0) {
				swap(Layers[l_currentLayer], Layers[l_currentLayer-1]);
				l_currentLayer = l_currentLayer-1;
			}
			return true;
		}
	}
	return false;
}

vector<unsigned int> KageScene::raiseSelectedShape(vector<unsigned int> p_selectedShapes) {
	unsigned int l_currentLayer = _document->_kage->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return Layers[l_currentLayer]->raiseSelectedShape(p_selectedShapes);
	}
	vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}
vector<unsigned int> KageScene::lowerSelectedShape(vector<unsigned int> p_selectedShapes) {
	unsigned int l_currentLayer = _document->_kage->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return Layers[l_currentLayer]->lowerSelectedShape(p_selectedShapes);
	}
	vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}
vector<unsigned int> KageScene::raiseToTopSelectedShape(vector<unsigned int> p_selectedShapes) {
	unsigned int l_currentLayer = _document->_kage->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return Layers[l_currentLayer]->raiseToTopSelectedShape(p_selectedShapes);
	}
	vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}
vector<unsigned int> KageScene::lowerToBottomSelectedShape(vector<unsigned int> p_selectedShapes) {
	unsigned int l_currentLayer = _document->_kage->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return Layers[l_currentLayer]->lowerToBottomSelectedShape(p_selectedShapes);
	}
	vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}

vector<unsigned int> KageScene::groupSelectedShapes(vector<unsigned int> p_selectedShapes) {
	unsigned int l_currentLayer = _document->_kage->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return Layers[l_currentLayer]->groupSelectedShapes(p_selectedShapes);
	}
	vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}
vector<unsigned int> KageScene::ungroupSelectedShapes(vector<unsigned int> p_selectedShapes) {
	unsigned int l_currentLayer = _document->_kage->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return Layers[l_currentLayer]->ungroupSelectedShapes(p_selectedShapes);
	}
	vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}

vector<unsigned int> KageScene::duplicateShapes(vector<unsigned int> p_selectedShapes) {
	unsigned int l_currentLayer = _document->_kage->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return Layers[l_currentLayer]->duplicateShapes(p_selectedShapes);
	}
	vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}

bool KageScene::flipHorizontalSelectedShape(vector<unsigned int> p_selectedShapes) {
	unsigned int l_currentLayer = _document->_kage->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return Layers[l_currentLayer]->flipHorizontalSelectedShape(p_selectedShapes);
	}
	
	return false;
}
bool KageScene::flipVerticalSelectedShape(vector<unsigned int> p_selectedShapes) {
	unsigned int l_currentLayer = _document->_kage->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return Layers[l_currentLayer]->flipVerticalSelectedShape(p_selectedShapes);
	}
	
	return false;
}

bool KageScene::recenterRotationPoint(vector<unsigned int> p_selectedShapes) {
	unsigned int l_currentLayer = _document->_kage->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return Layers[l_currentLayer]->recenterRotationPoint(p_selectedShapes);
	}
	
	return false;
}

bool KageScene::addDataToFrame(VectorDataManager p_vectorsData) {
	unsigned int l_currentLayer = _document->_kage->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return Layers[l_currentLayer]->addDataToFrame(p_vectorsData);
	}
	return false;
}

bool KageScene::setFrameData(VectorDataManager p_vectorsData) {
	unsigned int l_currentLayer = _document->_kage->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return Layers[l_currentLayer]->setFrameData(p_vectorsData);
	}
	return false;
}
VectorDataManager KageScene::getFrameData() {
	unsigned int l_currentLayer = _document->_kage->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return Layers[l_currentLayer]->getFrameData();
	}
	VectorDataManager l_nullReturn;
	return l_nullReturn;
}
VectorDataManager KageScene::getFrameDataAt(unsigned int p_frame) {
	unsigned int l_currentLayer = _document->_kage->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return Layers[l_currentLayer]->getFrameDataAt(p_frame);
	}
	VectorDataManager l_nullReturn;
	return l_nullReturn;
}

bool KageScene::forceSetTween(unsigned int p_tween) {
//	cout << " KageScene::forceSetTween() " << p_tween << endl;
	unsigned int l_currentLayer = _document->_kage->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return Layers[l_currentLayer]->forceSetTween(p_tween);
	}
	
	return false;
}

bool KageScene::setTween(unsigned int p_tween) {
	cout << " KageScene::setTween() " << p_tween << endl;
	unsigned int l_currentLayer = _document->_kage->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return Layers[l_currentLayer]->setTween(p_tween);
	}
	
	return false;
}

unsigned int KageScene::getTween() {
	unsigned int l_currentLayer = _document->_kage->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		unsigned int l_ret = Layers[l_currentLayer]->getTween();
		return l_ret;
	}
	
	return 0;
}

bool KageScene::switchToPreviousFrame() {
	unsigned int l_count = 1;
	if (Layers.size() > 0) {
		l_count = Layers.size();
	}
	bool l_return = false;
	for (unsigned int i = 0; i < l_count; ++i) {
		l_return = Layers[i]->switchToPreviousFrame();
		if (KageScene::LOADING_MODE == false) {
			Layers[i]->getFrame()->setSelected(false);
		}
	}
	if (KageScene::LOADING_MODE == false) {
		Layers[getActiveLayerID()-1]->getFrame()->setSelected(true);
	}
	return l_return;
}
bool KageScene::switchToPreviousFrame(unsigned int p_frameID) {
	unsigned int l_count = 1;
	if (Layers.size() > 0) {
		l_count = Layers.size();
	}
	unsigned int l_currentLayer = _document->_kage->getCurrentLayer();
	
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//do nothing
		return false;
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		unsigned int l_frameByID = Layers[l_currentLayer]->getFrameNumberByID(p_frameID);
		KageFrame * l_frame = Layers[l_currentLayer]->getFrameAt(l_frameByID-1);
		
		switchToPreviousFrame();	
		selectAll(false);
		
		if (l_frame) {
			l_frame->setSelected(true);
			l_frame->setCurrent(true);
		}
	}
	
	_document->_kage->forceRenderFrames();
	_document->_kage->refreshUI();
	
	return true;
}

bool KageScene::switchToNextFrame() {
	unsigned int l_count = 1;
	if (Layers.size() > 0) {
		l_count = Layers.size();
	}
	bool l_return = false;
	
	for (unsigned int i = 0; i < l_count; ++i) {
		l_return = Layers[i]->switchToNextFrame();
		if (KageScene::LOADING_MODE == false) {
			Layers[i]->getFrame()->setSelected(false);
			Layers[i]->getFrame()->setSelected(false);
		}
	}
	if (KageScene::LOADING_MODE == false) {
		Layers[getActiveLayerID()-1]->getFrame()->setSelected(true);
	}
	return l_return;
}
bool KageScene::switchToNextFrame(unsigned int p_frameID) {
	unsigned int l_count = 1;
	if (Layers.size() > 0) {
		l_count = Layers.size();
	}
	
	unsigned int l_currentLayer = _document->_kage->getCurrentLayer();
	
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//do nothing
		return false;
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		unsigned int l_frameByID = Layers[l_currentLayer]->getFrameNumberByID(p_frameID);
		KageFrame * l_frame = Layers[l_currentLayer]->getFrameAt(l_frameByID+1);
		
		switchToNextFrame();	
		selectAll(false);
		
		if (l_frame) {
			l_frame->setSelected(true);
			l_frame->setCurrent(true);
		}
	}
	
	_document->_kage->forceRenderFrames();
	_document->_kage->refreshUI();
	
	return true;
}

void KageScene::setFrameExtension(KageFrame::extension p_extension) {
	unsigned int l_currentLayer = _document->_kage->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		cout << "KageScene::setFrameExtension FAILED" << endl;
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		Layers[l_currentLayer]->setFrameExtension(p_extension);
	}
}

unsigned int KageScene::getActiveLayerID() {
	return _activeLayer;
}
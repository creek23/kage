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

#include "scene.h"
#include "../../kage.h"

KageScene::KageScene(KageDocument *p_document, unsigned int p_sceneID, string p_filepath) {
	init(p_document, p_sceneID);
	open(p_filepath);
}

void KageScene::init(KageDocument *p_document, unsigned int p_sceneID) {
	_document = p_document;
	
	ID = p_sceneID;
	setSelected(false);

	layerCtr = 0;

	_selected = false;
	_activeLayerID = UINT_MAX;
	addLayer();
	_activeLayerIndex = 0;
	_activeLayerID = Layers[_activeLayerIndex]->getID();
	_activeLayer = _activeLayerIndex; + 1;
}

unsigned int KageScene::getID() {
	return ID;
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

void KageScene::setSelected(bool p_selected) {
	_selected = p_selected;
}
bool KageScene::isSelected() {
	return _selected;
}
bool KageScene::isSaved() {
	return _saved;
}

bool KageScene::LOADING_MODE = false;

KageScene::KageScene(KageDocument *p_document, unsigned int p_sceneID) {
	init(p_document, p_sceneID);
}
KageScene::KageScene(KageDocument *p_document, unsigned int p_sceneID, KageScene *p_scene) {
	init(p_document, p_sceneID);
	_parent = p_scene;
}

KageScene::~KageScene() {
	Layers.clear();
	_document = NULL;
}

unsigned int KageScene::frameCount() {
	unsigned int l_count = 1;
	if (Layers.size() > 0) {
		l_count = (*Layers.back()).Frames.size();
	}
	return l_count;
}

void KageScene::addLayer() {
	++layerCtr;
	//TODO: add layer above currently selected Layer
	Layers.push_back(new KageLayer(this, layerCtr, frameCount()));
	if (_activeLayerID == UINT_MAX) {
		setActiveLayerID(layerCtr);
	}
}

void KageScene::deleteLayer(unsigned int p_layerID) {
	//TODO: should determine if Layer on Index is same layerID as p_layerID
	Layers[p_layerID-1]->removeAllFrames();
	Layers.erase (Layers.begin() + (p_layerID-1));
}

bool KageScene::removeLayerAt(unsigned int p_layerIndex) {
	if (p_layerIndex >= 0 && p_layerIndex < Layers.size()) {
		bool l_return = Layers[p_layerIndex]->removeAllFrames();
		if (l_return) {
			delete Layers[p_layerIndex];
			Layers.erase( Layers.begin() + p_layerIndex );
		}
		return l_return;
	}

	return false;
}

bool KageScene::addLayerFrame() {
	unsigned int l_currentLayer = getCurrentLayer();
	if (Layers[l_currentLayer-1]->canReUseNextFrame() == false) {
		extendLayerFrame();
	}
	Layers[l_currentLayer-1]->duplicateFrame(); ///NOTE: why duplicate?
	layerSwitchToNextFrame();
	
	KageFrame *l_frame = Layers[l_currentLayer-1]->getFrameAt(getCurrentFrame());
	if (l_frame) {
		Layers[l_currentLayer-1]->setSelected(l_frame); //why duplicate frame then delete? why not just create empty frame via Layer->addFrame?
		std::cout << "KageScene::addLayerFrame DELETED " << _document->_kage->doDeleteFrame() << std::endl;
	}
	
	return true;
}

bool KageScene::duplicateLayerFrame() {
	unsigned int l_currentLayer = getCurrentLayer();
	if (Layers[l_currentLayer-1]->canReUseNextFrame() == false) {
		extendLayerFrame();
	}
	Layers[l_currentLayer-1]->duplicateFrame();
	
	layerSwitchToNextFrame();
	
	KageFrame *l_frame = Layers[l_currentLayer-1]->getFrameAt(getCurrentFrame());
	if (l_frame) {
		Layers[l_currentLayer-1]->setSelected(l_frame);
	}
	
	return true;
}

bool KageScene::removeLayerFrame() {
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

bool KageScene::extendLayerFrame() {
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
	int l_count = Layers.size();
	
	for (int i = l_count-1; i > 0; --i) {
		Layers[i]->removeAllFrames();
	}
	
	Layers.clear();
	//addFrameset(0); //is this needed?!? -- 2020.03.14
	
	return true;
}

void KageScene::setActiveLayerID(unsigned int p_layerID) {
	_activeLayerID = p_layerID;
}

/** For use of Kage.  To keep track of active Layer in-check of all layers in Scene
 * \return index+1 of registered active layer
 * \sa setCurrentLayer()
 */
unsigned int KageScene::getCurrentLayer() {
	if (_activeLayerIndex < Layers.size() && Layers[_activeLayerIndex]->getID() == _activeLayerID) {
		_activeLayer = _activeLayerIndex+1;
		return _activeLayerIndex+1;
	} else {
		for (unsigned int i = 0; i < Layers.size(); ++i) {
			if (Layers[i]->getID() == _activeLayerID) {
				_activeLayerIndex = i;
				_activeLayer = i+1;
				return i+1; //temporarily add +1 as we integrate code to previous implementation
			}
		}
	}
	_activeLayer = Layers.size()+1;
	//should size+1 to indicate to easily identify if No Active Layer
	return Layers.size(); //temporarily DONT -1 as we integrate code to previous implementation
}
/** For use of Kage.
 * \param p_layer is index+1 of active layer
 * \param p_addSelected select another Layer or not
 * \sa getCurrentLayer()
 */
void KageScene::setCurrentLayer(unsigned int p_currentLayer, bool p_addSelected) {
	unsigned int l_fromLayerIndex = UINT_MAX;
	if (p_addSelected == true) {
		//get current layer to select range of layers
		if (_activeLayerIndex < Layers.size() && Layers[_activeLayerIndex]->getID() == _activeLayerID) {
			l_fromLayerIndex = _activeLayerIndex;
		} else {
			for (unsigned int i = 0; i < Layers.size(); ++i) {
				if (Layers[i]->getID() == _activeLayerID) {
					l_fromLayerIndex = i;
					break;
				}
			}
		}
	} else {
		//clear previous
		for (unsigned int i = 0; i < Layers.size(); ++i) {
			Layers[i]->setSelected(false);
		}
	}
	//select new layer
	unsigned int l_count = Layers.size();
	if (p_currentLayer > l_count) {
		p_currentLayer = l_count;
	}
	if (p_currentLayer < 1) {
		p_currentLayer = 1;
	}
	_activeLayerIndex = p_currentLayer-1;
	_activeLayerID  = Layers[_activeLayerIndex]->getID();
	_activeLayer    = p_currentLayer;
	Layers[_activeLayerIndex]->setSelected(true);
	
	if (p_addSelected == true) {
		unsigned int l_start = 0;
		unsigned int l_stop = 0;
		if (_activeLayerIndex < l_fromLayerIndex) {
			l_start = _activeLayerIndex+1;
			l_stop = l_fromLayerIndex+1;
		} else {
			l_start = l_fromLayerIndex;
			l_stop = _activeLayerIndex+1;
		}
		for (unsigned int i = l_start; i < l_stop; ++i) {
			Layers[i]->setSelected(true);
		}
	}
}
void KageScene::setCurrentLayerByID(unsigned int p_layerID) {
	//unselect current layer
	if (_activeLayerIndex < Layers.size() && Layers[_activeLayerIndex]->getID() == _activeLayerID) {
		Layers[_activeLayerIndex]->setSelected(false);
	} else {
		for (unsigned int i = 0; i < Layers.size(); ++i) {
			if (Layers[i]->getID() == _activeLayerID) {
				_activeLayerID = p_layerID;
				Layers[i]->setSelected(false);
				break;
			}
		}
	}
	//select new layer by ID	
	for (unsigned int i = 0; i < Layers.size(); ++i) {
		if (Layers[i]->getID() == p_layerID) {
			_activeLayerIndex = i;
			_activeLayerID = p_layerID;
			Layers[_activeLayerIndex]->setSelected(true);
			break;
		}
	}
}

unsigned int KageScene::getCurrentFrame() {
	//filter and make sure value is valid
	//NOTE: do NOT call setLayerCurrentFrame to avoid recursive call to Kage::renderFrames()
	unsigned int l_count = Layers.size();
	if (l_count > 0) {
		return Layers[0]->getCurrentFrame();
	}
	
	return 0;
}
void KageScene::setLayerCurrentFrame(unsigned int p_frame, bool p_addSelected) {
	unsigned int l_layerCount = Layers.size();
	if (l_layerCount > 0) {
		unsigned int l_fcount = Layers[0]->Frames.size();
		if (p_frame > l_fcount) {
			p_frame = l_fcount;
		}
		if (p_frame < 1) {
			p_frame = 1;
		}
		unsigned int l_currentLayer = getCurrentLayer();
		if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
			//do nothing
		} else {
			for (unsigned int i = 0; i < l_layerCount; ++i) {
				//TODO: only apply p_addSelected to current layer
				if (Layers[i]->isSelected()) {
				//if (i == l_currentLayer-1) {
					Layers[i]->setCurrentFrame(p_frame, p_addSelected);
				} else {
					Layers[i]->setCurrentFrame(p_frame, false);
				}
			}
			if (p_addSelected == false) {
				selectAllLayerFrame(false);
			}

			KageFrame * l_frame = Layers[l_currentLayer-1]->getFrameAt(p_frame);
			if (l_frame) {
				l_frame->setSelected(true);
			}
		}
	}
}
void KageScene::setCurrentFrameByID(unsigned int p_frameID) {
	unsigned int l_layerCount = Layers.size();
	if (l_layerCount > 0) {
		unsigned int l_currentLayer = getCurrentLayer();
		if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
			//do nothing
		} else {
			unsigned int l_frameByID = Layers[l_currentLayer-1]->getFrameNumberByID(p_frameID);
			for (unsigned int i = 0; i < l_layerCount; ++i) {
				Layers[i]->setCurrentFrame(l_frameByID, false);
			}
			selectAllLayerFrame(false);
			
			KageFrame * l_frame = Layers[l_currentLayer-1]->getFrameAt(l_frameByID);
			if (l_frame) {
				l_frame->setSelected(true);
			}
		}
		
		_document->_kage->forceRenderFrames();
		_document->_kage->_timeline.forceRender();
		_document->_kage->refreshUI();
	}
}

void KageScene::renderStage() {//who is calling renderStage?
	_document->_kage->forceRenderFrames();
	_document->_kage->refreshUI();
}

void KageScene::selectAllLayerFrame(bool p_selectAll) {
	unsigned int l_layerCount = Layers.size();
	for (unsigned int i = 0; i < l_layerCount; ++i) {
		Layers[i]->selectAllFrame(p_selectAll);
	}
}

KageFrame *KageScene::getFrame() {
	unsigned int l_currentLayer = getCurrentLayer();
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
	unsigned int l_currentLayer = getCurrentLayer();
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
	unsigned int l_currentLayer = getCurrentLayer();
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
	unsigned int l_currentLayer = getCurrentLayer();
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
	unsigned int l_currentLayer = getCurrentLayer();
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
	unsigned int l_currentLayer = getCurrentLayer();
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
	unsigned int l_currentLayer = getCurrentLayer();
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
	unsigned int l_currentLayer = getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return Layers[l_currentLayer]->raiseSelectedShape(p_selectedShapes);
	}
	std::vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}
vector<unsigned int> KageScene::lowerSelectedShape(vector<unsigned int> p_selectedShapes) {
	unsigned int l_currentLayer = getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return Layers[l_currentLayer]->lowerSelectedShape(p_selectedShapes);
	}
	std::vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}
vector<unsigned int> KageScene::raiseToTopSelectedShape(vector<unsigned int> p_selectedShapes) {
	unsigned int l_currentLayer = getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return Layers[l_currentLayer]->raiseToTopSelectedShape(p_selectedShapes);
	}
	std::vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}
vector<unsigned int> KageScene::lowerToBottomSelectedShape(vector<unsigned int> p_selectedShapes) {
	unsigned int l_currentLayer = getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return Layers[l_currentLayer]->lowerToBottomSelectedShape(p_selectedShapes);
	}
	std::vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}

vector<unsigned int> KageScene::groupSelectedShapes(vector<unsigned int> p_selectedShapes) {
	unsigned int l_currentLayer = getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return Layers[l_currentLayer]->groupSelectedShapes(p_selectedShapes);
	}
	std::vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}
vector<unsigned int> KageScene::ungroupSelectedShapes(vector<unsigned int> p_selectedShapes) {
	unsigned int l_currentLayer = getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return Layers[l_currentLayer]->ungroupSelectedShapes(p_selectedShapes);
	}
	std::vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}

vector<unsigned int> KageScene::duplicateShapes(vector<unsigned int> p_selectedShapes) {
	unsigned int l_currentLayer = getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return Layers[l_currentLayer]->duplicateShapes(p_selectedShapes);
	}
	std::vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}

bool KageScene::flipHorizontalSelectedShape(vector<unsigned int> p_selectedShapes) {
	unsigned int l_currentLayer = getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return Layers[l_currentLayer]->flipHorizontalSelectedShape(p_selectedShapes);
	}
	
	return false;
}
bool KageScene::flipVerticalSelectedShape(vector<unsigned int> p_selectedShapes) {
	unsigned int l_currentLayer = getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return Layers[l_currentLayer]->flipVerticalSelectedShape(p_selectedShapes);
	}
	
	return false;
}

bool KageScene::recenterRotationPoint(vector<unsigned int> p_selectedShapes) {
	unsigned int l_currentLayer = getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return Layers[l_currentLayer]->recenterRotationPoint(p_selectedShapes);
	}
	
	return false;
}

bool KageScene::addDataToFrame(VectorDataManager p_vectorsData) {
	unsigned int l_currentLayer = getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return Layers[l_currentLayer]->addDataToFrame(p_vectorsData);
	}
	return false;
}

bool KageScene::setFrameData(VectorDataManager p_vectorsData) {
	unsigned int l_currentLayer = getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return Layers[l_currentLayer]->setFrameData(p_vectorsData);
	}
	return false;
}
VectorDataManager KageScene::getFrameData() {
	
	unsigned int l_currentLayer = getCurrentLayer();
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
	unsigned int l_currentLayer = getCurrentLayer();
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
//	std::cout << " KageScene::forceSetTween() " << p_tween << std::endl;
	unsigned int l_currentLayer = getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return Layers[l_currentLayer]->forceSetTween(p_tween);
	}
	
	return false;
}

bool KageScene::setTween(unsigned int p_tween) {
	std::cout << " KageScene::setTween() " << p_tween << std::endl;
	unsigned int l_currentLayer = getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		return Layers[l_currentLayer]->setTween(p_tween);
	}
	
	return false;
}

unsigned int KageScene::getTween() {
	unsigned int l_currentLayer = getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		unsigned int l_ret = Layers[l_currentLayer]->getTween();
		return l_ret;
	}
	
	return 0;
}

bool KageScene::layerSwitchToPreviousFrame() {
	unsigned int l_count = 1;
	if (Layers.size() > 0) {
		l_count = Layers.size();
	}
	bool l_return = false;
	unsigned int l_currentLayer = getCurrentLayer();
	for (unsigned int i = 0; i < l_count; ++i) {
		l_return = Layers[i]->switchToPreviousFrame();
	}
	selectAllLayerFrame(false);

	if (KageScene::LOADING_MODE == false) {
		Layers[l_currentLayer-1]->getFrame()->setSelected(true);
	}
	
	setCurrentLayer(l_currentLayer, false);

	return l_return;
}
bool KageScene::layerSwitchToPreviousFrame(unsigned int p_frameID) {
	unsigned int l_count = 1;
	if (Layers.size() > 0) {
		l_count = Layers.size();
	}
	unsigned int l_currentLayer = getCurrentLayer();
	
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//do nothing
		return false;
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		unsigned int l_frameByID = Layers[l_currentLayer]->getFrameNumberByID(p_frameID);
		KageFrame * l_frame = Layers[l_currentLayer]->getFrameAt(l_frameByID-1);
		
		layerSwitchToPreviousFrame();	
		selectAllLayerFrame(false);
		
		if (l_frame) {
			l_frame->setSelected(true);
			l_frame->setCurrent(true);
		}
	}

	setCurrentLayer(l_currentLayer, false);
	
	return true;
}

bool KageScene::layerSwitchToNextFrame() {
	unsigned int l_count = 1;
	if (Layers.size() > 0) {
		l_count = Layers.size();
	}
	bool l_return = false;
	unsigned int l_currentLayer = getCurrentLayer();
	for (unsigned int i = 0; i < l_count; ++i) {
		l_return = Layers[i]->switchToNextFrame();
	}
	selectAllLayerFrame(false);

	if (KageScene::LOADING_MODE == false) {
		Layers[l_currentLayer-1]->getFrame()->setSelected(true);
	}

	setCurrentLayer(l_currentLayer, false);
	return l_return;
}
bool KageScene::layerSwitchToNextFrame(unsigned int p_frameID) {
	unsigned int l_count = 1;
	if (Layers.size() > 0) {
		l_count = Layers.size();
	}
	
	unsigned int l_currentLayer = getCurrentLayer();
	
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		//do nothing
		return false;
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		unsigned int l_frameByID = Layers[l_currentLayer]->getFrameNumberByID(p_frameID);
		KageFrame * l_frame = Layers[l_currentLayer]->getFrameAt(l_frameByID+1);
		
		layerSwitchToNextFrame();	
		selectAllLayerFrame(false);
		
		if (l_frame) {
			l_frame->setSelected(true);
			l_frame->setCurrent(true);
		}
	}
	
	setCurrentLayer(l_currentLayer, false);
	return true;
}

void KageScene::setFrameExtension(KageFrame::extension p_extension) {
	unsigned int l_currentLayer = getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > Layers.size()) {
		std::cout << "KageScene::setFrameExtension FAILED" << std::endl;
	} else {
		--l_currentLayer; //layer now becomes Layer Index
		Layers[l_currentLayer]->setFrameExtension(p_extension);
	}
}

/**
 * @brief will be called from KageTimeline from mouse gesture of stretching the frame
 * 
 * @param p_layerIndex 0 to Layer size less 1
 * @param p_frameCount how many frames to stretch
 */
void KageScene::extendFrameAt(unsigned int p_layerIndex, unsigned int p_frameIndex, unsigned int p_frameCount) {
	int l_layerSize = Layers.size();
	if (p_layerIndex >= l_layerSize) {
		std::cout << "KageScene::extendFrameAt FAILED" << std::endl;
	} else {
		for (unsigned int l_layerIndex = 0; l_layerIndex < l_layerSize; ++l_layerIndex) {
			if (l_layerIndex != p_layerIndex) {
				//TODO: stretch the layer's last frame by p_frameCount
				//Layers[l_layerIndex].goToFrame(p_frameIndex); //TODO: <-- implement this function
				for (unsigned int l_frameCount = 1; l_frameCount <= p_frameCount; ++l_frameCount) {
				//	Layers[l_layerIndex].extendFrame();
				}
			}
		}
	}
}

unsigned int KageScene::getActiveLayerID() {
	return _activeLayerID;
}

string KageScene::getLayerLabel() {
	return Layers[_activeLayerIndex]->getLabel();
}
void KageScene::setLayerLabel(string p_label) {
	Layers[_activeLayerIndex]->setLabel(p_label);
}

void KageScene::toggleLayerVisibility() {
	Layers[_activeLayerIndex]->toggleVisibility();
}
bool KageScene::isLayerVisible() {
	return Layers[_activeLayerIndex]->isVisible();
}
void KageScene::setLayerVisible(bool p_visible) {
	Layers[_activeLayerIndex]->setVisible(p_visible);
}

void KageScene::toggleLayerLock() {
	Layers[_activeLayerIndex]->toggleLock();
}
bool KageScene::isLayerLocked() {
	return Layers[_activeLayerIndex]->isLocked();
}
void KageScene::setLayerLocked(bool p_locked) {
	Layers[_activeLayerIndex]->setLock(p_locked);
}

string KageScene::getLabel() {
	return _label;
}
void KageScene::setLabel(string p_label) {
	_label = p_label;
}

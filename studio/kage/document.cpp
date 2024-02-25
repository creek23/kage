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

#include "document.h"
#include "../kage.h"

KageDocument::KageDocument() {
	init();
}

KageDocument::KageDocument(KageDocument &p_document) {
	if (this == &p_document) {
		return;
	}
	KageScene::LOADING_MODE = true;

	this->_saved            = p_document._saved;
	this->_activeSceneIndex = p_document._activeSceneIndex;
	this->_activeScene      = p_document._activeScene;
	this->sceneCtr          = p_document.sceneCtr;
	this->_activeSceneID    = p_document._activeSceneID;
	
		//TODO: copy asses for undo/redo of Assets -- Assets = p_document.Assets;
		//ClassPointer = new Class();
		//*ClassPointer = *(p_document.ClassPointer);
		this->removeAllScenes();
		for (unsigned int l_sceneIndex = 0; l_sceneIndex < p_document.Scenes.size(); ++l_sceneIndex) {
			KageScene::LOADING_MODE = true;
			this->addScene(p_document.Scenes[l_sceneIndex]->getLabel());
			*(this->Scenes[l_sceneIndex]) = *(p_document.Scenes[l_sceneIndex]);
		}

	this->sceneCtr          = p_document.sceneCtr;
	this->_activeSceneID    = p_document._activeSceneID;

	KageScene::LOADING_MODE = false;
}

KageDocument KageDocument::operator=(const KageDocument &p_document) {
	if (this == &p_document) {
		return *this;
	}
	KageScene::LOADING_MODE = true;

	this->_saved            = p_document._saved;
	this->_activeSceneIndex = p_document._activeSceneIndex;
	this->_activeScene      = p_document._activeScene;
	this->sceneCtr          = p_document.sceneCtr;
	this->_activeSceneID    = p_document._activeSceneID;
	
		//TODO: copy asses for undo/redo of Assets -- Assets = p_document.Assets;
		//ClassPointer = new Class();
		//*ClassPointer = *(p_document.ClassPointer);
		this->removeAllScenes();
		for (unsigned int l_sceneIndex = 0; l_sceneIndex < p_document.Scenes.size(); ++l_sceneIndex) {
			KageScene::LOADING_MODE = true;
			this->addScene(p_document.Scenes[l_sceneIndex]->getLabel());
			*(this->Scenes[l_sceneIndex]) = *(p_document.Scenes[l_sceneIndex]);
		}

	this->sceneCtr          = p_document.sceneCtr;
	this->_activeSceneID    = p_document._activeSceneID;
	
	KageScene::LOADING_MODE = false;
	
	return *this;
}

void KageDocument::init() {
	sceneCtr = 0;
	
	_activeSceneID = UINT_MAX;
	addScene(StringHelper::unsignedIntegerToString(sceneCtr+1));
	
	_activeSceneIndex = 0;
	_activeSceneID = Scenes[_activeSceneIndex]->getID();
	_activeScene      = _activeSceneIndex + 1;
}

KageDocument::~KageDocument() {
	//TODO: revisit
	//removeAllScenes();
	//Scenes.clear();
}

unsigned int KageDocument::openScene(std::string p_filepath) {
	++sceneCtr;
	
	_scenePtr = new KageScene(this, sceneCtr, p_filepath);
		_scenePtr->setLabel("openScene_p_sceneLabel");
		_scenePtr->setSelected(true);
		Scenes.push_back(_scenePtr);
	
	_activeSceneID = sceneCtr;
	
	return Scenes.size() - 1;
}

/**
 * @brief Kage Document will contain several scenes.  This will allow saving one Scene to be saved each time
 * 
 * @param p_sceneID which Scene do we want to save?
 * @return true 
 * @return false 
 */
bool KageDocument::saveScene(unsigned int p_sceneID) {
	if (p_sceneID > 0 && p_sceneID <= Scenes.size()) {
		return Scenes[p_sceneID]->save();
	}
	return false;
}
bool  KageDocument::openProject() {
	//TODO: call openScene
	return true;
}
bool  KageDocument::saveProject() {
	//TODO:
	// 1. save project info
	//std::cout << "<project " << Project.toString() << ">";
	// 2. save AssetData
	//		if image/video, filename, width, height
	//		if ksf, save as Scene with name, data
	// 3. save scene
	for (KageScene *l_scene : Scenes) {
		if (l_scene->isSaved() == false && l_scene->save() == false) {
			return false;
		}
	}
	std::cout << "</project>";
	_saved = true;
	return true;
}
bool  KageDocument::isSaved() {
	return _saved;
}
KageScene *KageDocument::getScene() {
	int l_sceneCount = Scenes.size();
	for (int i = 0; i < l_sceneCount; ++i) {
		if (Scenes[i]->getID() == _activeSceneID) {
			return Scenes[i];
		}
	}
	return Scenes[0]; //harder to handle NULL but needs to be reviewed why _activeSceneID is not matching the only Scene's ID
}
/**
 * @brief  returns activeSceneIndex+1
 * 
 * @return unsigned int activeSceneIndex+1
 * @sa setActiveScene
 */
unsigned int KageDocument::getCurrentScene() {
	int l_sceneCount = Scenes.size();
	for (int i = 0; i < l_sceneCount; ++i) {
		if (Scenes[i]->getID() == _activeSceneID) {
			return i+1;
		}
	}
	return UINT_MAX;
}
/**
 * @brief Used by KageSceneUI
 * 
 * @param p_currentScene p_currentScene-1 is index
 */
void KageDocument::setCurrentScene(unsigned int p_currentScene) {
	int l_sceneCount = Scenes.size();
	//unselect previously selected Scene
	if (_activeSceneIndex < l_sceneCount && Scenes[_activeSceneIndex]->getID() == _activeSceneID) {
		Scenes[_activeSceneIndex]->setSelected(false);
	} else {
		for (unsigned int i = 0; i < l_sceneCount; ++i) {
			if (Scenes[i]->getID() == _activeSceneID) {
				Scenes[i]->setSelected(false);
				break;
			}
		}
	}
	
	//select new scene
	if (p_currentScene > l_sceneCount) {
		p_currentScene = l_sceneCount;
	}
	if (p_currentScene < 1) {
		p_currentScene = 1;
	}
	_activeSceneIndex = p_currentScene-1;
	_activeSceneID = Scenes[_activeSceneIndex]->getID();
	_activeScene      = p_currentScene;
	Scenes[_activeSceneIndex]->setSelected(true);
}
			
unsigned int KageDocument::getActiveSceneID() {
	return _activeSceneID;
}
unsigned int KageDocument::getActiveLayerID() {
	try {
		return getScene()->getActiveLayerID();
	} catch (std::exception& e) {
		std::cout << "KageDocument::getActiveLayerID Exception : " << e.what() << std::endl;
		return false;
	}
}
unsigned int KageDocument::getActiveFrameID() {
	try {
		return getScene()->Layers[getActiveLayerID()]->getActiveFrameID();
	} catch (std::exception& e) {
		std::cout << "KageDocument::getActiveFrameID Exception : " << e.what() << std::endl;
		return false;
	}
}

/**
 * @brief Used by Kage when creating new Scene, or opening KSF file (even if via KAGE file)
 * 
 * @return Scene ID of newly created Scene
 */
unsigned int KageDocument::addScene(std::string p_sceneLabel) {
	++sceneCtr;
	//TODO: add scene above(or below) currently selected Scene
	_scenePtr = new KageScene(this, sceneCtr);
	Scenes.push_back(_scenePtr);
	if (_activeSceneID == UINT_MAX) {
		_activeSceneID = sceneCtr;
	}
	setCurrentScene(Scenes.size());
	try {
		getScene()->setLabel(p_sceneLabel);
	} catch (std::exception& e) {
		std::cout << "KageDocument::addScene Exception : " << e.what() << std::endl;
	}
	
	return sceneCtr;
}
bool KageDocument::removeAllScenes() {
	int l_sceneCount = Scenes.size();
	for (int i = l_sceneCount-1; i >= 0; --i) {
		if (removeSceneAt(i) == false) {
			std::cout << "KageDocument::removeAllScenes \t" << i << " Scenes.size() " << Scenes.size() << std::endl;
		}
	}
	return true;
}
/**
 * @brief Used by KageDocument; This will gracefully remove all Layers of this Scene along with its Frames
 * 
 * @param p_sceneIndex 
 * @return true 
 * @return false 
 * @sa KageDocument::removeAllScenes
 */
bool KageDocument::removeSceneAt(unsigned int p_sceneIndex) {
	if (p_sceneIndex >= 0 && p_sceneIndex < Scenes.size()) {
		int l_layerCount = Scenes[p_sceneIndex]->Layers.size();
		bool l_return = false;
		/* for each layer in layers
			for each frame in frames
				remove frame
			remove layer
		remove scene */
		for (int i = l_layerCount-1; i >= 0; --i) {
			l_return = Scenes[p_sceneIndex]->removeLayerAt(i);
			if (l_return == false) {
				break;
			}
		}
		if (l_return) {
			Scenes.erase( Scenes.begin() + p_sceneIndex );
		}
		return true;
	}

	return false;
}

unsigned int KageDocument::frameCount() {
	try {
		return getScene()->frameCount();
	} catch (std::exception& e) {
		std::cout << "KageDocument::frameCount Exception : " << e.what() << std::endl;
		return false;
	}
}

void KageDocument::setSceneLayerCurrentFrame(unsigned int p_frame) {
	try {
		getScene()->setLayerCurrentFrame(p_frame, false);
	} catch (std::exception& e) {
		std::cout << "KageDocument::setSceneLayerCurrentFrame Exception : " << e.what() << std::endl;
	}
}

std::string KageDocument::getLayerLabel() {
	try {
		return getScene()->getLayerLabel();
	} catch (std::exception& e) {
		std::cout << "KageDocument::getLayerLabel Exception : " << e.what() << std::endl;
		return "";
	}
}
void KageDocument::setLayerLabel(std::string p_label) {
	try {
		getScene()->setLayerLabel(p_label);
	} catch (std::exception& e) {
		std::cout << "KageDocument::setLayerLabel Exception : " << e.what() << std::endl;
	}
}

void KageDocument::toggleLayerVisibility() {
	try {
		getScene()->toggleLayerVisibility();
	} catch (std::exception& e) {
		std::cout << "KageDocument::toggleLayerVisibility Exception : " << e.what() << std::endl;
	}
}
bool KageDocument::isLayerVisible() {
	try {
		return getScene()->isLayerVisible();
	} catch (std::exception& e) {
		std::cout << "KageDocument::isLayerVisible Exception : " << e.what() << std::endl;
		return false;
	}
}
void KageDocument::setLayerVisible(bool p_visible) {
	try {
		getScene()->setLayerVisible(p_visible);
	} catch (std::exception& e) {
		std::cout << "KageDocument::setLayerVisible Exception : " << e.what() << std::endl;
	}
}

void KageDocument::toggleLayerLock() {
	try {
		getScene()->toggleLayerLock();
	} catch (std::exception& e) {
		std::cout << "KageDocument::toggleLayerLock Exception : " << e.what() << std::endl;
	}
}
bool KageDocument::isLayerLocked() {
	try {
		return getScene()->isLayerLocked();
	} catch (std::exception& e) {
		std::cout << "KageDocument::isLayerLocked Exception : " << e.what() << std::endl;
		return false;
	}
}
void KageDocument::setLayerLocked(bool p_lock) {
	try {
		getScene()->setLayerLocked(p_lock);
	} catch (std::exception& e) {
		std::cout << "KageDocument::setLayerLocked Exception : " << e.what() << std::endl;
	}
}

/**
 * NOTE: Scenes are organized as index 0 as BOTTOM and last index is TOP
 * \sa moveSceneDown() moveSceneToBottom() moveSceneUp()
 * \return True if successfully moved to bottom
 */
bool KageDocument::moveSceneToTop() {
	bool l_return = false;
	if (_activeSceneIndex < Scenes.size() && Scenes[_activeSceneIndex]->getID() == _activeSceneID) {
		while (_activeSceneIndex < Scenes.size()-1) {
			l_return = moveSceneUp();
		}
		return l_return;
	} else {
		for (unsigned int i = 0; i < Scenes.size(); ++i) {
			if (Scenes[i]->getID() == _activeSceneID) {
				_activeSceneIndex = i;
				while (_activeSceneIndex < Scenes.size()-1) {
					l_return = moveSceneUp();
				}
				return l_return;
			}
		}
	}
	return false;
}
/**
 * NOTE: Scenes are organized as index 0 as BOTTOM and last index is TOP
 * \sa moveSceneDown() moveSceneToBottom() moveSceneToTop()
 * \return True if successfully moved to bottom
 */
bool KageDocument::moveSceneUp() {
	if (_activeSceneIndex < Scenes.size() && Scenes[_activeSceneIndex]->getID() == _activeSceneID) {
		if (_activeSceneIndex < Scenes.size()-1) {
			std::swap(Scenes[_activeSceneIndex], Scenes[_activeSceneIndex+1]);
			_activeSceneIndex = _activeSceneIndex+1;
			return true;
		}
	} else {
		for (unsigned int i = 0; i < Scenes.size(); ++i) {
			if (Scenes[i]->getID() == _activeSceneID) {
				_activeSceneIndex = i;
				if (_activeSceneIndex < Scenes.size()-1) {
					std::swap(Scenes[_activeSceneIndex], Scenes[_activeSceneIndex+1]);
					_activeSceneIndex = _activeSceneIndex+1;
					return true;
				}
				break;
			}
		}
	}
	return false;
}
/**
 * NOTE: Scenes are organized as index 0 as BOTTOM and last index is TOP
 * \sa moveToBottom() moveToTop() moveUp()
 * \return True if successfully moved to bottom
 */
bool KageDocument::moveSceneDown() {
	if (_activeSceneIndex < Scenes.size() && Scenes[_activeSceneIndex]->getID() == _activeSceneID) {
		if (_activeSceneIndex > 0) {
			std::swap(Scenes[_activeSceneIndex], Scenes[_activeSceneIndex-1]);
			--_activeSceneIndex;
			return true;
		}
	} else {
		for (unsigned int i = 0; i < Scenes.size(); ++i) {
			if (Scenes[i]->getID() == _activeSceneID) {
				_activeSceneIndex = i;
				if (_activeSceneIndex > 0) {
					std::swap(Scenes[_activeSceneIndex], Scenes[_activeSceneIndex-1]);
					--_activeSceneIndex;
					return true;
				}
				break;
			}
		}
	}
	return false;
}
/**
 * NOTE: Scenes are organized as index 0 as BOTTOM and last index is TOP
 * \sa moveSceneDown() moveSceneToTop() moveSceneUp()
 * \return True if successfully moved to bottom
 */
bool KageDocument::moveSceneToBottom() {
	bool l_return = false;
	if (_activeSceneIndex < Scenes.size() && Scenes[_activeSceneIndex]->getID() == _activeSceneID) {
		while (_activeSceneIndex > 0) {
			l_return = moveSceneDown();
		}
		return l_return;
	} else {
		for (unsigned int i = 0; i < Scenes.size(); ++i) {
			if (Scenes[i]->getID() == _activeSceneID) {
				_activeSceneIndex = i;
				while (_activeSceneIndex > 0) {
					l_return = moveSceneDown();
				}
				return l_return;
			}
		}
	}
	return false;
}
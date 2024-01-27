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

KageDocument::KageDocument(Kage *p_kage) {
	Project = KageProject();

	init(p_kage);
}
KageDocument::KageDocument(Kage *p_kage, KageProject p_project) {
	setProjectInformation(p_project);
	
	init(p_kage);
}

void KageDocument::init(Kage *p_kage) {
	sceneCtr = 0;
	_kage = p_kage;
	
	_activeSceneID = UINT_MAX;
	addScene(StringHelper::unsignedIntegerToString(sceneCtr+1));
	
	_activeSceneIndex = 0;
	setActiveSceneID(Scenes[_activeSceneIndex]->getID());
	_activeScene      = _activeSceneIndex + 1;
}

KageDocument::~KageDocument() {
	Scenes.clear();
	_kage = NULL;
}

void KageDocument::setProjectInformation(KageProject p_project) {
	Project._name            = p_project._name;
	Project._width           = p_project._width;
	Project._height          = p_project._height;
	Project._backgroundColor = p_project._backgroundColor;
	Project._fps             = p_project._fps;
}

unsigned int KageDocument::openScene(string p_filepath) {
	++sceneCtr;
	
	KageScene *l_scene = new KageScene(this, sceneCtr, p_filepath);// (this, sceneCtr);
		l_scene->setLabel("openScene_p_sceneLabel");
		l_scene->setSelected(true);
		Scenes.push_back(l_scene);//new KageScene(this, sceneCtr));
	
	setActiveSceneID(sceneCtr);
	
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
	std::cout << "<project " << Project.toString() << ">";
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
	return NULL;
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
	std::cout << "\nKageDocument::setCurrentScene l_sceneCount  " << l_sceneCount << std::endl;
	std::cout << "KageDocument::setCurrentScene p_currentScene  " << p_currentScene << std::endl;
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
	setActiveSceneID(Scenes[_activeSceneIndex]->getID());
	_activeScene      = p_currentScene;
	Scenes[_activeSceneIndex]->setSelected(true);
}
			
unsigned int KageDocument::getActiveSceneID() {
	return _activeSceneID;
}
void KageDocument::setActiveSceneID(unsigned int p_activeSceneID) {
	std::cout << "\nKageDocument::setActiveSceneID _activeSceneID " << _activeSceneID << " = " << p_activeSceneID << std::endl << std::endl;
	_activeSceneID = p_activeSceneID;
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
unsigned int KageDocument::addScene(string p_sceneLabel) {
	++sceneCtr;
	//TODO: add scene above(or below) currently selected Scene
	Scenes.push_back(new KageScene(this, sceneCtr));
	if (_activeSceneID == UINT_MAX) {
		setActiveSceneID(sceneCtr);
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
//=======================================================================================
KageProject::KageProject() {
	_name            = "Untitled";
	_width           = 960.0f;
	_height          = 720.0f;
	_backgroundColor = ColorData(255, 255, 255);
	_fps             = 12;
}

KageProject::~KageProject() {
	//
}

string KageProject::toString() {
	return    "name=\"" + _name
		+ "\" width=\"" + StringHelper::doubleToString(_width)
		+ "\" height=\"" + StringHelper::doubleToString(_height)
		+ "\" background=\"rgb(" + StringHelper::integerToString(_backgroundColor.getR()) + ", " + StringHelper::integerToString(_backgroundColor.getG()) + ", " + StringHelper::integerToString(_backgroundColor.getB()) + ")\" " +
		+ "\" fps=\"" + StringHelper::unsignedIntegerToString(_fps) + "\"";
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

/** For use of Kage when User clicked Layer->Rename
 * \sa renameLayer(KageLayer)
 */
void KageDocument::renameLayer() {
	try {
		string l_layerLabel = getScene()->getLayerLabel();
		
		LabelRenameDialog* pDialog = new LabelRenameDialog(*_kage, l_layerLabel);
			pDialog->run();
		getScene()->setLayerLabel(pDialog->getLabel());
		delete pDialog;
	} catch (std::exception& e) {
		std::cout << "KageDocument::renameLayer Exception : " << e.what() << std::endl;
	}
}
string KageDocument::getLayerLabel() {
	try {
		return getScene()->getLayerLabel();
	} catch (std::exception& e) {
		std::cout << "KageDocument::getLayerLabel Exception : " << e.what() << std::endl;
		return "";
	}
}
void KageDocument::setLayerLabel(string p_label) {
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

/** For use of Kage when User clicked Scene->Rename
 * \sa renameScene(KageScenesUI)
 */
void KageDocument::renameScene() {
	try {
		string l_sceneLabel = getScene()->getLabel();
		
		LabelRenameDialog* pDialog = new LabelRenameDialog(*_kage, l_sceneLabel);
			pDialog->run();
		getScene()->setLabel(pDialog->getLabel());
		delete pDialog;
	} catch (std::exception& e) {
		std::cout << "KageDocument::renameScene Exception : " << e.what() << std::endl;
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
			swap(Scenes[_activeSceneIndex], Scenes[_activeSceneIndex+1]);
			_activeSceneIndex = _activeSceneIndex+1;
			return true;
		}
	} else {
		for (unsigned int i = 0; i < Scenes.size(); ++i) {
			if (Scenes[i]->getID() == _activeSceneID) {
				_activeSceneIndex = i;
				if (_activeSceneIndex < Scenes.size()-1) {
					swap(Scenes[_activeSceneIndex], Scenes[_activeSceneIndex+1]);
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
			swap(Scenes[_activeSceneIndex], Scenes[_activeSceneIndex-1]);
			--_activeSceneIndex;
			return true;
		}
	} else {
		for (unsigned int i = 0; i < Scenes.size(); ++i) {
			if (Scenes[i]->getID() == _activeSceneID) {
				_activeSceneIndex = i;
				if (_activeSceneIndex > 0) {
					swap(Scenes[_activeSceneIndex], Scenes[_activeSceneIndex-1]);
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
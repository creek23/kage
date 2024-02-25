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

KageProject::KageProject(Kage *p_kage) {
	init(p_kage);
}
KageProject KageProject::operator=(const KageProject &p_project) {
	KageDocument::operator= (p_project);
	
	if (this == &p_project) {
		return *this;
	}

	this->_name            = p_project._name;
	this->_width           = p_project._width;
	this->_height          = p_project._height;
	this->_backgroundColor = p_project._backgroundColor;
	this->_fps             = p_project._fps;
	this->_kage            = p_project._kage;
	this->sceneCtr         = p_project.sceneCtr;
	
	return *this;
}
KageProject KageProject::operator=(const KageDocument &p_document) {
	if (this == &p_document) {
		return *this;
	}
	
	KageDocument::operator= (p_document);
	
	return *this;
}

void KageProject::init(Kage *p_kage) {
	_name            = "Untitled";
	_width           = 960.0f;
	_height          = 720.0f;
	_backgroundColor = ColorData(255, 255, 255);
	_fps             = 12;

	sceneCtr = 0;
	_kage = p_kage;
	
	_activeSceneID = UINT_MAX;
	addScene(StringHelper::unsignedIntegerToString(sceneCtr+1));
	
	_activeSceneIndex = 0;
	_activeSceneID = Scenes[_activeSceneIndex]->getID();
	_activeScene      = _activeSceneIndex + 1;
}

KageProject::~KageProject() {
	//TODO: revisit
	//_kage = NULL;
}

std::string KageProject::toString() {
	return    "name=\"" + _name
		+ "\" width=\"" + StringHelper::doubleToString(_width)
		+ "\" height=\"" + StringHelper::doubleToString(_height)
		+ "\" background=\"rgb(" + StringHelper::integerToString(_backgroundColor.getR()) + ", " + StringHelper::integerToString(_backgroundColor.getG()) + ", " + StringHelper::integerToString(_backgroundColor.getB()) + ")\" " +
		+ "\" fps=\"" + StringHelper::unsignedIntegerToString(_fps) + "\"";
}

/** For use of Kage when User clicked Layer->Rename
 * \sa renameLayer(KageLayer)
 */
void KageProject::renameLayer() {
	try {
		std::string l_layerLabel = getScene()->getLayerLabel();
		
		LabelRenameDialog* pDialog = new LabelRenameDialog(*_kage, l_layerLabel);
			pDialog->run();
		getScene()->setLayerLabel(pDialog->getLabel());
		delete pDialog;
	} catch (std::exception& e) {
		std::cout << "KageProject::renameLayer Exception : " << e.what() << std::endl;
	}
}

/** For use of Kage when User clicked Scene->Rename
 * \sa renameScene(KageScenesUI)
 */
void KageProject::renameScene() {
	try {
		std::string l_sceneLabel = getScene()->getLabel();
		
		LabelRenameDialog* pDialog = new LabelRenameDialog(*_kage, l_sceneLabel);
			pDialog->run();
		getScene()->setLabel(pDialog->getLabel());
		delete pDialog;
	} catch (std::exception& e) {
		std::cout << "KageProject::renameScene Exception : " << e.what() << std::endl;
	}
}
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

#include "document.h"
#include "../kage.h"

unsigned int KageDocument::ACTIVE_SCENE = 0;

KageDocument::KageDocument(Kage *p_kage) {
	Project = KageProject();

	init(p_kage);
}
KageDocument::KageDocument(Kage *p_kage, KageProject p_project) {
	setProjectInformation(p_project);
	
	init(p_kage);
}

void KageDocument::init(Kage *p_kage) {
	_kage = p_kage;
	setActiveSceneID(0);
	KageScene p_scene(this);
	Scenes.push_back(p_scene);
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
	KageScene p_scene(this, p_filepath);
	Scenes.push_back(p_scene);
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
		return Scenes[p_sceneID].save();
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
	cout << "<project " << Project.toString() << ">";
	// 2. save AssetData
	//		if image/video, filename, width, height
	//		if ksf, save as Scene with name, data
	// 3. save scene
	for (KageScene l_scene : Scenes) {
		if (l_scene.isSaved() == false && l_scene.save() == false) {
			return false;
		}
	}
	cout << "</project>";
	_saved = true;
	return true;
}
bool  KageDocument::isSaved() {
	return _saved;
}

unsigned int KageDocument::getActiveSceneID() {
	return _activeSceneID;
}
void KageDocument::setActiveSceneID(unsigned int p_activeSceneID) {
	_activeSceneID = p_activeSceneID;
}
unsigned int KageDocument::getActiveLayerID() {
	return Scenes[getActiveSceneID()].getActiveLayerID();
}
unsigned int KageDocument::getActiveFrameID() {
	return Scenes[getActiveSceneID()].Layers[getActiveLayerID()]->getActiveFrameID();
}
//=======================================================================================
KageProject::KageProject() {
	_name            = "Untitled";
	_width           = 800.0f;
	_height          = 600.0f;
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
	return Scenes[getActiveSceneID()].frameCount();
}

void KageDocument::setCurrentFrame(unsigned int p_frame) {
	Scenes[getActiveSceneID()].setCurrentFrame(p_frame);
}
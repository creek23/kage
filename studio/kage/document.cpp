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
//#include "../kage.h"
//#include "stage.h"

KageDocument::KageDocument() {
	KageProject l_project;
		l_project._name            = "Untitled";
		l_project._width           = 800.0f;
		l_project._height          = 600.0f;
		l_project._backgroundColor = ColorData(255, 255, 255);
		l_project._fps             = 12;
	setProjectInformation(l_project);
}
KageDocument::KageDocument(KageProject p_project) {
	setProjectInformation(p_project);
}

void KageDocument::setProjectInformation(KageProject p_project) {
	Project._name            = p_project._name;
	Project._width           = p_project._width;
	Project._height          = p_project._height;
	Project._backgroundColor = p_project._backgroundColor;
	Project._fps             = p_project._fps;
}

KageDocument::~KageDocument() {
	//vectorData.clear();
}

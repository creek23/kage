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

#include "unre.h"

UnRe::UnRe() {
	_kageDocument = NULL;
	clear();
}

UnRe::~UnRe() {
	clear();
}

void UnRe::clear() {
	_stackIndex = UINT_MAX;
	_undoStack.clear();
}

void UnRe::stackDocument(KageDocument p_document) {
	unsigned int l_size = _undoStack.size();
	if (_stackIndex < l_size - 1) {// && _undoStack.size() > 0) {
		_undoStack[_stackIndex+1]->removeAllScenes();
		_undoStack.erase(_undoStack.begin() + _stackIndex+1, _undoStack.end());
	}
	
	_kageDocument = new KageDocument();
	*_kageDocument = p_document;
	_undoStack.push_back(_kageDocument);
	std::cout << "UNDO SIZE: " << _undoStack.size() << std::endl;
	
	++_stackIndex;
}

KageDocument UnRe::undoDocument() {
	if (_stackIndex > 0 && _stackIndex != -1) {
		--_stackIndex;
		return *(_undoStack[_stackIndex]);
	}
	
	return *(_undoStack[0]);
}

KageDocument UnRe::redoDocument() {
	unsigned int l_size = _undoStack.size();
	if (_stackIndex < l_size-1 && _stackIndex != -1) {
		++_stackIndex;
		return *(_undoStack[_stackIndex]);
	} else if (_stackIndex == -1 && l_size > 0) {
		++_stackIndex;
		return *(_undoStack[_stackIndex]);
	}
	
	if (l_size > 0) {
		return *(_undoStack[l_size-1]);
	}
	return *(_undoStack[0]);
}


/**
 * While undo() pops a KageDocument from a stack, this just gives a preview
 * of the top KageDocument on the stack.
 * \return latest state pushed in the stack
 * \sa undoDocument()
 */
KageDocument UnRe::previewUndoDocument() {
	if (_stackIndex > 0 && _stackIndex != -1) {
		return *(_undoStack[_stackIndex]);
	}
	
	return *(_undoStack[0]);
}
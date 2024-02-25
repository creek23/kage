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

#ifndef GTKMM_KAGE_MANAGER_UNRE_UNRE_H
	#define GTKMM_KAGE_MANAGER_UNRE_UNRE_H
	
	#include <vector>
	#include <iostream>
	
	#include "../data/vectordata.h"
	#include "../data/point.h"
	
	#include "../document.h"
	
	class Kage;//forward declaration
	
	class UnRe {
		public:
			UnRe();
			virtual ~UnRe();
			
			void stackDocument(KageDocument p_document);
			KageDocument undoDocument();
			KageDocument redoDocument();
			KageDocument previewUndoDocument();
			
			void clear();
		protected:
			unsigned int _stackIndex;
			KageDocument *_kageDocument;
			std::vector<KageDocument*> _undoStack;
	};
#endif //GTKMM_KAGE_MANAGER_UNRE_UNRE_H

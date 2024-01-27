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

#include "kagedo.h"

KageDo::KageDo() {
	_scene = UINT_MAX;
	_layer = UINT_MAX;
	_frame = UINT_MAX;
}
KageDo::KageDo(unsigned int p_scene, unsigned int p_layer, unsigned int p_frame) {
	_scene = p_scene;
	_layer = p_layer;
	_frame = p_frame;
}

KageDo::~KageDo() {
	_vectorData.clear();
}

void KageDo::setVectorData(vector<VectorData> p_vectorData) {
	_vectorData = p_vectorData;
}

vector<VectorData> KageDo::getVectorData() {
	return _vectorData;
}

void KageDo::clear() {
	_vectorData.clear();
}

void KageDo::push(KageDo p_vectorsData) {
	std::vector<VectorData> l_v = p_vectorsData.getVectorData();
	int isrc = l_v.size();
	
	for (int i = 0; i < isrc; ++i) {
		_vectorData.push_back(l_v[i]);
	}
}

void KageDo::add(VectorData::type p_type, ColorData p_fill, StrokeColorData p_stroke) {
	VectorData l_vectorData;
	
		l_vectorData.vectorType = p_type;
		l_vectorData.stroke = p_stroke;
		l_vectorData.fillColor = p_fill;
	
	_vectorData.push_back(l_vectorData);
}

KageDo KageDo::clone() {
	std::vector<VectorData> l_vectorData;
	unsigned int count = _vectorData.size();
		for (unsigned int i = 0; i < count; ++i) {
			l_vectorData.push_back(_vectorData[i].clone());
		}
	KageDo l_kageDo(_scene, _layer, _frame);
	l_kageDo.setVectorData(l_vectorData);
	
	return l_kageDo;
}

bool KageDo::isEmpty() {
	return (_vectorData.size() == 0);
}

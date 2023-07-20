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

#include "frame.h"
#include "layer.h"
#include "../../kage.h"

KageFrame::KageFrame(KageLayer *p_layer, unsigned p_layerID, unsigned int p_frameID) :
		vectorsData() {
	_layer = p_layer;
	layerID = p_layerID;
	frameID = p_frameID;
	setSelected(false);
	setCurrent(false);
	forceSetTween(false);
	setNull(false);
	setExtension(KageFrame::EXTENSION_NOT);
}

KageFrame::~KageFrame() {
	vectorsData.clear();
}

void KageFrame::setSelected(bool p_selected) {
	_selected = p_selected;
}
bool KageFrame::isSelected() {
	return _selected;
}

void KageFrame::setExtension(KageFrame::extension p_extension) {
	_extension = p_extension;
}
KageFrame::extension KageFrame::getExtension() {
	return _extension;
}

void KageFrame::setNull(bool p_null) {
	_null = p_null;
}
bool KageFrame::isNull() {
	return _null;
}
void KageFrame::forceSetTween(unsigned int p_tween) {
	_tweenX = p_tween/10;
	_tweenY = p_tween - (_tweenX*10);
	//cout << " KageFrame::forceSetTween() " << _tweenX << " " << _tweenY << " @" << layerID << " " << frameID << endl;
}
void KageFrame::setTween(unsigned int p_tween) {
	if (_null == true) {
		return;
	}
	cout << " KageFrame::setTween() @" << layerID << " " << frameID << " " << p_tween << endl;
	if (       _extension == KageFrame::EXTENSION_NOT) {
		forceSetTween(p_tween);
	} else if (_extension == KageFrame::EXTENSION_START) {
		forceSetTween(p_tween);
		_layer->setExtendedFrameTween(frameID, p_tween);
	} else {
		_layer->setPreviousFrameTween(frameID, p_tween);
	}
}
unsigned int KageFrame::getTween() {
//	cout << " KageFrame::getTween() " << _tweenX << " " << _tweenY << " | " << ((_tweenX * 10) + _tweenY) << endl;
	return (_tweenX * 10) + _tweenY;
}

void KageFrame::setCurrent(bool p_current) {
	_current = p_current;
}
bool KageFrame::isCurrent() {
	return _current;
}

VectorDataManager KageFrame::getFrameData() {
	if (_null == true) {
		cout << "KageFrame::getFrameData is returning empty" << endl;
		VectorDataManager l_nullReturn;
		return l_nullReturn;
	}
	if (       _extension == KageFrame::EXTENSION_NOT
			|| _extension == KageFrame::EXTENSION_START) {
		return vectorsData;
	} else {
		return _layer->getPreviousFrameData(frameID);
	}
}
void KageFrame::setFrameData(VectorDataManager p_vectorsData) {
	if (_null == true) {
		setNull(false);
	}
	if (       _extension == KageFrame::EXTENSION_NOT
			|| _extension == KageFrame::EXTENSION_START) {
		vectorsData = p_vectorsData;
	} else {
		_layer->setFrameDataToPreviousFrame(p_vectorsData, frameID);
	}
}

bool KageFrame::isEmpty() {
	if (       _extension == KageFrame::EXTENSION_NOT
			|| _extension == KageFrame::EXTENSION_START) {
		return (vectorsData.getVectorData().size() == 0);
	} else {
		return (_layer->getPreviousFrameData(frameID).getVectorData().size() <= 1);
	}
}

vector<unsigned int> KageFrame::raiseSelectedShape(vector<unsigned int> p_selectedShapes) {
	if (       _extension == KageFrame::EXTENSION_NOT
			|| _extension == KageFrame::EXTENSION_START) {
		return vectorsData.raiseSelectedShape(p_selectedShapes);
	} else {
		VectorDataManager l_vectorData = _layer->getPreviousFrameData(frameID);
		vector<unsigned int> l_selectedShapes = l_vectorData.raiseSelectedShape(p_selectedShapes);
		setFrameData(l_vectorData);
		return l_selectedShapes;
	}
}
vector<unsigned int> KageFrame::lowerSelectedShape(vector<unsigned int> p_selectedShapes) {
	if (       _extension == KageFrame::EXTENSION_NOT
			|| _extension == KageFrame::EXTENSION_START) {
		return vectorsData.lowerSelectedShape(p_selectedShapes);
	} else {
		VectorDataManager l_vectorData = _layer->getPreviousFrameData(frameID);
		vector<unsigned int> l_selectedShapes = l_vectorData.lowerSelectedShape(p_selectedShapes);
		setFrameData(l_vectorData);
		return l_selectedShapes;
	}
}
vector<unsigned int> KageFrame::raiseToTopSelectedShape(vector<unsigned int> p_selectedShapes) {
	if (       _extension == KageFrame::EXTENSION_NOT
			|| _extension == KageFrame::EXTENSION_START) {
		return vectorsData.raiseToTopSelectedShape(p_selectedShapes);
	} else {
		VectorDataManager l_vectorData = _layer->getPreviousFrameData(frameID);
		vector<unsigned int> l_selectedShapes = l_vectorData.raiseToTopSelectedShape(p_selectedShapes);
		setFrameData(l_vectorData);
		return l_selectedShapes;
	}
}
vector<unsigned int> KageFrame::lowerToBottomSelectedShape(vector<unsigned int> p_selectedShapes) {
	if (       _extension == KageFrame::EXTENSION_NOT
			|| _extension == KageFrame::EXTENSION_START) {
		return vectorsData.lowerToBottomSelectedShape(p_selectedShapes);
	} else {
		VectorDataManager l_vectorData = _layer->getPreviousFrameData(frameID);
		vector<unsigned int> l_selectedShapes = l_vectorData.lowerToBottomSelectedShape(p_selectedShapes);
		setFrameData(l_vectorData);
		return l_selectedShapes;
	}
}

vector<unsigned int> KageFrame::groupSelectedShapes(vector<unsigned int> p_selectedShapes) {
	if (       _extension == KageFrame::EXTENSION_NOT
			|| _extension == KageFrame::EXTENSION_START) {
		return vectorsData.groupSelectedShapes(p_selectedShapes);
	} else {
		VectorDataManager l_vectorData = _layer->getPreviousFrameData(frameID);
		vector<unsigned int> l_selectedShapes = l_vectorData.groupSelectedShapes(p_selectedShapes);
		setFrameData(l_vectorData);
		return l_selectedShapes;
	}
}
vector<unsigned int> KageFrame::ungroupSelectedShapes(vector<unsigned int> p_selectedShapes) {
	if (       _extension == KageFrame::EXTENSION_NOT
			|| _extension == KageFrame::EXTENSION_START) {
		return vectorsData.ungroupSelectedShapes(p_selectedShapes);
	} else {
		VectorDataManager l_vectorData = _layer->getPreviousFrameData(frameID);
		vector<unsigned int> l_selectedShapes = l_vectorData.ungroupSelectedShapes(p_selectedShapes);
		setFrameData(l_vectorData);
		return l_selectedShapes;
	}
}

vector<unsigned int> KageFrame::duplicateShapes(vector<unsigned int> p_selectedShapes) {
	if (       _extension == KageFrame::EXTENSION_NOT
			|| _extension == KageFrame::EXTENSION_START) {
		return vectorsData.duplicateShapes(p_selectedShapes);
	} else {
		VectorDataManager l_vectorData = _layer->getPreviousFrameData(frameID);
		vector<unsigned int> l_selectedShapes = l_vectorData.duplicateShapes(p_selectedShapes);
		setFrameData(l_vectorData);
		return l_selectedShapes;
	}
}

bool KageFrame::flipHorizontalSelectedShape(vector<unsigned int> p_selectedShapes) {
	if (       _extension == KageFrame::EXTENSION_NOT
			|| _extension == KageFrame::EXTENSION_START) {
		return vectorsData.flipHorizontalSelectedShape(p_selectedShapes);
	} else {
		VectorDataManager l_vectorData = _layer->getPreviousFrameData(frameID);
		bool l_selectedShapes = l_vectorData.flipHorizontalSelectedShape(p_selectedShapes);
		setFrameData(l_vectorData);
		return l_selectedShapes;
	}
}
bool KageFrame::flipVerticalSelectedShape(vector<unsigned int> p_selectedShapes) {
	if (       _extension == KageFrame::EXTENSION_NOT
			|| _extension == KageFrame::EXTENSION_START) {
		return vectorsData.flipVerticalSelectedShape(p_selectedShapes);
	} else {
		VectorDataManager l_vectorData = _layer->getPreviousFrameData(frameID);
		bool l_selectedShapes = l_vectorData.flipVerticalSelectedShape(p_selectedShapes);
		setFrameData(l_vectorData);
		return l_selectedShapes;
	}
}
bool KageFrame::recenterRotationPoint(vector<unsigned int> p_selectedShapes) {
	if (       _extension == KageFrame::EXTENSION_NOT
			|| _extension == KageFrame::EXTENSION_START) {
		return vectorsData.recenterRotationPoint(p_selectedShapes);
	} else {
		VectorDataManager l_vectorData = _layer->getPreviousFrameData(frameID);
		bool l_selectedShapes = l_vectorData.recenterRotationPoint(p_selectedShapes);
		setFrameData(l_vectorData);
		return l_selectedShapes;
	}
}
void KageFrame::addDataToFrame(VectorDataManager v) {
	if (       _extension == KageFrame::EXTENSION_NOT
			|| _extension == KageFrame::EXTENSION_START) {
		if (_null == true) {
			setNull(false);
		}
		vectorsData.push(v);
	} else {
		_layer->addDataToPreviousFrame(v, frameID);
	}
}
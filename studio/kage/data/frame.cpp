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

#include "frame.h"
#include "layer.h"
#include "../../kage.h"

KageFrame::KageFrame(KageLayer *p_layer, unsigned int p_frameID) :
		vectorsData() {
	_layer = p_layer;
	frameID = p_frameID;
	setSelected(false);
	setCurrent(false);
	forceSetTween(false);
	setNull(false);
	setExtension(KageFrame::extension::EXTENSION_NOT);
}

KageFrame KageFrame::operator=(const KageFrame &p_frame) {
	if (this == &p_frame) {
		return *this;
	}
	KageScene::LOADING_MODE = true;

	this->data       = p_frame.data;
	this->frameID    = p_frame.frameID;
	this->_null      = p_frame._null;
	this->_selected  = p_frame._selected;
	this->_current   = p_frame._current;
	this->_tweenX    = p_frame._tweenX;
	this->_tweenY    = p_frame._tweenY;
	this->_extension = p_frame._extension;
	
	this->vectorsData._selectedNodes.clear();
	this->vectorsData._selectedNodes = p_frame.vectorsData._selectedNodes;

	this->vectorsData._selectedShapes.clear();
	this->vectorsData._selectedShapes = p_frame.vectorsData._selectedShapes;
	this->vectorsData.vectorData.clear();
	//std::cout << "\t\t\t\tKageFrame KageFrame::operator= vectorData || " << p_frame.vectorsData.vectorData.size() << std::endl;
	for (unsigned int l_vectorIndex = 0; l_vectorIndex < p_frame.vectorsData.vectorData.size(); ++l_vectorIndex) {
		this->vectorsData.vectorData.push_back(
				p_frame.vectorsData.vectorData[l_vectorIndex]
			);
		//TODO: make sure X/Y of all vector data are copied over
		//std::cout << "\t\t\t\tKageFrame KageFrame::operator= vectorData || " << this->vectorsData.vectorData.size() << std::endl;
	}
	KageScene::LOADING_MODE = false;
	//std::cout << "\t\t\tKageFrame KageFrame::operator= >> " << this << "\t" << this->frameID << std::endl;
	return *this;
}

KageFrame::~KageFrame() {
	//TODO: revisit
//	vectorsData.clear();
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
	//std::cout << " KageFrame::forceSetTween() " << _tweenX << " " << _tweenY << " @ " << frameID << std::endl;
}
void KageFrame::setTween(unsigned int p_tween) {
	if (_null == true) {
		return;
	}
	std::cout << " KageFrame::setTween() @ " << frameID << " " << p_tween << std::endl;
	if (       _extension == KageFrame::extension::EXTENSION_NOT) {
		forceSetTween(p_tween);
	} else if (_extension == KageFrame::extension::EXTENSION_START) {
		forceSetTween(p_tween);
		_layer->setExtendedFrameTween(frameID, p_tween);
	} else {
		_layer->setPreviousFrameTween(frameID, p_tween);
	}
}
unsigned int KageFrame::getTween() {
//	std::cout << " KageFrame::getTween() " << _tweenX << " " << _tweenY << " | " << ((_tweenX * 10) + _tweenY) << std::endl;
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
		VectorDataManager l_nullReturn;
		return l_nullReturn;
	}
	
	if (       _extension == KageFrame::extension::EXTENSION_NOT
			|| _extension == KageFrame::extension::EXTENSION_START) {
		return vectorsData;
	} else {
		return _layer->getPreviousFrameData(frameID);
	}
}
void KageFrame::setFrameData(VectorDataManager p_vectorsData) {
	if (_null == true) {
		setNull(false);
	}
	if (       _extension == KageFrame::extension::EXTENSION_NOT
			|| _extension == KageFrame::extension::EXTENSION_START) {
		vectorsData.setVectorData(p_vectorsData.getVectorData());
	} else {
		_layer->setFrameDataToPreviousFrame(p_vectorsData, frameID);
	}
}

bool KageFrame::isEmpty() {
	if (       _extension == KageFrame::extension::EXTENSION_NOT
			|| _extension == KageFrame::extension::EXTENSION_START) {
		return (vectorsData.getVectorData().size() == 0);
	} else {
		return (_layer->getPreviousFrameData(frameID).getVectorData().size() <= 1);
	}
}

std::vector<unsigned int> KageFrame::raiseSelectedShape(std::vector<unsigned int> p_selectedShapes) {
	if (       _extension == KageFrame::extension::EXTENSION_NOT
			|| _extension == KageFrame::extension::EXTENSION_START) {
		return vectorsData.raiseSelectedShape(p_selectedShapes);
	} else {
		VectorDataManager l_vectorData = _layer->getPreviousFrameData(frameID);
		std::vector<unsigned int> l_selectedShapes = l_vectorData.raiseSelectedShape(p_selectedShapes);
		setFrameData(l_vectorData);
		return l_selectedShapes;
	}
}
std::vector<unsigned int> KageFrame::lowerSelectedShape(std::vector<unsigned int> p_selectedShapes) {
	if (       _extension == KageFrame::extension::EXTENSION_NOT
			|| _extension == KageFrame::extension::EXTENSION_START) {
		return vectorsData.lowerSelectedShape(p_selectedShapes);
	} else {
		VectorDataManager l_vectorData = _layer->getPreviousFrameData(frameID);
		std::vector<unsigned int> l_selectedShapes = l_vectorData.lowerSelectedShape(p_selectedShapes);
		setFrameData(l_vectorData);
		return l_selectedShapes;
	}
}
std::vector<unsigned int> KageFrame::raiseToTopSelectedShape(std::vector<unsigned int> p_selectedShapes) {
	if (       _extension == KageFrame::extension::EXTENSION_NOT
			|| _extension == KageFrame::extension::EXTENSION_START) {
		return vectorsData.raiseToTopSelectedShape(p_selectedShapes);
	} else {
		VectorDataManager l_vectorData = _layer->getPreviousFrameData(frameID);
		std::vector<unsigned int> l_selectedShapes = l_vectorData.raiseToTopSelectedShape(p_selectedShapes);
		setFrameData(l_vectorData);
		return l_selectedShapes;
	}
}
std::vector<unsigned int> KageFrame::lowerToBottomSelectedShape(std::vector<unsigned int> p_selectedShapes) {
	if (       _extension == KageFrame::extension::EXTENSION_NOT
			|| _extension == KageFrame::extension::EXTENSION_START) {
		return vectorsData.lowerToBottomSelectedShape(p_selectedShapes);
	} else {
		VectorDataManager l_vectorData = _layer->getPreviousFrameData(frameID);
		std::vector<unsigned int> l_selectedShapes = l_vectorData.lowerToBottomSelectedShape(p_selectedShapes);
		setFrameData(l_vectorData);
		return l_selectedShapes;
	}
}

std::vector<unsigned int> KageFrame::groupSelectedShapes(std::vector<unsigned int> p_selectedShapes) {
	if (       _extension == KageFrame::extension::EXTENSION_NOT
			|| _extension == KageFrame::extension::EXTENSION_START) {
		return vectorsData.groupSelectedShapes(p_selectedShapes);
	} else {
		VectorDataManager l_vectorData = _layer->getPreviousFrameData(frameID);
		std::vector<unsigned int> l_selectedShapes = l_vectorData.groupSelectedShapes(p_selectedShapes);
		setFrameData(l_vectorData);
		return l_selectedShapes;
	}
}
std::vector<unsigned int> KageFrame::ungroupSelectedShapes(std::vector<unsigned int> p_selectedShapes) {
	if (       _extension == KageFrame::extension::EXTENSION_NOT
			|| _extension == KageFrame::extension::EXTENSION_START) {
		return vectorsData.ungroupSelectedShapes(p_selectedShapes);
	} else {
		VectorDataManager l_vectorData = _layer->getPreviousFrameData(frameID);
		std::vector<unsigned int> l_selectedShapes = l_vectorData.ungroupSelectedShapes(p_selectedShapes);
		setFrameData(l_vectorData);
		return l_selectedShapes;
	}
}

std::vector<unsigned int> KageFrame::duplicateShapes(std::vector<unsigned int> p_selectedShapes) {
	if (       _extension == KageFrame::extension::EXTENSION_NOT
			|| _extension == KageFrame::extension::EXTENSION_START) {
		return vectorsData.duplicateShapes(p_selectedShapes);
	} else {
		VectorDataManager l_vectorData = _layer->getPreviousFrameData(frameID);
		std::vector<unsigned int> l_selectedShapes = l_vectorData.duplicateShapes(p_selectedShapes);
		setFrameData(l_vectorData);
		return l_selectedShapes;
	}
}

bool KageFrame::flipHorizontalSelectedShape(std::vector<unsigned int> p_selectedShapes) {
	if (       _extension == KageFrame::extension::EXTENSION_NOT
			|| _extension == KageFrame::extension::EXTENSION_START) {
		return vectorsData.flipHorizontalSelectedShape(p_selectedShapes);
	} else {
		VectorDataManager l_vectorData = _layer->getPreviousFrameData(frameID);
		bool l_selectedShapes = l_vectorData.flipHorizontalSelectedShape(p_selectedShapes);
		setFrameData(l_vectorData);
		return l_selectedShapes;
	}
}
bool KageFrame::flipVerticalSelectedShape(std::vector<unsigned int> p_selectedShapes) {
	if (       _extension == KageFrame::extension::EXTENSION_NOT
			|| _extension == KageFrame::extension::EXTENSION_START) {
		return vectorsData.flipVerticalSelectedShape(p_selectedShapes);
	} else {
		VectorDataManager l_vectorData = _layer->getPreviousFrameData(frameID);
		bool l_selectedShapes = l_vectorData.flipVerticalSelectedShape(p_selectedShapes);
		setFrameData(l_vectorData);
		return l_selectedShapes;
	}
}
bool KageFrame::recenterRotationPoint(std::vector<unsigned int> p_selectedShapes) {
	if (       _extension == KageFrame::extension::EXTENSION_NOT
			|| _extension == KageFrame::extension::EXTENSION_START) {
		return vectorsData.recenterRotationPoint(p_selectedShapes);
	} else {
		VectorDataManager l_vectorData = _layer->getPreviousFrameData(frameID);
		bool l_selectedShapes = l_vectorData.recenterRotationPoint(p_selectedShapes);
		setFrameData(l_vectorData);
		return l_selectedShapes;
	}
}
void KageFrame::addDataToFrame(VectorDataManager v) {
	if (       _extension == KageFrame::extension::EXTENSION_NOT
			|| _extension == KageFrame::extension::EXTENSION_START) {
		if (_null == true) {
			setNull(false);
		}
		vectorsData.push(v);
	} else {
		_layer->addDataToPreviousFrame(v, frameID);
	}
}

std::vector<VectorData> KageFrame::copySelectedShapes(std::vector<unsigned int> p_selectedShapes) {
	return vectorsData.copySelectedShapes(p_selectedShapes);
}

std::vector<unsigned int> KageFrame::pasteSelectedShapes(std::vector<VectorData> p_copiedShapes) {
	return vectorsData.pasteSelectedShapes(p_copiedShapes);
}

bool KageFrame::deleteSelectedShapes(std::vector<unsigned int> p_selectedShapes) {
	return vectorsData.deleteSelectedShapes(p_selectedShapes);
}
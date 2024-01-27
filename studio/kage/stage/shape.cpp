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

//part of stage.cpp

#include "stage.h"
#include "../../kage.h"
#include <cairomm/context.h>

void KageStage::setSelectedShapes(vector<unsigned int> p_selectedShapes) {
	selectedNodes = p_selectedShapes;
	tryMultiSelectShapes_populateShapes();
}
vector<unsigned int> KageStage::getSelectedShapes() {
	return selectedShapes;
}

bool KageStage::isSelectedShape(unsigned int p_index) {
	bool l_gotit = false;
	unsigned int ssize = selectedShapes.size();
	for (unsigned int i = 0; i < ssize; ++i) {
		if (selectedShapes[i] == p_index) {
			l_gotit = true;
			break;
		}
	}
	
	return l_gotit;
}
void KageStage::addSelectedShape(unsigned int p_index) {
	if (p_index == _NO_SELECTION) return;
	if (isSelectedShape(p_index) == false) {
		selectedShapes.push_back(p_index);
		anchor_rotate.x = 0;
		anchor_rotate.y = 0;
	}
}

void KageStage::handleShapes_scaleNorth() {
	if (_kage->isLayerLocked() == true) {
		return;
	}
	double diffOld = anchor_lowerRight.y - anchor_upperLeft.y;
	double diffNew = anchor_lowerRight.y - ((draw2.y - origin.y) / KageStage::currentScale / _zoomValue);
	double diffRatio = diffNew / diffOld;
	if (diffRatio == 0.0) {
		return;
	}
	
	std::vector<VectorData> v = _kage->getFrameData().getVectorData();
	unsigned int typeMovesIndex = -1;
	unsigned int vsize = v.size();
	unsigned int l_selectedShapes_size = selectedShapes.size();
	for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
		for (unsigned int i = selectedShapes[l_shapeIndex]; i < vsize; ++i) {
			if (v[i].vectorType == VectorData::TYPE_MOVE) {
				///register i for later use in ENDFILL
				typeMovesIndex = i;
			} else if (v[i].vectorType == VectorData::TYPE_LINE) {
				diffOld = anchor_lowerRight.y - v[i].points[0].y;
				diffNew = diffRatio * diffOld;
					v[i].points[0].y = anchor_lowerRight.y - diffNew;
			} else if (v[i].vectorType == VectorData::TYPE_CURVE_QUADRATIC
					|| v[i].vectorType == VectorData::TYPE_CURVE_CUBIC) {
				diffOld = anchor_lowerRight.y - v[i].points[0].y;
				diffNew = diffRatio * diffOld;
					v[i].points[0].y = anchor_lowerRight.y - diffNew;
				
				diffOld = anchor_lowerRight.y - v[i].points[1].y;
				diffNew = diffRatio * diffOld;
					v[i].points[1].y = anchor_lowerRight.y - diffNew;
				
				diffOld = anchor_lowerRight.y - v[i].points[2].y;
				diffNew = diffRatio * diffOld;
					v[i].points[2].y = anchor_lowerRight.y - diffNew;
			} else if (v[i].vectorType == VectorData::TYPE_ENDFILL) {
				diffOld = anchor_lowerRight.y - v[typeMovesIndex].points[0].y;
				diffNew = diffRatio * diffOld;
					v[typeMovesIndex].points[0].y = anchor_lowerRight.y - diffNew;
			} else if (v[i].vectorType == VectorData::TYPE_IMAGE) {
				//if rotation is factored-in then Scale North could be Scale XXX because rotation would change orientation 
				///NOTE: current implementation thinks image origin is top-left (or X/Y) or image
				diffOld = anchor_lowerRight.y - v[i].points[1].y;
				diffNew = diffRatio * diffOld;
					v[i].points[1].y = anchor_lowerRight.y - diffNew;
				v[i].points[3].y *= diffRatio;
			} else if (v[i].vectorType == VectorData::TYPE_INIT
					&& i != selectedShapes[l_shapeIndex]) {
				break;
			} else if (v[i].vectorType == VectorData::TYPE_INIT) {
				diffOld = anchor_lowerRight.y - v[i].points[0].y;
				diffNew = diffRatio * diffOld;
					v[i].points[0].y = anchor_lowerRight.y - diffNew;
			}
		}
	}
	
	anchor_upperLeft.y = (draw2.y - origin.y) / KageStage::currentScale / _zoomValue;
	
	_kage->setFrameData(v);
}
void KageStage::handleShapes_scaleEast() {
	if (_kage->isLayerLocked() == true) {
		return;
	}
	double diffOld = anchor_lowerRight.x - anchor_upperLeft.x;
	double diffNew = ((draw2.x - origin.x) / KageStage::currentScale / _zoomValue) - anchor_upperLeft.x;
	double diffRatio = diffNew / diffOld;
	if (diffRatio == 0.0) {
		return;
	}
	
	std::vector<VectorData> v = _kage->getFrameData().getVectorData();
	unsigned int typeMovesIndex = -1;
	unsigned int vsize = v.size();
	unsigned int l_selectedShapes_size = selectedShapes.size();
	for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
		for (unsigned int i = selectedShapes[l_shapeIndex]; i < vsize; ++i) {
			if (v[i].vectorType == VectorData::TYPE_MOVE) {
				///register i for later use in ENDFILL
				typeMovesIndex = i;
			} else if (v[i].vectorType == VectorData::TYPE_LINE) {
				diffOld = anchor_upperLeft.x - v[i].points[0].x;
				diffNew = diffRatio * diffOld;
					v[i].points[0].x = anchor_upperLeft.x - diffNew;
			} else if (v[i].vectorType == VectorData::TYPE_CURVE_QUADRATIC
					|| v[i].vectorType == VectorData::TYPE_CURVE_CUBIC) {
				diffOld = anchor_upperLeft.x - v[i].points[0].x;
				diffNew = diffRatio * diffOld;
					v[i].points[0].x = anchor_upperLeft.x - diffNew;
				
				diffOld = anchor_upperLeft.x - v[i].points[1].x;
				diffNew = diffRatio * diffOld;
					v[i].points[1].x = anchor_upperLeft.x - diffNew;
				
				diffOld = anchor_upperLeft.x - v[i].points[2].x;
				diffNew = diffRatio * diffOld;
					v[i].points[2].x = anchor_upperLeft.x - diffNew;
			} else if (v[i].vectorType == VectorData::TYPE_ENDFILL) {
				diffOld = anchor_upperLeft.x - v[typeMovesIndex].points[0].x;
				diffNew = diffRatio * diffOld;
					v[typeMovesIndex].points[0].x = anchor_upperLeft.x - diffNew;
			} else if (v[i].vectorType == VectorData::TYPE_IMAGE) {
				//if rotation is factored-in then Scale East could be Scale XXX because rotation would change orientation 
				///NOTE: current implementation thinks image origin is top-left (or X/Y) or image
				v[i].points[3].x *= diffRatio;
			} else if (v[i].vectorType == VectorData::TYPE_INIT
					&& i != selectedShapes[l_shapeIndex]) {
				break;
			} else if (v[i].vectorType == VectorData::TYPE_INIT) {
				diffOld = v[i].points[0].x - anchor_upperLeft.x;
				diffNew = diffRatio * diffOld;
					v[i].points[0].x = anchor_upperLeft.x + diffNew;
			}
		}
	}
	
	anchor_lowerRight.x = (draw2.x - origin.x) / KageStage::currentScale / _zoomValue;
	
	_kage->setFrameData(v);
}
void KageStage::handleShapes_scaleWest() {
	if (_kage->isLayerLocked() == true) {
		return;
	}
	double diffOld = anchor_upperLeft.x - anchor_lowerRight.x;
	double diffNew = ((draw2.x - origin.x) / KageStage::currentScale / _zoomValue) - anchor_lowerRight.x;
	double diffRatio = diffNew / diffOld;
	if (diffRatio == 0.0) {
		return;
	}
	
	std::vector<VectorData> v = _kage->getFrameData().getVectorData();
	unsigned int typeMovesIndex = -1;
	unsigned int vsize = v.size();
	unsigned int l_selectedShapes_size = selectedShapes.size();
	for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
		for (unsigned int i = selectedShapes[l_shapeIndex]; i < vsize; ++i) {
			if (v[i].vectorType == VectorData::TYPE_MOVE) {
				///register i for later use in ENDFILL
				typeMovesIndex = i;
			} else if (v[i].vectorType == VectorData::TYPE_LINE) {
				diffOld = anchor_lowerRight.x - v[i].points[0].x;
				diffNew = diffRatio * diffOld;
					v[i].points[0].x = anchor_lowerRight.x - diffNew;
			} else if (v[i].vectorType == VectorData::TYPE_CURVE_QUADRATIC
					|| v[i].vectorType == VectorData::TYPE_CURVE_CUBIC) {
				diffOld = anchor_lowerRight.x - v[i].points[0].x;
				diffNew = diffRatio * diffOld;
					v[i].points[0].x = anchor_lowerRight.x - diffNew;
				
				diffOld = anchor_lowerRight.x - v[i].points[1].x;
				diffNew = diffRatio * diffOld;
					v[i].points[1].x = anchor_lowerRight.x - diffNew;
				
				diffOld = anchor_lowerRight.x - v[i].points[2].x;
				diffNew = diffRatio * diffOld;
					v[i].points[2].x = anchor_lowerRight.x - diffNew;
			} else if (v[i].vectorType == VectorData::TYPE_ENDFILL) {
				diffOld = anchor_lowerRight.x - v[typeMovesIndex].points[0].x;
				diffNew = diffRatio * diffOld;
					v[typeMovesIndex].points[0].x = anchor_lowerRight.x - diffNew;
			} else if (v[i].vectorType == VectorData::TYPE_IMAGE) {
				//if rotation is factored-in then Scale West could be Scale XXX because rotation would change orientation 
				///NOTE: current implementation thinks image origin is top-left (or X/Y) or image
				diffOld = anchor_lowerRight.x - v[i].points[1].x;
				diffNew = diffRatio * diffOld;
					v[i].points[1].x = anchor_lowerRight.x - diffNew;
				v[i].points[3].x *= diffRatio;
			} else if (v[i].vectorType == VectorData::TYPE_INIT
					&& i != selectedShapes[l_shapeIndex]) {
				break;
			} else if (v[i].vectorType == VectorData::TYPE_INIT) {
				diffOld = anchor_lowerRight.x - v[i].points[0].x;
				diffNew = diffRatio * diffOld;
					v[i].points[0].x = anchor_lowerRight.x - diffNew;
			}
		}
	}
	
	anchor_upperLeft.x = (draw2.x - origin.x) / KageStage::currentScale / _zoomValue;
	
	_kage->setFrameData(v);
}
void KageStage::handleShapes_scaleSouth() {
	if (_kage->isLayerLocked() == true) {
		return;
	}
	double diffOld = anchor_lowerRight.y - anchor_upperLeft.y;
	double diffNew = ((draw2.y - origin.y) / KageStage::currentScale / _zoomValue) - anchor_upperLeft.y;
	double diffRatio = diffNew / diffOld;
	if (diffRatio == 0.0) {
		return;
	}
	
	std::vector<VectorData> v = _kage->getFrameData().getVectorData();
	unsigned int typeMovesIndex = -1;
	unsigned int vsize = v.size();
	unsigned int l_selectedShapes_size = selectedShapes.size();
	for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
		for (unsigned int i = selectedShapes[l_shapeIndex]; i < vsize; ++i) {
			if (v[i].vectorType == VectorData::TYPE_MOVE) {
				///register i for later use in ENDFILL
				typeMovesIndex = i;
			} else if (v[i].vectorType == VectorData::TYPE_LINE) {
				diffOld = anchor_upperLeft.y - v[i].points[0].y;
				diffNew = diffRatio * diffOld;
					v[i].points[0].y = anchor_upperLeft.y - diffNew;
			} else if (v[i].vectorType == VectorData::TYPE_CURVE_QUADRATIC
					|| v[i].vectorType == VectorData::TYPE_CURVE_CUBIC) {
				diffOld = anchor_upperLeft.y - v[i].points[0].y;
				diffNew = diffRatio * diffOld;
					v[i].points[0].y = anchor_upperLeft.y - diffNew;
				
				diffOld = anchor_upperLeft.y - v[i].points[1].y;
				diffNew = diffRatio * diffOld;
					v[i].points[1].y = anchor_upperLeft.y - diffNew;
				
				diffOld = anchor_upperLeft.y - v[i].points[2].y;
				diffNew = diffRatio * diffOld;
					v[i].points[2].y = anchor_upperLeft.y - diffNew;
			} else if (v[i].vectorType == VectorData::TYPE_ENDFILL) {
				diffOld = anchor_upperLeft.y - v[typeMovesIndex].points[0].y;
				diffNew = diffRatio * diffOld;
					v[typeMovesIndex].points[0].y = anchor_upperLeft.y - diffNew;
			} else if (v[i].vectorType == VectorData::TYPE_IMAGE) {
				//if rotation is factored-in then Scale South could be Scale XXX because rotation would change orientation 
				///NOTE: current implementation thinks image origin is top-left (or X/Y) or image
				v[i].points[3].y *= diffRatio;
			} else if (v[i].vectorType == VectorData::TYPE_INIT
					&& i != selectedShapes[l_shapeIndex]) {
				break;
			} else if (v[i].vectorType == VectorData::TYPE_INIT) {
				diffOld = v[i].points[0].y - anchor_upperLeft.y;
				diffNew = diffRatio * diffOld;
					v[i].points[0].y = anchor_upperLeft.y + diffNew;
			}
		}
	}
	
	anchor_lowerRight.y = (draw2.y - origin.y) / KageStage::currentScale / _zoomValue;
	
	_kage->setFrameData(v);
}
void KageStage::handleShapes_moveShape(double p_diffX, double p_diffY) {
	if (p_diffX == 0 && p_diffY == 0) {
		return;
	}
	
	std::vector<VectorData> v = _kage->getFrameData().getVectorData();
	unsigned int typeMovesIndex = UINT_MAX;
	unsigned int vsize = v.size();
	unsigned int l_selectedShapes_size = selectedShapes.size();
	for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
		for (unsigned int i = selectedShapes[l_shapeIndex]; i < vsize; ++i) {
			if (v[i].vectorType == VectorData::TYPE_MOVE) {
				///register i for later use in ENDFILL
				typeMovesIndex = i;
			} else if (v[i].vectorType == VectorData::TYPE_LINE
					|| v[i].vectorType == VectorData::TYPE_CURVE_QUADRATIC
					|| v[i].vectorType == VectorData::TYPE_CURVE_CUBIC) {
				v[i].points[0].x += p_diffX;
				v[i].points[0].y += p_diffY;
				v[i].points[1].x += p_diffX;
				v[i].points[1].y += p_diffY;
				v[i].points[2].x += p_diffX;
				v[i].points[2].y += p_diffY;
			} else if (v[i].vectorType == VectorData::TYPE_ENDFILL) {
				if (typeMovesIndex != UINT_MAX) {
					v[typeMovesIndex].points[0].x += p_diffX;
					v[typeMovesIndex].points[0].y += p_diffY;
				}
			} else if (v[i].vectorType == VectorData::TYPE_IMAGE) {
				v[i].points[1].x += p_diffX;
				v[i].points[1].y += p_diffY;
			} else if (v[i].vectorType == VectorData::TYPE_INIT
					&& i != selectedShapes[l_shapeIndex]) {
				break;
			} else if (v[i].vectorType == VectorData::TYPE_INIT) {
				//move rotation-anchor along
				v[i].points[0].x += p_diffX;
				v[i].points[0].y += p_diffY;
			}
		}
	}
	
	_kage->setFrameData(v);
}
void KageStage::handleShapes_updateAnchors(double p_x, double p_y) {
	if (p_x <= anchor_upperLeft.x) {
		anchor_upperLeft.x = p_x;
	}
	if (p_x >= anchor_lowerRight.x) {
		anchor_lowerRight.x = p_x;
	}
	if (p_y <= anchor_upperLeft.y) {
		anchor_upperLeft.y = p_y;
	}
	if (p_y >= anchor_lowerRight.y) {
		anchor_lowerRight.y = p_y;
	}
}

void KageStage::updateShapeColor(bool p_doFill, bool p_doStroke) {
	if (selectedShapes.size() == 0) {
		return;
	}
	if (p_doFill == false && p_doStroke == false) {
		return;
	}
	std::vector<VectorData> v = _kage->getFrameData().getVectorData();
	unsigned int vsize = v.size();
	unsigned int l_selectedShapes_size = selectedShapes.size();
	for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
		for (unsigned int i = selectedShapes[l_shapeIndex]; i < vsize; ++i) {
			if (v[i].vectorType == VectorData::TYPE_FILL) {
				if (p_doFill == true) {
					if (KageStage::toolMode == ToolMode::MODE_DRAW_PENCIL) {
						v[i].fillColor = pencilFillColor.clone();
					} else {
						v[i].fillColor = fillColor.clone();
					}
				}
			} else if (v[i].vectorType == VectorData::TYPE_STROKE) {
				if (p_doStroke == true) {
					if (KageStage::toolMode == ToolMode::MODE_DRAW_PENCIL) {
						v[i].stroke = pencilStroke.clone();
					} else {
						v[i].stroke = stroke.clone();
					}
				}
				
				break;
			}
		}
	}
	
	_kage->setFrameData(v);
	
	_kage->stackDo();
	
	invalidateToRender();
}

void KageStage::updateShapeX(double p_value, bool p_stackDo) {
	Kage::timestamp_IN();
	std::cout << " KageStage::updateShapeX " << selectedShapes.size() << " " << p_value << std::endl;
	
	if (selectedShapes.size() == 0) {
		Kage::timestamp_OUT();
		return;
	}
	
	std::vector<VectorData> v = _kage->getFrameData().getVectorData();
	
	propX = DBL_MAX;
	propXindex1 = UINT_MAX;
	propXindex2 = UINT_MAX;
	propY = DBL_MAX;
	propYindex1 = UINT_MAX;
	propYindex2 = UINT_MAX;
	
	unsigned int vsize = v.size();
	unsigned int l_selectedShapes_size = selectedShapes.size();
	for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
		//calling getSelectedShapeViaNode will update propXindex1/propXindex2 based on left-most VectorData.x
		if (_kage->_displayObjectIsShape == true) {
			getSelectedShapeViaNode(selectedShapes[l_shapeIndex]+3, v);
		} else {
			getSelectedShapeViaNode(selectedShapes[l_shapeIndex]+1, v);
		}
	}
	
	double l_propXprev = 0;
	double l_propXdiff = p_value / KageStage::currentScale / _zoomValue - l_propXprev;
	if (propXindex1 != UINT_MAX && propXindex2 != UINT_MAX) {
		l_propXprev = v[propXindex1].points[propXindex2].x;
		l_propXdiff = p_value / KageStage::currentScale / _zoomValue - l_propXprev;
	}
	
	for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
		for (unsigned int i = selectedShapes[l_shapeIndex]; i < vsize; ++i) {
			if (v[i].vectorType == VectorData::TYPE_CURVE_CUBIC) {
				v[i].points[0].x += l_propXdiff;
				v[i].points[1].x += l_propXdiff;
				v[i].points[2].x += l_propXdiff;
			} else if (v[i].vectorType == VectorData::TYPE_MOVE) {
				v[i].points[0].x += l_propXdiff;
			} else if (v[i].vectorType == VectorData::TYPE_IMAGE) {
				//p1 x/y == ID / imageBuff
				//p2 x/y == x / y
				//p3 x/y == width/height
				//p4 x/y == scaleX/Y
				//p5 x/y == rotate / alpha
				v[i].points[1].x += l_propXdiff;
			} else if (v[i].vectorType == VectorData::TYPE_INIT
					&& i != selectedShapes[l_shapeIndex]) {
				break;
			} else if (v[i].vectorType == VectorData::TYPE_INIT) {
				v[i].points[0].x += l_propXdiff;
			}
		}
	}
	
	_kage->setFrameData(v);
	
	if (p_stackDo) {
		_kage->stackDo();
	}
	
	invalidateToRender();
	
	Kage::timestamp_OUT();
}
void KageStage::updateShapeY(double p_value, bool p_stackDo) {
	Kage::timestamp_IN();
	std::cout << " KageStage::updateShapeY " << selectedShapes.size() << " " << p_value << std::endl;
	
	if (selectedShapes.size() == 0) {
		Kage::timestamp_OUT();
		return;
	}
	
	std::vector<VectorData> v = _kage->getFrameData().getVectorData();
	
	propX = DBL_MAX;
	propXindex1 = UINT_MAX;
	propXindex2 = UINT_MAX;
	propY = DBL_MAX;
	propYindex1 = UINT_MAX;
	propYindex2 = UINT_MAX;
	
	unsigned int vsize = v.size();
	unsigned int l_selectedShapes_size = selectedShapes.size();
	for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
		if (_kage->_displayObjectIsShape == true) {
			//calling getSelectedShapeViaNode will update propYindex1/propYindex2 based on top-most VectorData.y
			getSelectedShapeViaNode(selectedShapes[l_shapeIndex]+3, v);
		} else {
			getSelectedShapeViaNode(selectedShapes[l_shapeIndex]+1, v);
		}
	}
	
	double l_propYprev = 0;
	double l_propYdiff = p_value / KageStage::currentScale / _zoomValue - l_propYprev;
	if (propYindex1 != UINT_MAX && propYindex2 != UINT_MAX) {
		l_propYprev = v[propYindex1].points[propYindex2].y;
		l_propYdiff = p_value / KageStage::currentScale / _zoomValue - l_propYprev;
	}
	
	for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
		for (unsigned int i = selectedShapes[l_shapeIndex]; i < vsize; ++i) {
			if (v[i].vectorType == VectorData::TYPE_CURVE_CUBIC) {
				v[i].points[0].y += l_propYdiff;
				v[i].points[1].y += l_propYdiff;
				v[i].points[2].y += l_propYdiff;
			} else if (v[i].vectorType == VectorData::TYPE_MOVE) {
				v[i].points[0].y += l_propYdiff;
			} else if (v[i].vectorType == VectorData::TYPE_IMAGE) {
				//p1 x/y == ID / imageBuff
				//p2 x/y == x / y
				//p3 x/y == width/height
				//p4 x/y == scaleX/Y
				//p5 x/y == rotate / alpha
				v[i].points[1].y += l_propYdiff;
			} else if (v[i].vectorType == VectorData::TYPE_INIT
					&& i != selectedShapes[l_shapeIndex]) {
				break;
			} else if (v[i].vectorType == VectorData::TYPE_INIT) {
				v[i].points[0].y += l_propYdiff;
			}
		}
	}
	
	_kage->setFrameData(v);
	
	if (p_stackDo) {
		_kage->stackDo();
	}
	
	invalidateToRender();
	
	Kage::timestamp_OUT();
}

void KageStage::updateShapeWidth(double p_value) {
	Kage::timestamp_IN();
	std::cout << " KageStage::updateShapeWidth " << selectedShapes.size() << " " << p_value << std::endl;
	
	if (selectedShapes.size() == 0) {
		Kage::timestamp_OUT();
		return;
	}
	
	std::vector<VectorData> v = _kage->getFrameData().getVectorData();
	
	propX = DBL_MAX;
	propXindex1 = UINT_MAX;
	propXindex2 = UINT_MAX;
	propY = DBL_MAX;
	propYindex1 = UINT_MAX;
	propYindex2 = UINT_MAX;
	
	unsigned int vsize = v.size();
	unsigned int l_selectedShapes_size = selectedShapes.size();
	for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
		if (_kage->_displayObjectIsShape == true) {
			//calling getSelectedShapeViaNode will update propXindex1/propXindex2 based on left-most VectorData.x
			getSelectedShapeViaNode(selectedShapes[l_shapeIndex]+3, v);
		} else {
			getSelectedShapeViaNode(selectedShapes[l_shapeIndex]+1, v);
		}
	}
	
	double l_propXprev = 0;
	if (propXindex1 != UINT_MAX && propXindex2 != UINT_MAX) {
		l_propXprev = v[propXindex1].points[propXindex2].x;
	}
	
	for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
		for (unsigned int i = selectedShapes[l_shapeIndex]; i < vsize; ++i) {
			if (v[i].vectorType == VectorData::TYPE_CURVE_CUBIC) {
				v[i].points[0].x = propX + ((v[i].points[0].x-l_propXprev) / propWidth * (p_value / KageStage::currentScale));
				v[i].points[1].x = propX + ((v[i].points[1].x-l_propXprev) / propWidth * (p_value / KageStage::currentScale));
				v[i].points[2].x = propX + ((v[i].points[2].x-l_propXprev) / propWidth * (p_value / KageStage::currentScale));
			} else if (v[i].vectorType == VectorData::TYPE_MOVE) {
				v[i].points[0].x = propX + ((v[i].points[0].x-l_propXprev) / propWidth * (p_value / KageStage::currentScale));
			} else if (v[i].vectorType == VectorData::TYPE_IMAGE) {
				//p1 x/y == ID / imageBuff
				//p2 x/y == x / y
				//p3 x/y == width/height
				//p4 x/y == scaleX/Y
				//p5 x/y == rotate / alpha
				//TODO: factor-in rotation
				v[i].points[3].x = (v[i].points[1].x + (v[i].points[2].x * v[i].points[3].x)) / (p_value / KageStage::currentScale);
			} else if (v[i].vectorType == VectorData::TYPE_INIT
					&& i != selectedShapes[l_shapeIndex]) {
				break;
			} else if (v[i].vectorType == VectorData::TYPE_INIT) {
				v[i].points[0].x = propX + ((v[i].points[0].x-l_propXprev) / propWidth * (p_value / KageStage::currentScale));
			}
		}
	}
	
	_kage->setFrameData(v);
	
	_kage->stackDo();
	
	invalidateToRender();
	
	Kage::timestamp_OUT();
}
void KageStage::updateShapeHeight(double p_value) {
	Kage::timestamp_IN();
	std::cout << " KageStage::updateShapeHeight " << selectedShapes.size() << " " << p_value << std::endl;
	
	if (selectedShapes.size() == 0) {
		Kage::timestamp_OUT();
		return;
	}
	
	std::vector<VectorData> v = _kage->getFrameData().getVectorData();
	
	propX = DBL_MAX;
	propXindex1 = UINT_MAX;
	propXindex2 = UINT_MAX;
	propY = DBL_MAX;
	propYindex1 = UINT_MAX;
	propYindex2 = UINT_MAX;
	
	unsigned int vsize = v.size();
	unsigned int l_selectedShapes_size = selectedShapes.size();
	for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
		if (_kage->_displayObjectIsShape == true) {
			//calling getSelectedShapeViaNode will update propYindex1/propYindex2 based on top-most VectorData.y
			getSelectedShapeViaNode(selectedShapes[l_shapeIndex]+3, v);
		} else {
			getSelectedShapeViaNode(selectedShapes[l_shapeIndex]+1, v);
		}
	}
	
	double l_propYprev = 0;
	if (propYindex1 != UINT_MAX && propYindex2 != UINT_MAX) {
		l_propYprev = v[propYindex1].points[propYindex2].y;
	}
	
	//targetY = ((targetY-topY) / propHeight * p_value) + topY;
	//v[i].points[0].y = ((v[i].points[0].y-l_propYprev) / propHeight * p_value) + propY;
	for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
		for (unsigned int i = selectedShapes[l_shapeIndex]; i < vsize; ++i) {
			if (v[i].vectorType == VectorData::TYPE_CURVE_CUBIC) {
				v[i].points[0].y = propY + ((v[i].points[0].y-l_propYprev) / propHeight * (p_value / KageStage::currentScale));
				v[i].points[1].y = propY + ((v[i].points[1].y-l_propYprev) / propHeight * (p_value / KageStage::currentScale));
				v[i].points[2].y = propY + ((v[i].points[2].y-l_propYprev) / propHeight * (p_value / KageStage::currentScale));
			} else if (v[i].vectorType == VectorData::TYPE_MOVE) {
				v[i].points[0].y = propY + ((v[i].points[0].y-l_propYprev) / propHeight * (p_value / KageStage::currentScale));
			} else if (v[i].vectorType == VectorData::TYPE_IMAGE) {
				//p1 x/y == ID / imageBuff
				//p2 x/y == x / y
				//p3 x/y == width/height
				//p4 x/y == scaleX/Y
				//p5 x/y == rotate / alpha
				//TODO: factor-in rotation
				v[i].points[3].y = (v[i].points[1].y + (v[i].points[2].y * v[i].points[3].y)) / (p_value / KageStage::currentScale);
			} else if (v[i].vectorType == VectorData::TYPE_INIT
					&& i != selectedShapes[l_shapeIndex]) {
				break;
			} else if (v[i].vectorType == VectorData::TYPE_INIT) {
				v[i].points[0].y = propY + ((v[i].points[0].y-l_propYprev) / propHeight * (p_value / KageStage::currentScale));
			}
		}
	}
	
	_kage->setFrameData(v);
	
	_kage->stackDo();
	
	invalidateToRender();
	
	Kage::timestamp_OUT();
}

void KageStage::handleShapes_modifyingShapeRotate() {
	if (anchor_rotate.x == 0 && anchor_rotate.y == 0) {
		anchor_rotate.x = anchor_center.x;
		anchor_rotate.y = anchor_center.y;
	}
	PointData l_rotateNew;
		l_rotateNew.x = draw2.x - anchor_rotate.x;
		l_rotateNew.y = draw2.y - anchor_rotate.y;
	double l_angleNew = atan2(l_rotateNew.y, l_rotateNew.x);
	
	PointData l_rotateOld;
		l_rotateOld.x = draw1.x - anchor_rotate.x;
		l_rotateOld.y = draw1.y - anchor_rotate.y;
	double l_angleOld = atan2(l_rotateOld.y, l_rotateOld.x);
	
	double l_angleDiff;
	
	unsigned int l_selectedShapes_size = selectedShapes.size();
	std::vector<VectorData> v = _kage->getFrameData().getVectorData();
	if (l_selectedShapes_size > 0) {
		for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
			for (unsigned int i = selectedShapes[l_shapeIndex]; i < v.size(); ++i) {
				if (v[i].vectorType == VectorData::TYPE_INIT
					&& i != selectedShapes[l_shapeIndex]) {
					break;
				} else if (v[i].vectorType == VectorData::TYPE_INIT) {
					//use as rotation-anchor ONLY if just ONE selected shape
					if (l_selectedShapes_size == 1 && v[i].points.size() == 1) {
						anchor_center.x = v[i].points[0].x;
						anchor_center.y = v[i].points[0].y;
							l_rotateNew.x = draw2.x - anchor_center.x;
							l_rotateNew.y = draw2.y - anchor_center.y;
								l_angleNew = atan2(l_rotateNew.y, l_rotateNew.x);
							l_rotateOld.x = draw1.x - anchor_center.x;
							l_rotateOld.y = draw1.y - anchor_center.y;
								l_angleOld = atan2(l_rotateOld.y, l_rotateOld.x);
					}
				} else if (v[i].vectorType == VectorData::TYPE_MOVE) {
					unsigned int j = 0;
					l_angleDiff = l_angleNew - l_angleOld;
					
					PointData l_pointRotate;
						l_pointRotate.x = v[i].points[j].x - anchor_center.x;
						l_pointRotate.y = v[i].points[j].y - anchor_center.y;
					double l_pointAngle = atan2(l_pointRotate.y*KageStage::currentScale, l_pointRotate.x*KageStage::currentScale) + l_angleDiff;
					
					//apply new angle
					double l_hyp = sqrtf((l_pointRotate.x * l_pointRotate.x*KageStage::currentScale*KageStage::currentScale) + (l_pointRotate.y * l_pointRotate.y*KageStage::currentScale*KageStage::currentScale)); //why multiply twice?
					v[i].points[j].x = (cos(l_pointAngle) * l_hyp)/KageStage::currentScale + anchor_center.x;
					v[i].points[j].y = (sin(l_pointAngle) * l_hyp)/KageStage::currentScale + anchor_center.y;
				} else if (v[i].vectorType == VectorData::TYPE_CURVE_CUBIC
						|| v[i].vectorType == VectorData::TYPE_CURVE_QUADRATIC) {
					l_angleDiff = l_angleNew - l_angleOld;
					for (unsigned int j = 0; j < 3; ++j) {
						PointData l_pointRotate;
							l_pointRotate.x = v[i].points[j].x - anchor_center.x;
							l_pointRotate.y = v[i].points[j].y - anchor_center.y;
						double l_hyp = sqrtf((l_pointRotate.x * l_pointRotate.x*KageStage::currentScale*KageStage::currentScale) + (l_pointRotate.y * l_pointRotate.y*KageStage::currentScale*KageStage::currentScale)); //why multiply twice?
						
						double l_pointAngle = atan2(l_pointRotate.y*KageStage::currentScale, l_pointRotate.x*KageStage::currentScale) + l_angleDiff;
						//apply new angle
						v[i].points[j].x = (cos(l_pointAngle) * l_hyp)/KageStage::currentScale + anchor_center.x;
						v[i].points[j].y = (sin(l_pointAngle) * l_hyp)/KageStage::currentScale + anchor_center.y;
					}
				} else if (v[i].vectorType == VectorData::TYPE_IMAGE) {
					l_angleDiff = l_angleNew - l_angleOld;
						v[i].points[4].x += l_angleDiff;
				}
			}
		}
		_kage->setFrameData(v);
	}
	
	//_kage->stackDo();
	_rotateApply = false;
}
void KageStage::handleShapes_modifyingShape() {
	if (_isModifyingShape == false) {
		drawSelectionArea();
	}
	if (selectedShapes.size() == 0) {
		return;
	}
	
	if (mouseOnAnchor == AnchorData::TYPE_NONE) {
		if (isMouseOnNode(       (     anchor_center.x*KageStage::currentScale*_zoomValue)+origin.x    ,
								 (  anchor_upperLeft.y*KageStage::currentScale*_zoomValue)+origin.y - 7, 7) == true) {
			mouseOnAnchor = AnchorData::TYPE_NORTH;
		} else if (isMouseOnNode(( anchor_lowerRight.x*KageStage::currentScale*_zoomValue)+origin.x + 7,
								 (     anchor_center.y*KageStage::currentScale*_zoomValue)+origin.y    , 7) == true) {
			mouseOnAnchor = AnchorData::TYPE_EAST;
		} else if (isMouseOnNode((  anchor_upperLeft.x*KageStage::currentScale*_zoomValue)+origin.x - 7,
								 (     anchor_center.y*KageStage::currentScale*_zoomValue)+origin.y    , 7) == true) {
			mouseOnAnchor = AnchorData::TYPE_WEST;
		} else if (isMouseOnNode((     anchor_center.x*KageStage::currentScale*_zoomValue)+origin.x    ,
								 ( anchor_lowerRight.y*KageStage::currentScale*_zoomValue)+origin.y + 7, 7) == true) {
			mouseOnAnchor = AnchorData::TYPE_SOUTH;
		} else if (isMouseOnNode(( anchor_lowerRight.x*KageStage::currentScale*_zoomValue)+origin.x + 7,
								 (  anchor_upperLeft.y*KageStage::currentScale*_zoomValue)+origin.y - 7, 7) == true) {
			mouseOnAnchor = AnchorData::TYPE_NORTH_EAST;
		} else if (isMouseOnNode((  anchor_upperLeft.x*KageStage::currentScale*_zoomValue)+origin.x - 7,
								 (  anchor_upperLeft.y*KageStage::currentScale*_zoomValue)+origin.y - 7, 7) == true) {
			mouseOnAnchor = AnchorData::TYPE_NORTH_WEST;
		} else if (isMouseOnNode(( anchor_lowerRight.x*KageStage::currentScale*_zoomValue)+origin.x + 7,
								 ( anchor_lowerRight.y*KageStage::currentScale*_zoomValue)+origin.y + 7, 7) == true) {
			mouseOnAnchor = AnchorData::TYPE_SOUTH_EAST;
		} else if (isMouseOnNode((  anchor_upperLeft.x*KageStage::currentScale*_zoomValue)+origin.x - 7,
								 ( anchor_lowerRight.y*KageStage::currentScale*_zoomValue)+origin.y + 7, 7) == true) {
			mouseOnAnchor = AnchorData::TYPE_SOUTH_WEST;
		} else {
			double l_X = (anchor_lowerRight.x - anchor_center.x) * KageStage::currentScale * _zoomValue;
			double l_Y = (anchor_lowerRight.y - anchor_center.y) * KageStage::currentScale * _zoomValue;
			double l_distance = 7;
			if (l_X > l_Y) {
				l_distance = l_Y;
			} else {
				l_distance = l_X;
			}
			if (isMouseOnNode(  (anchor_center.x*KageStage::currentScale*_zoomValue)+origin.x,
								(anchor_center.y*KageStage::currentScale*_zoomValue)+origin.y, l_distance) == true) {
				mouseOnAnchor = AnchorData::TYPE_MOVE;
				if (_rotateMode == true) {
					mouseOnAnchor = AnchorData::TYPE_ROTATE;
				}
			}
		}
	}
	if (mouseOnAnchor != AnchorData::TYPE_NONE) {
		_isModifyingShape = true;
	}
	/// NOTE: don't use ELSE-IF to apply changes ASAP
	if (mouseOnAnchor != AnchorData::TYPE_NONE
			&& mouseOnAnchor != AnchorData::TYPE_MOVE
			&& _rotateMode == false) {
		if (mouseOnAnchor == AnchorData::TYPE_NORTH
				|| mouseOnAnchor == AnchorData::TYPE_NORTH_EAST
				|| mouseOnAnchor == AnchorData::TYPE_NORTH_WEST) {
			handleShapes_scaleNorth();
		} else if (mouseOnAnchor == AnchorData::TYPE_SOUTH
				|| mouseOnAnchor == AnchorData::TYPE_SOUTH_EAST
				|| mouseOnAnchor == AnchorData::TYPE_SOUTH_WEST) {
			handleShapes_scaleSouth();
		}
		
		if (mouseOnAnchor == AnchorData::TYPE_EAST
				|| mouseOnAnchor == AnchorData::TYPE_NORTH_EAST
				|| mouseOnAnchor == AnchorData::TYPE_SOUTH_EAST) {
			handleShapes_scaleEast();
		} else if (mouseOnAnchor == AnchorData::TYPE_WEST
				|| mouseOnAnchor == AnchorData::TYPE_NORTH_WEST
				|| mouseOnAnchor == AnchorData::TYPE_SOUTH_WEST) {
			handleShapes_scaleWest();
		}
		
		anchor_center.x = (anchor_upperLeft.x + anchor_lowerRight.x) / 2;
		anchor_center.y = (anchor_upperLeft.y + anchor_lowerRight.y) / 2;
		
	} else if (mouseOnAnchor == AnchorData::TYPE_MOVE) {
		//move the whole shape around
		handleShapes_moveShape(
			((draw2.x - origin.x) / KageStage::currentScale / _zoomValue) - ((draw1.x - origin.x) / KageStage::currentScale / _zoomValue),
			((draw2.y - origin.y) / KageStage::currentScale / _zoomValue) - ((draw1.y - origin.y) / KageStage::currentScale / _zoomValue));
		draw1.x = draw2.x;
		draw1.y = draw2.y;
	} else if (mouseOnAnchor == AnchorData::TYPE_ROTATE) {
		AnchorData l_anchor;
		std::vector<VectorData> v = _kage->getFrameData().getVectorData();
			unsigned int vsize = v.size();
			unsigned int l_selectedShapes_size = selectedShapes.size();
			for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
				for (unsigned int i = selectedShapes[l_shapeIndex]; i < vsize; ++i) {
					if (v[i].vectorType == VectorData::TYPE_INIT
							&& i != selectedShapes[l_shapeIndex]) {
						break;
					} else if (v[i].vectorType == VectorData::TYPE_INIT) {
						//do move Rotate-Anchor
						if (l_selectedShapes_size > 1) {
							//keep current X/Y for use if anchor value from init is 0/0 which indicates default value
							anchor_rotate.x = (draw2.x - origin.x) / KageStage::currentScale / _zoomValue;
							anchor_rotate.y = (draw2.y - origin.y) / KageStage::currentScale / _zoomValue;
						} else {
							v[i].points[0].x = (draw2.x - origin.x) / KageStage::currentScale / _zoomValue;
							v[i].points[0].y = (draw2.y - origin.y) / KageStage::currentScale / _zoomValue;
						}
						
						l_anchor.x = (draw2.x - origin.x) / KageStage::currentScale / _zoomValue;
						l_anchor.y = (draw2.y - origin.y) / KageStage::currentScale / _zoomValue;
					}
				}
			}
			
			_kage->setFrameData(v);
		renderFrame(cr, v);
		
		//draw rotate anchor
		if (l_anchor.x == 0 && l_anchor.y == 0) {
			Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_ROTATE,
						(anchor_center.x*KageStage::currentScale*_zoomValue)+origin.x -  7,
						(anchor_center.y*KageStage::currentScale*_zoomValue)+origin.y -  7);
		} else {
			Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_ROTATE,
						(l_anchor.x*KageStage::currentScale*_zoomValue)+origin.x -  7,
						(l_anchor.y*KageStage::currentScale*_zoomValue)+origin.y -  7);
		}
			cr->paint();
	} else if (mouseOnAnchor != AnchorData::TYPE_NONE
			&& mouseOnAnchor != AnchorData::TYPE_MOVE
			&& _rotateMode == true) {
		if (anchor_rotate.x == 0 && anchor_rotate.y == 0) {
			anchor_rotate.x = anchor_center.x;
			anchor_rotate.y = anchor_center.y;
		}
		PointData l_rotateNew;
			l_rotateNew.x = (((draw2.x - origin.x) / KageStage::currentScale) / _zoomValue) - anchor_rotate.x;
			l_rotateNew.y = (((draw2.y - origin.y) / KageStage::currentScale) / _zoomValue) - anchor_rotate.y;
		double l_angleNew = atan2(l_rotateNew.y, l_rotateNew.x);
		
		PointData l_rotateOld;
			l_rotateOld.x = (((draw1.x - origin.x) / KageStage::currentScale) / _zoomValue) - anchor_rotate.x;
			l_rotateOld.y = (((draw1.y - origin.y) / KageStage::currentScale) / _zoomValue) - anchor_rotate.y;
		double l_angleOld = atan2(l_rotateOld.y, l_rotateOld.x);
		
		double l_angleDiff;
		
		std::vector<VectorData> v = _kage->getFrameData().getVectorData();
		unsigned int l_selectedShapes_size = selectedShapes.size();
		if (l_selectedShapes_size > 0) {
			for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
				for (unsigned int i = selectedShapes[l_shapeIndex]; i < v.size(); ++i) {
					if (v[i].vectorType == VectorData::TYPE_INIT
						&& i != selectedShapes[l_shapeIndex]) {
						break;
					} else if (v[i].vectorType == VectorData::TYPE_INIT) {
						//use as rotation-anchor ONLY if just ONE selected shape
						if (l_selectedShapes_size == 1 && v[i].points.size() == 1) {
							anchor_center.x = v[i].points[0].x;
							anchor_center.y = v[i].points[0].y;
								l_rotateNew.x = (((draw2.x - origin.x) / KageStage::currentScale) / _zoomValue) - anchor_center.x;
								l_rotateNew.y = (((draw2.y - origin.y) / KageStage::currentScale) / _zoomValue) - anchor_center.y;
									l_angleNew = atan2(l_rotateNew.y, l_rotateNew.x);
								l_rotateOld.x = (((draw1.x - origin.x) / KageStage::currentScale) / _zoomValue) - anchor_center.x;
								l_rotateOld.y = (((draw1.y - origin.y) / KageStage::currentScale) / _zoomValue) - anchor_center.y;
									l_angleOld = atan2(l_rotateOld.y, l_rotateOld.x);
						}
					} else if (v[i].vectorType == VectorData::TYPE_MOVE) {
						l_angleDiff = l_angleNew - l_angleOld;
						PointData l_pointRotate;
							l_pointRotate.x = v[i].points[0].x - anchor_center.x;
							l_pointRotate.y = v[i].points[0].y - anchor_center.y;
						double l_pointAngle = atan2(l_pointRotate.y*KageStage::currentScale, l_pointRotate.x*KageStage::currentScale) + l_angleDiff;
						
						//apply new angle
						double l_hyp = sqrtf((l_pointRotate.x * l_pointRotate.x*KageStage::currentScale*KageStage::currentScale) + (l_pointRotate.y * l_pointRotate.y*KageStage::currentScale*KageStage::currentScale));
						v[i].points[0].x = (cos(l_pointAngle) * l_hyp)/KageStage::currentScale + anchor_center.x;
						v[i].points[0].y = (sin(l_pointAngle) * l_hyp)/KageStage::currentScale + anchor_center.y;
					} else if (v[i].vectorType == VectorData::TYPE_CURVE_CUBIC
							|| v[i].vectorType == VectorData::TYPE_CURVE_QUADRATIC) {
						l_angleDiff = l_angleNew - l_angleOld;
						for (unsigned int j = 0; j < 3; ++j) {
							PointData l_pointRotate;
								l_pointRotate.x = v[i].points[j].x - anchor_center.x;
								l_pointRotate.y = v[i].points[j].y - anchor_center.y;
							double l_hyp = sqrtf((l_pointRotate.x * l_pointRotate.x*KageStage::currentScale*KageStage::currentScale) + (l_pointRotate.y * l_pointRotate.y*KageStage::currentScale*KageStage::currentScale));
							
							double l_pointAngle = atan2(l_pointRotate.y*KageStage::currentScale, l_pointRotate.x*KageStage::currentScale) + l_angleDiff;
							//apply new angle
							v[i].points[j].x = (cos(l_pointAngle) * l_hyp)/KageStage::currentScale + anchor_center.x;
							v[i].points[j].y = (sin(l_pointAngle) * l_hyp)/KageStage::currentScale + anchor_center.y;
						}
					} else if (v[i].vectorType == VectorData::TYPE_IMAGE) {
						l_angleDiff = l_angleNew - l_angleOld;
							v[i].points[4].x += l_angleDiff;
					}
				}
			}
			
			renderFrame(cr, v);
			_rotateApply = true;
		}
	} else {
		if (_rotateMode == true) {
			std::vector<VectorData> v = _kage->getFrameData().getVectorData();
			renderFrame(cr, v);
		}
	}
	
	std::vector<double> dashes;
		dashes.push_back(4.0); /* ink */
		dashes.push_back(4.0); /* skip */
	
	cr->set_dash(dashes, 0.0);
	//draw bounding rectangle for the shape
	cr->move_to(      anchor_upperLeft.x*KageStage::currentScale*_zoomValue+origin.x,  anchor_upperLeft.y*KageStage::currentScale*_zoomValue+origin.y);
		cr->line_to( anchor_lowerRight.x*KageStage::currentScale*_zoomValue+origin.x,  anchor_upperLeft.y*KageStage::currentScale*_zoomValue+origin.y);
		cr->line_to( anchor_lowerRight.x*KageStage::currentScale*_zoomValue+origin.x, anchor_lowerRight.y*KageStage::currentScale*_zoomValue+origin.y);
		cr->line_to(  anchor_upperLeft.x*KageStage::currentScale*_zoomValue+origin.x, anchor_lowerRight.y*KageStage::currentScale*_zoomValue+origin.y);
		cr->line_to(  anchor_upperLeft.x*KageStage::currentScale*_zoomValue+origin.x,  anchor_upperLeft.y*KageStage::currentScale*_zoomValue+origin.y);
			cr->set_source_rgba(0.0,0.0,0.0,1.0);
			cr->set_line_width(1.0);
				cr->set_line_cap(Cairo::LINE_CAP_ROUND);
					cr->stroke();
	cr->unset_dash();
}

void KageStage::handleShapes() {
	if (selectedShapes.size() == 0) {
		return;
	}
	
	std::vector<VectorData> v = _kage->getFrameData().getVectorData();
	
	anchor_upperLeft.x = DBL_MAX;
	anchor_upperLeft.y = DBL_MAX;
	
	anchor_lowerRight.x = DBL_MIN;
	anchor_lowerRight.y = DBL_MIN;
	double l_anchorX = 0;
	double l_anchorY = 0;
	unsigned int vsize = v.size();
	unsigned int l_selectedShapes_size = selectedShapes.size();
	for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
		for (unsigned int i = selectedShapes[l_shapeIndex]; i < vsize; ++i) {
			if (v[i].vectorType == VectorData::TYPE_LINE) {
				handleShapes_updateAnchors(v[i].points[0].x, v[i].points[0].y);
			} else if (v[i].vectorType == VectorData::TYPE_CURVE_QUADRATIC
					|| v[i].vectorType == VectorData::TYPE_CURVE_CUBIC) {
				handleShapes_updateAnchors(v[i].points[0].x, v[i].points[0].y);
				handleShapes_updateAnchors(v[i].points[1].x, v[i].points[1].y);
				handleShapes_updateAnchors(v[i].points[2].x, v[i].points[2].y);
			} else if (v[i].vectorType == VectorData::TYPE_IMAGE) {
				//p1 IMAGE_ID_BUFF       x/y == ID / imageBuff
				//p2 IMAGE_X_Y           x/y == x / y
				//p3 IMAGE_WIDTH_HEIGHT  x/y == width / height
				//p4 IMAGE_SCALEX_SCALEY x/y == scaleX / scaleY
				//p5 IMAGE_ROTATE_ALPHA  x/y == rotate / alpha
				double xImagePos   = v[i].points[IMAGE_X_Y].x;
				double yImagePos   = v[i].points[IMAGE_X_Y].y;
				double imageWidth  = v[i].points[IMAGE_WIDTH_HEIGHT].x * v[i].points[IMAGE_SCALEX_SCALEY].x;//(double)cairoImageSurface->get_width() /KageStage::currentScale;
				double imageHeight = v[i].points[IMAGE_WIDTH_HEIGHT].y * v[i].points[IMAGE_SCALEX_SCALEY].y;//(double)cairoImageSurface->get_height()/KageStage::currentScale;
				//TODO: factor-in rotation
				std::cout << "xImagePos\t" << xImagePos << "\tyImagePos\t" << yImagePos << std::endl;
				std::cout << "imageWidth\t" << imageWidth << "\timageHeight\t" << imageHeight << std::endl;
				propX = xImagePos;
				propY = yImagePos;
				propWidth    = imageWidth;
				propHeight   = imageHeight;
				propRotation = v[i].points[4].x;
				handleShapes_updateAnchors(xImagePos             , yImagePos              );
				handleShapes_updateAnchors(xImagePos + imageWidth, yImagePos + imageHeight);
			} else if (v[i].vectorType == VectorData::TYPE_INIT
					&& i != selectedShapes[l_shapeIndex]) {
				break;
			} else if (v[i].vectorType == VectorData::TYPE_INIT) {
				if (l_selectedShapes_size == 1 && v[i].points.size() == 1) {
					l_anchorX = (v[i].points[0].x);
					l_anchorY = (v[i].points[0].y);
				}
			}
		}
	}
	
	std::vector<double> dashes;
		dashes.push_back(4.0); /* ink */
		dashes.push_back(4.0); /* skip */
	
	cr->set_dash(dashes, 0.0);
	//draw bounding rectangle for the shape
	cr->move_to(anchor_upperLeft.x*KageStage::currentScale * _zoomValue + origin.x, anchor_upperLeft.y*KageStage::currentScale * _zoomValue + origin.y);
		cr->line_to( anchor_lowerRight.x*KageStage::currentScale * _zoomValue + origin.x,  anchor_upperLeft.y*KageStage::currentScale * _zoomValue + origin.y);
		cr->line_to( anchor_lowerRight.x*KageStage::currentScale * _zoomValue + origin.x, anchor_lowerRight.y*KageStage::currentScale * _zoomValue + origin.y);
		cr->line_to(  anchor_upperLeft.x*KageStage::currentScale * _zoomValue + origin.x, anchor_lowerRight.y*KageStage::currentScale * _zoomValue + origin.y);
		cr->line_to(  anchor_upperLeft.x*KageStage::currentScale * _zoomValue + origin.x,  anchor_upperLeft.y*KageStage::currentScale * _zoomValue + origin.y);
			cr->set_source_rgba(0.0,0.0,0.0,1.0);
			cr->set_line_width(1.0);
				cr->set_line_cap(Cairo::LINE_CAP_ROUND);
					cr->stroke();
	cr->unset_dash();
	
	if (_rotateMode == true) {
		if (l_selectedShapes_size == 1) {
			anchor_center.x = l_anchorX;
			anchor_center.y = l_anchorY;
		} else {
			if (anchor_rotate.x == 0 && anchor_rotate.y == 0) {
				anchor_center.x = (anchor_upperLeft.x + anchor_lowerRight.x) / 2;
				anchor_center.y = (anchor_upperLeft.y + anchor_lowerRight.y) / 2;
			} else {
				anchor_center.x = anchor_rotate.x;
				anchor_center.y = anchor_rotate.y;
			}
		}
		//draw 4 corner rotate-anchors
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_SE,
				( anchor_upperLeft.x*KageStage::currentScale * _zoomValue) - 13 + origin.x,
				( anchor_upperLeft.y*KageStage::currentScale * _zoomValue) - 13 + origin.y);
			cr->paint();
		
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_SW,
				(anchor_lowerRight.x*KageStage::currentScale * _zoomValue)      + origin.x,
				( anchor_upperLeft.y*KageStage::currentScale * _zoomValue) - 13 + origin.y);
			cr->paint();
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_NW,
				(anchor_lowerRight.x*KageStage::currentScale * _zoomValue)      + origin.x,
				(anchor_lowerRight.y*KageStage::currentScale * _zoomValue)      + origin.y);
			cr->paint();
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_NE,
				( anchor_upperLeft.x*KageStage::currentScale * _zoomValue) - 13 + origin.x,
				(anchor_lowerRight.y*KageStage::currentScale * _zoomValue)      + origin.y);
			cr->paint();
		
		//TODO: once shape-Skewing is implemented, draw skewing-anchors
		/*Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_090,
				(    anchor_center.x*KageStage::currentScale * _zoomValue) -  7 + origin.x,
				( anchor_upperLeft.y*KageStage::currentScale * _zoomValue) - 13 + origin.x);
			cr->paint();
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_090,
				(	 anchor_center.x*KageStage::currentScale * _zoomValue) -  7 + origin.x,
				(anchor_lowerRight.y*KageStage::currentScale * _zoomValue)      + origin.x);
			cr->paint();
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_000,
				( anchor_upperLeft.x*KageStage::currentScale * _zoomValue) - 13 + origin.x,
				(	 anchor_center.y*KageStage::currentScale * _zoomValue) -  7 + origin.x);
			cr->paint();
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_000,
				(anchor_lowerRight.x*KageStage::currentScale * _zoomValue)      + origin.x,
				(	 anchor_center.y*KageStage::currentScale * _zoomValue) -  7 + origin.x);
			cr->paint();*/
		
		//draw rotate anchor
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_ROTATE,
				(	anchor_center.x*KageStage::currentScale * _zoomValue) -  7 + origin.x,
				(	anchor_center.y*KageStage::currentScale * _zoomValue) -  7 + origin.y);
			cr->paint();
	} else {
		anchor_center.x = (anchor_upperLeft.x + anchor_lowerRight.x) / 2;
		anchor_center.y = (anchor_upperLeft.y + anchor_lowerRight.y) / 2;
		
		//draw 4 corners anchors
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_135,
				( anchor_upperLeft.x*KageStage::currentScale * _zoomValue) - 13 + origin.x,
				( anchor_upperLeft.y*KageStage::currentScale * _zoomValue) - 13 + origin.y);
			cr->paint();
		
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_045,
				(anchor_lowerRight.x*KageStage::currentScale * _zoomValue)      + origin.x,
				( anchor_upperLeft.y*KageStage::currentScale * _zoomValue) - 13 + origin.y);
			cr->paint();
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_135,
				(anchor_lowerRight.x*KageStage::currentScale * _zoomValue)      + origin.x,
				(anchor_lowerRight.y*KageStage::currentScale * _zoomValue)      + origin.y);
			cr->paint();
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_045,
				( anchor_upperLeft.x*KageStage::currentScale * _zoomValue) - 13 + origin.x,
				(anchor_lowerRight.y*KageStage::currentScale * _zoomValue)      + origin.y);
			cr->paint();
		
		//draw mid anchors
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_090,
				(    anchor_center.x*KageStage::currentScale * _zoomValue) -  7 + origin.x,
				( anchor_upperLeft.y*KageStage::currentScale * _zoomValue) - 13 + origin.y);
			cr->paint();
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_090,
				(    anchor_center.x*KageStage::currentScale * _zoomValue) -  7 + origin.x,
				(anchor_lowerRight.y*KageStage::currentScale * _zoomValue)      + origin.y);
			cr->paint();
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_000,
				( anchor_upperLeft.x*KageStage::currentScale * _zoomValue) - 13 + origin.x,
				(    anchor_center.y*KageStage::currentScale * _zoomValue) -  7 + origin.y);
			cr->paint();
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_000,
				(anchor_lowerRight.x*KageStage::currentScale * _zoomValue)      + origin.x,
				(    anchor_center.y*KageStage::currentScale * _zoomValue) -  7 + origin.y);
			cr->paint();
		
		//draw move anchor
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_MOVE,
				(    anchor_center.x*KageStage::currentScale * _zoomValue) -  7 + origin.x,
				(    anchor_center.y*KageStage::currentScale * _zoomValue) -  7 + origin.y);
			cr->paint();
	}
}

void KageStage::trySingleSelectShape() {
	if (_mouseLocationShapeIndex != _NO_SELECTION) {
		if (isSelectedShape(_mouseLocationShapeIndex) == true) {
			if (KageStage::toolMode == MODE_SELECT) {
				_rotateMode = !_rotateMode;
			}
		} else {
			if (keyShiftDown == false) {
				selectedShapes.clear();
			}
			addSelectedShape(_mouseLocationShapeIndex);
			_rotateMode = false;
		}
		
		if (_kage->_displayObjectIsShape == true) {
			//this should be able to update Property pane's X/Y Width Height
			getSelectedShapeViaNode(_mouseLocationShapeIndex+3, _kage->getFrameData().getVectorData());
		} else {
			getSelectedShapeViaNode(_mouseLocationShapeIndex+1, _kage->getFrameData().getVectorData());
		}
		
		_kage->propStageSetVisible(false);
		_kage->propFillStrokeSetVisible(true);
		_kage->propDisplayObjectPropertiesSetVisible(true);
		_kage->propNodeXYSetVisible(false);
		
		_kage->updateColors();
//		_kage->updateShapeProperties(); already called inside propShapePropertiesSetVisible()
	} else if (keyShiftDown == false) {
		selectedShapes.clear();
	}
}
void KageStage::tryMultiSelectShapes_populateShapes() {
	selectedShapes.clear();
	std::vector<VectorData> v = _kage->getFrameData().getVectorData();
	if (v.size() == 0) {
		return;
	}
	for (unsigned int i = 0; i < selectedNodes.size(); ++i) {
		addSelectedShape(getShape(selectedNodes[i], v));
	}
}
void KageStage::tryMultiSelectShapes() {
	std::vector<VectorData> v = _kage->getFrameData().getVectorData();
	if (v.size() == 0) {
		return;
	}
	
	if (keyShiftDown == false) {
		selectedNodes.clear();
	}
	unsigned int vsize = v.size();
	for (unsigned int i = 0; i < vsize; ++i) {
		if (v[i].vectorType == VectorData::TYPE_CURVE_CUBIC) {
			if (isNodeOnSelectionBox(v[i].points[0].x, v[i].points[0].y)) {
				if (v[i-1].vectorType == VectorData::TYPE_CURVE_CUBIC
						|| v[i-1].vectorType == VectorData::TYPE_CURVE_QUADRATIC
						|| v[i-1].vectorType == VectorData::TYPE_LINE) {
					selectedNode = i-1; //visualization-wise, the control-point is for the previous Node -- it is its right control-point
					addSelectedNode(selectedNode);
				}	
			}
			if (isNodeOnSelectionBox(v[i].points[1].x, v[i].points[1].y)) {
				selectedNode = i;
				addSelectedNode(selectedNode);
			}
			if (isNodeOnSelectionBox(v[i].points[2].x, v[i].points[2].y)) {
				selectedNode = i;
				addSelectedNode(selectedNode);
			}
		} else if (v[i].vectorType == VectorData::TYPE_IMAGE) {
			double xImagePos   = v[i].points[1].x / KageStage::currentScale;
			double yImagePos   = v[i].points[1].y / KageStage::currentScale;
			double imageWidth  = v[i].points[2].x / KageStage::currentScale * v[i].points[3].x;//(double)cairoImageSurface->get_width() /KageStage::currentScale; //should be dynamic as per user's resize need
			double imageHeight = v[i].points[2].y / KageStage::currentScale * v[i].points[3].y;//(double)cairoImageSurface->get_height()/KageStage::currentScale; //should be dynamic as per user's resize need
			//TODO: factor-in rotation
			if (isNodeOnSelectionBox(xImagePos             , yImagePos              )) {
				selectedNode = i;
				addSelectedNode(selectedNode);
			}
			if (isNodeOnSelectionBox(xImagePos + imageWidth, yImagePos              )) {
				selectedNode = i;
				addSelectedNode(selectedNode);
			}
			if (isNodeOnSelectionBox(xImagePos + imageWidth, yImagePos + imageHeight)) {
				selectedNode = i;
				addSelectedNode(selectedNode);
			}
			if (isNodeOnSelectionBox(xImagePos             , yImagePos + imageHeight)) {
				selectedNode = i;
				addSelectedNode(selectedNode);
			}
		}
	}
	
	tryMultiSelectShapes_populateShapes();
	
	if (selectedShapes.size() > 0) {
		if (KageStage::toolMode == MODE_SELECT) {
			_kage->propStageSetVisible(false);
			_kage->propFillStrokeSetVisible(true);
			_kage->propDisplayObjectPropertiesSetVisible(true);
			_kage->propNodeXYSetVisible(false);
		}
	}
	
	invalidateToRender();
}

unsigned int KageStage::getShape(unsigned int p_index, std::vector<VectorData> p_v) {
	if (p_index == _NO_SELECTION) {
		return -1;
	}
	for (unsigned int i = p_index; i >= 0 && i != UINT_MAX; --i) {
		if (p_v[i].vectorType == VectorData::TYPE_INIT) {
			return i;
		}
	}
	
	return -1;
}

void KageStage::handleShapesMouseUp() {
	/// check if still be used; as of 2019 Nov 24, nothing is calling it
	mouseOnAnchor = AnchorData::TYPE_NONE;
	handleNodesMouseUp();
	
	_kage->updateColors();
	_kage->updateShapeProperties();
}

bool KageStage::cutSelectedShapes() {
	if (_kage->isLayerLocked() == true) {
		return false;
	}
	Kage::timestamp_IN();
	std::cout << " KageStage::cutSelectedShapes " << selectedShapes.size() << std::endl;
	
	if (selectedShapes.size() == 0) {
		Kage::timestamp_OUT();
		return false;
	}
	
	if (copySelectedShapes() == true) {
		Kage::timestamp_OUT();
		return deleteSelectedShapes();
	}
	
	Kage::timestamp_OUT();
	
	return false;
}

bool KageStage::copySelectedShapes() {
	Kage::timestamp_IN();
	std::cout << " KageStage::copySelectedShapes " << selectedShapes.size() << std::endl;
	
	if (selectedShapes.size() == 0) {
		Kage::timestamp_OUT();
		return false;
	}
	
	std::vector<unsigned int> l_selectedShapesOld(selectedShapes);
		sort(l_selectedShapesOld.begin(), l_selectedShapesOld.end());
	
	std::vector<VectorData> v = _kage->getFrameData().getVectorData();
	unsigned int vsize = v.size();
	_vectorDataCopyBuffer.clear();
	unsigned int l_selectedShapes_size = l_selectedShapesOld.size();
	for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
		for (unsigned int i = l_selectedShapesOld[l_shapeIndex]; i < vsize; ++i) {
			if (v[i].vectorType == VectorData::TYPE_INIT
					&& i != l_selectedShapesOld[l_shapeIndex]) {
				break;
			} else {
				_vectorDataCopyBuffer.push_back(v[i]);
			}
		}
	}
	
	_copyBufferMouse.x = draw2.x - origin.x;
	_copyBufferMouse.y = draw2.y - origin.y;
	
	Kage::timestamp_OUT();
	
	return true;
}

bool KageStage::selectAllShapes() {
	Kage::timestamp_IN();
	std::cout << " KageStage::selectAllShapes " << selectedShapes.size() << std::endl;
	
	unsigned int vsize = _kage->getFrameData().getVectorData().size();
	if (vsize == 0) {
		Kage::timestamp_OUT();
		return false;
	}
	
	selectedNodes.clear();
	for (unsigned int i = 0; i < vsize; ++i) {
		selectedNodes.push_back(i);
	}
	
	tryMultiSelectShapes_populateShapes();
	selectedNodes.clear();
	
	Kage::timestamp_OUT();
	
	return true;
}

bool KageStage::deselectSelectedShapes() {
	if (_kage->isLayerLocked() == true) {
		return false;
	}
	Kage::timestamp_IN();
	std::cout << " KageStage::deselectSelectedShapes " << selectedShapes.size() << std::endl;
	
	if (selectedShapes.size() == 0) {
		Kage::timestamp_OUT();
		return false;
	}
	
	initNodeTool();
	_kage->propStageSetVisible(true);
	_kage->propFillStrokeSetVisible(false);
	_kage->propDisplayObjectPropertiesSetVisible(false);
	
	Kage::timestamp_OUT();
	
	return true;
}

bool KageStage::pasteSelectedShapes() {
	if (_kage->isLayerLocked() == true) {
		return false;
	}
	unsigned int l_vectorDataCopyBuffer_size = _vectorDataCopyBuffer.size();
	Kage::timestamp_IN();
	std::cout << " KageStage::pasteSelectedShapes " << l_vectorDataCopyBuffer_size << std::endl;
	
	if (l_vectorDataCopyBuffer_size == 0) {
		Kage::timestamp_OUT();
		return false;
	}
	
	std::vector<VectorData> v = _kage->getFrameData().getVectorData();
	
	selectedNodes.clear();
	for (unsigned int i = 0; i < l_vectorDataCopyBuffer_size; ++i) {
		v.push_back(_vectorDataCopyBuffer[i]);
		//TODO: implement X/Y offset from _copyBufferMouse.x/y and draw2.x/y
		selectedNodes.push_back(v.size()-1);
	}
	
	_kage->setFrameData(v);
	
	_kage->stackDo();
	
	tryMultiSelectShapes_populateShapes();
	selectedNodes.clear();
	
	Kage::timestamp_OUT();
	
	return true;
}

bool KageStage::deleteSelectedShapes() {
	if (_kage->isLayerLocked() == true) {
		return false;
	}
	unsigned int l_selectedShapes_size = selectedShapes.size();
	Kage::timestamp_IN();
	std::cout << " KageStage::deleteSelectedShapes " << l_selectedShapes_size << std::endl;
	
	if (l_selectedShapes_size == 0) {
		Kage::timestamp_OUT();
		return false;
	}
	
	std::vector<unsigned int> l_selectedShapesOld(selectedShapes);
		sort(l_selectedShapesOld.begin(), l_selectedShapesOld.end(), greater  <unsigned int>());
	
	std::vector<VectorData> v = _kage->getFrameData().getVectorData();
	
	unsigned int vsize = v.size();
	std::cout << " vectordata size " << vsize << std::endl;
	if (vsize < l_selectedShapes_size) { //attempt to fix https://sourceforge.net/p/kage/tickets/15/
		initNodeTool();
		Kage::timestamp_OUT();
		return false;
	}
	for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
		unsigned int l_temp = vsize;
		for (unsigned int i = l_selectedShapesOld[l_shapeIndex]; i < vsize; ++i) {
			if (v[i].vectorType == VectorData::TYPE_INIT
					&& i != l_selectedShapesOld[l_shapeIndex]) {
				l_temp = i;
				break;
			}
		}
		if (l_selectedShapesOld[l_shapeIndex] > l_temp) {
			std::cout << "predicting a crash!!!" << std::endl;
		}
		v.erase (v.begin() + l_selectedShapesOld[l_shapeIndex],
				 v.begin() + l_temp);
	}
	selectedShapes.clear();
	
	_kage->setFrameData(v);
	_kage->_timeline.forceRender();
	Kage::timestamp_OUT();
	return true;
}

void KageStage::updateShapeXY() {
	std::vector<VectorData> v = _kage->getFrameData().getVectorData();
	
	propX = DBL_MAX;
	propXindex1 = UINT_MAX;
	propXindex2 = UINT_MAX;
	propY = DBL_MAX;
	propYindex1 = UINT_MAX;
	propYindex2 = UINT_MAX;
	
	unsigned int vsize = v.size();
	unsigned int l_selectedShapes_size = selectedShapes.size();
	for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
		if (_kage->_displayObjectIsShape == true) {
			//calling getSelectedShapeViaNode will update propXindex1/propXindex2 based on left-most VectorData.x
			getSelectedShapeViaNode(selectedShapes[l_shapeIndex]+3, v);
		} else {
			getSelectedShapeViaNode(selectedShapes[l_shapeIndex]+1, v);
		}
	}
	
	unsigned int nsize = selectedNodes.size();
	if (nsize == 0) {
		nodeX = 0;
		nodeY = 0;
		nodeIndexX = UINT_MAX;
		nodeIndexY = UINT_MAX;
	} else {
		nodeX = DBL_MAX;
		nodeY = DBL_MAX;
		std::vector<VectorData> v = _kage->getFrameData().getVectorData();
		unsigned int vsize = v.size();
		for (unsigned int i = 0; i < nsize; ++i) {
			for (unsigned int j = i; j < vsize; ++j) {
				if (isSelectedNode(j) == true) {
					if (       v[j].vectorType == VectorData::TYPE_CURVE_QUADRATIC
							|| v[j].vectorType == VectorData::TYPE_CURVE_CUBIC) {
						if (v[j].points[2].x < nodeX) {
							nodeX = v[j].points[2].x;
							nodeIndexX = j;
						}
						if (v[j].points[2].y < nodeY) {
							nodeY = v[j].points[2].y;
							nodeIndexY = j;
						}
					} else if (v[j].vectorType == VectorData::TYPE_INIT
							&& j != selectedShapes[i]) {
						break;
					} else if (v[j].vectorType == VectorData::TYPE_IMAGE) {
						if (v[j].points[1].x < nodeX) {
							nodeX = v[j].points[1].x;
							nodeIndexX = j;
						}
						if (v[j].points[1].y < nodeY) {
							nodeY = v[j].points[1].y;
							nodeIndexY = j;
						}
					}
				}
			}
		}
		if (nodeX == DBL_MAX) {
			nodeX = 0;
			nodeIndexX = UINT_MAX;
		}
		if (nodeY == DBL_MAX) {
			nodeY = 0;
			nodeIndexY = UINT_MAX;
		}
	}
	if (propX == DBL_MAX && propY == DBL_MAX) {
		_kage->propDisplayObjectPropertiesSetVisible(false);
	} else {
		_kage->propDisplayObjectPropertiesSetVisible(true);
	}
}

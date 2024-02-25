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

#include "vectordatamanager.h"
#include "../kage.h"
#include "stage/stage.h"

VectorDataManager::VectorDataManager() {
//	init(KageStage::fillColor, KageStage::stroke);
// ^ depracated: TYPE_INIT to be used as SHAPE identifier
	_selectedNodes.clear();
	_selectedShapes.clear();
	vectorData.clear();
}

VectorDataManager::VectorDataManager(std::vector<VectorData> p_vectorData) {
	_selectedNodes.clear();
	_selectedShapes.clear();
	vectorData.clear();
	
	setVectorData(p_vectorData);
}

VectorDataManager::~VectorDataManager() {
	//TODO: revisit
	//_selectedNodes.clear();
	//_selectedShapes.clear();
	//vectorData.clear();
}

void VectorDataManager::setVectorData(std::vector<VectorData> p_vectorData) {
	vectorData = p_vectorData;
}

std::vector<VectorData> VectorDataManager::getVectorData() {
	return vectorData;
}

void VectorDataManager::clear() {
	vectorData.clear();
}

void VectorDataManager::push(VectorDataManager p_vectorsData) {
	std::vector<VectorData> l_v = p_vectorsData.getVectorData();
	int isrc = l_v.size();
	
	for (int i = 0; i < isrc; ++i) {
		vectorData.push_back(l_v[i]);
	}
}

void VectorDataManager::add(VectorData::type p_type, ColorData p_fill, StrokeColorData p_stroke) {
	VectorData l_vectorData;
	
		l_vectorData.vectorType = p_type;
		l_vectorData.stroke = p_stroke;
		l_vectorData.fillColor = p_fill;
		l_vectorData.fillColorGradientID = "";
	
	vectorData.push_back(l_vectorData);
}
void VectorDataManager::add(VectorData::type p_type, std::string p_fillGradientID, StrokeColorData p_stroke) {
	VectorData l_vectorData;
	
		l_vectorData.vectorType = p_type;
		l_vectorData.stroke = p_stroke;
		l_vectorData.fillColor = ColorData();
		l_vectorData.fillColorGradientID = p_fillGradientID;
	
	vectorData.push_back(l_vectorData);
}

void VectorDataManager::addInit() {
	PointData l_point(0.0, 0.0);
	addInit(l_point);
}
void VectorDataManager::addInit(PointData p_point) {
	VectorData l_vectorData;
	if (vectorData.size() > 0) {
		l_vectorData = vectorData[vectorData.size()-1];
	} else {
		l_vectorData.setType(VectorData::TYPE_INIT);
		l_vectorData.fillColor = KageStage::fillColor;
		l_vectorData.stroke = KageStage::stroke;
	}
	add(VectorData::TYPE_INIT, l_vectorData.fillColor, l_vectorData.stroke);
	
	std::vector<PointData> ps;
		ps.push_back(p_point);
	vectorData[vectorData.size()-1].setPoints(ps);
}

bool VectorDataManager::recenterRotationPoint(std::vector<unsigned int> p_selectedShapes) {
	if (p_selectedShapes.size() == 0) {
		return false;
	}
	
	std::vector<unsigned int> l_selectedShapesOld(p_selectedShapes);
		sort(l_selectedShapesOld.begin(), l_selectedShapesOld.end(), std::greater <unsigned int>());
	
	std::vector<VectorData> v = getVectorData();
	
	unsigned l_initIndex = UINT_MAX;
	
	bool l_return = false;
	
	for (unsigned int l_selectedShape = 0; l_selectedShape < l_selectedShapesOld.size(); ++l_selectedShape) {
		double l_leftMost = DBL_MAX;
		double l_rightMost = DBL_MIN;
		double l_topMost = DBL_MAX;
		double l_bottomMost = DBL_MIN;
		
		unsigned int vsize = v.size();
		for (unsigned int i = l_selectedShapesOld[l_selectedShape]; i < vsize; ++i) {
			if (       v[i].vectorType == VectorData::TYPE_INIT
					&& i != l_selectedShapesOld[l_selectedShape]) {
				break;
			} else if (v[i].vectorType == VectorData::TYPE_INIT) {
				l_initIndex = i;
			} else if (v[i].vectorType == VectorData::TYPE_MOVE) {
				if (v[i].points[0].x < l_leftMost) {
					l_leftMost   = v[i].points[0].x;
				} else if (v[i].points[0].x > l_rightMost) {
					l_rightMost  = v[i].points[0].x;
				}
				if (v[i].points[0].y < l_topMost) {
					l_topMost    = v[i].points[0].y;
				} else if (v[i].points[0].y > l_bottomMost) {
					l_bottomMost = v[i].points[0].y;
				}
			} else if (v[i].vectorType == VectorData::TYPE_CURVE_CUBIC
					|| v[i].vectorType == VectorData::TYPE_CURVE_QUADRATIC) {
				for (unsigned int j = 0; j < 3; ++j) {
					if (       v[i].points[j].x < l_leftMost) {
						l_leftMost   = v[i].points[j].x;
					} else if (v[i].points[j].x > l_rightMost) {
						l_rightMost  = v[i].points[j].x;
					}
					if (       v[i].points[j].y < l_topMost) {
						l_topMost    = v[i].points[j].y;
					} else if (v[i].points[j].y > l_bottomMost) {
						l_bottomMost = v[i].points[j].y;
					}
				}
			} else if (v[i].vectorType == VectorData::TYPE_IMAGE) {
				//TODO: factor-in rotation
				//for (unsigned int j = 0; j < 3; ++j) {
				unsigned int j = 1;
					if (v[i].points[j].x < l_leftMost) {
						l_leftMost   = v[i].points[j].x;
					}
					if (v[i].points[j].x > l_rightMost) {
						l_rightMost  = v[i].points[j].x;
					}
					if (v[i].points[j].y < l_topMost) {
						l_topMost    = v[i].points[j].y;
					}
					if (v[i].points[j].y > l_bottomMost) {
						l_bottomMost = v[i].points[j].y;
					}
				j = 2;
					if (v[i].points[1].x + v[i].points[j].x < l_leftMost) {
						l_leftMost   = v[i].points[1].x + v[i].points[j].x;
					}
					if (v[i].points[1].x + v[i].points[j].x > l_rightMost) {
						l_rightMost  = v[i].points[1].x + v[i].points[j].x;
					}
					if (v[i].points[1].y + v[i].points[j].y < l_topMost) {
						l_topMost    = v[i].points[1].y + v[i].points[j].y;
					}
					if (v[i].points[1].x + v[i].points[j].y > l_bottomMost) {
						l_bottomMost = v[i].points[1].y + v[i].points[j].y;
					}
				//}
			}
		}
		
		if (l_initIndex != UINT_MAX) {
			PointData l_point((l_leftMost + l_rightMost ) / 2,
							  (l_topMost  + l_bottomMost) / 2);
			std::vector<PointData> ps;
				ps.push_back(l_point);
			v[l_initIndex].setPoints(ps);
			l_return = true;
		}
	}
	
	setVectorData(v);
	
	return l_return;
}

void VectorDataManager::addFill(ColorData p_color) {
	VectorData l_vectorData;
	if (vectorData.size() > 0) {
		l_vectorData = vectorData[vectorData.size()-1];
	} else {
		l_vectorData.setType(VectorData::TYPE_INIT);
		l_vectorData.fillColor = KageStage::fillColor;
		l_vectorData.stroke = KageStage::stroke;
	}
	add(VectorData::TYPE_FILL, p_color, l_vectorData.stroke);
}

void VectorDataManager::addFill(std::string p_gradientID) {
	VectorData l_vectorData;
	if (vectorData.size() > 0) {
		l_vectorData = vectorData[vectorData.size()-1];
	} else {
		l_vectorData.setType(VectorData::TYPE_INIT);
		l_vectorData.fillColor = KageStage::fillColor;
		l_vectorData.stroke = KageStage::stroke;
	}
	add(VectorData::TYPE_FILL, p_gradientID, l_vectorData.stroke);
}

void VectorDataManager::addEndFill() {
	ColorData p_fill(0,0,0,0);
	StrokeColorData p_stroke(0.0);
		p_stroke.setA(0);
	add(VectorData::TYPE_ENDFILL, p_fill, p_stroke);
}

VectorDataManager VectorDataManager::clone() {
	std::vector<VectorData> l_vectorData;
	unsigned int count = vectorData.size();
		for (unsigned int i = 0; i < count; ++i) {
			l_vectorData.push_back(vectorData[i].clone());
		}
	VectorDataManager l_vectorDataManager(l_vectorData);
	
	return l_vectorDataManager;
}

void VectorDataManager::addCubic(PointData p_point1, PointData p_point2, PointData p_point3) {
	addCurve(p_point1, p_point2, p_point3, VectorData::TYPE_CURVE_CUBIC);
}

void VectorDataManager::addQuadratic(PointData p_point1, PointData p_point2, PointData p_point3) {
	addCurve(p_point1, p_point2, p_point3, VectorData::TYPE_CURVE_QUADRATIC);
}

void VectorDataManager::addLinePoly(PointData p_point, double p_x, double p_y) {
	VectorData l_vectorData;
	if (vectorData.size() > 0) {
		l_vectorData = vectorData[vectorData.size()-1];
	} else {
		l_vectorData.setType(VectorData::TYPE_INIT);
		l_vectorData.fillColor = KageStage::fillColor;
		l_vectorData.stroke = KageStage::stroke;
	}
	add(VectorData::TYPE_CURVE_CUBIC, l_vectorData.fillColor, l_vectorData.stroke);
	
	std::vector<PointData> ps;
		ps.push_back(
			PointData(
				(p_x + p_point.x)/2,
				(p_y + p_point.y)/2
			)
				);
		ps.push_back(
			PointData(
				(p_x + p_point.x)/2,
				(p_y + p_point.y)/2
			)
				);
		ps.push_back(p_point);
	vectorData[vectorData.size()-1].setPoints(ps);
}

void VectorDataManager::addLine(PointData p_point) {
	std::vector<PointData> p_prevPoints = vectorData[vectorData.size()-1].getPoints();
	
	VectorData l_vectorData = vectorData[vectorData.size()-1];
	add(VectorData::TYPE_CURVE_CUBIC, l_vectorData.fillColor, l_vectorData.stroke);
	
	std::vector<PointData> ps;
		ps.push_back(
			PointData(
				(p_prevPoints[p_prevPoints.size()-1].x + p_point.x)/2,
				(p_prevPoints[p_prevPoints.size()-1].y + p_point.y)/2
			)
				);
		ps.push_back(
			PointData(
				(p_prevPoints[p_prevPoints.size()-1].x + p_point.x)/2,
				(p_prevPoints[p_prevPoints.size()-1].y + p_point.y)/2
			)
				);
		ps.push_back(p_point);
	vectorData[vectorData.size()-1].setPoints(ps);
}

void VectorDataManager::addMove(PointData p_point) {
	VectorData l_vectorData;
	if (vectorData.size() > 0) {
		l_vectorData = vectorData[vectorData.size()-1];
	} else {
		l_vectorData.setType(VectorData::TYPE_INIT);
		l_vectorData.fillColor = KageStage::fillColor;
		l_vectorData.stroke = KageStage::stroke;
	}
	add(VectorData::TYPE_MOVE, l_vectorData.fillColor, l_vectorData.stroke);
	
	std::vector<PointData> ps;
		ps.push_back(p_point);
	vectorData[vectorData.size()-1].setPoints(ps);
}

void VectorDataManager::addLineStyle(StrokeColorData p_stroke) {
	VectorData l_vectorData;
	if (vectorData.size() > 0) {
		l_vectorData = vectorData[vectorData.size()-1];
	} else {
		l_vectorData.setType(VectorData::TYPE_INIT);
		l_vectorData.fillColor = KageStage::fillColor;
		l_vectorData.stroke = KageStage::stroke;
	}
	add(VectorData::TYPE_STROKE, l_vectorData.fillColor, p_stroke);
}

void VectorDataManager::addClosePath() {
	VectorData l_vectorData;
	if (vectorData.size() > 0) {
		l_vectorData = vectorData[vectorData.size()-1];
	} else {
		l_vectorData.setType(VectorData::TYPE_INIT);
		l_vectorData.fillColor = KageStage::fillColor;
		l_vectorData.stroke = KageStage::stroke;
	}
	add(VectorData::TYPE_CLOSE_PATH, l_vectorData.fillColor, l_vectorData.stroke);
}

void VectorDataManager::addCurve(PointData p_point1, PointData p_point2, PointData p_point3, VectorData::type p_curveType) {
	VectorData l_vectorData;
	if (vectorData.size() > 0) {
		l_vectorData = vectorData[vectorData.size()-1];
	} else {
		l_vectorData.setType(VectorData::TYPE_INIT);
		l_vectorData.fillColor = KageStage::fillColor;
		l_vectorData.stroke = KageStage::stroke;
	}
	add(p_curveType, l_vectorData.fillColor, l_vectorData.stroke);
	
	std::vector<PointData> ps;
		ps.push_back(p_point1);
		ps.push_back(p_point2);
		ps.push_back(p_point3);
	vectorData[vectorData.size()-1].setPoints(ps);
}

void VectorDataManager::addImage(PointData p_IDandBuff, PointData p_xy, PointData p_size, PointData p_scale, PointData p_rotateAlpha) {
	VectorData l_vectorData;
	if (vectorData.size() > 0) {
		l_vectorData = vectorData[vectorData.size()-1];
	} else {
		l_vectorData.setType(VectorData::TYPE_INIT);
		l_vectorData.fillColor = KageStage::fillColor;
		l_vectorData.stroke = KageStage::stroke;
	}
	add(VectorData::TYPE_IMAGE, l_vectorData.fillColor, l_vectorData.stroke);
	
	std::vector<PointData> ps;
		ps.push_back(p_IDandBuff);
		ps.push_back(p_xy);
		ps.push_back(p_size);
		ps.push_back(p_scale);
		ps.push_back(p_rotateAlpha);
	vectorData[vectorData.size()-1].setPoints(ps);
}
bool VectorDataManager::isEmpty() {
	return (vectorData.size() == 0);
}

unsigned int VectorDataManager::getShape(unsigned int p_index) {
	if (p_index == _NO_SELECTION) {
		return -1;
	}
	for (unsigned int i = p_index; i >= 0 && i != UINT_MAX; --i) {
		if (vectorData[i].vectorType == VectorData::TYPE_INIT) {
			return i;
		}
	}
	
	return -1;
}
bool VectorDataManager::isSelectedShape(unsigned int p_index) {
	bool l_gotit = false;
	unsigned int ssize = _selectedShapes.size();
	for (unsigned int i = 0; i < ssize; ++i) {
		if (_selectedShapes[i] == p_index) {
			l_gotit = true;
			break;
		}
	}
	
	return l_gotit;
}
void VectorDataManager::addSelectedShape(unsigned int p_index) {
	if (p_index == _NO_SELECTION) return;
	if (isSelectedShape(p_index) == false) {
		_selectedShapes.push_back(p_index);
		//do we need below code from stage-shape.cpp?
		//anchor_rotate.x = 0;
		//anchor_rotate.y = 0;
	}
}


std::vector<unsigned int> VectorDataManager::tryMultiSelectShapes_populateShapes() {
	_selectedShapes.clear();
	if (vectorData.size() == 0) {
		return _selectedShapes;
	}
	for (unsigned int i = 0; i < _selectedNodes.size(); ++i) {
		addSelectedShape(getShape(_selectedNodes[i]));
	}
	return _selectedShapes;
}

std::vector<unsigned int> VectorDataManager::selectAllShapes() {
	std::vector<unsigned int> l_selectedShapes;
	l_selectedShapes.clear();
	
	unsigned int vsize = vectorData.size();
	if (vsize == 0) {
		return l_selectedShapes;
	}
	
	_selectedNodes.clear();
	for (unsigned int i = 0; i < vsize; ++i) {
		_selectedNodes.push_back(i);
	}
	
	l_selectedShapes = tryMultiSelectShapes_populateShapes();
	_selectedNodes.clear();
	
	return l_selectedShapes;
}

bool VectorDataManager::cutSelectedShapes() {
	//TODO: cut-prevention should be done on Frame as Frame got access to _layer->isLocked()
	//if (_kage->isLayerLocked() == true) {
	//	return false;
	//}
	if (_selectedShapes.size() == 0) {
		return false;
	}
	std::vector<VectorData> l_copiedItem;
	l_copiedItem = copySelectedShapes(_selectedShapes);
	if (l_copiedItem.size() > 0) {
		if (deleteSelectedShapes(_selectedShapes) == true) {
			_selectedShapes.clear();
			return true;
		}
	}
	
	return false;
}

std::vector<VectorData> VectorDataManager::copySelectedShapes(std::vector<unsigned int> p_selectedShapes) {
	std::vector<VectorData> l_vectorDataCopyBuffer;
	l_vectorDataCopyBuffer.clear();
	
	if (p_selectedShapes.size() == 0) {
		return l_vectorDataCopyBuffer;
	}
	
	std::vector<unsigned int> l_selectedShapesOld(p_selectedShapes);
		sort(l_selectedShapesOld.begin(), l_selectedShapesOld.end());
	
	unsigned int vsize = vectorData.size();
	
	unsigned int l_selectedShapes_size = l_selectedShapesOld.size();
	for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
		for (unsigned int i = l_selectedShapesOld[l_shapeIndex]; i < vsize; ++i) {
			if (vectorData[i].vectorType == VectorData::TYPE_INIT
					&& i != l_selectedShapesOld[l_shapeIndex]) {
				break;
				//are we not copying INITs?
			} else {
				l_vectorDataCopyBuffer.push_back(vectorData[i]);
			}
		}
	}
	
	return l_vectorDataCopyBuffer;
}

std::vector<unsigned int> VectorDataManager::pasteSelectedShapes(std::vector<VectorData> p_vectorDataCopyBuffer) {
	unsigned int l_vectorDataCopyBuffer_size = p_vectorDataCopyBuffer.size();
	
	_selectedNodes.clear();
	if (l_vectorDataCopyBuffer_size == 0) {
		std::cout << "VectorDataManager::pasteSelectedShapes FAIL" << std::endl;
		return _selectedNodes;
	}
	
	for (unsigned int i = 0; i < l_vectorDataCopyBuffer_size; ++i) {
		vectorData.push_back(p_vectorDataCopyBuffer[i]);
		_selectedNodes.push_back(vectorData.size()-1);
	}
	
	return _selectedNodes;
}

bool VectorDataManager::deleteSelectedShapes(std::vector<unsigned int> p_selectedShapes) {
	unsigned int l_selectedShapes_size = p_selectedShapes.size();
	
	if (l_selectedShapes_size == 0) {
		std::cout << "deleteSelectedShapes A " << std::endl;
		return false;
	}
	
	std::vector<unsigned int> l_selectedShapesOld(p_selectedShapes);
		sort(l_selectedShapesOld.begin(), l_selectedShapesOld.end(), std::greater  <unsigned int>());
	
	unsigned int vsize = vectorData.size();
	if (vsize < l_selectedShapes_size) { //attempt to fix https://sourceforge.net/p/kage/tickets/15/
		//initNodeTool();
		std::cout << "deleteSelectedShapes B " << std::endl;
		return false;
	}
	for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
		unsigned int l_temp = vsize;
		for (unsigned int i = l_selectedShapesOld[l_shapeIndex]; i < vsize; ++i) {
			if (vectorData[i].vectorType == VectorData::TYPE_INIT
					&& i != l_selectedShapesOld[l_shapeIndex]) {
				l_temp = i;
				break;
			}
		}
		if (l_selectedShapesOld[l_shapeIndex] > l_temp) {
			std::cout << "predicting a crash!!!" << std::endl;
		}
		vectorData.erase (vectorData.begin() + l_selectedShapesOld[l_shapeIndex],
		                    vectorData.begin() + l_temp);
	}
	
	std::cout << "deleteSelectedShapes C " << std::endl;
	return true;
}


/** Multiple selected Shapes can be raised one step above its z-ordering by
 *  splicing the VectorData array into group of four.
 *  1.) Whatever shape is behind the selected Shape
 *  2.) The selected Shape itself
 *  3.) The one directly infront of the selected Shape
 *  4.) All others infront of selected Shape less the one infront of it
 * These four groups will be merged back to one in this exact order:
 *  1 + 3 + 2 + 4
  \return True if it successfully lowered back selected Shape
  \sa lowerSelectedShape(), raiseToTopSelectedShape() and lowerToBottomSelectedShape()
*/
std::vector<unsigned int> VectorDataManager::raiseSelectedShape(std::vector<unsigned int> p_selectedShapes) {
	std::vector<unsigned int> l_selectedShapes;
	l_selectedShapes.clear();
	if (p_selectedShapes.size() == 0) {
		return l_selectedShapes;
	}
	
	l_selectedShapes = p_selectedShapes;
		sort(l_selectedShapes.begin(), l_selectedShapes.end(), std::greater <unsigned int>());
	std::vector<unsigned int> l_selectedShapesNew;
		l_selectedShapesNew.clear();
	
	std::vector<VectorData> l_vectorDataZOrderBufferA;
	std::vector<VectorData> l_vectorDataZOrderBufferB;
	std::vector<VectorData> l_vectorDataZOrderBufferC;
	l_vectorDataZOrderBufferA.clear();
	l_vectorDataZOrderBufferB.clear();
	l_vectorDataZOrderBufferC.clear();
	
	for (unsigned int l_selectedShape = 0; l_selectedShape < l_selectedShapes.size(); ++l_selectedShape) {
		unsigned int vsize = vectorData.size();
		unsigned int l_temp = UINT_MAX;
		unsigned int l_shapeBstart = UINT_MAX;
		for (unsigned int i = l_selectedShapes[l_selectedShape]; i < vsize; ++i) {
			if (vectorData[i].vectorType == VectorData::TYPE_INIT
					&& i != l_selectedShapes[l_selectedShape]) {			
				l_shapeBstart = i;
				l_temp = i;
				for (i = i; i < vsize; ++i) {
					if (vectorData[i].vectorType == VectorData::TYPE_INIT
							&& i != l_shapeBstart) {
						for (i = i; i < vsize; ++i) {
							l_vectorDataZOrderBufferC.push_back(vectorData[i]);
						}
						l_temp = vsize;
						break;
					} else {
						l_vectorDataZOrderBufferB.push_back(vectorData[i]);
						l_temp = i+1;
					}
				}
				break;
			} else {
				l_vectorDataZOrderBufferA.push_back(vectorData[i]);
			}
		}
		if (l_temp == UINT_MAX) {
			l_vectorDataZOrderBufferA.clear();
			l_selectedShapesNew.push_back(l_selectedShapes[l_selectedShape]);
		} else {
			vectorData.erase (vectorData.begin() + l_selectedShapes[l_selectedShape],
					vectorData.begin() + l_temp);
				for (unsigned int i = 0; i < l_vectorDataZOrderBufferB.size(); ++i) {
					vectorData.push_back(l_vectorDataZOrderBufferB[i]);
				}
					l_vectorDataZOrderBufferB.clear();
				
				l_selectedShapesNew.push_back(vectorData.size());
				
				for (unsigned int i = 0; i < l_vectorDataZOrderBufferA.size(); ++i) {
					vectorData.push_back(l_vectorDataZOrderBufferA[i]);
				}
					l_vectorDataZOrderBufferA.clear();
				
				for (unsigned int i = 0; i < l_vectorDataZOrderBufferC.size(); ++i) {
					vectorData.push_back(l_vectorDataZOrderBufferC[i]);
				}
					l_vectorDataZOrderBufferC.clear();
		}
	}
	if (l_selectedShapesNew.size() > 0) {
		l_selectedShapes.clear();
		for (unsigned int i = 0; i < l_selectedShapesNew.size(); ++i) {
			l_selectedShapes.push_back(l_selectedShapesNew[i]);
		}
	}
	
	return l_selectedShapes;
}

/** Multiple selected Shapes can be lowered one step below its z-ordering by
 *  splicing the VectorData array into group of four.
 *  1.) Whatever shape is behind the selected Shape less the one behind selected Shape
 *  2.) The one behind the selected Shape
 *  3.) The selected Shape itself
 *  4.) All others infront of selected Shape
 * These four groups will be merged back to one in this exact order:
 *  1 + 3 + 2 + 4
  \return True if it successfully lowered back selected Shape
  \sa raiseSelectedShape(), raiseToTopSelectedShape() and lowerToBottomSelectedShape()
*/
std::vector<unsigned int> VectorDataManager::lowerSelectedShape(std::vector<unsigned int> p_selectedShapes) {
	std::vector<unsigned int> l_selectedShapes;
	l_selectedShapes.clear();
	if (p_selectedShapes.size() == 0) {
		return l_selectedShapes;
	}
	
	l_selectedShapes = p_selectedShapes;
		sort(l_selectedShapes.begin(), l_selectedShapes.end(), std::greater <unsigned int>());
	std::vector<unsigned int> l_selectedShapesNew;
		l_selectedShapesNew.clear();
	
	std::vector<VectorData> l_vectorDataZOrderBufferA;
	std::vector<VectorData> l_vectorDataZOrderBufferB;
	std::vector<VectorData> l_vectorDataZOrderBufferC;
	l_vectorDataZOrderBufferA.clear();
	l_vectorDataZOrderBufferB.clear();
	l_vectorDataZOrderBufferC.clear();
	
	for (unsigned int l_selectedShape = 0; l_selectedShape < l_selectedShapes.size(); ++l_selectedShape) {
		unsigned int vsize = vectorData.size();
		unsigned int l_temp = vsize;
		for (unsigned int i = l_selectedShapes[l_selectedShape]; i < vsize; ++i) {
			if (vectorData[i].vectorType == VectorData::TYPE_INIT
					&& i != l_selectedShapes[l_selectedShape]) {
				for (i = i; i < vsize; ++i) {
					l_vectorDataZOrderBufferC.push_back(vectorData[i]);
				}
				l_temp = i;
				break;
			} else {
				l_vectorDataZOrderBufferA.push_back(vectorData[i]);
			}
		}
			for (unsigned int i = l_selectedShapes[l_selectedShape]-1; i >= 0 && i != UINT_MAX; --i) {
				if (vectorData[i].vectorType == VectorData::TYPE_INIT) {
					unsigned int l_tempSelectedShape = i;
					for (i = i; i < l_selectedShapes[l_selectedShape]; ++i) {
						if (vectorData[i].vectorType == VectorData::TYPE_INIT
								&& i != l_tempSelectedShape) {
							break;
						} else {
							l_vectorDataZOrderBufferB.push_back(vectorData[i]);
						}
					}
					l_selectedShapes[l_selectedShape] = l_tempSelectedShape;
					break;
				}
			}
		
		vectorData.erase (vectorData.begin() + l_selectedShapes[l_selectedShape],
				vectorData.begin() + l_temp);
			
			l_selectedShapes[l_selectedShape] = vectorData.size();
			
			l_selectedShapesNew.push_back(vectorData.size());
			
			for (unsigned int i = 0; i < l_vectorDataZOrderBufferA.size(); ++i) {
				vectorData.push_back(l_vectorDataZOrderBufferA[i]);
			}
				l_vectorDataZOrderBufferA.clear();
			
			for (unsigned int i = 0; i < l_vectorDataZOrderBufferB.size(); ++i) {
				vectorData.push_back(l_vectorDataZOrderBufferB[i]);
			}
				l_vectorDataZOrderBufferB.clear();
			
			for (unsigned int i = 0; i < l_vectorDataZOrderBufferC.size(); ++i) {
				vectorData.push_back(l_vectorDataZOrderBufferC[i]);
			}
				l_vectorDataZOrderBufferC.clear();
	}
	if (l_selectedShapesNew.size() > 0) {
		l_selectedShapes.clear();
		for (unsigned int i = 0; i < l_selectedShapesNew.size(); ++i) {
			l_selectedShapes.push_back(l_selectedShapesNew[i]);
		}
	}
	
	return l_selectedShapes;
}

/** To move selected item to the top of z-ordering,<br/>
 *  1. Copy selected shapes to Buffer<br/>
 *  2. Delete selected shapes<br/>
 *  3. Paste Buffer<br/>
 *  \return True if it successfully raised selected Shape on top of z-ordering
 *  \sa raiseSelectedShape(), lowerSelectedShape() and lowerToBottomSelectedShape()
 */
std::vector<unsigned int> VectorDataManager::raiseToTopSelectedShape(std::vector<unsigned int> p_selectedShapes) {
	std::vector<unsigned int> l_selectedShapes;
	l_selectedShapes.clear();
	if (p_selectedShapes.size() == 0) {
		return l_selectedShapes;
	}
	
	l_selectedShapes = p_selectedShapes;
	
	std::vector<VectorData> l_vectorDataCopyBuffer = copySelectedShapes(l_selectedShapes);
	if (l_vectorDataCopyBuffer.size() > 0) {
		if (deleteSelectedShapes(p_selectedShapes) == true) {
			return pasteSelectedShapes(l_vectorDataCopyBuffer);
		}
	}
	
	l_selectedShapes.clear();
	return l_selectedShapes;
}

/** To move selected item to the bottom of z-ordering,<br/>
 *  1. Copy selected shapes to Buffer<br/>
 *  2. Delete selected shapes<br/>
 *  3. Append remaining shapes to Buffer<br/>
 *  4. Delete all remaining shapes<br/>
 *  5. Paste Buffer
 *  \return True if it successfully lowered selected Shape to bottom of z-ordering
 *  \sa raiseSelectedShape(), lowerSelectedShape() and raiseToTopSelectedShape()
 */
std::vector<unsigned int> VectorDataManager::lowerToBottomSelectedShape(std::vector<unsigned int> p_selectedShapes) {
	std::vector<unsigned int> l_selectedShapes;
	l_selectedShapes.clear();
	if (p_selectedShapes.size() == 0) {
		return l_selectedShapes;
	}
	
	l_selectedShapes = p_selectedShapes;
	
	std::vector<VectorData> l_vectorDataCopyBuffer = copySelectedShapes(l_selectedShapes);
	if (l_vectorDataCopyBuffer.size() > 0) {
		if (deleteSelectedShapes(p_selectedShapes) == true) {
			l_selectedShapes.clear();
			for (unsigned int i = 0; i < l_vectorDataCopyBuffer.size(); ++i) {
				if (l_vectorDataCopyBuffer[i].vectorType == VectorData::TYPE_INIT) {
					l_selectedShapes.push_back(i);
				}
			}
			std::vector<VectorData> v = getVectorData();
				for (unsigned int i = 0; i < v.size(); ++i) {
					l_vectorDataCopyBuffer.push_back(v[i]);
				}
			
			setVectorData(l_vectorDataCopyBuffer);
			return l_selectedShapes;
		}
	}
	
	l_selectedShapes.clear();
	return l_selectedShapes;
}

std::vector<unsigned int> VectorDataManager::groupSelectedShapes(std::vector<unsigned int> p_selectedShapes) {
	std::vector<unsigned int> l_selectedShapes;
	l_selectedShapes.clear();
	if (p_selectedShapes.size() == 0) {
		return l_selectedShapes;
	}
	
	l_selectedShapes = p_selectedShapes;
	
	std::vector<VectorData> l_vectorDataCopyBuffer = copySelectedShapes(l_selectedShapes);
	if (l_vectorDataCopyBuffer.size() > 0) {
		if (deleteSelectedShapes(p_selectedShapes) == true) {			
			///erase index if vectorType is TYPE INIT
			for (unsigned int i = 0; i < l_vectorDataCopyBuffer.size(); ++i) {
				if (l_vectorDataCopyBuffer[i].vectorType == VectorData::TYPE_INIT
						&& i != 0) {
					l_vectorDataCopyBuffer.erase( l_vectorDataCopyBuffer.begin() + i );
				}
			}
			return pasteSelectedShapes(l_vectorDataCopyBuffer);
		}
	}
	
	l_selectedShapes.clear();
	return l_selectedShapes;
}
std::vector<unsigned int> VectorDataManager::ungroupSelectedShapes(std::vector<unsigned int> p_selectedShapes) {
	std::vector<unsigned int> l_selectedShapes;
	l_selectedShapes.clear();
	if (p_selectedShapes.size() == 0) {
		return l_selectedShapes;
	}
	
	l_selectedShapes = p_selectedShapes;
	
	std::vector<VectorData> l_vectorDataCopyBuffer = copySelectedShapes(l_selectedShapes);
	if (l_vectorDataCopyBuffer.size() > 0) {
		if (deleteSelectedShapes(p_selectedShapes) == true) {			
			///insert TYPE INIT between ENDFILL and FILL
			std::vector<VectorData> l_vectorDataZOrderBuffer;
			l_vectorDataZOrderBuffer.clear();
			for (unsigned int i = 0; i < l_vectorDataCopyBuffer.size(); ++i) {
				l_vectorDataZOrderBuffer.push_back(l_vectorDataCopyBuffer[i]);
				if (l_vectorDataCopyBuffer[i].vectorType == VectorData::TYPE_ENDFILL
						&& i+1 != l_vectorDataCopyBuffer.size()
						&& l_vectorDataCopyBuffer[i+1].vectorType == VectorData::TYPE_FILL) {
					VectorData l_vectorData;
						l_vectorData.vectorType = VectorData::TYPE_INIT;
						l_vectorData.stroke = StrokeColorData();
						l_vectorData.fillColor = ColorData();
					l_vectorDataZOrderBuffer.push_back(l_vectorData);
						std::vector<PointData> ps;
							PointData l_point(0.0, 0.0);
								ps.push_back(l_point);
						l_vectorDataZOrderBuffer[l_vectorDataZOrderBuffer.size()-1].setPoints(ps);
				}
			}
			l_vectorDataCopyBuffer.clear();
			for (unsigned int i = 0; i < l_vectorDataZOrderBuffer.size(); ++i) {
				l_vectorDataCopyBuffer.push_back(l_vectorDataZOrderBuffer[i]);
			}
			return pasteSelectedShapes(l_vectorDataCopyBuffer);
		}
	}
	
	l_selectedShapes.clear();
	return l_selectedShapes;
}

std::vector<unsigned int> VectorDataManager::duplicateShapes(std::vector<unsigned int> p_selectedShapes) {
	std::vector<unsigned int> l_selectedShapes;
	l_selectedShapes.clear();
	if (p_selectedShapes.size() == 0) {
		return l_selectedShapes;
	}
	
	l_selectedShapes = p_selectedShapes;
	
	std::vector<VectorData> l_vectorDataCopyBuffer = copySelectedShapes(l_selectedShapes);
	if (l_vectorDataCopyBuffer.size() > 0) {
		return pasteSelectedShapes(l_vectorDataCopyBuffer);
	}
	
	l_selectedShapes.clear();
	return l_selectedShapes;
}
bool VectorDataManager::flipHorizontalSelectedShape(std::vector<unsigned int> p_selectedShapes) {
	if (p_selectedShapes.size() == 0) {
		return false;
	}
	
	std::vector<unsigned int> l_selectedShapesOld(p_selectedShapes);
		sort(l_selectedShapesOld.begin(), l_selectedShapesOld.end(), std::greater <unsigned int>());
	
	double l_leftPoint = DBL_MAX;
	double l_rightPoint = DBL_MIN;
	double l_midPoint = 0;
	
	/// 1. get left-most, right-most, mid-point
	for (unsigned int l_selectedShape = 0; l_selectedShape < l_selectedShapesOld.size(); ++l_selectedShape) {
		unsigned int vsize = vectorData.size();
		for (unsigned int i = l_selectedShapesOld[l_selectedShape]; i < vsize; ++i) {
			if (vectorData[i].vectorType == VectorData::TYPE_INIT
					&& i != l_selectedShapesOld[l_selectedShape]) {
				break;
			} else if (vectorData[i].vectorType == VectorData::TYPE_CURVE_CUBIC
					|| vectorData[i].vectorType == VectorData::TYPE_CURVE_QUADRATIC) {
				if (vectorData[i].points[0].x < l_leftPoint) {
					l_leftPoint = vectorData[i].points[0].x;
				} else if (vectorData[i].points[0].x > l_rightPoint) {
					l_rightPoint = vectorData[i].points[0].x;
				}
				if (vectorData[i].points[1].x < l_leftPoint) {
					l_leftPoint = vectorData[i].points[1].x;
				} else if (vectorData[i].points[1].x > l_rightPoint) {
					l_rightPoint = vectorData[i].points[1].x;
				}
				if (vectorData[i].points[2].x < l_leftPoint) {
					l_leftPoint = vectorData[i].points[2].x;
				} else if (vectorData[i].points[2].x > l_rightPoint) {
					l_rightPoint = vectorData[i].points[2].x;
				}
			} else if (vectorData[i].vectorType == VectorData::TYPE_MOVE) {
				if (vectorData[i].points[0].x < l_leftPoint) {
					l_leftPoint = vectorData[i].points[0].x;
				} else if (vectorData[i].points[0].x > l_rightPoint) {
					l_rightPoint = vectorData[i].points[0].x;
				}
			}
		}
	}
	l_midPoint = (l_leftPoint + l_rightPoint) / 2;
	
	/// 2. loop among selected shapes' points and invert point against location in left-mid-right
	for (unsigned int l_selectedShape = 0; l_selectedShape < l_selectedShapesOld.size(); ++l_selectedShape) {
		unsigned int vsize = vectorData.size();
		for (unsigned int i = l_selectedShapesOld[l_selectedShape]; i < vsize; ++i) {
			if (vectorData[i].vectorType == VectorData::TYPE_INIT
					&& i != l_selectedShapesOld[l_selectedShape]) {
				break;
			} else if (vectorData[i].vectorType == VectorData::TYPE_CURVE_CUBIC
					|| vectorData[i].vectorType == VectorData::TYPE_CURVE_QUADRATIC) {
				if (vectorData[i].points[0].x < l_midPoint) {
					vectorData[i].points[0].x += ((l_midPoint-vectorData[i].points[0].x) * 2);
				} else if (vectorData[i].points[0].x > l_midPoint) {
					vectorData[i].points[0].x -= ((vectorData[i].points[0].x-l_midPoint) * 2);
				}
				if (vectorData[i].points[1].x < l_midPoint) {
					vectorData[i].points[1].x += ((l_midPoint-vectorData[i].points[1].x) * 2);
				} else if (vectorData[i].points[1].x > l_midPoint) {
					vectorData[i].points[1].x -= ((vectorData[i].points[1].x-l_midPoint) * 2);
				}
				if (vectorData[i].points[2].x < l_midPoint) {
					vectorData[i].points[2].x += ((l_midPoint-vectorData[i].points[2].x) * 2);
				} else if (vectorData[i].points[2].x > l_midPoint) {
					vectorData[i].points[2].x -= ((vectorData[i].points[2].x-l_midPoint) * 2);
				}
			} else if (vectorData[i].vectorType == VectorData::TYPE_MOVE) {
				if (vectorData[i].points[0].x < l_midPoint) {
					vectorData[i].points[0].x += ((l_midPoint-vectorData[i].points[0].x) * 2);
				} else if (vectorData[i].points[0].x > l_midPoint) {
					vectorData[i].points[0].x -= ((vectorData[i].points[0].x-l_midPoint) * 2);
				}
			}
		}
	}
	
	return true;
}
bool VectorDataManager::flipVerticalSelectedShape(std::vector<unsigned int> p_selectedShapes) {
	if (p_selectedShapes.size() == 0) {
		return false;
	}
	
	std::vector<unsigned int> l_selectedShapesOld(p_selectedShapes);
		sort(l_selectedShapesOld.begin(), l_selectedShapesOld.end(), std::greater <unsigned int>());
	
	double l_upperPoint = DBL_MAX;
	double l_lowerPoint = DBL_MIN;
	double l_midPoint = 0;
	
	/// 1. get upper-most, lower-most, mid-point
	for (unsigned int l_selectedShape = 0; l_selectedShape < l_selectedShapesOld.size(); ++l_selectedShape) {
		unsigned int vsize = vectorData.size();
		for (unsigned int i = l_selectedShapesOld[l_selectedShape]; i < vsize; ++i) {
			if (vectorData[i].vectorType == VectorData::TYPE_INIT
					&& i != l_selectedShapesOld[l_selectedShape]) {
				break;
			} else if (vectorData[i].vectorType == VectorData::TYPE_CURVE_CUBIC
					|| vectorData[i].vectorType == VectorData::TYPE_CURVE_QUADRATIC) {
				if (vectorData[i].points[0].y < l_upperPoint) {
					l_upperPoint = vectorData[i].points[0].y;
				} else if (vectorData[i].points[0].y > l_lowerPoint) {
					l_lowerPoint = vectorData[i].points[0].y;
				}
				if (vectorData[i].points[1].y < l_upperPoint) {
					l_upperPoint = vectorData[i].points[1].y;
				} else if (vectorData[i].points[1].y > l_lowerPoint) {
					l_lowerPoint = vectorData[i].points[1].y;
				}
				if (vectorData[i].points[2].y < l_upperPoint) {
					l_upperPoint = vectorData[i].points[2].y;
				} else if (vectorData[i].points[2].y > l_lowerPoint) {
					l_lowerPoint = vectorData[i].points[2].y;
				}
			} else if (vectorData[i].vectorType == VectorData::TYPE_MOVE) {
				if (vectorData[i].points[0].y < l_upperPoint) {
					l_upperPoint = vectorData[i].points[0].y;
				} else if (vectorData[i].points[0].y > l_lowerPoint) {
					l_lowerPoint = vectorData[i].points[0].y;
				}
			}
		}
	}
	l_midPoint = (l_upperPoint + l_lowerPoint) / 2;
	
	/// 2. loop among selected shapes' points and invert point against location in upper-mid-lower
	for (unsigned int l_selectedShape = 0; l_selectedShape < l_selectedShapesOld.size(); ++l_selectedShape) {
		unsigned int vsize = vectorData.size();
		for (unsigned int i = l_selectedShapesOld[l_selectedShape]; i < vsize; ++i) {
			if (vectorData[i].vectorType == VectorData::TYPE_INIT
					&& i != l_selectedShapesOld[l_selectedShape]) {
				break;
			} else if (vectorData[i].vectorType == VectorData::TYPE_CURVE_CUBIC
					|| vectorData[i].vectorType == VectorData::TYPE_CURVE_QUADRATIC) {
				if (vectorData[i].points[0].y < l_midPoint) {
					vectorData[i].points[0].y += ((l_midPoint-vectorData[i].points[0].y) * 2);
				} else if (vectorData[i].points[0].y > l_midPoint) {
					vectorData[i].points[0].y -= ((vectorData[i].points[0].y-l_midPoint) * 2);
				}
				if (vectorData[i].points[1].y < l_midPoint) {
					vectorData[i].points[1].y += ((l_midPoint-vectorData[i].points[1].y) * 2);
				} else if (vectorData[i].points[1].y > l_midPoint) {
					vectorData[i].points[1].y -= ((vectorData[i].points[1].y-l_midPoint) * 2);
				}
				if (vectorData[i].points[2].y < l_midPoint) {
					vectorData[i].points[2].y += ((l_midPoint-vectorData[i].points[2].y) * 2);
				} else if (vectorData[i].points[2].y > l_midPoint) {
					vectorData[i].points[2].y -= ((vectorData[i].points[2].y-l_midPoint) * 2);
				}
			} else if (vectorData[i].vectorType == VectorData::TYPE_MOVE) {
				if (vectorData[i].points[0].y < l_midPoint) {
					vectorData[i].points[0].y += ((l_midPoint-vectorData[i].points[0].y) * 2);
				} else if (vectorData[i].points[0].y > l_midPoint) {
					vectorData[i].points[0].y -= ((vectorData[i].points[0].y-l_midPoint) * 2);
				}
			}
		}
	}
	
	return true;
}

/*
 * Kage Studio - a simple free and open source vector-based 2D animation software
 * Copyright (C) 2011~2022  Mj Mendoza IV
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
#include "../kage.h"
#include <cairomm/context.h>

void KageStage::initNodeTool() {
	selectedNode = -1;
	selectedShape = -1;
	selectedNodes.clear();
	selectedShapes.clear();
	_mouseLocationShapeIndex = _NO_SELECTION;
	
	_rotateMode = false;
}

bool KageStage::isSelectedNode(unsigned int p_index) {
	bool l_gotit = false;
	unsigned int nsize = selectedNodes.size();
	if (nsize == 0) {
		return false;
	}
	
	for (unsigned int i = 0; i < nsize; ++i) {
		if (selectedNodes[i] == p_index) {
			l_gotit = true;
			break;
		}
	}
	return l_gotit;
}
void KageStage::addSelectedNode(unsigned int p_index) {
	if (p_index == _NO_SELECTION) return;
	if (isSelectedNode(p_index) == false) {
		selectedNodes.push_back(p_index);
		if (KageStage::toolMode == MODE_NODE) {
			updateNodeXY();
		}
	}
}

void KageStage::updateNodeX(double p_value, bool p_stackDo) {
	Kage::timestamp_IN();
	std::cout << " KageStage::updateNodeX " << selectedNodes.size() << " " << p_value << std::endl;
	
	if (selectedNodes.size() == 0) {
		Kage::timestamp_OUT();
		return;
	}
	
	double l_propXdiff = 0;
	vector<VectorData> v = win->getFrameData().getVectorData();
	std::cout << " nodeIndexX " << nodeIndexX << " " << isSelectedNode(nodeIndexX) << endl;
	//calculate xDIFF from selected base Node
	for (unsigned int l_selectedNode = 0; l_selectedNode < selectedNodes.size(); ++l_selectedNode) {
		if (selectedNodes[l_selectedNode] == nodeIndexX) {
			l_propXdiff = p_value - v[selectedNodes[l_selectedNode]].points[2].x;
			break;
		}
	}
	std::cout << " l_propXdiff " << l_propXdiff << endl;
	for (unsigned int l_selectedNode = 0; l_selectedNode < selectedNodes.size(); ++l_selectedNode) {
//		l_propXdiff = p_value - v[selectedNodes[l_selectedNode]].points[2].x;
		if (v[selectedNodes[l_selectedNode]].vectorType == VectorData::TYPE_CURVE_CUBIC) {
			v[selectedNodes[l_selectedNode]].points[1].x += l_propXdiff;
			v[selectedNodes[l_selectedNode]].points[2].x += l_propXdiff;
			
			if (v[selectedNodes[l_selectedNode]+1].getPoints().size() != 0) {
				v[selectedNodes[l_selectedNode]+1].points[0].x += l_propXdiff;
			} else {
			//this part of code is trying to resolve this problematic code
			// above code is problematic if for last Node; this will use shape's first Node's point 0 instead
				std::cout << " KageStage::updateNodeX points.size is zero" << std::endl;
				for (unsigned int i = selectedNodes[l_selectedNode]-1; i >= 0; --i) {
					if (v[i].vectorType == VectorData::TYPE_MOVE) {
						std::cout << " KageStage::updateNodeX got MOVE" << std::endl;
						if (i+1 != selectedNodes[l_selectedNode]) {
							std::cout << " KageStage::updateNodeX moving control point :) " << std::endl;
							v[i  ].points[0].x += l_propXdiff; // for Node MOVE
							v[i+1].points[0].x += l_propXdiff; // for Node Curve's control point being used by Node MOVE
						}
						break;
					}
				}
			}
		} else if (v[selectedNodes[l_selectedNode]].vectorType == VectorData::TYPE_IMAGE) {
			//TODO:handle X for images
		}
	}
	
	win->setFrameData(v);
	
	if (p_stackDo) {
		win->stackDo();
	}
	
	invalidateToRender();
	
	Kage::timestamp_OUT();
}
void KageStage::updateNodeY(double p_value, bool p_stackDo) {
	Kage::timestamp_IN();
	std::cout << " KageStage::updateNodeY " << selectedNodes.size() << " " << p_value << std::endl;
	
	if (selectedNodes.size() == 0) {
		Kage::timestamp_OUT();
		return;
	}
	
	double l_propYdiff = 0;
	vector<VectorData> v = win->getFrameData().getVectorData();
	
	//calculate yDIFF from selected base Node
	for (unsigned int l_selectedNode = 0; l_selectedNode < selectedNodes.size(); ++l_selectedNode) {
		if (selectedNodes[l_selectedNode] == nodeIndexY) {
			l_propYdiff = p_value - v[selectedNodes[l_selectedNode]].points[2].y;
			break;
		}
	}
	for (unsigned int l_selectedNode = 0; l_selectedNode < selectedNodes.size(); ++l_selectedNode) {
//		l_propYdiff = p_value - v[selectedNodes[l_selectedNode]].points[2].y;
		if (v[selectedNodes[l_selectedNode]].vectorType == VectorData::TYPE_CURVE_CUBIC) {
			v[selectedNodes[l_selectedNode]].points[1].y += l_propYdiff;
			v[selectedNodes[l_selectedNode]].points[2].y += l_propYdiff;
			
			if (v[selectedNodes[l_selectedNode]+1].getPoints().size() != 0) {
				v[selectedNodes[l_selectedNode]+1].points[0].y += l_propYdiff;
			} else {
				for (unsigned int i = selectedNodes[l_selectedNode]-1; i >= 0; --i) {
					if (v[i].vectorType == VectorData::TYPE_MOVE) {
						if (i+1 != selectedNodes[l_selectedNode]) {
							v[i  ].points[0].y += l_propYdiff;
							v[i+1].points[0].y += l_propYdiff;
						}
						break;
					}
				}
			}
		} else if (v[selectedNodes[l_selectedNode]].vectorType == VectorData::TYPE_IMAGE) {
			//TODO:handle Y for images
		}
	}
	
	win->setFrameData(v);
	
	if (p_stackDo) {
		win->stackDo();
	}
	
	invalidateToRender();
	
	Kage::timestamp_OUT();
}




void KageStage::handleNodes() {
	handleNodes_selection();
	handleNodes_relocation();
	handleNodes_rendering();
}
void KageStage::handleNodes_selection() {
	if (mouseDown == true && _isModifyingShape == false) {
		drawSelectionArea();
	}
	
	vector<VectorData> v = win->getFrameData().getVectorData();
	unsigned int vsize = v.size();
	mouseOnNodeHover = UINT_MAX;
	unsigned int typeMovesIndex = _NO_SELECTION;
	bool l_shapeGotClose = false;
	for (unsigned int l_selectedShape = 0; l_selectedShape < selectedShapes.size(); ++l_selectedShape) {
		for (unsigned int i = selectedShapes[l_selectedShape]; i < vsize; ++i) {
			switch (v[i].vectorType) {
				case VectorData::TYPE_CLOSE_PATH:
					l_shapeGotClose = true;
					break;
				case VectorData::TYPE_FILL:
					l_shapeGotClose = false;
					break;
				case VectorData::TYPE_ENDFILL:
					if (l_shapeGotClose == false && typeMovesIndex != _NO_SELECTION) {
						//check if mouse is on Move' location
						if (isMouseOnNode(
								v[typeMovesIndex].points[0].x + origin.x,
								v[typeMovesIndex].points[0].y + origin.y) == true) {
							mouseOnNodeHover = typeMovesIndex;
							if (mouseDown == true) {
								if (mouseOnNode == _NO_SELECTION) {
									mouseOnNode = typeMovesIndex;
								}
								if (isSelectedNode(typeMovesIndex) == false && keyShiftDown == false) {
									selectedNodes.clear();
								}
									addSelectedNode(typeMovesIndex);
								if (mouseOnNodeIndex == _NO_SELECTION) {
									mouseOnNodeIndex = 3;
								}
								mouseOnNodeOffset.x = _mouseLocation.x - v[typeMovesIndex].points[0].x - origin.x;
								mouseOnNodeOffset.y = _mouseLocation.y - v[typeMovesIndex].points[0].y - origin.y;
							}
						}
					}
					i = vsize;
					break;
				case VectorData::TYPE_STROKE:
					break;
				case VectorData::TYPE_MOVE:
					typeMovesIndex = i;
				case VectorData::TYPE_LINE:
					break;
				case VectorData::TYPE_CURVE_QUADRATIC:
				case VectorData::TYPE_CURVE_CUBIC:
					//check anchor
					if (isMouseOnNode(
							v[i].points[2].x + origin.x,
							v[i].points[2].y + origin.y) == true) {
						mouseOnNodeHover = i;
						if (mouseDown == true) {
							if (mouseOnNode == _NO_SELECTION) {
								mouseOnNode = i;
							}
							if (isSelectedNode(i) == false && keyShiftDown == false) {
								selectedNodes.clear();
							}
								addSelectedNode(i);
							if (mouseOnNodeIndex == _NO_SELECTION) {
								mouseOnNodeIndex = 3;
							}
							mouseOnNodeOffset.x = _mouseLocation.x - v[i].points[2].x - origin.x;
							mouseOnNodeOffset.y = _mouseLocation.y - v[i].points[2].y - origin.y;
						}
					}
					
					//check control points
					if (isMouseOnNode(
							v[i].points[0].x + origin.x,
							v[i].points[0].y + origin.y) == true) {
						if (mouseDown == true) {
							if (mouseOnNode == _NO_SELECTION) {
								mouseOnNode = i;
							}
							if (keyShiftDown == false) { selectedNodes.clear(); }
							if (i > 0 && (
									v[i-1].vectorType == VectorData::TYPE_LINE
									|| v[i-1].vectorType == VectorData::TYPE_CURVE_QUADRATIC
									|| v[i-1].vectorType == VectorData::TYPE_CURVE_CUBIC)) {
								addSelectedNode(i-1);
							}
							if (mouseOnNodeIndex == _NO_SELECTION) {
								mouseOnNodeIndex = 1;
							}
						}
					}
					
					if (isMouseOnNode(
							v[i].points[1].x + origin.x,
							v[i].points[1].y + origin.y) == true) {
						if (mouseDown == true) {
							if (mouseOnNode == _NO_SELECTION) {
								mouseOnNode = i;
							}
							if (keyShiftDown == false) { selectedNodes.clear(); }
							addSelectedNode(i);
							if (mouseOnNodeIndex == _NO_SELECTION) {
								mouseOnNodeIndex = 2;
							}
						}
					}
					break;
				case VectorData::TYPE_TEXT:
				case VectorData::TYPE_IMAGE:
				case VectorData::TYPE_INIT:
					typeMovesIndex = _NO_SELECTION;
				default:
					break;
			}
		}
	}
}
void KageStage::handleNodes_relocation() {
	if (selectedShapes.size() == 0) {
		return;
	}
	
	vector<VectorData> v = win->getFrameData().getVectorData();
	bool l_editingNode = false;
	double l_x;
	double l_y;
	unsigned int typeMovesIndex = _NO_SELECTION;
	unsigned int typeEndsPointSize;
	unsigned int typeCubicCurvesIndex = _NO_SELECTION;
	bool l_useEndPoint = false;
	unsigned int vsize = v.size();
	bool l_shapeGotClose = false;
	PointData g_typeMovesXY;
	for (unsigned int l_selectedShape = 0; l_selectedShape < selectedShapes.size(); ++l_selectedShape) {
		for (unsigned int i = selectedShapes[l_selectedShape]; i < vsize; ++i) {
			switch (v[i].vectorType) {
				case VectorData::TYPE_CLOSE_PATH:
					l_shapeGotClose = true;
					if (typeMovesIndex != UINT_MAX) {
						typeEndsPointSize = v[i-1].points.size()-1;
						if (v[typeMovesIndex].points[0].x != v[typeMovesIndex].points[typeEndsPointSize].x
								|| v[typeMovesIndex].points[0].y != v[typeMovesIndex].points[typeEndsPointSize].y) {
							v[typeMovesIndex].points[0].x = v[i-1].points[typeEndsPointSize].x;
							v[typeMovesIndex].points[0].y = v[i-1].points[typeEndsPointSize].y;
						}
						typeMovesIndex = UINT_MAX;
					}
					l_useEndPoint = true;
					break;
				case VectorData::TYPE_FILL:
					l_shapeGotClose = false;
					break;
				case VectorData::TYPE_ENDFILL:
					///make sure last point before end fill is in-sync with MOVE(shape's start point)
					if (typeMovesIndex != UINT_MAX && mouseDown == true && mouseOnNode == i-1) {
						if (l_useEndPoint == true) {
							typeEndsPointSize = v[i-1].points.size()-1;
							if (v[typeMovesIndex].points[0].x != v[typeMovesIndex].points[typeEndsPointSize].x
									|| v[typeMovesIndex].points[0].y != v[typeMovesIndex].points[typeEndsPointSize].y) {
								v[typeMovesIndex].points[0].x = v[i-1].points[typeEndsPointSize].x;
								v[typeMovesIndex].points[0].y = v[i-1].points[typeEndsPointSize].y;
							}
						}
					}
					if (l_shapeGotClose == false && mouseDown == true && mouseOnNode == typeMovesIndex && mouseOnNodeIndex == 3) {
						l_editingNode = true;
							l_x = ((draw1.x - origin.x - mouseOnNodeOffset.x) - v[typeMovesIndex].points[0].x);
							l_y = ((draw1.y - origin.y - mouseOnNodeOffset.y) - v[typeMovesIndex].points[0].y);
							if (keyControlDown == true) {
								if (keyShiftDown == true) {
									l_x = 0; //restraint on X axis
								} else {
									l_y = 0; //restraint on Y axis
								}
							}
							for (unsigned int j = 0; j < selectedNodes.size(); ++j) {
								unsigned int k = selectedNodes[j];
								//move control A of selected node's next curve
								if (k+1 < v.size()-1 && v[k+1].vectorType == VectorData::TYPE_CURVE_CUBIC) {
									v[k+1].points[0].x += l_x;
									v[k+1].points[0].y += l_y;
								} else if (k+2 < vsize && v[k+2].vectorType == VectorData::TYPE_ENDFILL) {
									///get index of MOVE of selectedNode
									for (unsigned int l_move_index = k; l_move_index > 0; --l_move_index) {
										if (v[l_move_index].vectorType == VectorData::TYPE_MOVE) {
											v[l_move_index+1].points[0].x += l_x;
											v[l_move_index+1].points[0].y += l_y;
											break;
										}
									}
								}
								//move anchor of selected node
								if (k != typeMovesIndex) {
									v[k].points[2].x += l_x;
									v[k].points[2].y += l_y;
								}
							}
							v[typeMovesIndex].points[0].x += l_x;
							v[typeMovesIndex].points[0].y += l_y;
					}
					i = vsize;
					break;
				case VectorData::TYPE_STROKE:
					break;
				case VectorData::TYPE_MOVE:
					///register i for later use in ENDFILL
					typeMovesIndex = i;
					break;
				case VectorData::TYPE_LINE:
					if (mouseDown == true && mouseOnNode == i) {
						v[i].points[0].x = draw1.x - origin.x;
						v[i].points[0].y = draw1.y - origin.y;
					}
					l_x = v[i].points[0].x + origin.x;
					l_y = v[i].points[0].y + origin.y;
					
					if (isMouseOnNode(l_x, l_y) == true) {
						if (mouseDown == true) {
							v[i].points[0].x = draw1.x - origin.x;
							v[i].points[0].y = draw1.y - origin.y;
							
							l_x = v[i].points[0].x + origin.x;
							l_y = v[i].points[0].y + origin.y;
						}
					}
					break;
				case VectorData::TYPE_CURVE_QUADRATIC:
				case VectorData::TYPE_CURVE_CUBIC:
					///register i for later use in ENDFILL <-- note from newer handling of Poly
					if (typeCubicCurvesIndex == _NO_SELECTION) {
						typeCubicCurvesIndex = i;
					}
					
					//relocate anchors
					if (mouseDown == true && mouseOnNode == i && mouseOnNodeIndex == 3) {
						l_editingNode = true;
							l_x = ((draw1.x - origin.x - mouseOnNodeOffset.x) - v[i].points[2].x);
							l_y = ((draw1.y - origin.y - mouseOnNodeOffset.y) - v[i].points[2].y);
							if (keyControlDown == true) {
								if (keyShiftDown == true) {
									l_x = 0; //restraint on X axis
								} else {
									l_y = 0; //restraint on Y axis
								}
							}
							for (unsigned int j = 0; j < selectedNodes.size(); ++j) {
								unsigned int k = selectedNodes[j];
								//move control B of selected node's current curve
								v[k].points[1].x += l_x;
								v[k].points[1].y += l_y;
								//move control A of selected node's next curve
								if (k+1 < v.size()-1 && v[k+1].vectorType == VectorData::TYPE_CURVE_CUBIC) {
									v[k+1].points[0].x += l_x;
									v[k+1].points[0].y += l_y;
								} else if (k+2 < vsize && v[k+2].vectorType == VectorData::TYPE_ENDFILL) {
									///get index of MOVE of selectedNode
									for (unsigned int l_move_index = k; l_move_index > 0; --l_move_index) {
										if (v[l_move_index].vectorType == VectorData::TYPE_MOVE) {
											v[l_move_index+1].points[0].x += l_x;
											v[l_move_index+1].points[0].y += l_y;
											break;
										}
									}
								}
								//move anchor of selected node
								if (k != i) {
									v[k].points[2].x += l_x;
									v[k].points[2].y += l_y;
								}
							}
							
							v[i].points[2].x += l_x;
							v[i].points[2].y += l_y;
					}
					
					//relocate control points
					if (mouseDown == true && mouseOnNode == i && mouseOnNodeIndex == 1) {
						l_editingNode = true;
						v[i].points[0].x = draw1.x - origin.x;
						v[i].points[0].y = draw1.y - origin.y;
					}
					
					if (mouseDown == true && mouseOnNode == i && mouseOnNodeIndex == 2) {
						l_editingNode = true;
						v[i].points[1].x = draw1.x - origin.x;
						v[i].points[1].y = draw1.y - origin.y;
					}
					break;
				case VectorData::TYPE_TEXT:
				case VectorData::TYPE_IMAGE:
				case VectorData::TYPE_INIT:
				default:
					break;
			}
		}
	}
	
	if (l_editingNode == true) {
		win->setFrameData(v);
	}
//	cout << " \t l_editingNode " << l_editingNode << endl;
	_isModifyingShape = l_editingNode;
}
void KageStage::handleNodes_rendering() {
	if (selectedShapes.size() == 0) {
		return;
	}
	
	vector<VectorData> v = win->getFrameData().getVectorData();
	double l_x;
	double l_y;
	bool l_shapeGotClose = false;
	unsigned int vsize = v.size();
	unsigned int typeMovesIndex = _NO_SELECTION;
	for (unsigned int l_selectedShape = 0; l_selectedShape < selectedShapes.size(); ++l_selectedShape) {
		for (unsigned int i = selectedShapes[l_selectedShape]; i < vsize; ++i) {
			switch (v[i].vectorType) {
				case VectorData::TYPE_CLOSE_PATH:
					l_shapeGotClose = true;
					break;
				case VectorData::TYPE_FILL:
					l_shapeGotClose = false;
					break;
				case VectorData::TYPE_ENDFILL:
//					i = vsize;
					if (l_shapeGotClose == false && typeMovesIndex != UINT_MAX) {
						//draw anchors
						cr->move_to(
							v[typeMovesIndex].points[0].x + origin.x,
							v[typeMovesIndex].points[0].y + origin.y
						);
						
						l_x = v[typeMovesIndex].points[0].x + origin.x;
						l_y = v[typeMovesIndex].points[0].y + origin.y;
						if (mouseOnNodeHover == typeMovesIndex) {
							renderNode(l_x, l_y, 1);
						} else {
							if (selectedNodes.size() == 0) {
								renderNode(l_x, l_y, 0);
							} else {
								bool l_selectedNode = false;
								for (unsigned int j = 0; j < selectedNodes.size(); ++j) {
									if (selectedNodes[j] == i) {
										l_selectedNode = true;
										break;
									}
								}
								if (l_selectedNode == true) {
									renderNode(l_x, l_y, 2);
								} else {
									renderNode(l_x, l_y, 0);
								}
							}
						}
					}
					break;
				case VectorData::TYPE_STROKE:
					break;
				case VectorData::TYPE_MOVE:
					typeMovesIndex = i;
					break;
				case VectorData::TYPE_LINE:
					if (isMouseOnNode(l_x, l_y) == true) {
						renderNode(l_x, l_y, 1);
					} else {
						renderNode(l_x, l_y, 0);
					}
					break;
				case VectorData::TYPE_CURVE_QUADRATIC:
				case VectorData::TYPE_CURVE_CUBIC:
					//draw lines to anchor
					cr->move_to(
						v[i-1].points[v[i-1].points.size()-1].x + origin.x,
						v[i-1].points[v[i-1].points.size()-1].y + origin.y
					);
						cr->line_to(
							v[i].points[0].x + origin.x,
							v[i].points[0].y + origin.y
						);
						cr->set_source_rgba(0.0,0.7,0.7,1.0);
						cr->set_line_width(1.0);
							cr->set_line_cap(Cairo::Context::LineCap::ROUND);
								cr->stroke();
					
					cr->move_to(
						v[i].points[1].x + origin.x,
						v[i].points[1].y + origin.y
					);
						cr->line_to(
							v[i].points[2].x + origin.x,
							v[i].points[2].y + origin.y
						);
						cr->set_source_rgba(0.0,0.7,0.7,1.0);
						cr->set_line_width(1.0);
							cr->set_line_cap(Cairo::Context::LineCap::ROUND);
								cr->stroke();
					
					//draw anchors
					cr->move_to(
						v[i].points[2].x + origin.x,
						v[i].points[2].y + origin.y
					);
					
					l_x = v[i].points[2].x + origin.x;
					l_y = v[i].points[2].y + origin.y;
					if (mouseOnNodeHover == i) {
						renderNode(l_x, l_y, 1);
					} else {
						if (selectedNodes.size() == 0) {
							renderNode(l_x, l_y, 0);
						} else {
							bool l_selectedNode = false;
							for (unsigned int j = 0; j < selectedNodes.size(); ++j) {
								if (selectedNodes[j] == i) {
									l_selectedNode = true;
									break;
								}
							}
							if (l_selectedNode == true) {
								renderNode(l_x, l_y, 2);
							} else {
								renderNode(l_x, l_y, 0);
							}
						}
					}
					
					//draw control points
					if (mouseDown == true && mouseOnNode == i && mouseOnNodeIndex == 1) {
						renderNodeControl(v[i].points[0].x + origin.x, v[i].points[0].y + origin.y, true);
					} else {
						renderNodeControl(v[i].points[0].x + origin.x, v[i].points[0].y + origin.y, false);
					}
					
					if (mouseDown == true && mouseOnNode == i && mouseOnNodeIndex == 2) {
						renderNodeControl(v[i].points[1].x + origin.x, v[i].points[1].y + origin.y, true);
					} else {
						renderNodeControl(v[i].points[1].x + origin.x, v[i].points[1].y + origin.y, false);
					}
					break;
				case VectorData::TYPE_TEXT:
				case VectorData::TYPE_IMAGE:
				case VectorData::TYPE_INIT:
					if (i != selectedShapes[l_selectedShape]) {
						i = vsize;
					}
				default:
					break;
			}
		}
	}
}
void KageStage::handleNodesMouseDown() {
//	handleNodesMouseUp();
}
void KageStage::handleNodesMouseUp() {
	_nodeToMouseDistance = 1080;
	vector<VectorData> v = win->getFrameData().getVectorData();
	if (v.size() == 0) {
		return;
	}
	selectedNode = _NO_SELECTION;
	mouseOnNode = _NO_SELECTION;
	if (keyShiftDown == false) {
		selectedShapes.clear();
	}
	selectedNodes.clear();
	unsigned int vsize = v.size();
	unsigned int l_vStartIndex = 0;
	if (_mouseLocationShapeIndex != _NO_SELECTION) {
		l_vStartIndex = _mouseLocationShapeIndex;
	}
	for (unsigned int i = l_vStartIndex; i < vsize; ++i) {
		if (v[i].vectorType == VectorData::TYPE_CURVE_CUBIC) {
			if (handleNodes_getNearestShape(v[i].points[0].x + origin.x, v[i].points[0].y + origin.y, i, v)) {
				selectedNodes.push_back(i-1); //visualization-wise, the control-point is for the previous Node -- it is its right control-point
			}
			if (handleNodes_getNearestShape(v[i].points[1].x + origin.x, v[i].points[1].y + origin.y, i, v)) {
				selectedNodes.push_back(i);
			}
			if (handleNodes_getNearestShape(v[i].points[2].x + origin.x, v[i].points[2].y + origin.y, i, v)) {
				selectedNodes.push_back(i);
			}
		}
	}
	if (selectedShape != UINT_MAX) {
		addSelectedShape(selectedShape);
		if (KageStage::toolMode == MODE_SELECT) {
			win->propStageSetVisible(false);
			win->propFillStrokeSetVisible(true);
			win->propShapePropertiesSetVisible(true);
		}
	}
	if (selectedNodes.size() > 0) {
		if (KageStage::toolMode == MODE_NODE) {
			double l_nodeX = DBL_MAX;
			double l_nodeY = DBL_MAX;
				for (unsigned int i = 0; i < selectedNodes.size(); ++i) {
					if (v[selectedNodes[i]].getPoints().size() == 3) {
						if (v[selectedNodes[i]].points[2].x < l_nodeX) {
							l_nodeX = v[selectedNodes[i]].points[2].x;
						}
						if (v[selectedNodes[i]].points[2].y < l_nodeY) {
							l_nodeY = v[selectedNodes[i]].points[2].y;
						}
					}
				}
			if (l_nodeX == DBL_MAX) { l_nodeX = 0; }
			nodeX = l_nodeX;
			if (l_nodeY == DBL_MAX) { l_nodeY = 0; }
			nodeY = l_nodeY;
			win->updateNodeXY();
			win->propStageSetVisible(false);
			win->propNodeXYSetVisible(true);
		}
	}
	
	invalidateToRender();
}
bool KageStage::handleNodes_getNearestShape(double p_x, double p_y, unsigned int p_index, vector<VectorData> p_v) {
	double l_vXdiff, l_vYdiff;
	double l_distance;
	
	if (p_x >= draw1.x) {
		l_vXdiff = p_x - draw1.x;
	} else {
		l_vXdiff = draw1.x - p_x;
	}
	if (p_y >= draw1.y) {
		l_vYdiff = p_y - draw1.y;
	} else {
		l_vYdiff = draw1.y - p_y;
	}
	
	l_distance = sqrt((l_vXdiff * l_vXdiff) + (l_vYdiff * l_vYdiff));
	if (l_distance <= _nodeToMouseDistance
			&& isMouseOnNode(p_x, p_y, 100) == true) {
		_nodeToMouseDistance = l_distance;
		selectedShape = getSelectedShapeViaNode(p_index, p_v);
//		addSelectedShape(selectedShape);
		return true;
	}
	
	return false;
}

unsigned int KageStage::getSelectedShapeViaNode(unsigned int p_index, vector<VectorData> p_v) {
	if (p_index >= p_v.size()) {
		return UINT_MAX;
	}
	
	unsigned int l_tmp = selectedShape; //why store -- seems not being replaced anyway?
	
	//start ticket#8 -- added since updateX/Y/Width/Height re-use this code for updating X/Y/Width/Height
	//we just want to select the next shape the code after this for-loop will select backwards, back to this shape
	unsigned int l_index = p_index;
	for (unsigned int i = p_index; i < p_v.size(); ++i) {
		if (p_v[i].vectorType == VectorData::TYPE_INIT) {
			l_index = i-1;
			break;
		}
	}
	//we're assuming that the currently-selected item is the last one in stack, so we just try to select the last node in the array
	if (l_index == p_index) {
		l_index = p_v.size()-1;
	}
	//end ticket#8
	
	propWidth = 0.0;
	propHeight = 0.0;
	for (unsigned int i = l_index; i >= 0; --i) {
		switch (p_v[i].vectorType) {
			case VectorData::TYPE_FILL:
				fillColor = p_v[i].fillColor.clone();
				break;
			case VectorData::TYPE_ENDFILL:
				break;
			case VectorData::TYPE_STROKE:
				stroke = p_v[i].stroke.clone();
				
				break;
			case VectorData::TYPE_MOVE: {
				if (propX > p_v[i].points[0].x) {
					propX = p_v[i].points[0].x;
					propXindex1 = i;
					propXindex2 = 0;
				}
				if (propY > p_v[i].points[0].y) {
					propY = p_v[i].points[0].y;
					propYindex1 = i;
					propYindex2 = 0;
				}
				break;
			}
			case VectorData::TYPE_LINE:
			case VectorData::TYPE_CURVE_QUADRATIC:
				break;
			case VectorData::TYPE_CURVE_CUBIC: {
				if (propX > p_v[i].points[0].x) {
					propX = p_v[i].points[0].x;
					propXindex1 = i;
					propXindex2 = 0;
				}
				if (propX > p_v[i].points[1].x) {
					propX = p_v[i].points[1].x;
					propXindex1 = i;
					propXindex2 = 1;
				}
				if (propX > p_v[i].points[2].x) {
					propX = p_v[i].points[2].x;
					propXindex1 = i;
					propXindex2 = 2;
				}
				if (propY > p_v[i].points[0].y) {
					propY = p_v[i].points[0].y;
					propYindex1 = i;
					propYindex2 = 0;
				}
				if (propY > p_v[i].points[1].y) {
					propY = p_v[i].points[1].y;
					propYindex1 = i;
					propYindex2 = 1;
				}
				if (propY > p_v[i].points[2].y) {
					propY = p_v[i].points[2].y;
					propYindex1 = i;
					propYindex2 = 2;
				}
				break;
			}
			case VectorData::TYPE_TEXT:
			case VectorData::TYPE_IMAGE:
				break;
			case VectorData::TYPE_INIT: {
				unsigned int vsize = p_v.size();
				for (unsigned int j = i; j < vsize; ++j) {
					if (p_v[j].vectorType == VectorData::TYPE_ENDFILL) {
						break;
					} else if (p_v[j].vectorType == VectorData::TYPE_MOVE) {
						if (propX < p_v[j].points[0].x
								&& propWidth < (p_v[j].points[0].x - propX)) {
							propWidth = p_v[j].points[0].x - propX;
//							cout << "\npropWidth " << propWidth << " _zoomValueShapeProperty " << _zoomValueShapeProperty << endl;
							propWidth = propWidth * _zoomValueShapeProperty;
						}
						if (propY < p_v[j].points[0].y
								&& propHeight < (p_v[j].points[0].y - propY)) {
							propHeight = p_v[j].points[0].y - propY;
							propHeight = propHeight * _zoomValueShapeProperty;
						}
					} else if (p_v[j].vectorType == VectorData::TYPE_CURVE_CUBIC) {
						for (int k = 0; k < 3; ++k) {
							if (propX < p_v[j].points[k].x
									&& propWidth < (p_v[j].points[k].x - propX)) {
								propWidth = p_v[j].points[k].x - propX;
//								cout << "\npropWidth " << propWidth << " _zoomValueShapeProperty " << _zoomValueShapeProperty << endl;
								propWidth = propWidth * _zoomValueShapeProperty;
							}
							if (propY < p_v[j].points[k].y
									&& propHeight < (p_v[j].points[k].y - propY)) {
								propHeight = p_v[j].points[k].y - propY;
								propHeight = propHeight * _zoomValueShapeProperty;
							}
						}
					}
				}
				//got it!
				return i;
			}
			default:
				break;
		}
	}
	return l_tmp;
}

void KageStage::renderNode(double p_x, double p_y, unsigned int p_state) {
	cr->move_to(p_x-5, p_y);
		cr->line_to(p_x  , p_y-5);
		cr->line_to(p_x+5, p_y  );
		cr->line_to(p_x  , p_y+5);
	cr->close_path();
	if (p_state == 1) {
		cr->set_source_rgba(1.00, 0.00, 0.00, 1.00);
	} else if (p_state == 0) {
		cr->set_source_rgba(0.75, 0.75, 0.75, 1.00);
	} else if (p_state == 2) {
		cr->set_source_rgba(0.00, 0.00, 1.00, 1.00);
	}
		cr->fill_preserve();
	
	cr->set_source_rgba(0.0, 0.0, 0.0, 1.0);
		cr->set_line_width(0.5);
			cr->set_line_cap(Cairo::Context::LineCap::ROUND);
				cr->stroke();
}

void KageStage::renderNodeControl(double p_x, double p_y, bool p_selected) {
	cr->set_line_width(1.0);
	cr->arc(p_x, p_y, 3, 0, 2 * M_PI);
	
	if (p_selected == true) {
		cr->set_source_rgba(1.0,0.0,0.0,1.0);
		cr->fill_preserve();
	} else {
		cr->set_source_rgba(1.0,1.0,1.0,1.0);
		cr->fill_preserve();
	}
	
	cr->set_source_rgba(0.0, 0.0, 0.0, 1.0);
		cr->stroke();
}

bool KageStage::isSelectionBoxNormalized() {
	if (selectionBox1.x <= selectionBox2.x && selectionBox1.y <= selectionBox2.y) {
		return true;
	}
	return false;
}
void KageStage::normalizeSelectionBox() {
	if (isSelectionBoxNormalized() == true) {
		return;
	}
	double l_x1 = 0.0f;
	double l_y1 = 0.0f;
	double l_x2 = 0.0f;
	double l_y2 = 0.0f;
	if (selectionBox1.x < selectionBox2.x) {
		l_x1 = selectionBox1.x;
		l_x2 = selectionBox2.x;
	} else {
		l_x1 = selectionBox2.x;
		l_x2 = selectionBox1.x;
	}
	if (selectionBox1.y < selectionBox2.y) {
		l_y1 = selectionBox1.y;
		l_y2 = selectionBox2.y;
	} else {
		l_y1 = selectionBox2.y;
		l_y2 = selectionBox1.y;
	}
	
	selectionBox1.x = l_x1;
	selectionBox1.y = l_y1;
	selectionBox2.x = l_x2;
	selectionBox2.y = l_y2;
}
bool KageStage::isNodeOnSelectionBox(double p_nodeX, double p_nodeY) {
	normalizeSelectionBox();
	
	if (selectionBox1.x <= p_nodeX && selectionBox2.x >= p_nodeX
			&& selectionBox1.y <= p_nodeY && selectionBox2.y >= p_nodeY) {
		return true;
	}
	return false;
}
bool KageStage::isMouseOnNode(double p_x, double p_y, unsigned int p_buffer) {
	if (p_x-p_buffer <= draw1.x && p_x+p_buffer >= draw1.x
			&& p_y-p_buffer <= draw1.y && p_y+p_buffer >= draw1.y) {
		return true;
	}
	return false;
}

bool KageStage::deselectSelectedNodes() {
	if (selectedNodes.size() == 0) {
		return false;
	}
	Kage::timestamp_IN();
	std::cout << " KageStage::deselectSelectedNodes " << selectedNodes.size() << std::endl;
	
	initNodeTool();
	win->propStageSetVisible(true);
	win->propNodeXYSetVisible(false);
	
	Kage::timestamp_OUT();
	
	return true;
}

bool KageStage::deleteSelectedNodes() {
	if (win->isLayerLocked() == true) {
		return false;
	}
	bool l_return = false;
	
	if (selectedNodes.size() == 0) {
		return false;
	}
	
	Kage::timestamp_IN();
	std::cout << " KageStage::deleteSelectedNodes " << selectedNodes.size() << endl;
	
	sort(selectedNodes.begin(), selectedNodes.end(), greater <unsigned int>());
	
	for (unsigned int l_selectedNode = 0; l_selectedNode < selectedNodes.size(); ++l_selectedNode) {
		Kage::timestamp_IN();
		std::cout << " KageStage::deleteSelectedNodes " << l_selectedNode << " " << selectedNodes.size() << endl;
			l_return = deleteSelectedNode(selectedNodes[l_selectedNode]);
		Kage::timestamp_OUT();
	}
	
	selectedNodes.clear();
	
	if (l_return == true) {
		win->stackDo();
	}
	
	Kage::timestamp_OUT();
	
	return l_return;
}
bool KageStage::deleteSelectedNode(unsigned int p_index) {
	if (win->isLayerLocked() == true) {
		return false;
	}
	vector<VectorData> v = win->getFrameData().getVectorData();
	
	selectedShape = getSelectedShapeViaNode(p_index, v);
	
	//avoid deletion of selectedNode as non-Curve/Line; ex: selectedNode is Move
	if (selectedShape == _NO_SELECTION
			|| (v[p_index].vectorType != VectorData::TYPE_CURVE_CUBIC
			&& v[p_index].vectorType != VectorData::TYPE_CURVE_QUADRATIC
			&& v[p_index].vectorType != VectorData::TYPE_LINE)) {
		return false;
	}
	
	unsigned int l_sequence = 0;
	bool l_closepath = false;
	if (v.size() >= 5) {
		if (v[selectedShape + 0].vectorType == VectorData::TYPE_INIT       ) ++l_sequence;
		if (v[selectedShape + 1].vectorType == VectorData::TYPE_FILL       ) ++l_sequence;
		if (v[selectedShape + 2].vectorType == VectorData::TYPE_STROKE     ) ++l_sequence;
		if (v[selectedShape + 3].vectorType == VectorData::TYPE_MOVE       ) ++l_sequence;
		if (v[selectedShape + 4].vectorType == VectorData::TYPE_CURVE_CUBIC
			|| v[selectedShape + 4].vectorType == VectorData::TYPE_CURVE_QUADRATIC
			|| v[selectedShape + 4].vectorType == VectorData::TYPE_LINE) {
			++l_sequence;
		}
		if (v[selectedShape + 5].vectorType == VectorData::TYPE_CLOSE_PATH ) {
			++l_sequence;
			l_closepath = true;
			if (v[selectedShape + 6].vectorType == VectorData::TYPE_ENDFILL) {
				++l_sequence;
			}
		} else if (v[selectedShape + 5].vectorType == VectorData::TYPE_ENDFILL) {
			++l_sequence;
		}
	}
	
	if (l_sequence == 6 && l_closepath == false) {
		v.erase (v.begin() + selectedShape,
				 v.begin() + selectedShape + l_sequence);
		
		std::cout << " DELETE A " << endl;
//			selectedShapes.erase(selectedShapes.begin() + l_selectedShape);
	} else if (l_sequence == 7 && l_closepath == true) {
		v.erase (v.begin() + selectedShape,
				 v.begin() + selectedShape + l_sequence);
		
		std::cout << " DELETE B " << endl;
//			selectedShapes.erase(selectedShapes.begin() + l_selectedShape);
	} else {
		if (v[p_index-1].vectorType != VectorData::TYPE_MOVE &&
				(v[p_index+1].vectorType == VectorData::TYPE_ENDFILL ||
				 v[p_index+1].vectorType == VectorData::TYPE_CLOSE_PATH)) {
			for (unsigned int i = p_index; i > 0; --i) {
				if (v[i].vectorType == VectorData::TYPE_MOVE) {
					v[i+1].points[0].x = v[p_index].points[0].x;
					v[i+1].points[0].y = v[p_index].points[0].y;
					v[i].points[0].x = v[p_index-1].points[2].x;
					v[i].points[0].y = v[p_index-1].points[2].y;
					break;
				}
			}
		} else {
			v[p_index+1].points[0].x = v[p_index].points[0].x;
			v[p_index+1].points[0].y = v[p_index].points[0].y;
		}
		std::cout << " DELETE C " << endl;
		
		v.erase (v.begin() + p_index);
	}
	
	win->setFrameData(v);
	
	return true;
}

bool KageStage::toggleLineSelectedNodes() {
	vector<VectorData> v = win->getFrameData().getVectorData();
	
	unsigned int nsize = selectedNodes.size();
	
	bool l_gotit = false;
	unsigned int l_currentNode;
	unsigned int l_nextNode;
	for (unsigned int i = 0; i < nsize; ++i) {
		l_currentNode = selectedNodes[i];
		l_nextNode = l_currentNode+1;
		if (l_currentNode < v.size()
				&& isSelectedNode(l_nextNode) == true) {
			if (       (v[l_nextNode   ].vectorType == VectorData::TYPE_CURVE_CUBIC || v[l_nextNode   ].vectorType == VectorData::TYPE_CURVE_QUADRATIC)
					&& (v[l_currentNode].vectorType == VectorData::TYPE_CURVE_CUBIC || v[l_currentNode].vectorType == VectorData::TYPE_CURVE_QUADRATIC)) {
				v[l_nextNode].points[0].x = (v[l_currentNode].points[2].x + v[l_nextNode].points[2].x) / 2;
				v[l_nextNode].points[0].y = (v[l_currentNode].points[2].y + v[l_nextNode].points[2].y) / 2;
				
				v[l_nextNode].points[1].x = v[l_nextNode].points[0].x;
				v[l_nextNode].points[1].y = v[l_nextNode].points[0].y;
				l_gotit = true;
			}
		} else if (v[l_nextNode].vectorType == VectorData::TYPE_CLOSE_PATH) {
			//The user just selected the very end of an enclosed shape and the very first node
			//so, search for very first curve/line if also currently selected
			for (unsigned int l_firstNode = l_currentNode-1; l_firstNode > 0; --l_firstNode) {
				if (v[l_firstNode].vectorType == VectorData::TYPE_MOVE) {
					++l_firstNode;
					if (isSelectedNode(l_firstNode) == true) {
						if (       (v[l_firstNode  ].vectorType == VectorData::TYPE_CURVE_CUBIC || v[l_firstNode  ].vectorType == VectorData::TYPE_CURVE_QUADRATIC)
								&& (v[l_currentNode].vectorType == VectorData::TYPE_CURVE_CUBIC || v[l_currentNode].vectorType == VectorData::TYPE_CURVE_QUADRATIC)) {
							v[l_firstNode].points[0].x = (v[l_currentNode].points[2].x + v[l_firstNode].points[2].x) / 2;
							v[l_firstNode].points[0].y = (v[l_currentNode].points[2].y + v[l_firstNode].points[2].y) / 2;
							
							v[l_firstNode].points[1].x = v[l_firstNode].points[0].x;
							v[l_firstNode].points[1].y = v[l_firstNode].points[0].y;
							l_gotit = true;
						}
					}
					break;
				}
			}
		}
	}
	
	if (l_gotit == true) {
		win->setFrameData(v);
		
		win->stackDo();
	}
	
	return l_gotit;
}

void KageStage::updateNodeXY() {
	unsigned int nsize = selectedNodes.size();
	if (nsize == 0) {
		nodeX = 0;
		nodeY = 0;
		nodeIndexX = UINT_MAX;
		nodeIndexY = UINT_MAX;
	} else {
		nodeX = DBL_MAX;
		nodeY = DBL_MAX;
		vector<VectorData> v = win->getFrameData().getVectorData();
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
	win->propNodeXYSetVisible(true);
}



#include "stage.h"
#include "../kage.h"
#include <cairomm/context.h>

KageStage::KageStage(Kage *p_win) {
	win = p_win;
	set_can_focus(true); //to accept key_press
	add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
	add_events(Gdk::ENTER_NOTIFY_MASK | Gdk::LEAVE_NOTIFY_MASK);
	add_events(Gdk::FOCUS_CHANGE_MASK);
	add_events(Gdk::POINTER_MOTION_MASK);
	
	keyUpDown = false;
	keyDownDown =  false;
	keyLeftDown =  false;
	keyRightDown =  false;
	keyShiftDown = false;
	keyControlDown = false;
	
	_gotFocus = false;
}

KageStage::~KageStage() {
	//
}

bool KageStage::on_key_press_event(GdkEventKey *e) {
	if (e->keyval == GDK_KEY_space) { // [hand tool]
		if (KageStage::toolMode != MODE_MOVE_STAGE) {
			prevTool = KageStage::toolMode;
			KageStage::toolMode = MODE_MOVE_STAGE;
//			Gdk::Cursor handcur(Gdk::HAND1);
//			get_window()->set_cursor(handcur);
		}
	} else if (e->keyval == GDK_KEY_period) {
		win->switchToNextFrame();
	} else if (e->keyval == GDK_KEY_comma) {
		win->switchToPreviousFrame();
	} else {
		if (KageStage::toolMode == MODE_SELECT
				|| KageStage::toolMode == MODE_NODE
				|| KageStage::toolMode == MODE_ZOOM) {
			if (e->keyval == GDK_KEY_Up || e->keyval == GDK_KEY_KP_Up) {
				keyUpDown = true;
				keyDownDown = false;
			} else if (e->keyval == GDK_KEY_Down || e->keyval == GDK_KEY_KP_Down) {
				keyDownDown =  true;
				keyUpDown = false;
			} else if (e->keyval == GDK_KEY_Left || e->keyval == GDK_KEY_KP_Left) {
				keyLeftDown =  true;
				keyRightDown = false;
			} else if (e->keyval == GDK_KEY_Right || e->keyval == GDK_KEY_KP_Right) {
				keyRightDown =  true;
				keyLeftDown = false;
			} else if (e->keyval == GDK_KEY_Shift_L || e->keyval == GDK_KEY_Shift_R) {
				keyShiftDown = true;
			} else if (e->keyval == GDK_KEY_Control_L || e->keyval == GDK_KEY_Control_R) {
				keyControlDown = true;
			} else {
				std::cout << "KageStage::::on_key_press_event " << e->keyval << "_" << e->string << std::endl;
				return true;
			}
			
			double l_diffX = 0;
			double l_diffY = 0;
			if (keyUpDown == true) {
				l_diffY = -1;
			} else if (keyDownDown == true) {
				l_diffY =  1;
			}
			if (keyLeftDown == true) {
				l_diffX = -1;
			} else if (keyRightDown == true) {
				l_diffX =  1;
			}
			if (keyShiftDown == true) {
				if (l_diffX != 0) {
					l_diffX *= 10;
				}
				if (l_diffY != 0) {
					l_diffY *= 10;
				}
			}
			if (keyControlDown == true) {
				origin.x -= l_diffX;
				origin.y -= l_diffY;
			} else {
				handleShapes_moveShape(l_diffX, l_diffY);
				propX += l_diffX;
				propY += l_diffY;
				win->updateProperties();
			}
			if (KageStage::toolMode == MODE_SELECT) {
				render();
			}
		} else {
			std::cout << "stage " << e->keyval << "_" << e->string << std::endl;
		}
	}
	return true;
}
bool KageStage::on_key_release_event(GdkEventKey *e) {
	if (e->keyval == GDK_KEY_space) { // [hand tool]
		KageStage::toolMode = prevTool;
		Kage::timestamp();
		std::cout << " KageStage::on_key_release_event No Hand" << std::endl;
	} else if (e->keyval == GDK_KEY_Up || e->keyval == GDK_KEY_KP_Up) {
		keyUpDown = false;
	} else if (e->keyval == GDK_KEY_Down || e->keyval == GDK_KEY_KP_Down) {
		keyDownDown =  false;
	} else if (e->keyval == GDK_KEY_Left || e->keyval == GDK_KEY_KP_Left) {
		keyLeftDown =  false;
	} else if (e->keyval == GDK_KEY_Right || e->keyval == GDK_KEY_KP_Right) {
		keyRightDown =  false;
	} else if (e->keyval == GDK_KEY_Shift_L || e->keyval == GDK_KEY_Shift_R) {
		keyShiftDown = false;
	} else if (e->keyval == GDK_KEY_Control_L || e->keyval == GDK_KEY_Control_R) {
		keyControlDown = false;
	}
	return true;
}

bool KageStage::on_expose_event(GdkEventExpose* e) {
	if (!window) {
		window = get_window();
	}
	
	if (window) {
		render();
	}
	return true;
}

bool KageStage::on_event(GdkEvent *e) {
	if (e->type == GDK_ENTER_NOTIFY) {
		render();
		//mouse hover in
	} else if (e->type == GDK_LEAVE_NOTIFY) {
		render();
		//mouse hover out
	} else if (e->type == GDK_KEY_PRESS) {
		on_key_press_event((GdkEventKey*) e);
	} else if (e->type == GDK_KEY_RELEASE) {
		on_key_release_event((GdkEventKey*) e);
	} else if (e->type == GDK_DOUBLE_BUTTON_PRESS) {
		if (selectedShapes.size() > 0) {
			if (KageStage::toolMode == MODE_SELECT) {
				KageStage::toolMode = MODE_NODE;
				win->toolsButtonToggle("Node Tool");
			}
		}
	} else if (e->type == GDK_BUTTON_PRESS) { //mouse down
		mouseDown = true;
		draw1.x = e->button.x;
		draw1.y = e->button.y;
		if (KageStage::toolMode == MODE_DRAW_OVAL
				|| KageStage::toolMode == MODE_DRAW_RECT) {
			draw2.x = e->button.x;
			draw2.y = e->button.y;
		} else if (KageStage::toolMode == MODE_SELECT) {
			draw2.x = e->button.x;
			draw2.y = e->button.y;
		} else if (KageStage::toolMode == MODE_NODE) {
			draw2.x = e->button.x; //added because of multi/single-selection of shape/s
			draw2.y = e->button.y;
			render();
		} else if (KageStage::toolMode == MODE_DRAW_POLY) {
			//
		} else {
			//
		}
	} else if (e->type == GDK_BUTTON_RELEASE) { //mouse up
		mouseOnAnchor = AnchorData::TYPE_NONE;
		mouseDown = false;
		grab_focus();
		
		draw1.x = (draw1.x - origin.x);
		draw1.y = (draw1.y - origin.y);
			draw2.x = (draw2.x - origin.x);
			draw2.y = (draw2.y - origin.y);
		
		if (KageStage::toolMode == MODE_SELECT) {
			if (_isModifyingShape == false) {
				if (draw1.x >= draw2.x-5 && draw1.x <= draw2.x+5 && draw1.y >= draw2.y-5 && draw1.y <= draw2.y+5) {
					draw1.x = e->button.x;
					draw1.y = e->button.y;
						trySingleSelectShape();
				} else {
					selectionBox1 = draw1;
					selectionBox2 = draw2;
						tryMultiSelectShapes();
				}
			} else {
				if (_rotateApply == true) {
					draw1.x = (draw1.x + origin.x);
					draw1.y = (draw1.y + origin.y);
						draw2.x = (draw2.x + origin.x);
						draw2.y = (draw2.y + origin.y);
					
					handleShapes_modifyingShapeRotate();
					
					draw1.x = (draw1.x - origin.x);
					draw1.y = (draw1.y - origin.y);
						draw2.x = (draw2.x - origin.x);
						draw2.y = (draw2.y - origin.y);
				}
				
				win->stackDo();
			}
			_isModifyingShape = false;
			render();
		} else if (KageStage::toolMode == MODE_NODE) {
			if (_isModifyingShape == false) {
				if (draw1.x >= draw2.x-5 && draw1.x <= draw2.x+5 && draw1.y >= draw2.y-5 && draw1.y <= draw2.y+5) {
					draw1.x = e->button.x;
					draw1.y = e->button.y;
						trySingleSelectShape();
				} else {
					selectionBox1 = draw1;
					selectionBox2 = draw2;
						tryMultiSelectShapes();
				}
				win->propFillStrokeSetVisible(false);
				win->propLocationSizeSetVisible(false);
				win->propNodeXYSetVisible(true);
			} else {
				win->stackDo();
			}
			_isModifyingShape = false;
			render();
		} else if (KageStage::toolMode == MODE_STROKE) {
			draw1.x = e->button.x;
			draw1.y = e->button.y;
				handleStrokeMouseUp();
		} else if (KageStage::toolMode == MODE_FILL) {
			draw1.x = e->button.x;
			draw1.y = e->button.y;
				handleFillMouseUp();
		} else if (KageStage::toolMode == MODE_ZOOM) {
			if (keyShiftDown == true || keyControlDown == true) {
				if (e->button.button == 3) {
					_zoomRatio = 1.1;
					_zoomValue = _zoomValue * 1.1f;
				} else if (e->button.button == 1) {
					_zoomRatio = 0.909090909f;
					_zoomValue = _zoomValue / 1.1f;
				}
			} else {
				if (e->button.button == 1) {
					_zoomRatio = 1.1;
					_zoomValue = _zoomValue * 1.1f;
				} else if (e->button.button == 3) {
					_zoomRatio = 0.909090909f;
					_zoomValue = _zoomValue / 1.1f;
				}
			}
			e->button.button = 0;
			_zoomReference.x = e->button.x;
			_zoomReference.y = e->button.y;
			
			applyZoom();
			render();
		} else if (KageStage::toolMode == MODE_EYEDROP) {
			draw1.x = e->button.x;
			draw1.y = e->button.y;
				handleEyedropMouseUp();
		} else if (KageStage::toolMode == MODE_DRAW_OVAL) {
			handleDrawOvalMouseUp();
		} else if (KageStage::toolMode == MODE_DRAW_RECT) {
			handleDrawRectMouseUp();
		} else if (KageStage::toolMode == MODE_DRAW_POLY) {
			handleDrawPolyMouseUp();
				///NOTE: why origin.x/.y is not taken into consideration?
				/// try moving stage if this is still properly working
				draw1.x = (e->button.x);
				draw1.y = (e->button.y);
					draw2.x = (e->button.x);
					draw2.y = (e->button.y);
		} else {
			//
		}
	} else if (e->type == GDK_MOTION_NOTIFY) { ///mouse move
		_mouseLocation.x = e->button.x;
		_mouseLocation.y = e->button.y;
		
		if (KageStage::toolMode == MODE_NODE) {
			draw1.x = e->button.x;
			draw1.y = e->button.y;
			
			render();
		} else if (KageStage::toolMode == MODE_SELECT) {
			draw2.x = e->button.x;
			draw2.y = e->button.y;
			
			_zoomReference.x = e->button.x;
			_zoomReference.y = e->button.y;
			render();
		} else if (KageStage::toolMode == MODE_DRAW_OVAL
				|| KageStage::toolMode == MODE_DRAW_RECT) {
			if (mouseDown == true) {
				draw2.x = (e->button.x);
				draw2.y = (e->button.y);
				
				render();
			}
		} else if (KageStage::toolMode == MODE_DRAW_POLY) {
			if (drawCtr > 0) {
				draw2.x = (e->button.x);
				draw2.y = (e->button.y);
				
				render();
			}
		} else if (KageStage::toolMode == MODE_STROKE) {
			handleStrokeMouseMove();
			render();
		} else if (KageStage::toolMode == MODE_EYEDROP) {
			draw1.x = (e->button.x);
			draw1.y = (e->button.y);
			handleEyedropMouseMove();
			render();
		} else if (KageStage::toolMode == MODE_MOVE_STAGE) {
			double distx = e->button.x - KageStage::moveStageXY.x;
			double disty = e->button.y - KageStage::moveStageXY.y;
			
			origin.x += distx;
			origin.y += disty;
			
			render();
		}
		win->forceRenderFrames();
		KageStage::moveStageXY.x = e->button.x;
		KageStage::moveStageXY.y = e->button.y;
	} else if (e->type == GDK_EXPOSE) {
		on_expose_event((GdkEventExpose*) e);
	} else if (e->type == GDK_FOCUS_CHANGE) {
		//filter out from echos
		cout << "GDK_FOCUS_CHANGE e->send_event " << ((GdkEventFocus*)e)->send_event << " e->in " << ((GdkEventFocus*)e)->in << endl;
		if (((GdkEventFocus*)e)->in) {
			_gotFocus = true;
		} else {
			_gotFocus = false;
		}
	} else if (e->type == GDK_CONFIGURE) {
		//filter out from echos
		/**
		 * struct _GdkEventConfigure {
		 * 	GdkEventType type;
		 * 	GdkWindow *window;
		 * 	gint8 send_event;
		 * 	gint16 x, y;
		 * 	gint16 width;
		 * 	gint16 height;
		 * };
		 */
	} else {
		//
	}
	
	return true;
}

PointData KageStage::applyZoomRatio(PointData p_value) {
	if (_zoomRatio == 0.0f) { return p_value; }
	
	double l_value;
	
	if (p_value.x < _zoomReference.x) {
		l_value = (_zoomReference.x - p_value.x) * _zoomRatio;
		p_value.x = _zoomReference.x - l_value;
	} else if (p_value.x > _zoomReference.x) {
		l_value = (p_value.x - _zoomReference.x) * _zoomRatio;
		p_value.x = _zoomReference.x + l_value;
	}
	if (p_value.y < _zoomReference.y) {
		l_value = (_zoomReference.y - p_value.y) * _zoomRatio;
		p_value.y = _zoomReference.y - l_value;
	} else if (p_value.y > _zoomReference.y) {
		l_value = (p_value.y - _zoomReference.y) * _zoomRatio;
		p_value.y = _zoomReference.y + l_value;
	}
	
	return p_value.clone();
}

double KageStage::applyZoomRatioX(double p_value) {
	if (_zoomRatio == 0.0f) { return p_value; }
	
	double l_value;
	
	if (p_value < _zoomReference.x) {
		l_value = (_zoomReference.x - p_value) * _zoomRatio;
		return _zoomReference.x - l_value;
	} else if (p_value > _zoomReference.x) {
		l_value = (p_value - _zoomReference.x) * _zoomRatio;
		return _zoomReference.x + l_value;
	}
	
	return p_value;
}

double KageStage::applyZoomRatioY(double p_value) {
	if (_zoomRatio == 0.0f) { return p_value; }
	
	double l_value;
	
	if (p_value < _zoomReference.y) {
		l_value = (_zoomReference.y - p_value) * _zoomRatio;
		return _zoomReference.y - l_value;
	} else if (p_value > _zoomReference.y) {
		l_value = (p_value - _zoomReference.y) * _zoomRatio;
		return _zoomReference.y + l_value;
	}
	
	return p_value;
}

double KageStage::getZoomRatioX(double p_value) {
	if (_zoomRatio == 0.0f) { return p_value; }
	
	double l_value;
	
	if (p_value < _zoomReference.x) {
		l_value = (_zoomReference.x - p_value) * _zoomValue;
		return _zoomReference.x - l_value;
	} else if (p_value > _zoomReference.x) {
		l_value = (p_value - _zoomReference.x) * _zoomValue;
		return _zoomReference.x + l_value;
	}
	
	return p_value;
}

double KageStage::getZoomRatioY(double p_value) {
	if (_zoomRatio == 0.0f) { return p_value; }
	
	double l_value;
	
	if (p_value < _zoomReference.y) {
		l_value = (_zoomReference.y - p_value) * _zoomValue;
		return _zoomReference.y - l_value;
	} else if (p_value > _zoomReference.y) {
		l_value = (p_value - _zoomReference.y) * _zoomValue;
		return _zoomReference.y + l_value;
	}
	
	return p_value;
}

void KageStage::setSelectedShapes(vector<unsigned int> p_selectedShapes) {
	selectedNodes = p_selectedShapes;
	tryMultiSelectShapes_populateShapes();
}
vector<unsigned int> KageStage::getSelectedShapes() {
	return selectedShapes;
}
vector<VectorData> KageStage::applyZoom(vector<VectorData> v) {
	return v;
}
void KageStage::applyZoom() {
		
	PointData __stageArea(stageWidth  + origin.x, stageHeight  + origin.y);
	__stageArea = applyZoomRatio(__stageArea);
	
	__origin = applyZoomRatio(origin);
	
		__stageArea.x -= __origin.x;
		__stageArea.y -= __origin.y;
	
		win->stackDoZoom(origin, __origin, _zoomReference, _zoomRatio);
	
	unsigned int l_layerCount = win->_layerManager.layerCount();
	unsigned int l_frameCount = win->_framesetManager.frameCount();
	unsigned int l_currentFrame = win->_framesetManager.getCurrentFrame();
	unsigned int l_currentLayer = win->getCurrentLayer();
	
	for (unsigned int l_frame = 1; l_frame <= l_frameCount; ++l_frame) {
		win->_framesetManager.setCurrentFrame(l_frame);
		
		for (unsigned int l_layer = 1; l_layer <= l_layerCount; ++l_layer) {
			win->_framesetManager.setCurrentLayer(l_layer);
			
			vector<VectorData> v = win->getFrameData(true).getVectorData();
			
			
			for (unsigned int i = 0; i < v.size(); ++i) {
				if (v[i].vectorType == VectorData::TYPE_MOVE
						|| v[i].vectorType == VectorData::TYPE_LINE) {
					v[i].points[0].x += origin.x;
					v[i].points[0].y += origin.y;
						v[i].points[0] = applyZoomRatio(v[i].points[0]);
					v[i].points[0].x -= __origin.x;
					v[i].points[0].y -= __origin.y;
				} else if (v[i].vectorType == VectorData::TYPE_CURVE_CUBIC
						|| v[i].vectorType == VectorData::TYPE_CURVE_QUADRATIC) {
					for (unsigned int j = 0; j < 3; ++j) {
						v[i].points[j].x += origin.x;
						v[i].points[j].y += origin.y;
							v[i].points[j] = applyZoomRatio(v[i].points[j]);
						v[i].points[j].x -= __origin.x;
						v[i].points[j].y -= __origin.y;
					}
				}
			}
			
			win->setFrameData(v, true);
		}
	}
	
	//TODO: is it needed to access _framesetManager? why not just win->setCurrentLayer(l_currentLayer)?
	win->_framesetManager.setCurrentLayer(l_currentLayer);
	win->_framesetManager.setCurrentFrame(l_currentFrame);
	
	origin = __origin.clone();
	stageWidth  = __stageArea.x;
	stageHeight = __stageArea.y;
	
}

void KageStage::setStageBG(Gdk::Color p_c) {
	KageStage::stageBG.setR(p_c.get_red() / 256);
	KageStage::stageBG.setG(p_c.get_green() / 256);
	KageStage::stageBG.setB(p_c.get_blue() / 256);
	render();
}
Gdk::Color KageStage::getStageBG() {
	Gdk::Color l_c;
	l_c.set_red((gushort)KageStage::stageBG.getR() * 256);
	l_c.set_green((gushort)KageStage::stageBG.getG() * 256);
	l_c.set_blue((gushort)KageStage::stageBG.getB() * 256);
	
	return l_c;
}

void KageStage::setFill(Gdk::Color p_Color) {
	KageStage::fillColor.setR(p_Color.get_red() / 256);
	KageStage::fillColor.setG(p_Color.get_green() / 256);
	KageStage::fillColor.setB(p_Color.get_blue() / 256);
	render();
}
Gdk::Color KageStage::getFill() {
	Gdk::Color tColor;
	tColor.set_red((gushort)KageStage::fillColor.getR() * 256);
	tColor.set_green((gushort)KageStage::fillColor.getG() * 256);
	tColor.set_blue((gushort)KageStage::fillColor.getB() * 256);
	
	return tColor;
}

void KageStage::setStroke(Gdk::Color p_c) {
	KageStage::stroke.setR(p_c.get_red() / 256);
	KageStage::stroke.setG(p_c.get_green() / 256);
	KageStage::stroke.setB(p_c.get_blue() / 256);
	render();
}
Gdk::Color KageStage::getStroke() {
	Gdk::Color l_c;
	l_c.set_red((gushort)KageStage::stroke.getR() * 256);
	l_c.set_green((gushort)KageStage::stroke.getG() * 256);
	l_c.set_blue((gushort)KageStage::stroke.getB() * 256);
	
	return l_c;
}

void KageStage::printVectors() {
	Kage::timestamp();
	std::cout << " KageStage::printVectors <" << std::endl;
	vector<VectorData> v = win->getFrameData().getVectorData();
	
	unsigned int vsize = v.size();
	Kage::timestamp();
	std::cout << "                         " << vsize << std::endl;
	std::cout << std::endl << std::endl << std::endl << std::endl << std::endl;
	unsigned int fillCtr = 0;
	ColorData fcolor;
	StrokeColorData scolor;
	PointData p;
	double x1;
	double y1;
	double x2;
	double y2;
	double x3;
	double y3;
	for (unsigned int i = 0; i < vsize; ++i) {
		switch (v[i].vectorType) {
			case VectorData::TYPE_CLOSE_PATH:
				std::cout << "CLOSE_PATH" << std::endl;
				break;
			case VectorData::TYPE_FILL:
				fcolor = v[i].fillColor;
				std::cout << "FILL = " << (double)fcolor.getR()/255 << ", " << 
										(double)fcolor.getG()/255 << ", " << 
										(double)fcolor.getB()/255 << ", " << 
										(double)fcolor.getA()/255 << std::endl;
				fillCtr++;
				break;
			case VectorData::TYPE_ENDFILL:
				std::cout << "ENDFILL - fillCtr = " << fillCtr << std::endl;
				if (fillCtr > 0) {
					std::cout << "         set_source_rgba" << std::endl;
					std::cout << "         fill_preserve" << std::endl;
					fillCtr--;
				}
//				if (scolor.getThickness() > 0) {
				std::cout << "set_source_rgba" << std::endl;
				std::cout << "set_line_width = " << scolor.getThickness() << std::endl;
				std::cout << "set_line_cap = Cairo::LINE_CAP_ROUND" << std::endl;
				std::cout << "stroke" << std::endl;
//				}
				break;
			case VectorData::TYPE_STROKE:
				scolor = v[i].stroke;
				std::cout << "STROKE = " << (double)scolor.getR()/255 << ", " << 
											(double)scolor.getG()/255 << ", " << 
											(double)scolor.getB()/255 << ", " << 
											(double)scolor.getA()/255 << std::endl;
				break;
			case VectorData::TYPE_MOVE:
				std::cout << "MOVE " << v[i].points[0].x + origin.x << ", " << v[i].points[0].y + origin.y << std::endl;
				p.x = v[i].points[0].x;
				p.y = v[i].points[0].y;
				break;
			case VectorData::TYPE_LINE:
				std::cout << "LINE " << v[i].points[0].x + origin.x << ", " << v[i].points[0].y + origin.y << std::endl;
				p.x = v[i].points[0].x;
				p.y = v[i].points[0].y;
				break;
			case VectorData::TYPE_CURVE_QUADRATIC:
				std::cout << "CURVE_QUADRATIC " << v[i].points[0].x + origin.x << ", " << v[i].points[0].y + origin.y << std::endl;
				std::cout << "                " << v[i].points[1].x + origin.x << ", " << v[i].points[1].y + origin.y << std::endl;
				//cubic-to-quad algo borrowed from Mono/Moonlight's moon_quad_curve_to
				x1 = v[i].points[0].x;
				y1 = v[i].points[0].y;
				x2 = v[i].points[1].x;
				y2 = v[i].points[1].y;
				x3 = x2;
				y3 = y2;
				
				x2 = x1 + (x2 - x1) / 3;
				y2 = y1 + (y2 - y1) / 3;
				x1 = p.x + 2 * (x1 - p.x) / 3;
				y1 = p.y + 2 * (y1 - p.y) / 3;
				
				std::cout << "        curve_to " << x1 + origin.x << ", " << y1 + origin.y << std::endl;
				std::cout << "                 " << x2 + origin.x << ", " << y2 + origin.y << std::endl;
				std::cout << "                 " << x3 + origin.x << ", " << y3 + origin.y << std::endl;
				
				p.x = x3;
				p.y = y3;
				break;
			case VectorData::TYPE_CURVE_CUBIC:
				std::cout << "CURVE_CUBIC" << std::endl;
				std::cout << "        curve_to " << v[i].points[0].x + origin.x << ", " << v[i].points[0].y + origin.y << std::endl;
				std::cout << "                 " << v[i].points[1].x + origin.x << ", " << v[i].points[1].y + origin.y << std::endl;
				std::cout << "                 " << v[i].points[2].x + origin.x << ", " << v[i].points[2].y + origin.y << std::endl;
				break;
			case VectorData::TYPE_TEXT:
				std::cout << "TEXT" << std::endl;
				break;
			case VectorData::TYPE_IMAGE:
				std::cout << "IMAGE" << std::endl;
				break;
			case VectorData::TYPE_INIT:
				std::cout << "INIT (new shape!)" << std::endl;
				break;
			default:
				std::cout << "default " << v[i].vectorType << std::endl;
				break;
		}
	}
	Kage::timestamp();
	std::cout << " KageStage::printVectors >" << std::endl;
}

void KageStage::cleanSlate() {
	origin.x = 50;
	origin.y = 50;
	stageWidth = 800.0f;
	stageHeight = 600.0f;
	fps = 12;
	mouseDown = false;
	stroke.setThickness(3.0);
	drawCtr = 0;
	mouseOnNode = -1;
	mouseOnNodeOffset.x = 0.0f;
	mouseOnNodeOffset.y = 0.0f;
	mouseOnNodeHover = -1;
	mouseOnNodeIndex = 0;
	polyXhead = 0;
	polyYhead = 0;
	polyXtail = 0;
	polyYtail = 0;
	
	keyControlDown = false;
	keyShiftDown = false;
	keyUpDown = false;
	keyDownDown = false;
	keyLeftDown = false;
	keyRightDown = false;
	
	_isModifyingShape = false;
	
	_zoomValue = 1.0f;
	_zoomRatio = 1.0f;
	__stageArea.x = stageWidth;
	__stageArea.y = stageHeight;
	
	initNodeTool();
	
	mouseOnAnchor = AnchorData::TYPE_NONE;
}

void KageStage::render() {
	if (!window) {
		window = get_window();
	}
	
	if (window) {
		// force our program to redraw the entire stage
		Gdk::Rectangle r(0, 0, get_allocation().get_width(),
				get_allocation().get_height());
		window->invalidate_rect(r, false);
	}
}

bool KageStage::on_draw(const Cairo::RefPtr<Cairo::Context>& p_cr) {
	if (!window) {
		window = get_window();
	}
	if (window) {
		cr = p_cr;
		if (mouseDown) {
			win->renderFramesBelowCurrentLayer();
			if (_rotateMode == false) {
				renderFrame(cr);
			}
		} else {
			win->renderFrames();
		}
		
		///why can't it be loaded when after `window = get_window()'?
		if (!KageStage::imageSHAPE_000) {
			KageStage::imageSHAPE_000 = Gdk::Pixbuf::create_from_resource("/kage/share/icons/shape_000.png");
			KageStage::imageSHAPE_045 = Gdk::Pixbuf::create_from_resource("/kage/share/icons/shape_045.png");
			KageStage::imageSHAPE_090 = Gdk::Pixbuf::create_from_resource("/kage/share/icons/shape_090.png");
			KageStage::imageSHAPE_135 = Gdk::Pixbuf::create_from_resource("/kage/share/icons/shape_135.png");
			KageStage::imageSHAPE_MOVE = Gdk::Pixbuf::create_from_resource("/kage/share/icons/shape_move.png");
			
			KageStage::imageSHAPE_NE = Gdk::Pixbuf::create_from_resource("/kage/share/icons/shape_ne.png");
			KageStage::imageSHAPE_NW = Gdk::Pixbuf::create_from_resource("/kage/share/icons/shape_nw.png");
			KageStage::imageSHAPE_SW = Gdk::Pixbuf::create_from_resource("/kage/share/icons/shape_sw.png");
			KageStage::imageSHAPE_SE = Gdk::Pixbuf::create_from_resource("/kage/share/icons/shape_se.png");
			KageStage::imageSHAPE_ROTATE = Gdk::Pixbuf::create_from_resource("/kage/share/icons/shape_rotate.png");
		}
		
		if (_registerWidth != get_width()) {
			_registerWidth = get_width();
			origin.x = (_registerWidth - stageWidth) / 2;
		}
		if (_registerHeight != get_height()) {
			_registerHeight = get_height();
			origin.y = (_registerHeight - stageHeight) / 2;
		}
		
		//draw user-drawn object
		if (KageStage::toolMode == MODE_SELECT) {
			if (mouseDown == true) {
				handleShapes_modifyingShape();
			} else {
				handleShapes();
			}
		} else if (KageStage::toolMode == MODE_NODE) {
			handleNodes();
		} else if (KageStage::toolMode == MODE_DRAW_POLY) {
			handlePoly();
		} else if (KageStage::toolMode == MODE_DRAW_RECT && mouseDown == true) {
			handleRect();
		} else if (KageStage::toolMode == MODE_DRAW_OVAL && mouseDown == true) {
			handleOval();
		} else if (KageStage::toolMode == MODE_FILL) {
			handleFill();
		} else if (KageStage::toolMode == MODE_EYEDROP) {
			handleEyedrop();
		}
		if (mouseDown) {
			win->renderFramesAboveCurrentLayer();
		}
	}
	return true;
}

void KageStage::clearScreen(Cairo::RefPtr<Cairo::Context> p_context) {
	if (!p_context) return;
	if (window) {
		Gtk::Allocation allocation = get_allocation();
		const int width = allocation.get_width();
		const int height = allocation.get_height();
		
		//draw stage area
			p_context->move_to(0, 0);
			p_context->line_to(width, 0);
			p_context->line_to(width, height);
			p_context->line_to(0, height);
			p_context->close_path();
		p_context->set_source_rgb(0.75, 0.75, 0.75);
		p_context->fill();
		
		//draw viewable area
		p_context->move_to(             origin.x, origin.y              );
		p_context->line_to(stageWidth + origin.x, origin.y              );
		p_context->line_to(stageWidth + origin.x, origin.y + stageHeight);
		p_context->line_to(             origin.x, origin.y + stageHeight);
		
		bool l_mouseLocationOnFill = p_context->in_fill(_mouseLocation.x, _mouseLocation.y);
			if (l_mouseLocationOnFill != 0) {
				_mouseLocationShapeIndex = _NO_SELECTION;
			}
		
		p_context->close_path();
			p_context->set_source_rgb((double)KageStage::stageBG.getR()/255, (double)KageStage::stageBG.getG()/255, (double)KageStage::stageBG.getB()/255);
	//		p_context->fill_preserve();
			p_context->fill();
				p_context->set_line_width(0.2f);
				p_context->set_source_rgb(0.0, 0.0, 0.0);
				p_context->stroke();
	}
}

bool KageStage::renderToPNG(string p_path, bool p_transparent) {
	#ifdef CAIRO_HAS_PNG_FUNCTIONS
		Cairo::RefPtr<Cairo::ImageSurface> surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, stageWidth, stageHeight);
		
		Cairo::RefPtr<Cairo::Context> l_context = Cairo::Context::create(surface);
		
		GdkPoint l_tempOrigin;
		l_tempOrigin.x = origin.x;
		l_tempOrigin.y = origin.y;
		
		origin.x = 0;
		origin.y = 0;
		
		if (p_transparent == false) {
			clearScreen(l_context);
		}
		
		unsigned int l_layerCount = win->_layerManager.layerCount();
		unsigned int l_currentLayer = win->getCurrentLayer();
			for (unsigned int i = 1; i <= l_layerCount; ++i) {
				win->_framesetManager.setCurrentLayer(i);
				renderFrameToPNG(l_context);
			}
		win->setCurrentLayer(l_currentLayer);
		
		CairoKage::writeToPNG(p_path, surface);
		
		origin.x = l_tempOrigin.x;
		origin.y = l_tempOrigin.y;
		
		return true;
	#else
		std::cout << "You must compile cairo with PNG support for this example to work." << std::endl;
		return false;
	#endif
}
void KageStage::renderFrameToPNG(Cairo::RefPtr<Cairo::Context> p_context) {
	renderFrame(p_context, true);
}
void KageStage::renderFrame(Cairo::RefPtr<Cairo::Context> p_context, bool p_force) {
	vector<VectorData> v = win->getFrameData(p_force).getVectorData();
	renderFrame(p_context, v);
	if (_polyVectors.isEmpty() == false) {
		renderFrame(p_context, _polyVectors.getVectorData());
	}
}
/**
 * For use by Kage, when Onion Skin is toggled
 */
void KageStage::renderOnionFrame(Cairo::RefPtr<Cairo::Context> p_context, vector<VectorData> p_vectorData, double p_alpha) {
	renderFrame(p_context, p_vectorData, p_alpha);
}
void KageStage::renderFrame(Cairo::RefPtr<Cairo::Context> p_context, vector<VectorData> p_vectorData, double p_alpha) {
	unsigned int vsize = p_vectorData.size();
	unsigned int fillCtr = 0;
	ColorData fcolor;
	StrokeColorData scolor;
	PointData p;
	double x1;
	double y1;
	double x2;
	double y2;
	double x3;
	double y3;
	bool l_mouseLocationOnFill = false;
	unsigned int l_mouseLocationShapeIndex = _NO_SELECTION;
	for (unsigned int i = 0; i < vsize; ++i) {
		switch (p_vectorData[i].vectorType) {
			case VectorData::TYPE_CLOSE_PATH:
				p_context->close_path();
				break;
			case VectorData::TYPE_FILL:
				fcolor = p_vectorData[i].fillColor;
				fillCtr++;
				p_context->begin_new_path();
				break;
			case VectorData::TYPE_ENDFILL:
				l_mouseLocationOnFill = p_context->in_fill(_mouseLocation.x, _mouseLocation.y);
				if (l_mouseLocationOnFill != 0) {
					_mouseLocationShapeIndex = l_mouseLocationShapeIndex;
				}
				if (fillCtr > 0) {
					p_context->set_source_rgba((double)fcolor.getR()/255.0f * p_alpha,
											   (double)fcolor.getG()/255.0f * p_alpha,
											   (double)fcolor.getB()/255.0f * p_alpha,
											   (double)fcolor.getA()/255.0f * p_alpha);
					p_context->fill_preserve();
					fillCtr--;
				}
				if (scolor.getThickness() > 0) {
					p_context->set_source_rgba((double)scolor.getR()/255.0f * p_alpha,
											   (double)scolor.getG()/255.0f * p_alpha,
											   (double)scolor.getB()/255.0f * p_alpha,
											   (double)scolor.getA()/255.0f * p_alpha);
					p_context->set_line_width(scolor.getThickness() * _zoomValue);
						p_context->set_line_cap(Cairo::LINE_CAP_ROUND);
							p_context->stroke();
				}
				if (l_mouseLocationShapeIndex == _NO_SELECTION) {
					l_mouseLocationOnFill = p_context->in_stroke(_mouseLocation.x, _mouseLocation.y);
					if (l_mouseLocationOnFill != 0) {
						_mouseLocationShapeIndex = l_mouseLocationShapeIndex;
					}
				}
				break;
			case VectorData::TYPE_STROKE:
				scolor = p_vectorData[i].stroke;
				break;
			case VectorData::TYPE_MOVE:
				p_context->move_to(p_vectorData[i].points[0].x + origin.x, p_vectorData[i].points[0].y + origin.y);
				
				p.x = p_vectorData[i].points[0].x;
				p.y = p_vectorData[i].points[0].y;
				break;
			case VectorData::TYPE_LINE:
				p_context->line_to(p_vectorData[i].points[0].x + origin.x, p_vectorData[i].points[0].y + origin.y);
				
				p.x = p_vectorData[i].points[0].x;
				p.y = p_vectorData[i].points[0].y;
				break;
			case VectorData::TYPE_CURVE_QUADRATIC:
				//cubic-to-quad algo borrowed from Mono/Moonlight's moon_quad_curve_to
				x1 = p_vectorData[i].points[0].x;
				y1 = p_vectorData[i].points[0].y;
				x2 = p_vectorData[i].points[1].x;
				y2 = p_vectorData[i].points[1].y;
				x3 = x2;
				y3 = y2;
				
				x2 = x1 + (x2 - x1) / 3;
				y2 = y1 + (y2 - y1) / 3;
				x1 = p.x + 2 * (x1 - p.x) / 3;
				y1 = p.y + 2 * (y1 - p.y) / 3;
				
				p_context->curve_to(
						x1 + origin.x, y1 + origin.y,
						x2 + origin.x, y2 + origin.y,
						x3 + origin.x, y3 + origin.y
					);
				
				p.x = x3;
				p.y = y3;
				break;
			case VectorData::TYPE_CURVE_CUBIC:
				p_context->curve_to(
						p_vectorData[i].points[0].x + origin.x, p_vectorData[i].points[0].y + origin.y,
						p_vectorData[i].points[1].x + origin.x, p_vectorData[i].points[1].y + origin.y,
						p_vectorData[i].points[2].x + origin.x, p_vectorData[i].points[2].y + origin.y
				);
				break;
			case VectorData::TYPE_TEXT:
					p_context->select_font_face ("Verdana", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD);
					p_context->set_font_size(32);
					p_context->set_source_rgb(0, 0, 1);
					p_context->move_to(10, 50);
					p_context->show_text("Hello, world");
				break;
			case VectorData::TYPE_IMAGE:
				//2 '1st is for X/Y, 2nd is for width/height  -- ?!?
				break;
			case VectorData::TYPE_INIT:
				l_mouseLocationShapeIndex = i;
				break;
			default:
				break;
		}
	}
}

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
	}
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
	if (win->isLayerLocked() == true) {
		return;
	}
	double diffOld = anchor_lowerRight.y - anchor_upperLeft.y;
	double diffNew = anchor_lowerRight.y - draw2.y;
	double diffRatio = diffNew / diffOld;
	if (diffRatio == 0.0) {
		return;
	}
	
	vector<VectorData> v = win->getFrameData().getVectorData();
	unsigned int typeMovesIndex = -1;
	unsigned int vsize = v.size();
	unsigned int l_selectedShapes_size = selectedShapes.size();
	for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
		for (unsigned int i = selectedShapes[l_shapeIndex]; i < vsize; ++i) {
			if (v[i].vectorType == VectorData::TYPE_MOVE) {
				///register i for later use in ENDFILL
				typeMovesIndex = i;
			} else if (v[i].vectorType == VectorData::TYPE_LINE) {
				diffOld = anchor_lowerRight.y - (v[i].points[0].y + origin.y);
				diffNew = diffRatio * diffOld;
					v[i].points[0].y = anchor_lowerRight.y - diffNew - origin.y;
			} else if (v[i].vectorType == VectorData::TYPE_CURVE_QUADRATIC
					|| v[i].vectorType == VectorData::TYPE_CURVE_CUBIC) {
				diffOld = anchor_lowerRight.y - (v[i].points[0].y + origin.y);
				diffNew = diffRatio * diffOld;
					v[i].points[0].y = anchor_lowerRight.y - diffNew - origin.y;
				
				diffOld = anchor_lowerRight.y - (v[i].points[1].y + origin.y);
				diffNew = diffRatio * diffOld;
					v[i].points[1].y = anchor_lowerRight.y - diffNew - origin.y;
				
				diffOld = anchor_lowerRight.y - (v[i].points[2].y + origin.y);
				diffNew = diffRatio * diffOld;
					v[i].points[2].y = anchor_lowerRight.y - diffNew - origin.y;
			} else if (v[i].vectorType == VectorData::TYPE_ENDFILL) {
				diffOld = anchor_lowerRight.y - (v[typeMovesIndex].points[0].y + origin.y);
				diffNew = diffRatio * diffOld;
					v[typeMovesIndex].points[0].y = anchor_lowerRight.y - diffNew - origin.y;
			} else if (v[i].vectorType == VectorData::TYPE_INIT
					&& i != selectedShapes[l_shapeIndex]) {
				break;
			} else if (v[i].vectorType == VectorData::TYPE_INIT) {
				diffOld = anchor_lowerRight.y - (v[i].points[0].y + origin.y);
				diffNew = diffRatio * diffOld;
					v[i].points[0].y = anchor_lowerRight.y - diffNew - origin.y;
			}
		}
	}
	
	anchor_upperLeft.y = draw2.y;
	
	win->setFrameData(v);
}
void KageStage::handleShapes_scaleEast() {
	if (win->isLayerLocked() == true) {
		return;
	}
	double diffOld = anchor_lowerRight.x - anchor_upperLeft.x;
	double diffNew = draw2.x - anchor_upperLeft.x;
	double diffRatio = diffNew / diffOld;
	if (diffRatio == 0.0) {
		return;
	}
	
	vector<VectorData> v = win->getFrameData().getVectorData();
	unsigned int typeMovesIndex = -1;
	unsigned int vsize = v.size();
	unsigned int l_selectedShapes_size = selectedShapes.size();
	for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
		for (unsigned int i = selectedShapes[l_shapeIndex]; i < vsize; ++i) {
			if (v[i].vectorType == VectorData::TYPE_MOVE) {
				///register i for later use in ENDFILL
				typeMovesIndex = i;
			} else if (v[i].vectorType == VectorData::TYPE_LINE) {
				diffOld = anchor_upperLeft.x - (v[i].points[0].x + origin.x);
				diffNew = diffRatio * diffOld;
					v[i].points[0].x = anchor_upperLeft.x - diffNew - origin.x;
			} else if (v[i].vectorType == VectorData::TYPE_CURVE_QUADRATIC
					|| v[i].vectorType == VectorData::TYPE_CURVE_CUBIC) {
				diffOld = anchor_upperLeft.x - (v[i].points[0].x + origin.x);
				diffNew = diffRatio * diffOld;
					v[i].points[0].x = anchor_upperLeft.x - diffNew - origin.x;
				
				diffOld = anchor_upperLeft.x - (v[i].points[1].x + origin.x);
				diffNew = diffRatio * diffOld;
					v[i].points[1].x = anchor_upperLeft.x - diffNew - origin.x;
				
				diffOld = anchor_upperLeft.x - (v[i].points[2].x + origin.x);
				diffNew = diffRatio * diffOld;
					v[i].points[2].x = anchor_upperLeft.x - diffNew - origin.x;
			} else if (v[i].vectorType == VectorData::TYPE_ENDFILL) {
				diffOld = anchor_upperLeft.x - (v[typeMovesIndex].points[0].x + origin.x);
				diffNew = diffRatio * diffOld;
					v[typeMovesIndex].points[0].x = anchor_upperLeft.x - diffNew - origin.x;
			} else if (v[i].vectorType == VectorData::TYPE_INIT
					&& i != selectedShapes[l_shapeIndex]) {
				break;
			} else if (v[i].vectorType == VectorData::TYPE_INIT) {
				diffOld = anchor_upperLeft.x - (v[i].points[0].x + origin.x);
				diffNew = diffRatio * diffOld;
					v[i].points[0].x = anchor_upperLeft.x - diffNew - origin.x;
			}
		}
	}
	
	anchor_lowerRight.x = draw2.x;
	
	win->setFrameData(v);
}
void KageStage::handleShapes_scaleWest() {
	if (win->isLayerLocked() == true) {
		return;
	}
	double diffOld = anchor_upperLeft.x - anchor_lowerRight.x;
	double diffNew = draw2.x - anchor_lowerRight.x;
	double diffRatio = diffNew / diffOld;
	if (diffRatio == 0.0) {
		return;
	}
	
	vector<VectorData> v = win->getFrameData().getVectorData();
	unsigned int typeMovesIndex = -1;
	unsigned int vsize = v.size();
	unsigned int l_selectedShapes_size = selectedShapes.size();
	for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
		for (unsigned int i = selectedShapes[l_shapeIndex]; i < vsize; ++i) {
			if (v[i].vectorType == VectorData::TYPE_MOVE) {
				///register i for later use in ENDFILL
				typeMovesIndex = i;
			} else if (v[i].vectorType == VectorData::TYPE_LINE) {
				diffOld = anchor_lowerRight.x - (v[i].points[0].x + origin.x);
				diffNew = diffRatio * diffOld;
					v[i].points[0].x = anchor_lowerRight.x - diffNew - origin.x;
			} else if (v[i].vectorType == VectorData::TYPE_CURVE_QUADRATIC
					|| v[i].vectorType == VectorData::TYPE_CURVE_CUBIC) {
				diffOld = anchor_lowerRight.x - (v[i].points[0].x + origin.x);
				diffNew = diffRatio * diffOld;
					v[i].points[0].x = anchor_lowerRight.x - diffNew - origin.x;
				
				diffOld = anchor_lowerRight.x - (v[i].points[1].x + origin.x);
				diffNew = diffRatio * diffOld;
					v[i].points[1].x = anchor_lowerRight.x - diffNew - origin.x;
				
				diffOld = anchor_lowerRight.x - (v[i].points[2].x + origin.x);
				diffNew = diffRatio * diffOld;
					v[i].points[2].x = anchor_lowerRight.x - diffNew - origin.x;
			} else if (v[i].vectorType == VectorData::TYPE_ENDFILL) {
				diffOld = anchor_lowerRight.x - (v[typeMovesIndex].points[0].x + origin.x);
				diffNew = diffRatio * diffOld;
					v[typeMovesIndex].points[0].x = anchor_lowerRight.x - diffNew - origin.x;
			} else if (v[i].vectorType == VectorData::TYPE_INIT
					&& i != selectedShapes[l_shapeIndex]) {
				break;
			} else if (v[i].vectorType == VectorData::TYPE_INIT) {
				diffOld = anchor_lowerRight.x - (v[i].points[0].x + origin.x);
				diffNew = diffRatio * diffOld;
					v[i].points[0].x = anchor_lowerRight.x - diffNew - origin.x;
			}
		}
	}
	
	anchor_upperLeft.x = draw2.x;
	
	win->setFrameData(v);
}
void KageStage::handleShapes_scaleSouth() {
	if (win->isLayerLocked() == true) {
		return;
	}
	double diffOld = anchor_lowerRight.y - anchor_upperLeft.y;
	double diffNew = draw2.y - anchor_upperLeft.y;
	double diffRatio = diffNew / diffOld;
	if (diffRatio == 0.0) {
		return;
	}
	
	vector<VectorData> v = win->getFrameData().getVectorData();
	unsigned int typeMovesIndex = -1;
	unsigned int vsize = v.size();
	unsigned int l_selectedShapes_size = selectedShapes.size();
	for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
		for (unsigned int i = selectedShapes[l_shapeIndex]; i < vsize; ++i) {
			if (v[i].vectorType == VectorData::TYPE_MOVE) {
				///register i for later use in ENDFILL
				typeMovesIndex = i;
			} else if (v[i].vectorType == VectorData::TYPE_LINE) {
				diffOld = anchor_upperLeft.y - (v[i].points[0].y + origin.y);
				diffNew = diffRatio * diffOld;
					v[i].points[0].y = anchor_upperLeft.y - diffNew - origin.y;
			} else if (v[i].vectorType == VectorData::TYPE_CURVE_QUADRATIC
					|| v[i].vectorType == VectorData::TYPE_CURVE_CUBIC) {
				diffOld = anchor_upperLeft.y - (v[i].points[0].y + origin.y);
				diffNew = diffRatio * diffOld;
					v[i].points[0].y = anchor_upperLeft.y - diffNew - origin.y;
				
				diffOld = anchor_upperLeft.y - (v[i].points[1].y + origin.y);
				diffNew = diffRatio * diffOld;
					v[i].points[1].y = anchor_upperLeft.y - diffNew - origin.y;
				
				diffOld = anchor_upperLeft.y - (v[i].points[2].y + origin.y);
				diffNew = diffRatio * diffOld;
					v[i].points[2].y = anchor_upperLeft.y - diffNew - origin.y;
			} else if (v[i].vectorType == VectorData::TYPE_ENDFILL) {
				diffOld = anchor_upperLeft.y - (v[typeMovesIndex].points[0].y + origin.y);
				diffNew = diffRatio * diffOld;
					v[typeMovesIndex].points[0].y = anchor_upperLeft.y - diffNew - origin.y;
			} else if (v[i].vectorType == VectorData::TYPE_INIT
					&& i != selectedShapes[l_shapeIndex]) {
				break;
			} else if (v[i].vectorType == VectorData::TYPE_INIT) {
				diffOld = anchor_upperLeft.y - (v[i].points[0].y + origin.y);
				diffNew = diffRatio * diffOld;
					v[i].points[0].y = anchor_upperLeft.y - diffNew - origin.y;
			}
		}
	}
	
	anchor_lowerRight.y = draw2.y;
	
	win->setFrameData(v);
}
void KageStage::handleShapes_moveShape(double p_diffX, double p_diffY) {
	if (p_diffX == 0 && p_diffY == 0) {
		return;
	}
	
	vector<VectorData> v = win->getFrameData().getVectorData();
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
	
	win->setFrameData(v);
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
	
	vector<VectorData> v = win->getFrameData().getVectorData();
	unsigned int vsize = v.size();
	unsigned int l_selectedShapes_size = selectedShapes.size();
	for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
		for (unsigned int i = selectedShapes[l_shapeIndex]; i < vsize; ++i) {
			if (v[i].vectorType == VectorData::TYPE_FILL) {
				if (p_doFill == true) {
					v[i].fillColor = fillColor.clone();
				}
			} else if (v[i].vectorType == VectorData::TYPE_STROKE) {
				if (p_doStroke == true) {
					v[i].stroke = stroke.clone();
				}
				
				break;
			}
		}
	}
	
	win->setFrameData(v);
	
	win->stackDo();
	
	render();
}

void KageStage::updateShapeX(double p_value) {
	Kage::timestamp();
	std::cout << " KageStage::updateShapeX " << selectedShapes.size() << " " << p_value << std::endl;
	
	if (selectedShapes.size() == 0) {
		return;
	}
	
	vector<VectorData> v = win->getFrameData().getVectorData();
	
	unsigned int vsize = v.size();
	unsigned int l_selectedShapes_size = selectedShapes.size();
	for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
		//calling getSelectedShapeViaNode will update propXindex1/propXindex2
		getSelectedShapeViaNode(selectedShapes[l_shapeIndex]+3, v);
		
		double l_propXprev = v[propXindex1].points[propXindex2].x;
		double l_propXdiff = p_value - l_propXprev;
		
		for (unsigned int i = selectedShapes[l_shapeIndex]; i < vsize; ++i) {
			if (v[i].vectorType == VectorData::TYPE_CURVE_CUBIC) {
				v[i].points[0].x += l_propXdiff;
				v[i].points[1].x += l_propXdiff;
				v[i].points[2].x += l_propXdiff;
			} else if (v[i].vectorType == VectorData::TYPE_MOVE) {
				v[i].points[0].x += l_propXdiff;
			} else if (v[i].vectorType == VectorData::TYPE_IMAGE) {
				//TODO:handle X for images
			} else if (v[i].vectorType == VectorData::TYPE_INIT
					&& i != selectedShapes[l_shapeIndex]) {
				break;
			} else if (v[i].vectorType == VectorData::TYPE_INIT) {
				v[i].points[0].x += l_propXdiff;
			}
		}
	}
	
	win->setFrameData(v);
	
	win->stackDo();
	
	render();
}
void KageStage::updateShapeY(double p_value) {
	Kage::timestamp();
	std::cout << " KageStage::updateShapeY " << selectedShapes.size() << " " << p_value << std::endl;
	
	if (selectedShapes.size() == 0) {
		return;
	}
	
	vector<VectorData> v = win->getFrameData().getVectorData();
	
	unsigned int vsize = v.size();
	unsigned int l_selectedShapes_size = selectedShapes.size();
	for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
		//calling getSelectedShapeViaNode will update propXindex1/propXindex2
		getSelectedShapeViaNode(selectedShapes[l_shapeIndex]+3, v);
		
		double l_propYprev = v[propYindex1].points[propYindex2].y;
		double l_propYdiff = p_value - l_propYprev;
		
		for (unsigned int i = selectedShapes[l_shapeIndex]; i < vsize; ++i) {
			if (v[i].vectorType == VectorData::TYPE_CURVE_CUBIC) {
				v[i].points[0].y += l_propYdiff;
				v[i].points[1].y += l_propYdiff;
				v[i].points[2].y += l_propYdiff;
			} else if (v[i].vectorType == VectorData::TYPE_MOVE) {
				v[i].points[0].y += l_propYdiff;
			} else if (v[i].vectorType == VectorData::TYPE_IMAGE) {
				//TODO:handle Y for images
			} else if (v[i].vectorType == VectorData::TYPE_INIT
					&& i != selectedShapes[l_shapeIndex]) {
				break;
			} else if (v[i].vectorType == VectorData::TYPE_INIT) {
				v[i].points[0].y += l_propYdiff;
			}
		}
	}
	
	win->setFrameData(v);
	
	win->stackDo();
	
	render();
}
void KageStage::updateShapeWidth(double p_value) {
	std::cout << " KageStage::updateShapeWidth " << selectedShape << " " << p_value << std::endl;
	GdkPoint l_tmpPt;
	l_tmpPt.x = draw2.x;
	draw2.x = propX + p_value + origin.x;
	mouseOnAnchor = AnchorData::TYPE_EAST;
	mouseDown = true;
		handleShapes_modifyingShape();
	mouseDown = false;
	mouseOnAnchor = AnchorData::TYPE_NONE;
	draw2.x = l_tmpPt.x;
	
	handleNodesMouseUp();
	win->updateProperties();
	
	render();
}
void KageStage::updateShapeHeight(double p_value) {
	std::cout << " KageStage::updateShapeHeight " << selectedShape << " " << p_value << std::endl;
	GdkPoint l_tmpPt;
	l_tmpPt.y = draw2.y;
	draw2.y = propY + p_value + origin.y;
	mouseOnAnchor = AnchorData::TYPE_SOUTH;
	mouseDown = true;
		handleShapes_modifyingShape();
	mouseDown = false;
	mouseOnAnchor = AnchorData::TYPE_NONE;
	draw2.y = l_tmpPt.y;
	
	handleNodesMouseUp();
	win->updateProperties();
	
	render();
}

void KageStage::updateNodeX(double p_value) {
	Kage::timestamp();
	std::cout << " KageStage::updateNodeX " << selectedNodes.size() << " " << p_value << std::endl;
	
	if (selectedNodes.size() == 0) {
		return;
	}
	
	vector<VectorData> v = win->getFrameData().getVectorData();
	for (unsigned int l_selectedNode = 0; l_selectedNode < selectedNodes.size(); ++l_selectedNode) {
		double l_propXdiff = p_value - v[selectedNodes[l_selectedNode]].points[2].x;
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
								v[i].points[0].x += l_propXdiff; // for Node MOVE
								v[i+1].points[0].x += l_propXdiff; // for Node Curve's control point being used by Node MOVE
							}
							break;
						}
					}
				}
			} else if (v[selectedNodes[l_selectedNode]].vectorType == VectorData::TYPE_IMAGE) {
				//TODO:handle X for images
			} else if (v[selectedNodes[l_selectedNode]].vectorType == VectorData::TYPE_ENDFILL) {
				//
			}
	}
	
	win->setFrameData(v);
	
	win->stackDo();
	
	render();
}

void KageStage::updateNodeY(double p_value) {
	Kage::timestamp();
	std::cout << " KageStage::updateNodeY " << selectedNodes.size() << " " << p_value << std::endl;
	
	if (selectedNodes.size() == 0) {
		return;
	}
	
	vector<VectorData> v = win->getFrameData().getVectorData();
	for (unsigned int l_selectedNode = 0; l_selectedNode < selectedNodes.size(); ++l_selectedNode) {
		double l_propYdiff = p_value - v[selectedNodes[l_selectedNode]].points[2].y;
			if (v[selectedNodes[l_selectedNode]].vectorType == VectorData::TYPE_CURVE_CUBIC) {
				v[selectedNodes[l_selectedNode]].points[1].y += l_propYdiff;
				v[selectedNodes[l_selectedNode]].points[2].y += l_propYdiff;
				
				if (v[selectedNodes[l_selectedNode]+1].getPoints().size() != 0) {
					v[selectedNodes[l_selectedNode]+1].points[0].y += l_propYdiff;
				} else {
					for (unsigned int i = selectedNodes[l_selectedNode]-1; i >= 0; --i) {
						if (v[i].vectorType == VectorData::TYPE_MOVE) {
							if (i+1 != selectedNodes[l_selectedNode]) {
								v[i].points[0].y += l_propYdiff;
								v[i+1].points[0].y += l_propYdiff;
							}
							break;
						}
					}
				}
			} else if (v[selectedNodes[l_selectedNode]].vectorType == VectorData::TYPE_IMAGE) {
				//TODO:handle Y for images
			} else if (v[selectedNodes[l_selectedNode]].vectorType == VectorData::TYPE_ENDFILL) {
				//
			}
	}
	
	win->setFrameData(v);
	
	win->stackDo();
	
	render();
}

void KageStage::drawSelectionArea() {
	vector<double> dashes;
		dashes.push_back(4.0); /* ink */
		dashes.push_back(4.0); /* skip */
	
	cr->set_dash(dashes, 0.0);
	//draw bounding rectangle for the shape
	cr->move_to(draw1.x, draw1.y);
		cr->line_to(draw2.x, draw1.y);
		cr->line_to(draw2.x, draw2.y);
		cr->line_to(draw1.x, draw2.y);
		cr->line_to(draw1.x, draw1.y);
			cr->set_source_rgba(0.0,0.0,0.0,1.0);
			cr->set_line_width(1.0);
				cr->set_line_cap(Cairo::LINE_CAP_ROUND);
					cr->stroke();
	cr->unset_dash();
}
void KageStage::handleShapes_modifyingShapeRotate() {
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
	vector<VectorData> v = win->getFrameData().getVectorData();
	if (l_selectedShapes_size > 0) {
		for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
			for (unsigned int i = selectedShapes[l_shapeIndex]; i < v.size(); ++i) {
				if (v[i].vectorType == VectorData::TYPE_INIT
					&& i != selectedShapes[l_shapeIndex]) {
					break;
				} else if (v[i].vectorType == VectorData::TYPE_INIT) {
					//use as rotation-anchor ONLY if just ONE selected shape
					if (l_selectedShapes_size == 1) {
						anchor_center.x = v[i].points[0].x + origin.x;
						anchor_center.y = v[i].points[0].y + origin.y;
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
					
					v[i].points[j].x += origin.x;
					v[i].points[j].y += origin.y;
					
						PointData l_pointRotate;
							l_pointRotate.x = v[i].points[j].x - anchor_center.x;
							l_pointRotate.y = v[i].points[j].y - anchor_center.y;
						double l_pointAngle = atan2(l_pointRotate.y, l_pointRotate.x) + l_angleDiff;
						
						//apply new angle
						double l_hyp = sqrtf((l_pointRotate.x * l_pointRotate.x) + (l_pointRotate.y * l_pointRotate.y));
						v[i].points[j].x = cos(l_pointAngle) * l_hyp + anchor_center.x;
						v[i].points[j].y = sin(l_pointAngle) * l_hyp + anchor_center.y;
					
					v[i].points[j].x -= origin.x;
					v[i].points[j].y -= origin.y;
				} else if (v[i].vectorType == VectorData::TYPE_CURVE_CUBIC
						|| v[i].vectorType == VectorData::TYPE_CURVE_QUADRATIC) {
					l_angleDiff = l_angleNew - l_angleOld;
					for (unsigned int j = 0; j < 3; ++j) {
						v[i].points[j].x += origin.x;
						v[i].points[j].y += origin.y;
						
							PointData l_pointRotate;
								l_pointRotate.x = v[i].points[j].x - anchor_center.x;
								l_pointRotate.y = v[i].points[j].y - anchor_center.y;
							double l_hyp = sqrtf((l_pointRotate.x * l_pointRotate.x) + (l_pointRotate.y * l_pointRotate.y));
							
							double l_pointAngle = atan2(l_pointRotate.y, l_pointRotate.x) + l_angleDiff;
							//apply new angle
							v[i].points[j].x = cos(l_pointAngle) * l_hyp + anchor_center.x;
							v[i].points[j].y = sin(l_pointAngle) * l_hyp + anchor_center.y;
						
						v[i].points[j].x -= origin.x;
						v[i].points[j].y -= origin.y;
					}
				}
			}
		}
		win->setFrameData(v);
	}
	
	//win->stackDo();
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
		if (isMouseOnNode(           anchor_center.x    ,  anchor_upperLeft.y - 7, 7) == true) {
			mouseOnAnchor = AnchorData::TYPE_NORTH;
		} else if (isMouseOnNode(anchor_lowerRight.x + 7,     anchor_center.y    , 7) == true) {
			mouseOnAnchor = AnchorData::TYPE_EAST;
		} else if (isMouseOnNode( anchor_upperLeft.x - 7,     anchor_center.y    , 7) == true) {
			mouseOnAnchor = AnchorData::TYPE_WEST;
		} else if (isMouseOnNode(    anchor_center.x    , anchor_lowerRight.y + 7, 7) == true) {
			mouseOnAnchor = AnchorData::TYPE_SOUTH;
		} else if (isMouseOnNode(anchor_lowerRight.x + 7,  anchor_upperLeft.y - 7, 7) == true) {
			mouseOnAnchor = AnchorData::TYPE_NORTH_EAST;
		} else if (isMouseOnNode( anchor_upperLeft.x - 7,  anchor_upperLeft.y - 7, 7) == true) {
			mouseOnAnchor = AnchorData::TYPE_NORTH_WEST;
		} else if (isMouseOnNode(anchor_lowerRight.x + 7, anchor_lowerRight.y + 7, 7) == true) {
			mouseOnAnchor = AnchorData::TYPE_SOUTH_EAST;
		} else if (isMouseOnNode( anchor_upperLeft.x - 7, anchor_lowerRight.y + 7, 7) == true) {
			mouseOnAnchor = AnchorData::TYPE_SOUTH_WEST;
		} else {
			double l_X = anchor_lowerRight.x - anchor_center.x;
			double l_Y = anchor_lowerRight.y - anchor_center.y;
			double l_distance = 7;
			if (l_X > l_Y) {
				l_distance = l_Y;
			} else {
				l_distance = l_X;
			}
			if (isMouseOnNode(    anchor_center.x    ,     anchor_center.y    , l_distance) == true) {
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
			draw2.x - anchor_center.x,
			draw2.y - anchor_center.y);
		
		anchor_center.x = draw2.x;
		anchor_center.y = draw2.y;
	} else if (mouseOnAnchor == AnchorData::TYPE_ROTATE) {
		AnchorData l_anchor;
		vector<VectorData> v = win->getFrameData().getVectorData();
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
							anchor_rotate.x = draw2.x;
							anchor_rotate.y = draw2.y;
						} else {
							v[i].points[0].x = draw2.x - origin.x;
							v[i].points[0].y = draw2.y - origin.y;
						}
						
						l_anchor.x = draw2.x;
						l_anchor.y = draw2.y;
					}
				}
			}
			
			win->setFrameData(v);
		renderFrame(cr, v);
		
		//draw rotate anchor
		if (l_anchor.x == 0 && l_anchor.y == 0) {
			Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_ROTATE,
						anchor_center.x -  7,
						anchor_center.y -  7);
		} else {
			Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_ROTATE,
						l_anchor.x -  7,
						l_anchor.y -  7);
		}
			cr->paint();
	} else if (mouseOnAnchor != AnchorData::TYPE_NONE
			&& mouseOnAnchor != AnchorData::TYPE_MOVE
			&& _rotateMode == true) {
		PointData l_rotateNew;
			l_rotateNew.x = draw2.x - anchor_rotate.x;
			l_rotateNew.y = draw2.y - anchor_rotate.y;
		double l_angleNew = atan2(l_rotateNew.y, l_rotateNew.x);
		
		PointData l_rotateOld;
			l_rotateOld.x = draw1.x - anchor_rotate.x;
			l_rotateOld.y = draw1.y - anchor_rotate.y;
		double l_angleOld = atan2(l_rotateOld.y, l_rotateOld.x);
		
		double l_angleDiff;
		
		vector<VectorData> v = win->getFrameData().getVectorData();
		unsigned int l_selectedShapes_size = selectedShapes.size();
		if (l_selectedShapes_size > 0) {
			for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
				for (unsigned int i = selectedShapes[l_shapeIndex]; i < v.size(); ++i) {
					if (v[i].vectorType == VectorData::TYPE_INIT
						&& i != selectedShapes[l_shapeIndex]) {
						break;
					} else if (v[i].vectorType == VectorData::TYPE_INIT) {
						//use as rotation-anchor ONLY if just ONE selected shape
						if (l_selectedShapes_size == 1) {
							anchor_center.x = v[i].points[0].x + origin.x;
							anchor_center.y = v[i].points[0].y + origin.y;
								l_rotateNew.x = draw2.x - anchor_center.x;
								l_rotateNew.y = draw2.y - anchor_center.y;
									l_angleNew = atan2(l_rotateNew.y, l_rotateNew.x);
								l_rotateOld.x = draw1.x - anchor_center.x;
								l_rotateOld.y = draw1.y - anchor_center.y;
									l_angleOld = atan2(l_rotateOld.y, l_rotateOld.x);
						}
					} else if (v[i].vectorType == VectorData::TYPE_MOVE) {
						l_angleDiff = l_angleNew - l_angleOld;
						
						v[i].points[0].x += origin.x;
						v[i].points[0].y += origin.y;
						
							PointData l_pointRotate;
								l_pointRotate.x = v[i].points[0].x - anchor_center.x;
								l_pointRotate.y = v[i].points[0].y - anchor_center.y;
							double l_pointAngle = atan2(l_pointRotate.y, l_pointRotate.x) + l_angleDiff;
							
							//apply new angle
							double l_hyp = sqrtf((l_pointRotate.x * l_pointRotate.x) + (l_pointRotate.y * l_pointRotate.y));
							v[i].points[0].x = cos(l_pointAngle) * l_hyp + anchor_center.x;
							v[i].points[0].y = sin(l_pointAngle) * l_hyp + anchor_center.y;
						
						v[i].points[0].x -= origin.x;
						v[i].points[0].y -= origin.y;
					} else if (v[i].vectorType == VectorData::TYPE_CURVE_CUBIC
							|| v[i].vectorType == VectorData::TYPE_CURVE_QUADRATIC) {
						l_angleDiff = l_angleNew - l_angleOld;
						for (unsigned int j = 0; j < 3; ++j) {
							v[i].points[j].x += origin.x;
							v[i].points[j].y += origin.y;
							
								PointData l_pointRotate;
									l_pointRotate.x = v[i].points[j].x - anchor_center.x;
									l_pointRotate.y = v[i].points[j].y - anchor_center.y;
								double l_hyp = sqrtf((l_pointRotate.x * l_pointRotate.x) + (l_pointRotate.y * l_pointRotate.y));
								
								double l_pointAngle = atan2(l_pointRotate.y, l_pointRotate.x) + l_angleDiff;
								//apply new angle
								v[i].points[j].x = cos(l_pointAngle) * l_hyp + anchor_center.x;
								v[i].points[j].y = sin(l_pointAngle) * l_hyp + anchor_center.y;
							
							v[i].points[j].x -= origin.x;
							v[i].points[j].y -= origin.y;
						}
					}
				}
			}
			
			renderFrame(cr, v);
			_rotateApply = true;
		}
	} else {
		if (_rotateMode == true) {
			vector<VectorData> v = win->getFrameData().getVectorData();
			renderFrame(cr, v);
		}
	}
	
	vector<double> dashes;
		dashes.push_back(4.0); /* ink */
		dashes.push_back(4.0); /* skip */
	
	cr->set_dash(dashes, 0.0);
	//draw bounding rectangle for the shape
	cr->move_to(anchor_upperLeft.x, anchor_upperLeft.y);
		cr->line_to( anchor_lowerRight.x,  anchor_upperLeft.y);
		cr->line_to( anchor_lowerRight.x, anchor_lowerRight.y);
		cr->line_to(  anchor_upperLeft.x, anchor_lowerRight.y);
		cr->line_to(  anchor_upperLeft.x,  anchor_upperLeft.y);
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
	
	vector<VectorData> v = win->getFrameData().getVectorData();
	
	anchor_upperLeft.x = 100000;
	anchor_upperLeft.y = 100000;
	
	anchor_lowerRight.x = -100000;
	anchor_lowerRight.y = -100000;
	double l_anchorX = 0;
	double l_anchorY = 0;
	unsigned int vsize = v.size();
	unsigned int l_selectedShapes_size = selectedShapes.size();
	for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
		for (unsigned int i = selectedShapes[l_shapeIndex]; i < vsize; ++i) {
			if (v[i].vectorType == VectorData::TYPE_LINE) {
				handleShapes_updateAnchors(
					v[i].points[0].x + origin.x,
					v[i].points[0].y + origin.y);
			} else if (v[i].vectorType == VectorData::TYPE_CURVE_QUADRATIC
					|| v[i].vectorType == VectorData::TYPE_CURVE_CUBIC) {
				handleShapes_updateAnchors(
					v[i].points[0].x + origin.x,
					v[i].points[0].y + origin.y);
				handleShapes_updateAnchors(
					v[i].points[1].x + origin.x,
					v[i].points[1].y + origin.y);
				handleShapes_updateAnchors(
					v[i].points[2].x + origin.x,
					v[i].points[2].y + origin.y);
			} else if (v[i].vectorType == VectorData::TYPE_INIT
					&& i != selectedShapes[l_shapeIndex]) {
				break;
			} else if (v[i].vectorType == VectorData::TYPE_INIT) {
				if (l_selectedShapes_size == 1) {
					l_anchorX = v[i].points[0].x + origin.x;
					l_anchorY = v[i].points[0].y + origin.y;
				}
			}
		}
	}
	
	vector<double> dashes;
		dashes.push_back(4.0); /* ink */
		dashes.push_back(4.0); /* skip */
	
	cr->set_dash(dashes, 0.0);
	//draw bounding rectangle for the shape
	cr->move_to(anchor_upperLeft.x, anchor_upperLeft.y);
		cr->line_to( anchor_lowerRight.x,  anchor_upperLeft.y);
		cr->line_to( anchor_lowerRight.x, anchor_lowerRight.y);
		cr->line_to(  anchor_upperLeft.x, anchor_lowerRight.y);
		cr->line_to(  anchor_upperLeft.x,  anchor_upperLeft.y);
			cr->set_source_rgba(0.0,0.0,0.0,1.0);
			cr->set_line_width(1.0);
				cr->set_line_cap(Cairo::LINE_CAP_ROUND);
					cr->stroke();
	cr->unset_dash();
	
	//draw center anchor
/*		cr->set_line_width(1.0);
	cr->arc(anchor_center.x, anchor_center.y, 7, 0, 2 * M_PI);
		cr->set_source_rgba(1.0,1.0,1.0,1.0);
			cr->fill_preserve();
		cr->set_source_rgba(0.0, 0.0, 0.0, 1.0);
			cr->stroke();*/
	
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
				 anchor_upperLeft.x - 13,
				 anchor_upperLeft.y - 13);
			cr->paint();
		
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_SW,
				anchor_lowerRight.x     ,
				 anchor_upperLeft.y - 13);
			cr->paint();
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_NW,
				anchor_lowerRight.x     ,
				anchor_lowerRight.y     );
			cr->paint();
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_NE,
				 anchor_upperLeft.x - 13,
				anchor_lowerRight.y     );
			cr->paint();
		
		//TODO: once shape-Skewing is implemented, draw skewing-anchors
		/*Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_090,
					anchor_center.x -  7,
				 anchor_upperLeft.y - 13);
			cr->paint();
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_090,
					anchor_center.x -  7,
				anchor_lowerRight.y     );
			cr->paint();
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_000,
				 anchor_upperLeft.x - 13,
					anchor_center.y -  7);
			cr->paint();
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_000,
				anchor_lowerRight.x     ,
					anchor_center.y -  7);
			cr->paint();*/
		
		//draw rotate anchor
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_ROTATE,
					anchor_center.x -  7,
					anchor_center.y -  7);
			cr->paint();
	} else {
		anchor_center.x = (anchor_upperLeft.x + anchor_lowerRight.x) / 2;
		anchor_center.y = (anchor_upperLeft.y + anchor_lowerRight.y) / 2;
		
		//draw 4 corners anchors
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_135,
				 anchor_upperLeft.x - 13,
				 anchor_upperLeft.y - 13);
			cr->paint();
		
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_045,
				anchor_lowerRight.x     ,
				 anchor_upperLeft.y - 13);
			cr->paint();
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_135,
				anchor_lowerRight.x     ,
				anchor_lowerRight.y     );
			cr->paint();
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_045,
				 anchor_upperLeft.x - 13,
				anchor_lowerRight.y     );
			cr->paint();
		
		//draw mid anchors
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_090,
					anchor_center.x -  7,
				 anchor_upperLeft.y - 13);
			cr->paint();
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_090,
					anchor_center.x -  7,
				anchor_lowerRight.y     );
			cr->paint();
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_000,
				 anchor_upperLeft.x - 13,
					anchor_center.y -  7);
			cr->paint();
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_000,
				anchor_lowerRight.x     ,
					anchor_center.y -  7);
			cr->paint();
		
		//draw move anchor
		Gdk::Cairo::set_source_pixbuf(cr, KageStage::imageSHAPE_MOVE,
					anchor_center.x -  7,
					anchor_center.y -  7);
			cr->paint();
	}
}

void KageStage::trySingleSelectShape() {
	if (_mouseLocationShapeIndex != _NO_SELECTION) {
		if (isSelectedShape(_mouseLocationShapeIndex) == true) {
			_rotateMode = !_rotateMode;
		} else {
			if (keyShiftDown == false) {
				selectedShapes.clear();
			}
			addSelectedShape(_mouseLocationShapeIndex);
			_rotateMode = false;
		}
		
		//this should be able to update Property pane's X/Y Width Height
		getSelectedShapeViaNode(_mouseLocationShapeIndex+3, win->getFrameData().getVectorData());
		
		win->propStageSetVisible(false);
		win->propFillStrokeSetVisible(true);
		win->propLocationSizeSetVisible(true);
		win->propNodeXYSetVisible(false);
		
		win->updateColors();
		win->updateProperties();
	} else if (keyShiftDown == false) {
		selectedShapes.clear();
	}
}
void KageStage::tryMultiSelectShapes_populateShapes() {
	selectedShapes.clear();
	vector<VectorData> v = win->getFrameData().getVectorData();
	for (unsigned int i = 0; i < selectedNodes.size(); ++i) {
		addSelectedShape(getShape(selectedNodes[i], v));
	}
}
void KageStage::tryMultiSelectShapes() {
	vector<VectorData> v = win->getFrameData().getVectorData();
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
		}
	}
	
	tryMultiSelectShapes_populateShapes();
	
	if (selectedShapes.size() > 0) {
		if (KageStage::toolMode == MODE_SELECT) {
			win->propStageSetVisible(false);
			win->propFillStrokeSetVisible(true);
			win->propLocationSizeSetVisible(true);
		}
	}
	
	render();
}

unsigned int KageStage::getShape(unsigned int p_index, vector<VectorData> p_v) {
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
	
	win->updateColors();
	win->updateProperties();
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
	for (unsigned int l_selectedShape = 0; l_selectedShape < selectedShapes.size(); ++l_selectedShape) {
		for (unsigned int i = selectedShapes[l_selectedShape]; i < vsize; ++i) {
			switch (v[i].vectorType) {
				case VectorData::TYPE_CLOSE_PATH:
				case VectorData::TYPE_FILL:
					break;
				case VectorData::TYPE_ENDFILL:
					i = vsize;
				case VectorData::TYPE_STROKE:
				case VectorData::TYPE_MOVE:
				case VectorData::TYPE_LINE:
					break;
				case VectorData::TYPE_CURVE_QUADRATIC:
				case VectorData::TYPE_CURVE_CUBIC:
					if (isMouseOnNode(
							v[i].points[2].x + origin.x,
							v[i].points[2].y + origin.y) == true) {
						mouseOnNodeHover = i;
						if (mouseDown == true) {
							mouseOnNode = i;
							if (isSelectedNode(i) == false && keyShiftDown == false) {
								selectedNodes.clear();
							}
								addSelectedNode(i);
							mouseOnNodeIndex = 3;
							mouseOnNodeOffset.x = _mouseLocation.x - v[i].points[2].x - origin.x;
							mouseOnNodeOffset.y = _mouseLocation.y - v[i].points[2].y - origin.y;
//							((draw1.x - origin.x - mouseOnNodeOffset.x) - v[i].points[2].x)
						}
					}
					
					//draw control points
					if (isMouseOnNode(
							v[i].points[0].x + origin.x,
							v[i].points[0].y + origin.y) == true) {
						if (mouseDown == true) {
							mouseOnNode = i;
							if (keyShiftDown == false) { selectedNodes.clear(); }
							if (i > 0 && (
									v[i-1].vectorType == VectorData::TYPE_LINE
									|| v[i-1].vectorType == VectorData::TYPE_CURVE_QUADRATIC
									|| v[i-1].vectorType == VectorData::TYPE_CURVE_CUBIC)) {
								addSelectedNode(i-1);
							}
							mouseOnNodeIndex = 1;
						}
					}
					
					if (isMouseOnNode(
							v[i].points[1].x + origin.x,
							v[i].points[1].y + origin.y) == true) {
						if (mouseDown == true) {
							mouseOnNode = i;
							if (keyShiftDown == false) { selectedNodes.clear(); }
							addSelectedNode(i);
							mouseOnNodeIndex = 2;
						}
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
	for (unsigned int l_selectedShape = 0; l_selectedShape < selectedShapes.size(); ++l_selectedShape) {
		for (unsigned int i = selectedShapes[l_selectedShape]; i < vsize; ++i) {
			switch (v[i].vectorType) {
				case VectorData::TYPE_CLOSE_PATH:
					//TODO: add flag as needed to support node-editing of non-closed shapes
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
							}/*
							typeMovesIndex = UINT_MAX;*///*
						} else {/*
							typeEndsPointSize = v[i-1].points.size()-1;
							if (v[typeCubicCurvesIndex].points[0].x != v[typeCubicCurvesIndex].points[typeEndsPointSize].x
									|| v[typeCubicCurvesIndex].points[0].y != v[typeCubicCurvesIndex].points[typeEndsPointSize].y) {
								v[typeCubicCurvesIndex].points[0].x = v[i-1].points[typeEndsPointSize].x;
								v[typeCubicCurvesIndex].points[0].y = v[i-1].points[typeEndsPointSize].y;
							}/*
							typeCubicCurvesIndex = UINT_MAX;/ **/
						}
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
							mouseOnNode = i;
							
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
									v[typeMovesIndex+1].points[0].x += l_x;
									v[typeMovesIndex+1].points[0].y += l_y;
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
	unsigned int vsize = v.size();
	for (unsigned int l_selectedShape = 0; l_selectedShape < selectedShapes.size(); ++l_selectedShape) {
		for (unsigned int i = selectedShapes[l_selectedShape]; i < vsize; ++i) {
			switch (v[i].vectorType) {
				case VectorData::TYPE_CLOSE_PATH:
				case VectorData::TYPE_FILL:
				case VectorData::TYPE_ENDFILL:
//					i = vsize;
					break;
				case VectorData::TYPE_STROKE:
				case VectorData::TYPE_MOVE:
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
							cr->set_line_cap(Cairo::LINE_CAP_ROUND);
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
							cr->set_line_cap(Cairo::LINE_CAP_ROUND);
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
	selectedNode = -1;
	mouseOnNode = -1;
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
			win->propLocationSizeSetVisible(true);
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
			nodeX = l_nodeX;
			nodeY = l_nodeY;
			win->updateNodeXY();
			win->propStageSetVisible(false);
			win->propNodeXYSetVisible(true);
		}
	}
	
	render();
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
	
	unsigned int l_tmp = selectedShape;
	
	propX = DBL_MAX;
	propXindex1 = -1;
	propXindex2 = -1;
	propY = DBL_MAX;
	propYindex1 = -1;
	propYindex2 = -1;
	propWidth = 0.0;
	propHeight = 0.0;
	nodeX = DBL_MAX;
	nodeY = DBL_MAX;
	for (unsigned int i = p_index; i >= 0; --i) {
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
						}
						if (propY < p_v[j].points[0].y
								&& propHeight < (p_v[j].points[0].y - propY)) {
							propHeight = p_v[j].points[0].y - propY;
						}
					} else if (p_v[j].vectorType == VectorData::TYPE_CURVE_CUBIC) {
						if (propX < p_v[j].points[0].x
								&& propWidth < (p_v[j].points[0].x - propX)) {
							propWidth = p_v[j].points[0].x - propX;
						}
						if (propX < p_v[j].points[1].x
								&& propWidth < (p_v[j].points[1].x - propX)) {
							propWidth = p_v[j].points[1].x - propX;
						}
						if (propX < p_v[j].points[2].x
								&& propWidth < (p_v[j].points[2].x - propX)) {
							propWidth = p_v[j].points[2].x - propX;
						}

						if (propY < p_v[j].points[0].y
								&& propHeight < (p_v[j].points[0].y - propY)) {
							propHeight = p_v[j].points[0].y - propY;
						}
						if (propY < p_v[j].points[1].y
								&& propHeight < (p_v[j].points[1].y - propY)) {
							propHeight = p_v[j].points[1].y - propY;
						}
						if (propY < p_v[j].points[2].y
								&& propHeight < (p_v[j].points[2].y - propY)) {
							propHeight = p_v[j].points[2].y - propY;
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
			cr->set_line_cap(Cairo::LINE_CAP_ROUND);
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

void KageStage::handleDrawOvalMouseUp() {
	if (win->isLayerLocked() == true) {
		return;
	}
	if (draw1.x == draw2.x && draw1.y == draw2.y) { return; }
	
	double l_x1, l_y1;
	double l_x2, l_y2;
	double l_x3, l_y3;
	l_x1 = ((draw1.x + draw2.x)/2);
	l_y1 = ((draw1.y + draw2.y)/2);
	l_x2 = ((l_x1 - draw1.x) * 0.4) + draw1.x;
	l_x3 = ((l_x1 - draw1.x) * 1.6) + draw1.x;
	l_y2 = ((l_y1 - draw1.y) * 0.4) + draw1.y;
	l_y3 = ((l_y1 - draw1.y) * 1.6) + draw1.y;
	PointData  p1(draw1.x ,     l_y1);
	PointData  p2(draw1.x ,     l_y2);
	PointData  p3(    l_x2, draw1.y );
	PointData  p4(    l_x1, draw1.y );
	PointData  p5(    l_x3, draw1.y );
	PointData  p6(draw2.x ,     l_y2);
	PointData  p7(draw2.x ,     l_y1);
	PointData  p8(draw2.x ,     l_y3);
	PointData  p9(    l_x3, draw2.y );
	PointData p10(    l_x1, draw2.y );
	PointData p11(    l_x2, draw2.y );
	PointData p12(draw1.x ,     l_y3);
	PointData p13(draw1.x ,     l_y1);
	PointData p_anchor(l_x1,    l_y1);
	VectorDataManager v;
		v.addInit(p_anchor);
		v.addFill(KageStage::fillColor.clone());
		v.addLineStyle(KageStage::stroke.clone());
		v.addMove(p1);
		v.addCubic( p2,  p3,  p4);
		v.addCubic( p5,  p6,  p7);
		v.addCubic( p8,  p9, p10);
		v.addCubic(p11, p12, p13);
		v.addClosePath();
		v.addEndFill();
	win->addDataToFrame(v);
	
	win->stackDo();
	
	render();
}

void KageStage::handleDrawRectMouseUp() {
	if (win->isLayerLocked() == true) {
		return;
	}
	if (draw1.x == draw2.x && draw1.y == draw2.y) { return; }
	
	PointData p1(draw1);
	PointData p2(draw2.x, draw1.y);
	PointData p3(draw2);
	PointData p4(draw1.x, draw2.y);
	PointData p5(draw1);
	PointData p_anchor((draw1.x+draw2.x)/2, (draw1.y+draw2.y)/2);
	VectorDataManager v;
		v.addInit(p_anchor);
		v.addFill(KageStage::fillColor.clone());
		v.addLineStyle(KageStage::stroke.clone());
		v.addMove(p1);
		v.addLine(p2);
		v.addLine(p3);
		v.addLine(p4);
		v.addLine(p5);
		v.addClosePath();
		v.addEndFill();
	win->addDataToFrame(v);
	
	win->stackDo();
	
	render();
}

void KageStage::handleStrokeMouseMove() {
	handleNodesMouseUp();
}

void KageStage::handleStrokeMouseUp() {
	handleNodesMouseUp();
	
	win->updateSelectedShapeColor(false, true);
	initNodeTool();
}

bool KageStage::cutSelectedShapes() {
	if (win->isLayerLocked() == true) {
		return false;
	}
	Kage::timestamp();
	std::cout << " KageStage::cutSelectedShapes " << selectedShapes.size() << std::endl;
	
	if (selectedShapes.size() == 0) {
		return false;
	}
	
	if (copySelectedShapes() == true) {
		return deleteSelectedShapes();
	}
	
	return false;
}

bool KageStage::copySelectedShapes() {
	Kage::timestamp();
	std::cout << " KageStage::copySelectedShapes " << selectedShapes.size() << std::endl;
	
	if (selectedShapes.size() == 0) {
		return false;
	}
	
	vector<unsigned int> l_selectedShapesOld(selectedShapes);
		sort(l_selectedShapesOld.begin(), l_selectedShapesOld.end());
	
	vector<VectorData> v = win->getFrameData().getVectorData();
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
	
	return true;
}

bool KageStage::selectAllShapes() {
	Kage::timestamp();
	std::cout << " KageStage::selectAllShapes " << selectedShapes.size() << std::endl;
	
	unsigned int vsize = win->getFrameData().getVectorData().size();
	if (vsize == 0) {
		return false;
	}
	
	selectedNodes.clear();
	for (unsigned int i = 0; i < vsize; ++i) {
		selectedNodes.push_back(i);
	}
	
	tryMultiSelectShapes_populateShapes();
	selectedNodes.clear();
	
	return true;
}

bool KageStage::deselectSelectedShapes() {
	if (win->isLayerLocked() == true) {
		return false;
	}
	Kage::timestamp();
	std::cout << " KageStage::deselectSelectedShapes " << selectedShapes.size() << std::endl;
	
	if (selectedShapes.size() == 0) {
		return false;
	}
	
	initNodeTool();
	win->propStageSetVisible(true);
	win->propFillStrokeSetVisible(false);
	win->propLocationSizeSetVisible(false);
	
	return true;
}
bool KageStage::cancelDrawingPoly() {
	if (win->isLayerLocked() == true) {
		return false;
	}
	_polyVectors.clear();
	drawCtr = 0;
	
	render();
	
	return true;
}
bool KageStage::deselectSelectedNodes() {
	Kage::timestamp();
	std::cout << " KageStage::deselectSelectedNodes " << selectedNodes.size() << std::endl;
	
	if (selectedNodes.size() == 0) {
		return false;
	}
	
	initNodeTool();
	win->propStageSetVisible(true);
	win->propNodeXYSetVisible(false);
	
	return true;
}
bool KageStage::pasteSelectedShapes() {
	if (win->isLayerLocked() == true) {
		return false;
	}
	unsigned int l_vectorDataCopyBuffer_size = _vectorDataCopyBuffer.size();
	Kage::timestamp();
	std::cout << " KageStage::pasteSelectedShapes " << l_vectorDataCopyBuffer_size << std::endl;
	
	if (l_vectorDataCopyBuffer_size == 0) {
		return false;
	}
	
	vector<VectorData> v = win->getFrameData().getVectorData();
	
	selectedNodes.clear();
	for (unsigned int i = 0; i < l_vectorDataCopyBuffer_size; ++i) {
		v.push_back(_vectorDataCopyBuffer[i]);
		//TODO: implement X/Y offset from _copyBufferMouse.x/y and draw2.x/y
		selectedNodes.push_back(v.size()-1);
	}
	
	win->setFrameData(v);
	
	win->stackDo();
	
	tryMultiSelectShapes_populateShapes();
	selectedNodes.clear();
	
	return true;
}

bool KageStage::deleteSelectedShapes() {
	if (win->isLayerLocked() == true) {
		return false;
	}
	unsigned int l_selectedShapes_size = selectedShapes.size();
	Kage::timestamp();
	std::cout << " KageStage::deleteSelectedShapes " << l_selectedShapes_size << std::endl;
	
	if (l_selectedShapes_size == 0) {
		return false;
	}
	
	vector<unsigned int> l_selectedShapesOld(selectedShapes);
		sort(l_selectedShapesOld.begin(), l_selectedShapesOld.end(), greater  <unsigned int>());
	
	vector<VectorData> v = win->getFrameData().getVectorData();
	
	unsigned int vsize = v.size();
	std::cout << " vectordata size " << vsize << std::endl;
	if (vsize < l_selectedShapes_size) { //attempt to fix https://sourceforge.net/p/kage/tickets/15/
		initNodeTool();
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
			cout << "predicting a crash!!!" << endl;
		}
		v.erase (v.begin() + l_selectedShapesOld[l_shapeIndex],
				 v.begin() + l_temp);
	}
	selectedShapes.clear();
	
	win->setFrameData(v);
	
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
	
	Kage::timestamp();
	std::cout << " KageStage::deleteSelectedNodes " << selectedNodes.size() << endl;
	
	sort(selectedNodes.begin(), selectedNodes.end(), greater <unsigned int>());
	
	for (unsigned int l_selectedNode = 0; l_selectedNode < selectedNodes.size(); ++l_selectedNode) {
	Kage::timestamp();
	std::cout << " KageStage::deleteSelectedNodes " << l_selectedNode << " " << selectedNodes.size() << endl;
		l_return = deleteSelectedNode(selectedNodes[l_selectedNode]);
	}
	
	selectedNodes.clear();
	
	if (l_return == true) {
		win->stackDo();
	}
	
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
		
		cout << " DELETE A " << endl;
//			selectedShapes.erase(selectedShapes.begin() + l_selectedShape);
	} else if (l_sequence == 7 && l_closepath == true) {
		v.erase (v.begin() + selectedShape,
				 v.begin() + selectedShape + l_sequence);
		
		cout << " DELETE B " << endl;
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
		cout << " DELETE C " << endl;
		
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

void KageStage::handleFill() {
//	handleEyedrop();
}

void KageStage::handleFillMouseUp() {
	handleNodesMouseUp();
	
	win->updateSelectedShapeColor(true, false);
	initNodeTool();
}

void KageStage::handleEyedrop() {
	Kage::timestamp();
	std::cout << " KageStage::handleEyedrop " << selectedShape << std::endl;
	
	if (selectedShape == _NO_SELECTION) {
		return;
	}
	
	vector<VectorData> v = win->getFrameData().getVectorData();
	bool l_move = false;
	
	anchor_upperLeft.x = 100000;
	anchor_upperLeft.y = 100000;
	
	anchor_lowerRight.x = -100000;
	anchor_lowerRight.y = -100000;
	
	unsigned int vsize = v.size();
	for (unsigned int i = selectedShape; i < vsize; ++i) {
		if (v[i].vectorType == VectorData::TYPE_LINE) {
			handleShapes_updateAnchors(
				v[i].points[0].x + origin.x,
				v[i].points[0].y + origin.y);
		} else if (v[i].vectorType == VectorData::TYPE_CURVE_QUADRATIC
				|| v[i].vectorType == VectorData::TYPE_CURVE_CUBIC) {
			handleShapes_updateAnchors(
				v[i].points[0].x + origin.x,
				v[i].points[0].y + origin.y);
			handleShapes_updateAnchors(
				v[i].points[1].x + origin.x,
				v[i].points[1].y + origin.y);
			handleShapes_updateAnchors(
				v[i].points[2].x + origin.x,
				v[i].points[2].y + origin.y);
		} else if (v[i].vectorType == VectorData::TYPE_INIT
				&& i != selectedShape) {
			i = vsize;
		}
	}
	anchor_center.x = (anchor_upperLeft.x + anchor_lowerRight.x) / 2;
	anchor_center.y = (anchor_upperLeft.y + anchor_lowerRight.y) / 2;
	
	vector<double> dashes;
		dashes.push_back(4.0); /* ink */
		dashes.push_back(4.0); /* skip */
	
	cr->set_dash(dashes, 0.0);
	//draw bounding rectangle for the shape
	cr->move_to(anchor_upperLeft.x, anchor_upperLeft.y);
		cr->line_to( anchor_lowerRight.x,  anchor_upperLeft.y);
		cr->line_to( anchor_lowerRight.x, anchor_lowerRight.y);
		cr->line_to(  anchor_upperLeft.x, anchor_lowerRight.y);
		cr->line_to(  anchor_upperLeft.x,  anchor_upperLeft.y);
			cr->set_source_rgba(0.0,0.0,0.0,1.0);
			cr->set_line_width(1.0);
				cr->set_line_cap(Cairo::LINE_CAP_ROUND);
					cr->stroke();
	dashes.clear();
	cr->set_dash(dashes, 0.0);

	if (l_move == true) {
		win->setFrameData(v);
		
		win->stackDo();
	}
}

void KageStage::handleEyedropMouseMove() {
//	Kage::timestamp();
//	std::cout << " KageStage::handleEyedropMouseMove ??? " << std::endl;
//	handleNodesMouseUp();
//	win->updateColors();
}

void KageStage::handleEyedropMouseUp() {
	handleNodesMouseUp();
	win->updateColors();
}

void KageStage::handleDrawPolyMouseUp() {
	if (win->isLayerLocked() == true) {
		return;
	}
	
	Kage::timestamp();
	std::cout << " KageStage::handleDrawPolyMouseUp" << std::endl;
	
	if (drawCtr > 0
			&& draw1.x-5 <= polyXhead && draw1.x+5 >= polyXhead
			&& draw1.y-5 <= polyYhead && draw1.y+5 >= polyYhead) {
		_polyVectors.addLinePoly(PointData(polyXhead,polyYhead), polyXtail, polyYtail);
		_polyVectors.addClosePath();
		_polyVectors.addEndFill();
		win->addDataToFrame(_polyVectors);
		drawCtr = 0;
		_polyVectors.clear();
		win->ToolSelect_onClick();
		win->stackDo();
	} else if (drawCtr > 0
			&& draw1.x-5 <= polyXtail && draw1.x+5 >= polyXtail
			&& draw1.y-5 <= polyYtail && draw1.y+5 >= polyYtail) {
		_polyVectors.addEndFill();
		win->addDataToFrame(_polyVectors);
		_polyVectors.clear();
		drawCtr = 0;
		win->ToolSelect_onClick();
		win->stackDo();
	} else {
		if (drawCtr > 0) {
			PointData p2(draw1);
				_polyVectors.addLinePoly(p2, polyXtail, polyYtail);
		} else {
			polyXhead = draw1.x;
			polyYhead = draw1.y;
			PointData p1(draw1);
				_polyVectors.addInit();
				_polyVectors.addFill(KageStage::fillColor.clone());
				_polyVectors.addLineStyle(KageStage::stroke);
				_polyVectors.addMove(p1);
		}
		polyXtail = draw1.x;
		polyYtail = draw1.y;
		++drawCtr;
	}
	
	render();
}

void KageStage::handlePoly() {
	if (win->isLayerLocked() == true) {
		return;
	}
	if (drawCtr == 0) return;

	cr->move_to(draw1.x, draw1.y);
	cr->line_to(draw2.x, draw2.y);
		cr->set_source_rgba((double)KageStage::fillColor.getR()/255, (double)KageStage::fillColor.getG()/255, (double)KageStage::fillColor.getB()/255, (double)KageStage::fillColor.getA()/255);
		cr->fill_preserve();
			cr->set_line_width(stroke.getThickness() / 2);
			cr->set_source_rgba((double)KageStage::stroke.getR()/255, (double)KageStage::stroke.getG()/255, (double)KageStage::stroke.getB()/255, (double)KageStage::stroke.getA()/255);
			cr->stroke();
	//draw guide
	cr->move_to(draw1.x, draw1.y);
	cr->line_to(draw2.x, draw2.y);
		cr->set_line_width(0.5);
		cr->set_source_rgba(1.0, 0.0, 0.0, 1.0);
		cr->stroke();

	cr->move_to(polyXhead-3 + origin.x, polyYhead-3 + origin.y);
		cr->line_to(polyXhead+3 + origin.x, polyYhead-3 + origin.y);
		cr->line_to(polyXhead+3 + origin.x, polyYhead+3 + origin.y);
		cr->line_to(polyXhead-3 + origin.x, polyYhead+3 + origin.y);
	cr->close_path();
		cr->set_source_rgb(1.0, 1.0, 1.0);
		cr->fill_preserve();
			cr->set_line_width(1.0);
			cr->set_source_rgb(0.0, 0.0, 0.0);
			cr->stroke();
}

void KageStage::handleRect() {
	if (win->isLayerLocked() == true) {
		return;
	}
	cr->move_to(draw1.x                      , draw1.y                      );
		cr->line_to(draw1.x + (draw2.x - draw1.x), draw1.y                      );
		cr->line_to(draw1.x + (draw2.x - draw1.x), draw1.y + (draw2.y - draw1.y));
		cr->line_to(draw1.x                      , draw1.y + (draw2.y - draw1.y));
		cr->close_path();
			cr->set_source_rgba((double)KageStage::fillColor.getR()/255, (double)KageStage::fillColor.getG()/255, (double)KageStage::fillColor.getB()/255, (double)KageStage::fillColor.getA()/255);
			cr->fill_preserve();
				cr->set_line_width(stroke.getThickness());
				cr->set_source_rgba((double)KageStage::stroke.getR()/255, (double)KageStage::stroke.getG()/255, (double)KageStage::stroke.getB()/255, (double)KageStage::stroke.getA()/255);
				cr->stroke();
}

void KageStage::handleOval() {
	if (win->isLayerLocked() == true) {
		return;
	}
	double l_x1, l_y1;
	double l_x2, l_y2;
	double l_x3, l_y3;
	l_x1 = ((draw1.x + draw2.x)/2);
	l_y1 = ((draw1.y + draw2.y)/2);
	l_x2 = ((l_x1 - draw1.x) * 0.4) + draw1.x;
	l_x3 = ((l_x1 - draw1.x) * 1.6) + draw1.x;
	l_y2 = ((l_y1 - draw1.y) * 0.4) + draw1.y;
	l_y3 = ((l_y1 - draw1.y) * 1.6) + draw1.y;
	
	cr->move_to(draw1.x , l_y1);
		cr->curve_to(draw1.x ,     l_y2,     l_x2, draw1.y ,     l_x1, draw1.y );
		cr->curve_to(    l_x3, draw1.y , draw2.x ,     l_y2, draw2.x ,     l_y1);
		cr->curve_to(draw2.x ,     l_y3,     l_x3, draw2.y ,     l_x1, draw2.y );
		cr->curve_to(    l_x2, draw2.y , draw1.x ,     l_y3, draw1.x ,     l_y1);
	cr->close_path();
		cr->set_source_rgba((double)KageStage::fillColor.getR()/255, (double)KageStage::fillColor.getG()/255, (double)KageStage::fillColor.getB()/255, (double)KageStage::fillColor.getA()/255);
		cr->fill_preserve();
			cr->set_line_width(stroke.getThickness());
			cr->set_source_rgba((double)KageStage::stroke.getR()/255, (double)KageStage::stroke.getG()/255, (double)KageStage::stroke.getB()/255, (double)KageStage::stroke.getA()/255);
			cr->stroke();
}

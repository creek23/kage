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

#include "stage.h"
#include "../../kage.h"
#include <cairomm/context.h>

ColorData KageStage::fillColor(239, 41, 41, 255);
StrokeColorData KageStage::stroke(164, 0, 0, 255);

ColorData KageStage::pencilFillColor(239, 41, 41, 0);
StrokeColorData KageStage::pencilStroke(64, 64, 64, 64);

KageStage::ToolMode KageStage::toolMode = MODE_NONE;
GdkPoint KageStage::moveStageXY;

Glib::RefPtr<Gdk::Pixbuf> KageStage::imageSHAPE_000;
Glib::RefPtr<Gdk::Pixbuf> KageStage::imageSHAPE_045;
Glib::RefPtr<Gdk::Pixbuf> KageStage::imageSHAPE_090;
Glib::RefPtr<Gdk::Pixbuf> KageStage::imageSHAPE_135;
Glib::RefPtr<Gdk::Pixbuf> KageStage::imageSHAPE_MOVE;
Glib::RefPtr<Gdk::Pixbuf> KageStage::imageSHAPE_NE;
Glib::RefPtr<Gdk::Pixbuf> KageStage::imageSHAPE_NW;
Glib::RefPtr<Gdk::Pixbuf> KageStage::imageSHAPE_SW;
Glib::RefPtr<Gdk::Pixbuf> KageStage::imageSHAPE_SE;
Glib::RefPtr<Gdk::Pixbuf> KageStage::imageSHAPE_ROTATE;

KageStage::KageStage(Kage *p_kage) {
	_kage = p_kage;
	set_can_focus(true); //to accept key_press
	add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
	add_events(Gdk::ENTER_NOTIFY_MASK | Gdk::LEAVE_NOTIFY_MASK);
	add_events(Gdk::KEY_PRESS_MASK | Gdk::KEY_RELEASE_MASK);
	add_events(Gdk::FOCUS_CHANGE_MASK);
	add_events(Gdk::POINTER_MOTION_MASK);
	add_events(Gdk::SCROLL_MASK);
	
	unpressKeys();
	
	_invalidated = true;
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
		_kage->switchToNextFrame();
	} else if (e->keyval == GDK_KEY_comma) {
		_kage->switchToPreviousFrame();
	} else {
		if (KageStage::toolMode == MODE_SELECT
				|| KageStage::toolMode == MODE_NODE
				|| KageStage::toolMode == MODE_ZOOM
				|| KageStage::toolMode == MODE_DRAW_POLY
				|| KageStage::toolMode == MODE_DRAW_PENCIL) { //why restrict keydown handling to these tools?
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
				Kage::timestamp_IN();
					std::cout << "KageStage::on_key_press_event " << e->keyval << "_" << e->string << std::endl;
				Kage::timestamp_OUT();
				return true;
			}
			
			double l_diffX = 0;
			double l_diffY = 0;
			if (keyUpDown == true) {
				l_diffY = -1/KageStage::currentScale;
			} else if (keyDownDown == true) {
				l_diffY =  1/KageStage::currentScale;
			}
			if (keyLeftDown == true) {
				l_diffX = -1/KageStage::currentScale;
			} else if (keyRightDown == true) {
				l_diffX =  1/KageStage::currentScale;
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
				origin.x -= (l_diffX * 5);
				origin.y -= (l_diffY * 5);
			} else if (KageStage::toolMode == MODE_SELECT) {
				handleShapes_moveShape(l_diffX, l_diffY);
				propX += l_diffX;
				propY += l_diffY;
				
				updateShapeXY();
				
				_stackDo = true;
				grab_focus();
			} else if (KageStage::toolMode == MODE_NODE) {
				nodeX += l_diffX;
				nodeY += l_diffY;
				updateNodeX(nodeX, false);
				updateNodeY(nodeY, false);
				updateNodeXY();
				_stackDo = true;
			} else {
				return true;
			}
			
			invalidateToRender();
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
	} else if ((e->state & (GDK_SHIFT_MASK | GDK_CONTROL_MASK)) == GDK_CONTROL_MASK) {
		cout << "\n\n\nKEY UP KEY UP KEY UP KEY UP KEY UP\n\n\n";
		bool operationSuccess = false;
			if (       e->keyval == GDK_KEY_X || e->keyval == GDK_KEY_x) {
				operationSuccess = cutSelectedShapes();
			} else if (e->keyval == GDK_KEY_C || e->keyval == GDK_KEY_c) {
				operationSuccess = copySelectedShapes();
			} else if (e->keyval == GDK_KEY_V || e->keyval == GDK_KEY_v) {
				operationSuccess = pasteSelectedShapes();
			} else if (e->keyval == GDK_KEY_A || e->keyval == GDK_KEY_a) {
				if (toolMode != KageStage::MODE_SELECT) {
					_kage->ToolSelect_onClick();
				}
				operationSuccess = selectAllShapes();
			/*} else if (e->keyval == GDK_KEY_Z || e->keyval == GDK_KEY_z) {
				_kage->Undo_onClick();
				return true;
			} else if (e->keyval == GDK_KEY_Y || e->keyval == GDK_KEY_y) {
				_kage->Redo_onClick();
				return true;*/
			}
		if (operationSuccess == true) {
			_kage->forceRenderFrames();
			return true;
		}
	} else if (e->keyval == GDK_KEY_Delete) {
		if (toolMode == KageStage::MODE_SELECT) {
			if (deleteSelectedShapes() == true) {
				_kage->stackDo();
				_kage->forceRenderFrames();
				return true;
			}
		} else if (KageStage::toolMode == KageStage::MODE_NODE) {
			if (deleteSelectedNodes() == true) {
				_kage->forceRenderFrames();
				return true;
			}
		}
	}
	
	if (e->keyval == GDK_KEY_Control_L || e->keyval == GDK_KEY_Control_R) {
		keyControlDown = false;
	}
	
	if (e->keyval == GDK_KEY_Escape) {
		bool operationSuccess = false;
			if (_kage->_isPlaying == true) {
				_kage->Stop_onClick();
			} else if (toolMode == KageStage::MODE_SELECT) {
				operationSuccess = deselectSelectedShapes();
			} else if (toolMode == KageStage::MODE_NODE) {
				operationSuccess = deselectSelectedNodes();
			} else if (toolMode == KageStage::MODE_DRAW_POLY) {
				operationSuccess = cancelDrawingPoly();
			}
		if (operationSuccess == true) {
			_kage->forceRenderFrames();
			return true;
		}
	}

	if (_stackDo == true) {
		_kage->stackDo();
		_stackDo = false;
	}
	
	return true;
}

bool KageStage::on_expose_event(GdkEventExpose* e) {
	invalidateToRender();
	
	return true;
}

void KageStage::unpressKeys() {
	//dirty hack on fixing KEY event-handling problem with Kage's AccelKeys and KageStage
	keyControlDown = false;
	keyShiftDown = false;
	keyUpDown = false;
	keyDownDown = false;
	keyLeftDown = false;
	keyRightDown = false;

	grab_focus();
}
#include <iomanip>
bool KageStage::on_event(GdkEvent *e) {
	if (e->type == GDK_ENTER_NOTIFY) {
		Kage::timestamp_IN(); cout << " mouse enter" << endl;
		//invalidateToRender(); //do we need to render when mouse enters Stage or wait for mouse movement instead?
		Kage::timestamp_OUT();
		//mouse hover in
	} else if (e->type == GDK_LEAVE_NOTIFY) {
		Kage::timestamp_IN(); cout << " mouse leave" << endl;
		//invalidateToRender(); // do we need to render when mouse leaves Stage?!?
		Kage::timestamp_OUT();
		//mouse hover out
	} else if (e->type == GDK_KEY_PRESS) {
		return on_key_press_event((GdkEventKey*) e);
	} else if (e->type == GDK_KEY_RELEASE) {
		return on_key_release_event((GdkEventKey*) e);
	} else if (e->type == GDK_DOUBLE_BUTTON_PRESS) {
		if (selectedShapes.size() > 0) {
			if (KageStage::toolMode == MODE_SELECT) {
				KageStage::toolMode = MODE_NODE;
				_kage->toolsButtonToggle("Node Tool");
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
		} else if (KageStage::toolMode == MODE_DRAW_PENCIL) {
			handleDrawPencilMouseDown(PointData(draw1));
		} else if (KageStage::toolMode == MODE_SELECT) {
			draw2.x = e->button.x;
			draw2.y = e->button.y;
		} else if (KageStage::toolMode == MODE_NODE) {
			draw2.x = e->button.x; //added because of multi/single-selection of shape/s
			draw2.y = e->button.y;
			//invalidateToRender(); //why again?
		} else if (KageStage::toolMode == MODE_DRAW_POLY) {
			//
		//} else if (KageStage::toolMode == MODE_DRAW_POLY) {
			//
		} else {
		//how to determine if user just imported an Asset what will be the value for draw2
		//	draw2.x = e->button.x;
		//	draw2.y = e->button.y;
		}
	} else if (e->type == GDK_BUTTON_RELEASE) { //mouse up
		mouseOnAnchor = AnchorData::TYPE_NONE;
		mouseDown = false;
		grab_focus();
		
		draw1.x = (draw1.x - origin.x);
		draw1.y = (draw1.y - origin.y);
			draw2.x = (e->button.x - origin.x);
			draw2.y = (e->button.y - origin.y);
		
		if (KageStage::toolMode == MODE_SELECT) {
			if (_isModifyingShape == false) {
				if (       draw1.x >= draw2.x-(5/KageStage::currentScale / _zoomValue)
						&& draw1.x <= draw2.x+(5/KageStage::currentScale / _zoomValue)
						&& draw1.y >= draw2.y-(5/KageStage::currentScale / _zoomValue)
						&& draw1.y <= draw2.y+(5/KageStage::currentScale / _zoomValue)) {
					draw1.x = e->button.x;
					draw1.y = e->button.y;
						trySingleSelectShape();
				} else {
					selectionBox1.x = draw1.x / KageStage::currentScale  / _zoomValue;
					selectionBox1.y = draw1.y / KageStage::currentScale / _zoomValue;
						selectionBox2.x = draw2.x / KageStage::currentScale  / _zoomValue;
						selectionBox2.y = draw2.y / KageStage::currentScale / _zoomValue;
					
						tryMultiSelectShapes();
				}
			} else {
				if (_rotateApply == true) {
					draw1.x = draw1.x / KageStage::currentScale / _zoomValue;
					draw1.y = draw1.y / KageStage::currentScale / _zoomValue;
						draw2.x = draw2.x / KageStage::currentScale / _zoomValue;
						draw2.y = draw2.y / KageStage::currentScale / _zoomValue;
					
					handleShapes_modifyingShapeRotate();
				}
				
				_kage->stackDo();
			}
			updateShapeXY();
			_isModifyingShape = false;
			invalidateToRender();
		} else if (KageStage::toolMode == MODE_NODE) {
			if (_isModifyingShape == false) {
				if (       draw1.x >= draw2.x-(5/KageStage::currentScale)
						&& draw1.x <= draw2.x+(5/KageStage::currentScale)
						&& draw1.y >= draw2.y-(5/KageStage::currentScale)
						&& draw1.y <= draw2.y+(5/KageStage::currentScale)) {
					cout << "logic error via Zoom++" << endl;
					draw1.x = e->button.x;
					draw1.y = e->button.y;
						trySingleSelectShape();
				} else {
					draw1.x = draw1.x / KageStage::currentScale / _zoomValue;
					draw1.y = draw1.y / KageStage::currentScale / _zoomValue;
						draw2.x = draw2.x / KageStage::currentScale / _zoomValue;
						draw2.y = draw2.y / KageStage::currentScale / _zoomValue;
					
					selectionBox1 = draw1;
					selectionBox2 = draw2;
						tryMultiSelectShapes();

					draw1.x = draw1.x * KageStage::currentScale * _zoomValue;
					draw1.y = draw1.y * KageStage::currentScale * _zoomValue;
						draw2.x = e->button.x * KageStage::currentScale * _zoomValue;
						draw2.y = e->button.y * KageStage::currentScale * _zoomValue;
				}
				_kage->propFillStrokeSetVisible(false);
				_kage->propDisplayObjectPropertiesSetVisible(false);
				_kage->propNodeXYSetVisible(true);
			} else {
				_kage->stackDo();
				
				//reset selected Node's control-point
				mouseOnNodeIndex = _NO_SELECTION;
				mouseOnNode = _NO_SELECTION;
				
				updateNodeXY();
			}
			_isModifyingShape = false;
			invalidateToRender();
		} else if (KageStage::toolMode == MODE_STROKE) {
			cout << "logic error via Zoom++" << endl;
			draw1.x = e->button.x;
			draw1.y = e->button.y;
				handleStrokeMouseUp();
		} else if (KageStage::toolMode == MODE_FILL) {
			cout << "logic error via Zoom++" << endl;
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
			
			//applyZoom();
			origin = applyZoomRatio(origin); //zoom++
			invalidateToRender();
			_kage->displayMouseXY(
					(int) ((e->button.x - origin.x) / _zoomValue),
					(int) ((e->button.y - origin.y) / _zoomValue)
				);
		} else if (KageStage::toolMode == MODE_EYEDROP) {
			cout << "logic error via Zoom++" << endl;
			draw1.x = e->button.x;
			draw1.y = e->button.y;
				handleEyedropMouseUp();
		} else if (KageStage::toolMode == MODE_DRAW_OVAL) {
		
			draw1.x = draw1.x / KageStage::currentScale / _zoomValue;
			draw1.y = draw1.y / KageStage::currentScale / _zoomValue;
				draw2.x = draw2.x / KageStage::currentScale / _zoomValue;
				draw2.y = draw2.y / KageStage::currentScale / _zoomValue;

			handleDrawOvalMouseUp();
		
			draw1.x = draw1.x * KageStage::currentScale * _zoomValue;
			draw1.y = draw1.y * KageStage::currentScale * _zoomValue;
				draw2.x = e->button.x * KageStage::currentScale * _zoomValue;
				draw2.y = e->button.y * KageStage::currentScale * _zoomValue;
		
		} else if (KageStage::toolMode == MODE_DRAW_RECT) {
			draw1.x = draw1.x / KageStage::currentScale / _zoomValue;
			draw1.y = draw1.y / KageStage::currentScale / _zoomValue;
				draw2.x = draw2.x / KageStage::currentScale / _zoomValue;
				draw2.y = draw2.y / KageStage::currentScale / _zoomValue;
			
			handleDrawRectMouseUp();

			draw1.x = draw1.x * KageStage::currentScale * _zoomValue;
			draw1.y = draw1.y * KageStage::currentScale * _zoomValue;
				draw2.x = e->button.x * KageStage::currentScale * _zoomValue;
				draw2.y = e->button.y * KageStage::currentScale * _zoomValue;
		
		} else if (KageStage::toolMode == MODE_DRAW_PENCIL) {
			handleDrawPencilMouseUp();
		} else if (KageStage::toolMode == MODE_DRAW_POLY) {
			if (keyControlDown == true) {
				draw1.x = drawConstraint.x - origin.x;
				draw1.y = drawConstraint.y - origin.y;
			}

			draw1.x = draw1.x / KageStage::currentScale / _zoomValue;
			draw1.y = draw1.y / KageStage::currentScale / _zoomValue;
				draw2.x = draw2.x / KageStage::currentScale / _zoomValue;
				draw2.y = draw2.y / KageStage::currentScale / _zoomValue;
			
			handleDrawPolyMouseUp();
				//TODO: why origin.x/.y is not taken into consideration?
				// try moving stage if this is still properly working
				draw1.x = (e->button.x);
				draw1.y = (e->button.y);
					draw2.x = (e->button.x);
					draw2.y = (e->button.y);

			if (keyControlDown == true) {
				draw1.x = drawConstraint.x;
				draw1.y = drawConstraint.y;
			}
		}
	} else if (e->type == GDK_MOTION_NOTIFY) { //mouse move
		_mouseLocation.x = e->button.x;
		_mouseLocation.y = e->button.y;
		
		if (KageStage::toolMode == MODE_NODE) {
			draw2.x = e->button.x;
			draw2.y = e->button.y;
			
			_zoomReference.x = e->button.x; //zoom++;
			_zoomReference.y = e->button.y; //zoom++;
			
			invalidateToRender();
		} else if (KageStage::toolMode == MODE_SELECT) {
			draw2.x = e->button.x;
			draw2.y = e->button.y;
			
			_zoomReference.x = e->button.x; //zoom++;
			_zoomReference.y = e->button.y; //zoom++;
			
			if (mouseDown == true) {
				invalidateToRender();
			}
		} else if (KageStage::toolMode == MODE_DRAW_OVAL
				|| KageStage::toolMode == MODE_DRAW_RECT) {
			if (mouseDown == true) {
				draw2.x = (e->button.x);
				draw2.y = (e->button.y);
				
				invalidateToRender();
			}
		} else if (KageStage::toolMode == MODE_DRAW_PENCIL) {
			draw2.x = e->button.x;
			draw2.y = e->button.y;
			handleDrawPencilMouseMove(PointData(e->button.x, e->button.y));
		} else if (KageStage::toolMode == MODE_DRAW_POLY) {
			if (drawCtr > 0) {
				draw2.x = (e->button.x);
				draw2.y = (e->button.y);
				
				if (keyControlDown == true) {
					//restrain to 15% increments from draw1 location
					PointData l_rotate;
						l_rotate.x = draw2.x - draw1.x;
						l_rotate.y = draw2.y - draw1.y;
					double l_angle = atan2(l_rotate.y, l_rotate.x) * 180 / 3.14159265;
					
					int l_angleConstraint = (int) (l_angle - ((int)l_angle % 15));
					if (l_angle > 172 || l_angle < -172) {
						l_angleConstraint = 180;
					}
					
					double l_hyp = sqrtf((l_rotate.x * l_rotate.x) + (l_rotate.y * l_rotate.y));
						draw2.x = cos((double)l_angleConstraint * 3.14159265 / 180.0) * l_hyp + draw1.x;
						draw2.y = sin((double)l_angleConstraint * 3.14159265 / 180.0) * l_hyp + draw1.y;
						
						drawConstraint.x = draw2.x;
						drawConstraint.y = draw2.y;
					cout << "\t drawConstraint.x " << drawConstraint.x << endl;
				}
				
				invalidateToRender();
			}
		} else if (KageStage::toolMode == MODE_STROKE) {
			handleStrokeMouseMove();
			//invalidateToRender(); //handleStrokeMouseMove will call invalidateToRender()
		} else if (KageStage::toolMode == MODE_EYEDROP) {
			draw1.x = (e->button.x);
			draw1.y = (e->button.y);
			handleEyedropMouseMove();
			
			if (mouseDown == true) {
				invalidateToRender(); //eyedrop will only pick color of selected item, do we need to invalidate display?
			}
		} else if (KageStage::toolMode == MODE_MOVE_STAGE) {
			double distx = e->button.x - KageStage::moveStageXY.x;
			double disty = e->button.y - KageStage::moveStageXY.y;
			
			origin.x += distx;
			origin.y += disty;
			
			invalidateToRender();
		}
		_kage->displayMouseXY(
				(int) ((e->button.x - origin.x) / _zoomValue),
				(int) ((e->button.y - origin.y) / _zoomValue)
			);
		//_kage->forceRenderFrames(); //why there's a need to FORCE render?
		_kage->renderFrames(); //trying to work around ^
		KageStage::moveStageXY.x = e->button.x;
		KageStage::moveStageXY.y = e->button.y;
	} else if (e->type == GDK_EXPOSE) {
		on_expose_event((GdkEventExpose*) e);
	} else if (e->type == GDK_FOCUS_CHANGE) {
		//filter out from echos
		/*cout << "GDK_FOCUS_CHANGE e->send_event " << ((GdkEventFocus*)e)->send_event << " e->in " << ((GdkEventFocus*)e)->in << endl;
		if (((GdkEventFocus*)e)->in) {
			_gotFocus = true;
		} else {
			_gotFocus = false;
		}*/
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
	} else if (e->type == GDK_SCROLL) {
		GdkEventScroll* foobar = (GdkEventScroll*)(e);
		cout << " " << foobar->x << " " << foobar->y << " " << foobar->direction << endl;
		if (foobar->direction == GDK_SCROLL_UP) {
			_zoomRatio = 1.1;
			_zoomValue = _zoomValue * 1.1f;
		} else {
			_zoomRatio = 0.909090909f;
			_zoomValue = _zoomValue / 1.1f;
		}
		//e->button.button = 0;
		_zoomReference.x = e->button.x;
		_zoomReference.y = e->button.y;
		
		origin = applyZoomRatio(origin); //zoom++
		invalidateToRender();
		_kage->displayMouseXY(
				(int) ((e->button.x - origin.x) / _zoomValue),
				(int) ((e->button.y - origin.y) / _zoomValue)
			);
	} else {
		cout << "unknown event: " << e->type << endl;
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

vector<VectorData> KageStage::applyZoom(vector<VectorData> v) {
	return v;
}
/**
 * zoom++ : TODO: to become obsolete; new zoom implementation makes this useless
 */
void KageStage::applyZoom() {
		
	PointData __stageArea(_kage->_document.Project._width  + origin.x, _kage->_document.Project._height  + origin.y);
	__stageArea = applyZoomRatio(__stageArea);
	
	__origin = applyZoomRatio(origin);
	
		__stageArea.x -= __origin.x;
		__stageArea.y -= __origin.y;
	
		_kage->stackDoZoom(origin, __origin, _zoomReference, _zoomRatio);
	
	unsigned int l_layerCount = _kage->_layerManager.layerCount();
	unsigned int l_frameCount = _kage->_document.frameCount();
	unsigned int l_currentFrame = _kage->_document.Scenes[_kage->_document.getActiveSceneID()].getCurrentFrame();
	unsigned int l_currentLayer = _kage->getCurrentLayer();
	
	for (unsigned int l_frame = 1; l_frame <= l_frameCount; ++l_frame) {
		_kage->_document.setCurrentFrame(l_frame);
		
		for (unsigned int l_layer = 1; l_layer <= l_layerCount; ++l_layer) {
			_kage->_document.Scenes[_kage->_document.getActiveSceneID()].setCurrentLayer(l_layer);
			
			if (_kage->getTween() == 0) {
				vector<VectorData> v = _kage->getFrameData(true).getVectorData();
				
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
				
				_kage->setFrameData(v, true);
			}
		}
	}
	
	//TODO: is it needed to access _framesetManager? why not just _kage->setCurrentLayer(l_currentLayer)?
	_kage->_document.Scenes[_kage->_document.getActiveSceneID()].setCurrentLayer(l_currentLayer);
	_kage->_document.setCurrentFrame(l_currentFrame);
	
	origin = __origin.clone();
	_kage->_document.Project._width  = __stageArea.x;
	_kage->_document.Project._height = __stageArea.y;
}

void KageStage::setFill(Gdk::Color p_Color) {
	if (KageStage::toolMode == ToolMode::MODE_DRAW_PENCIL) {
		KageStage::pencilFillColor.setR(p_Color.get_red() / 255);
		KageStage::pencilFillColor.setG(p_Color.get_green() / 255);
		KageStage::pencilFillColor.setB(p_Color.get_blue() / 255);
	} else {
		KageStage::fillColor.setR(p_Color.get_red() / 255);
		KageStage::fillColor.setG(p_Color.get_green() / 255);
		KageStage::fillColor.setB(p_Color.get_blue() / 255);
	}
	invalidateToRender();
}
Gdk::Color KageStage::getFill() {
	Gdk::Color tColor;
	if (KageStage::toolMode == ToolMode::MODE_DRAW_PENCIL) {
		tColor.set_red((gushort)KageStage::pencilFillColor.getR() * 255);
		tColor.set_green((gushort)KageStage::pencilFillColor.getG() * 255);
		tColor.set_blue((gushort)KageStage::pencilFillColor.getB() * 255);
	} else {
		tColor.set_red((gushort)KageStage::fillColor.getR() * 255);
		tColor.set_green((gushort)KageStage::fillColor.getG() * 255);
		tColor.set_blue((gushort)KageStage::fillColor.getB() * 255);
	}
	return tColor;
}

void KageStage::setStroke(Gdk::Color p_c) {
	if (KageStage::toolMode == ToolMode::MODE_DRAW_PENCIL) {
		KageStage::pencilStroke.setR(p_c.get_red() / 255);
		KageStage::pencilStroke.setG(p_c.get_green() / 255);
		KageStage::pencilStroke.setB(p_c.get_blue() / 255);
	} else {
		KageStage::stroke.setR(p_c.get_red() / 255);
		KageStage::stroke.setG(p_c.get_green() / 255);
		KageStage::stroke.setB(p_c.get_blue() / 255);
	}
	invalidateToRender();
}
Gdk::Color KageStage::getStroke() {
	Gdk::Color l_c;
	if (KageStage::toolMode == ToolMode::MODE_DRAW_PENCIL) {
		l_c.set_red((gushort)KageStage::pencilStroke.getR() * 255);
		l_c.set_green((gushort)KageStage::pencilStroke.getG() * 255);
		l_c.set_blue((gushort)KageStage::pencilStroke.getB() * 255);
	} else {
		l_c.set_red((gushort)KageStage::stroke.getR() * 255);
		l_c.set_green((gushort)KageStage::stroke.getG() * 255);
		l_c.set_blue((gushort)KageStage::stroke.getB() * 255);
	}
	return l_c;
}

void KageStage::printVectors() {
	Kage::timestamp_IN();
	std::cout << " KageStage::printVectors <" << std::endl;
	vector<VectorData> v = _kage->getFrameData().getVectorData();
	
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
				//p1 IMAGE_ID_BUFF       x/y == ID / imageBuff
				//p2 IMAGE_X_Y           x/y == x / y
				//p3 IMAGE_WIDTH_HEIGHT  x/y == width / height
				//p4 IMAGE_SCALEX_SCALEY x/y == scaleX / scaleY
				//p5 IMAGE_ROTATE_ALPHA  x/y == rotate / alpha
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
	Kage::timestamp_OUT();
}

void KageStage::cleanSlate() {
	origin.x = 50;
	origin.y = 50;
	_kage->_document.Project._width = 800.0f;
	_kage->_document.Project._height = 600.0f;
	
	currentScale = _kage->_document.Project._width;
	
	_kage->_document.Project._fps = 12;
	mouseDown = false;
	stroke.setThickness(3.0);
	drawCtr = 0;
	mouseOnNode = _NO_SELECTION;
	mouseOnNodeOffset.x = 0.0f;
	mouseOnNodeOffset.y = 0.0f;
	mouseOnNodeHover = _NO_SELECTION;
	mouseOnNodeIndex = _NO_SELECTION;
	polyXhead = 0;
	polyYhead = 0;
	polyXtail = 0;
	polyYtail = 0;
	
	drawConstraint.x  = 0;
	drawConstraint.y  = 0;
	
	unpressKeys();
	
	_isModifyingShape = false;
	
	_zoomValue = 1.0f;
	_zoomRatio = 1.0f;
	__stageArea.x = _kage->_document.Project._width;
	__stageArea.y = _kage->_document.Project._height;

	cairoPNG.clear();
	
	initNodeTool();
	
	mouseOnAnchor = AnchorData::TYPE_NONE;
}

void KageStage::invalidateToRender() {
//	Kage::timestamp_IN(); cout << " KageStage::invalidateToRender()" << endl;
	if (!window) {
		window = get_window();
	}
	
	if (window) {
		// force our program to redraw the entire stage
		Gdk::Rectangle r(0, 0, get_allocation().get_width(),
				get_allocation().get_height());
		window->invalidate_rect(r, false);
	}
//	Kage::timestamp_OUT();
}

bool KageStage::on_draw(const Cairo::RefPtr<Cairo::Context>& p_cr) {
	Kage::timestamp_IN(); cout << " KageStage::on_draw" << endl;
	if (!window) {
		window = get_window();
	}
	if (window) {
		cr = p_cr;
		if (mouseDown) {
			_kage->renderFramesBelowCurrentLayer();
			if (_rotateMode == false) {
				renderFrame(cr);
			}
		} else {
			_kage->renderFrames();
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
			origin.x = (_registerWidth - _kage->_document.Project._width) / 2;

			_kage->registerPropertiesPane();
		}
		if (_registerHeight != get_height()) {
			_registerHeight = get_height();
			origin.y = (_registerHeight - _kage->_document.Project._height) / 2;
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
		} else if (KageStage::toolMode == MODE_DRAW_PENCIL) {
			handlePencil();
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
			_kage->renderFramesAboveCurrentLayer();
		}
	}
	
	Kage::timestamp_OUT();
	
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
		p_context->set_source_rgb(0.38f, 0.38f, 0.38f);
		p_context->fill();
		
		//draw viewable area
		p_context->move_to(                                                 origin.x, origin.y                                                );
		p_context->line_to((_kage->_document.Project._width * _zoomValue) + origin.x, origin.y                                                );
		p_context->line_to((_kage->_document.Project._width * _zoomValue) + origin.x, origin.y + (_kage->_document.Project._height * _zoomValue));
		p_context->line_to(                                                 origin.x, origin.y + (_kage->_document.Project._height * _zoomValue));
		
		p_context->close_path();
			p_context->set_source_rgb((double)_kage->_document.Project._backgroundColor.getR()/255, (double)_kage->_document.Project._backgroundColor.getG()/255, (double)_kage->_document.Project._backgroundColor.getB()/255);
	//		p_context->fill_preserve();
			p_context->fill();
				p_context->set_line_width(0.2f);
				p_context->set_source_rgb(0.0, 0.0, 0.0);
				p_context->stroke();
		
		_mouseLocationShapeIndex = _NO_SELECTION;
	}
}

bool KageStage::renderToPNG(string p_path, bool p_transparent) {
	#ifdef CAIRO_HAS_PNG_FUNCTIONS
		Cairo::RefPtr<Cairo::ImageSurface> surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, _kage->_document.Project._width, _kage->_document.Project._height);
		
		Cairo::RefPtr<Cairo::Context> l_context = Cairo::Context::create(surface);
		
		GdkPoint l_tempOrigin;
		l_tempOrigin.x = origin.x;
		l_tempOrigin.y = origin.y;
		
		origin.x = 0;
		origin.y = 0;
		double l_zoomValue = _zoomValue;
		_zoomValue = 1.0f;

		if (p_transparent == false) {
			clearScreen(l_context);
		}
		
		unsigned int l_layerCount = _kage->_layerManager.layerCount();
		unsigned int l_currentLayer = _kage->getCurrentLayer();
			for (unsigned int i = 1; i <= l_layerCount; ++i) {
				_kage->_document.Scenes[_kage->_document.getActiveSceneID()].setCurrentLayer(i);
				renderFrameToPNG(l_context);
			}
		_kage->setCurrentLayer(l_currentLayer);
		
		CairoKage::writeToPNG(p_path, surface);
		
		origin.x = l_tempOrigin.x;
		origin.y = l_tempOrigin.y;
		_zoomValue = l_zoomValue;
		
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
	Kage::timestamp_IN();
	cout << " KageStage::renderFrame p_force " << p_force << endl;
	vector<VectorData> v = _kage->getFrameData(p_force).getVectorData();
	renderFrame(p_context, v);
	if (_polyVectors.isEmpty() == false) {
		Kage::timestamp_IN();
		cout << " KageStage::renderFrame _polyVectors " << _polyVectors.getVectorData().size() << endl;
		renderFrame(p_context, _polyVectors.getVectorData());
		Kage::timestamp_OUT();
	}
	Kage::timestamp_OUT();
}


bool KageStage::renderToPNGOffset(string p_path, bool p_transparent, double p_offsetX, double p_offsetY) {
	#ifdef CAIRO_HAS_PNG_FUNCTIONS
		Cairo::RefPtr<Cairo::ImageSurface> surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, _kage->_document.Project._width, _kage->_document.Project._height);
		
		Cairo::RefPtr<Cairo::Context> l_context = Cairo::Context::create(surface);
		
		GdkPoint l_tempOrigin;
		l_tempOrigin.x = origin.x;
		l_tempOrigin.y = origin.y;
		
		origin.x = 0;
		origin.y = 0;
		double l_zoomValue = _zoomValue;
		_zoomValue = 1.0f;
		
		if (p_transparent == false) {
			clearScreen(l_context);
		}
		
		unsigned int l_layerCount = _kage->_layerManager.layerCount();
		unsigned int l_currentLayer = _kage->getCurrentLayer();
			for (unsigned int i = 1; i <= l_layerCount; ++i) {
				_kage->_document.Scenes[_kage->_document.getActiveSceneID()].setCurrentLayer(i);
				renderFrameToPNGOffset(l_context, p_offsetX, p_offsetY);
			}
		_kage->setCurrentLayer(l_currentLayer);
		
		CairoKage::writeToPNG(p_path, surface);
		
		origin.x = l_tempOrigin.x;
		origin.y = l_tempOrigin.y;
		_zoomValue = l_zoomValue;
		
		return true;
	#else
		std::cout << "You must compile cairo with PNG support for this example to work." << std::endl;
		return false;
	#endif
}
void KageStage::renderFrameToPNGOffset(Cairo::RefPtr<Cairo::Context> p_context, double p_offsetX, double p_offsetY) {
	renderFrameOffset(p_context, true, p_offsetX, p_offsetY);
}

void KageStage::renderFrameOffset(Cairo::RefPtr<Cairo::Context> p_context, bool p_force, double p_offsetX, double p_offsetY) {
	Kage::timestamp_IN();
	cout << " KageStage::renderFrame p_force " << p_force << endl;
	vector<VectorData> v = _kage->getFrameData(p_force).getVectorData();
	renderFrameOffset(p_context, v, 1.0, p_offsetX, p_offsetY);
	if (_polyVectors.isEmpty() == false) {
		Kage::timestamp_IN();
		cout << " KageStage::renderFrame _polyVectors " << _polyVectors.getVectorData().size() << endl;
		renderFrameOffset(p_context, _polyVectors.getVectorData(), 1.0, p_offsetX, p_offsetY);
		Kage::timestamp_OUT();
	}
	Kage::timestamp_OUT();
}

/**
 * For use by Kage, when Onion Skin is toggled
 */
void KageStage::renderOnionFrame(Cairo::RefPtr<Cairo::Context> p_context, vector<VectorData> p_vectorData, double p_alpha) {
	Kage::timestamp_IN();
	cout << " KageStage::renderOnionFrame v " << p_vectorData.size() << endl;
	renderFrame(p_context, p_vectorData, p_alpha);
	Kage::timestamp_OUT();
}

Cairo::RefPtr<Cairo::ImageSurface> KageStage::cairoImageSurface;
cairo_surface_t *image;
float g_rotate = 0;
void KageStage::renderFrame(Cairo::RefPtr<Cairo::Context> p_context, vector<VectorData> p_vectorData, double p_alpha) {
	Kage::timestamp_IN();
	cout << " KageStage::renderFrame v " << p_vectorData.size() << " " << endl;
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
	bool l_mouseLocationOnShape = false;
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
				l_mouseLocationOnShape = p_context->in_fill(_mouseLocation.x, _mouseLocation.y);
				if (l_mouseLocationOnShape != 0) {
					_mouseLocationShapeIndex = l_mouseLocationShapeIndex;
					_kage->_displayObjectIsShape = true;
				} else if (_mouseLocationShapeIndex == _NO_SELECTION) {
					l_mouseLocationOnShape = p_context->in_stroke(_mouseLocation.x, _mouseLocation.y);
					if (l_mouseLocationOnShape != 0) {
						_mouseLocationShapeIndex = l_mouseLocationShapeIndex;
						_kage->_displayObjectIsShape = true;
					}
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
				break;
			case VectorData::TYPE_STROKE:
				scolor = p_vectorData[i].stroke;
				break;
			case VectorData::TYPE_MOVE:
				p_context->move_to((p_vectorData[i].points[0].x*KageStage::currentScale * _zoomValue) + origin.x, (p_vectorData[i].points[0].y*KageStage::currentScale * _zoomValue) + origin.y);
				
				p.x = p_vectorData[i].points[0].x*KageStage::currentScale * _zoomValue;
				p.y = p_vectorData[i].points[0].y*KageStage::currentScale * _zoomValue;
				break;
			case VectorData::TYPE_LINE:
				p_context->line_to(p_vectorData[i].points[0].x + origin.x, p_vectorData[i].points[0].y + origin.y);
				
				p.x = p_vectorData[i].points[0].x*KageStage::currentScale;
				p.y = p_vectorData[i].points[0].y*KageStage::currentScale;
				break;
			case VectorData::TYPE_CURVE_QUADRATIC:
				//cubic-to-quad algo borrowed from Mono/Moonlight's moon_quad_curve_to
				x1 = p_vectorData[i].points[0].x*KageStage::currentScale * _zoomValue;
				y1 = p_vectorData[i].points[0].y*KageStage::currentScale * _zoomValue;
				x2 = p_vectorData[i].points[1].x*KageStage::currentScale * _zoomValue;
				y2 = p_vectorData[i].points[1].y*KageStage::currentScale * _zoomValue;
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
						(p_vectorData[i].points[0].x*KageStage::currentScale*_zoomValue) + origin.x, (p_vectorData[i].points[0].y*KageStage::currentScale*_zoomValue) + origin.y,
						(p_vectorData[i].points[1].x*KageStage::currentScale*_zoomValue) + origin.x, (p_vectorData[i].points[1].y*KageStage::currentScale*_zoomValue) + origin.y,
						(p_vectorData[i].points[2].x*KageStage::currentScale*_zoomValue) + origin.x, (p_vectorData[i].points[2].y*KageStage::currentScale*_zoomValue) + origin.y
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
				//p1 IMAGE_ID_BUFF       x/y == ID / imageBuff
				//p2 IMAGE_X_Y           x/y == x / y
				//p3 IMAGE_WIDTH_HEIGHT  x/y == width / height
				//p4 IMAGE_SCALEX_SCALEY x/y == scaleX / scaleY
				//p5 IMAGE_ROTATE_ALPHA  x/y == rotate / alpha
				{
					try {
						p_context->save();
						if (!cairoPNG[p_vectorData[i].points[IMAGE_ID_BUFF].y]) {
							cairoPNG[p_vectorData[i].points[IMAGE_ID_BUFF].y] = Cairo::ImageSurface::create_from_png(_kage->_assetManager.getImagePathByID(p_vectorData[i].points[IMAGE_ID_BUFF].x));
							cout << "image loaded" << endl;
						}
						cairoImageSurface = cairoPNG[p_vectorData[i].points[0].y];
						
						float xImagePos   = p_vectorData[i].points[IMAGE_X_Y].x;
						float yImagePos   = p_vectorData[i].points[IMAGE_X_Y].y;
						float xPos = xImagePos * KageStage::currentScale * _zoomValue;
						float yPos = yImagePos * KageStage::currentScale * _zoomValue;
						float imageWidth  = p_vectorData[i].points[IMAGE_WIDTH_HEIGHT].x;
						float imageHeight = p_vectorData[i].points[IMAGE_WIDTH_HEIGHT].y;
						float xScale      = p_vectorData[i].points[IMAGE_SCALEX_SCALEY].x;
						float yScale      = p_vectorData[i].points[IMAGE_SCALEX_SCALEY].y;
						float Rotate      = p_vectorData[i].points[IMAGE_ROTATE_ALPHA].x;
						float alphaValue  = p_vectorData[i].points[IMAGE_ROTATE_ALPHA].y * p_alpha;
						float rotationPointX = p_vectorData[l_mouseLocationShapeIndex].points[IMAGE_ID_BUFF].x * KageStage::currentScale * _zoomValue;
						float rotationPointY = p_vectorData[l_mouseLocationShapeIndex].points[IMAGE_ID_BUFF].y * KageStage::currentScale * _zoomValue;
						bool l_bMirrored = false;
						bool l_bFlipped  = false;
						bool l_bTransparent = (alphaValue != 1.0f);
						
						if (l_bMirrored == true) {
							xPos += imageWidth;
						}
						if (l_bFlipped == true) {
							yPos += imageHeight;
						}
						
						float translateX = rotationPointX + origin.x;
						float translateY = rotationPointY + origin.y;
						if (imageWidth >= 0) {
							p_context->translate(translateX, translateY);
						} else {
							p_context->translate(xPos*_zoomValue + origin.x, yPos*_zoomValue + origin.y);
	//		cout << "image B translated" << endl;
						}
						
						p_context->rotate(Rotate);

						if (imageWidth >= 0) {
							p_context->translate(-translateX, -translateY);
						} else {
							p_context->translate(-(xPos*_zoomValue - origin.x), -(yPos*_zoomValue - origin.y));
	//		cout << "image B re translated" << endl;
						}
						
						if (xScale != 0 && yScale != 0) {
							p_context->scale(xScale * _zoomValue, yScale * _zoomValue);
						}
						
						p_context->set_source(cairoImageSurface,
												(origin.x / _zoomValue) + (xImagePos * KageStage::currentScale),
												(origin.y / _zoomValue) + (yImagePos * KageStage::currentScale));
						
						if (       _mouseLocation.x <= xPos + ( imageWidth * KageStage::currentScale * _zoomValue) + origin.x
								&& _mouseLocation.x >= xPos                                                        + origin.x
								&& _mouseLocation.y <= yPos + (imageHeight * KageStage::currentScale * _zoomValue) + origin.y
								&& _mouseLocation.y >= yPos                                                        + origin.y) {
							_mouseLocationShapeIndex = l_mouseLocationShapeIndex;
							propAlpha = alphaValue;
							_kage->_displayObjectIsShape = false;
						}
						
						if (l_bTransparent == false) {
							p_context->paint();
						} else {
							p_context->paint_with_alpha(alphaValue);
						}
						
						p_context->restore();
					} catch (std::bad_alloc const &ex) {
						cout << "handling zoom error: bad_alloc" << endl;
					} catch (std::exception ex) {
						//unhandled exception (type ) in signal handler:
						//what: std::bad_alloc
					}
				}
				break;
			case VectorData::TYPE_INIT:
				l_mouseLocationShapeIndex = i;
				break;
			default:
				break;
		}
	}
	
	Kage::timestamp_OUT();
}

/**
 * @brief This will return an Cairo::ImageSurface array-index to be used as ID for rendering on Stage or Library
 * 
 * @param p_ID reference ID from Assets
 * @return unsigned int
 */
unsigned int KageStage::addImage(unsigned int p_ID) {
	if (_kage->isLayerLocked() == true) {
		return UINT_MAX;
	}
	
	double l_x1, l_y1;
	draw1.x = (draw1.x - origin.x);
	draw1.y = (draw1.y - origin.y);
	draw1.x = draw1.x / KageStage::currentScale / _zoomValue;
	draw1.y = draw1.y / KageStage::currentScale / _zoomValue;

		Cairo::RefPtr<Cairo::ImageSurface> l_tmp;
		
		l_tmp = Cairo::ImageSurface::create_from_png(_kage->_assetManager.getImagePathByID(p_ID));
		
		cairoPNG.push_back(l_tmp);
		
		double imageWidth  = (double)l_tmp->get_width() /KageStage::currentScale / _zoomValue;
		double imageHeight = (double)l_tmp->get_height()/KageStage::currentScale / _zoomValue;
		
		l_x1 = draw1.x + (imageWidth /2);
		l_y1 = draw1.y + (imageHeight/2);
		

		PointData  p1(      p_ID, cairoPNG.size()-1);
		PointData  p2(   draw1.x,           draw1.y);
		PointData  p3(imageWidth,       imageHeight);
		PointData  p4(       1.0,               1.0); //for scale X/Y; not implemented yet
		PointData  p5(       0.0,               1.0); //for rotate and alpha (alpha not implemented yet) 
		PointData p_anchor( l_x1,              l_y1);
		VectorDataManager v;
			v.addInit(p_anchor);
			v.addImage(p1, p2, p3, p4, p5);
		_kage->addDataToFrame(v);
	
	draw1.x = draw1.x * KageStage::currentScale * _zoomValue;
	draw1.y = draw1.y * KageStage::currentScale * _zoomValue;
	draw1.x = (draw1.x + origin.x);
	draw1.y = (draw1.y + origin.y);
	
	_kage->stackDo();
	
	invalidateToRender();

	return cairoPNG.size()-1;
}

void KageStage::renderFrameOffset(Cairo::RefPtr<Cairo::Context> p_context, vector<VectorData> p_vectorData, double p_alpha, double p_offsetX, double p_offsetY) {
	Kage::timestamp_IN();
	cout << " KageStage::renderFrame v " << p_vectorData.size() << " " << endl;
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
	bool l_mouseLocationOnShape = false;
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
				l_mouseLocationOnShape = p_context->in_fill(_mouseLocation.x, _mouseLocation.y);
				if (l_mouseLocationOnShape != 0) {
					_mouseLocationShapeIndex = l_mouseLocationShapeIndex;
				} else if (_mouseLocationShapeIndex == _NO_SELECTION) {
					l_mouseLocationOnShape = p_context->in_stroke(_mouseLocation.x, _mouseLocation.y);
					if (l_mouseLocationOnShape != 0) {
						_mouseLocationShapeIndex = l_mouseLocationShapeIndex;
					}
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
				break;
			case VectorData::TYPE_STROKE:
				scolor = p_vectorData[i].stroke;
				break;
			case VectorData::TYPE_MOVE:
				p_context->move_to(p_offsetX + (p_vectorData[i].points[0].x*KageStage::currentScale * _zoomValue) + origin.x, p_offsetY + (p_vectorData[i].points[0].y*KageStage::currentScale * _zoomValue) + origin.y);
				
				p.x = p_vectorData[i].points[0].x*KageStage::currentScale * _zoomValue;
				p.y = p_vectorData[i].points[0].y*KageStage::currentScale * _zoomValue;
				break;
			case VectorData::TYPE_LINE:
				p_context->line_to(p_offsetX + (p_vectorData[i].points[0].x*KageStage::currentScale * _zoomValue) + origin.x, p_offsetY + (p_vectorData[i].points[0].y*KageStage::currentScale * _zoomValue) + origin.y);
				
				p.x = p_vectorData[i].points[0].x*KageStage::currentScale;
				p.y = p_vectorData[i].points[0].y*KageStage::currentScale;
				break;
			case VectorData::TYPE_CURVE_QUADRATIC:
				//cubic-to-quad algo borrowed from Mono/Moonlight's moon_quad_curve_to
				x1 = p_vectorData[i].points[0].x*KageStage::currentScale * _zoomValue;
				y1 = p_vectorData[i].points[0].y*KageStage::currentScale * _zoomValue;
				x2 = p_vectorData[i].points[1].x*KageStage::currentScale * _zoomValue;
				y2 = p_vectorData[i].points[1].y*KageStage::currentScale * _zoomValue;
				x3 = x2;
				y3 = y2;
				
				x2 = x1 + (x2 - x1) / 3;
				y2 = y1 + (y2 - y1) / 3;
				x1 = p.x + 2 * (x1 - p.x) / 3;
				y1 = p.y + 2 * (y1 - p.y) / 3;
				
				p_context->curve_to(
						p_offsetX + x1 + origin.x, p_offsetY + y1 + origin.y,
						p_offsetX + x2 + origin.x, p_offsetY + y2 + origin.y,
						p_offsetX + x3 + origin.x, p_offsetY + y3 + origin.y
					);
				
				p.x = x3;
				p.y = y3;
				break;
			case VectorData::TYPE_CURVE_CUBIC:
				p_context->curve_to(
						p_offsetX + (p_vectorData[i].points[0].x*KageStage::currentScale*_zoomValue) + origin.x, p_offsetY + (p_vectorData[i].points[0].y*KageStage::currentScale*_zoomValue) + origin.y,
						p_offsetX + (p_vectorData[i].points[1].x*KageStage::currentScale*_zoomValue) + origin.x, p_offsetY + (p_vectorData[i].points[1].y*KageStage::currentScale*_zoomValue) + origin.y,
						p_offsetX + (p_vectorData[i].points[2].x*KageStage::currentScale*_zoomValue) + origin.x, p_offsetY + (p_vectorData[i].points[2].y*KageStage::currentScale*_zoomValue) + origin.y
				);
				break;
			case VectorData::TYPE_TEXT:
					p_context->select_font_face ("Verdana", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD);
					p_context->set_font_size(32);
					p_context->set_source_rgb(0, 0, 1);
					p_context->move_to(p_offsetX + 10, p_offsetY + 50);
					p_context->show_text("Hello, world");
				break;
			case VectorData::TYPE_IMAGE:
				//p1 IMAGE_ID_BUFF       x/y == ID / imageBuff
				//p2 IMAGE_X_Y           x/y == x / y
				//p3 IMAGE_WIDTH_HEIGHT  x/y == width / height
				//p4 IMAGE_SCALEX_SCALEY x/y == scaleX / scaleY
				//p5 IMAGE_ROTATE_ALPHA  x/y == rotate / alpha
				{
					try {
						p_context->save();
						if (!cairoPNG[p_vectorData[i].points[IMAGE_ID_BUFF].y]) {
							cairoPNG[p_vectorData[i].points[IMAGE_ID_BUFF].y] = Cairo::ImageSurface::create_from_png(_kage->_assetManager.getImagePathByID(p_vectorData[i].points[IMAGE_ID_BUFF].x));
							cout << "image loaded" << endl;
						}
						cairoImageSurface = cairoPNG[p_vectorData[i].points[0].y];
						
						float xImagePos   = p_vectorData[i].points[IMAGE_X_Y].x;
						float yImagePos   = p_vectorData[i].points[IMAGE_X_Y].y;
						float xPos = xImagePos * KageStage::currentScale * _zoomValue;
						float yPos = yImagePos * KageStage::currentScale * _zoomValue;
						float imageWidth  = p_vectorData[i].points[IMAGE_WIDTH_HEIGHT].x;
						float imageHeight = p_vectorData[i].points[IMAGE_WIDTH_HEIGHT].y;
						float xScale      = p_vectorData[i].points[IMAGE_SCALEX_SCALEY].x;
						float yScale      = p_vectorData[i].points[IMAGE_SCALEX_SCALEY].y;
						float Rotate      = p_vectorData[i].points[IMAGE_ROTATE_ALPHA].x;
						float alphaValue  = p_vectorData[i].points[IMAGE_ROTATE_ALPHA].y;
						float rotationPointX = p_vectorData[l_mouseLocationShapeIndex].points[IMAGE_ID_BUFF].x * KageStage::currentScale * _zoomValue;
						float rotationPointY = p_vectorData[l_mouseLocationShapeIndex].points[IMAGE_ID_BUFF].y * KageStage::currentScale * _zoomValue;
						bool l_bMirrored = false;
						bool l_bFlipped  = false;
						bool l_bTransparent = (alphaValue != 1.0f);
						
						if (l_bMirrored == true) {
							xPos += imageWidth;
						}
						if (l_bFlipped == true) {
							yPos += imageHeight;
						}
						
						float translateX = rotationPointX + origin.x;
						float translateY = rotationPointY + origin.y;
						if (imageWidth >= 0) {
							p_context->translate(translateX, translateY);
						} else {
							p_context->translate(xPos*_zoomValue + origin.x, yPos*_zoomValue + origin.y);
	//		cout << "image B translated" << endl;
						}
						
						p_context->rotate(Rotate);

						if (imageWidth >= 0) {
							p_context->translate(-translateX, -translateY);
						} else {
							p_context->translate(-(xPos*_zoomValue - origin.x), -(yPos*_zoomValue - origin.y));
	//		cout << "image B re translated" << endl;
						}
						
						if (xScale != 0 && yScale != 0) {
							p_context->scale(xScale * _zoomValue, yScale * _zoomValue);
						}
						
						p_context->set_source(cairoImageSurface,
												(origin.x / _zoomValue) + (xImagePos * KageStage::currentScale),
												(origin.y / _zoomValue) + (yImagePos * KageStage::currentScale));
						
						if (       _mouseLocation.x <= xPos + ( imageWidth * KageStage::currentScale * _zoomValue) + origin.x
								&& _mouseLocation.x >= xPos                                                        + origin.x
								&& _mouseLocation.y <= yPos + (imageHeight * KageStage::currentScale * _zoomValue) + origin.y
								&& _mouseLocation.y >= yPos                                                        + origin.y) {
							_mouseLocationShapeIndex = l_mouseLocationShapeIndex;
							_kage->_displayObjectIsShape = false;
						}
						
						if (l_bTransparent == false) {
							p_context->paint();
						} else {
							p_context->paint_with_alpha(alphaValue);
						}
						
						p_context->restore();
					} catch (std::bad_alloc const &ex) {
						cout << "handling zoom error: bad_alloc" << endl;
					} catch (std::exception ex) {
						//unhandled exception (type ) in signal handler:
						//what: std::bad_alloc
					}
				}
				break;
			case VectorData::TYPE_INIT:
				l_mouseLocationShapeIndex = i;
				break;
			default:
				break;
		}
	}
	
	Kage::timestamp_OUT();
}

void KageStage::drawSelectionArea() {
	vector<double> dashes;
		dashes.push_back(4.0); /* ink */
		dashes.push_back(4.0); /* skip */
	
	cr->set_dash(dashes, 0.0);
	//draw bounding rectangle for the shape
	cr->move_to(draw1.x , draw1.y);
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

void KageStage::handleStrokeMouseMove() {
	handleNodesMouseUp();
}

void KageStage::handleStrokeMouseUp() {
	handleNodesMouseUp();
	
	_kage->updateSelectedShapeColor(false, true);
	initNodeTool();
}

void KageStage::handleFill() {
//	handleEyedrop();
}

void KageStage::handleFillMouseUp() {
	handleNodesMouseUp();
	
	_kage->updateSelectedShapeColor(true, false);
	initNodeTool();
}

void KageStage::handleEyedrop() {
	if (selectedShape == _NO_SELECTION) {
		return;
	}
	Kage::timestamp_IN();
	std::cout << " KageStage::handleEyedrop " << selectedShape << std::endl;
	
	vector<VectorData> v = _kage->getFrameData().getVectorData();
	bool l_move = false;
	
	anchor_upperLeft.x = DBL_MAX;
	anchor_upperLeft.y = DBL_MAX;
	
	anchor_lowerRight.x = DBL_MIN;
	anchor_lowerRight.y = DBL_MIN;
	
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
		_kage->setFrameData(v);
		
		_kage->stackDo();
	}
	
	Kage::timestamp_OUT();
}

void KageStage::handleEyedropMouseMove() {
//	Kage::timestamp_IN();
//	std::cout << " KageStage::handleEyedropMouseMove ??? " << std::endl;
//	handleNodesMouseUp();
//	_kage->updateColors();
//	Kage::timestamp_OUT();
}

void KageStage::handleEyedropMouseUp() {
	handleNodesMouseUp();
	_kage->updateColors();
}


/**
 * @brief updates selected Image's Alpha
 * 
 * @param l_alpha
 */
void KageStage::updateImageAlpha(float l_alpha) {
	if (_kage->_displayObjectIsShape == true) {
		//TODO: should also apply Alpha to the whole selected shape
	} else {
		unsigned int l_selectedShapes_size = selectedShapes.size();
		for (unsigned int l_shapeIndex = 0; l_shapeIndex < l_selectedShapes_size; ++l_shapeIndex) {
			vector<VectorData> v = _kage->getFrameData().getVectorData();
			unsigned int vsize = v.size();
			unsigned int l_vStartIndex = selectedShapes[l_shapeIndex];
			for (unsigned int i = l_vStartIndex; i < vsize; ++i) {
				if (v[i].vectorType == VectorData::TYPE_IMAGE) {
					v[i].points[IMAGE_ROTATE_ALPHA].y = l_alpha;
					_kage->setFrameData(v);

					_kage->stackDo();
					
					invalidateToRender();
					
					return;
				}
			}
			
		}
	}
}
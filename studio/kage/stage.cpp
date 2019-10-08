
#include "stage.h"
#include "../kage.h"
#include <cairomm/context.h>

KageStage::KageStage(Kage *p_win) {
	origin.x = 50;
	origin.y = 50;
	wid = 640;
	hgt = 480;
	fps = 12;
	win = p_win;
//	set_flags(Gtk::CAN_FOCUS);
	set_can_focus(true); //to accept key_press
	add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
	add_events(Gdk::ENTER_NOTIFY_MASK | Gdk::LEAVE_NOTIFY_MASK);
	add_events(Gdk::FOCUS_CHANGE_MASK);
	add_events(Gdk::POINTER_MOTION_MASK);
	gotContext = false;
	mouseDown = false;
	stageWidth = 800;
	stageHeight = 600;
	stroke.setThickness(3.0);
	drawCtr = 0;
	mouseOnNode = -1;
	mouseOnNodeIndex = 0;
	polyX = 0;
	polyY = 0;
	polyXprev = 0;
	polyYprev = 0;
	
	mouseOnAnchor = AnchorData::TYPE_NONE;
}

KageStage::~KageStage() {
	//
}

bool KageStage::on_key_press_event(GdkEventKey *e) {
	if (e->keyval == 32) { // space [hand tool]
		if (KageStage::toolMode != MODE_MOVE_STAGE) {
			prevTool = KageStage::toolMode;
			KageStage::toolMode = MODE_MOVE_STAGE;
//			Gdk::Cursor handcur(Gdk::HAND1);
//			get_window()->set_cursor(handcur);
		}
	} else if (e->keyval == 65307) { //ESCape key
		if (KageStage::toolMode != MODE_DRAW_POLY) {
			//
		} else {
			drawCtr = 0;
			VectorDataManager v;
				v.addLinePoly(PointData(polyX,polyY), polyXprev, polyYprev);
				v.addEndFill();
			win->addDataToFrame(v);
			render();
		}
	} else if (e->keyval == 46) { //. dot
		win->switchToNextFrame();
	} else if (e->keyval == 44) { //, comma
		win->switchToPreviousFrame();
	} else {
		if (KageStage::toolMode == MODE_SELECT
				|| KageStage::toolMode == MODE_NODE) {
			if (e->keyval == 65362) { // UP-ARROW
				keyUpDown = true;
			} else if (e->keyval == 65364) { // DOWN-ARROW
				keyDownDown =  true;
			} else if (e->keyval == 65361) { // LEFT-ARROW
				keyLeftDown =  true;
			} else if (e->keyval == 65363) { // RIGHT-ARROW
				keyRightDown =  true;
			} else if (e->keyval == 65505 || e->keyval == 65506) { //[SHIFT]
				keyShiftDown = true;
			} else if (e->keyval == 65507 || e->keyval == 65508) { //[CONTROL]
				keyControlDown = true;
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
	if (e->keyval == 32) { // space [hand tool]
		KageStage::toolMode = prevTool;
		Kage::timestamp();
		std::cout << " KageStage::on_key_release_event No Hand" << std::endl;
	} else if (e->keyval == 65362) { // UP-ARROW
		keyUpDown = false;
	} else if (e->keyval == 65364) { // DOWN-ARROW
		keyDownDown =  false;
	} else if (e->keyval == 65361) { // LEFT-ARROW
		keyLeftDown =  false;
	} else if (e->keyval == 65363) { // RIGHT-ARROW
		keyRightDown =  false;
	} else if (e->keyval == 65505 || e->keyval == 65506) { //[SHIFT]
		keyShiftDown = false;
	} else if (e->keyval == 65507 || e->keyval == 65508) { //[CONTROL]
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
	} else if (e->type == GDK_LEAVE_NOTIFY) {
		render();
	} else if (e->type == GDK_KEY_PRESS) {
		on_key_press_event((GdkEventKey*) e);
	} else if (e->type == GDK_KEY_RELEASE) {
		on_key_release_event((GdkEventKey*) e);
	} else if (e->type == GDK_BUTTON_PRESS) { ///mouse down
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
		} else if (KageStage::toolMode == MODE_DRAW_POLY) {
			//
		} else {
			//
		}
	} else if (e->type == GDK_BUTTON_RELEASE) { ///mouse up
		mouseDown = false;
		grab_focus();
		
		draw1.x = (draw1.x - origin.x);
		draw1.y = (draw1.y - origin.y);
			draw2.x = (draw2.x - origin.x);
			draw2.y = (draw2.y - origin.y);
		
		if (KageStage::toolMode == MODE_SELECT) {
			draw1.x = e->button.x;
			draw1.y = e->button.y;
				handleShapesMouseUp();
		} else if (KageStage::toolMode == MODE_NODE) {
			//DEBUG INFO: does not need `draw2' data
			draw1.x = e->button.x;
			draw1.y = e->button.y;
				handleNodesMouseUp();
		} else if (KageStage::toolMode == MODE_STROKE) {
			draw1.x = e->button.x;
			draw1.y = e->button.y;
				handleStrokeMouseUp();
		} else if (KageStage::toolMode == MODE_FILL) {
			draw1.x = e->button.x;
			draw1.y = e->button.y;
				handleFillMouseUp();
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
				draw1.x = (e->button.x);
				draw1.y = (e->button.y);
					draw2.x = (e->button.x);
					draw2.y = (e->button.y);
		} else {
			//
		}
	} else if (e->type == GDK_MOTION_NOTIFY) { ///mouse move
		if (KageStage::toolMode == MODE_NODE) {
			draw1.x = (e->button.x);
			draw1.y = (e->button.y);
			
			render();
		} else if (KageStage::toolMode == MODE_SELECT) {
			draw2.x = (e->button.x);
			draw2.y = (e->button.y);
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
		} else if (KageStage::toolMode == MODE_MOVE_STAGE) {
			double distx = e->button.x - KageStage::moveStageXY.x;
			double disty = e->button.y - KageStage::moveStageXY.y;
			
			origin.x += distx;
			origin.y += disty;
			
			render();
		}
		KageStage::moveStageXY.x = e->button.x;
		KageStage::moveStageXY.y = e->button.y;
	} else if (e->type == GDK_EXPOSE) {
		on_expose_event((GdkEventExpose*) e);
	} else if (e->type == GDK_FOCUS_CHANGE) {
		//filter out from echos
	} else if (e->type == GDK_CONFIGURE) {
		//filter out from echos 
	} else {
		//
	}
	
	return true;
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
			renderFrame();
		} else {
			win->renderFrames();
		}
		
		if (!KageStage::imageSHAPE_000) {
			KageStage::imageSHAPE_000 = Gdk::Pixbuf::create_from_file("shared/icons/shape_000.png");
		}
		if (!KageStage::imageSHAPE_045) {
			KageStage::imageSHAPE_045 = Gdk::Pixbuf::create_from_file("shared/icons/shape_045.png");
		}
		if (!KageStage::imageSHAPE_090) {
			KageStage::imageSHAPE_090 = Gdk::Pixbuf::create_from_file("shared/icons/shape_090.png");
		}
		if (!KageStage::imageSHAPE_135) {
			KageStage::imageSHAPE_135 = Gdk::Pixbuf::create_from_file("shared/icons/shape_135.png");
		}
		//draw user-drawn object
		if (KageStage::toolMode == MODE_SELECT) {
			handleShapes();
		} else if (KageStage::toolMode == MODE_NODE) {
			handleNodes();
		} else if (KageStage::toolMode == MODE_DRAW_POLY) {
			handlePoly();
		} else if (KageStage::toolMode == MODE_DRAW_RECT && mouseDown == true) {
			handleRect();
		} else if (KageStage::toolMode == MODE_DRAW_OVAL && mouseDown == true) {
			handleOval();
		} else if (KageStage::toolMode == MODE_EYEDROP) {
			handleEyedrop();
		}
		if (mouseDown) {
			win->renderFramesAboveCurrentLayer();
		}
	}
	return true;
}

void KageStage::clearScreen() {
	if (!cr) return;
	if (window) {
		Gtk::Allocation allocation = get_allocation();
		const int width = allocation.get_width();
		const int height = allocation.get_height();
		
		//draw stage area
			cr->move_to(0, 0);
			cr->line_to(width, 0);
			cr->line_to(width, height);
			cr->line_to(0, height);
			cr->close_path();
		cr->set_source_rgb(0.8, 0.8, 0.8);
		cr->fill();
		
		//draw viewable area
		cr->move_to(      origin.x, origin.y);
		cr->line_to(wid + origin.x, origin.y);
		cr->line_to(wid + origin.x, origin.y + hgt);
		cr->line_to(      origin.x, origin.y + hgt);
		cr->close_path();
			cr->set_source_rgb((double)KageStage::stageBG.getR()/255, (double)KageStage::stageBG.getG()/255, (double)KageStage::stageBG.getB()/255);
	//		cr->fill_preserve();
			cr->fill();
				cr->set_line_width(0.2);
				cr->set_source_rgb(0.0, 0.0, 0.0);
				cr->stroke();
	}
}

void KageStage::renderFrame() {
	vector<VectorData> v = win->getFrameData().getVectorData();
	
	unsigned int vsize = v.size();
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
			case VectorData::TYPE_FILL:
				fcolor = v[i].fillColor;
				fillCtr++;
				break;
			case VectorData::TYPE_ENDFILL:
				if (fillCtr > 0) {
					cr->close_path();
					
					cr->set_source_rgba((double)fcolor.getR()/255,
										(double)fcolor.getG()/255,
										(double)fcolor.getB()/255,
										(double)fcolor.getA()/255);
					cr->fill_preserve();
					fillCtr--;
				}
//				if (scolor.getThickness() > 0) {
					cr->set_source_rgba((double)scolor.getR()/255,
										(double)scolor.getG()/255,
										(double)scolor.getB()/255,
										(double)scolor.getA()/255);
					cr->set_line_width(scolor.getThickness());
						cr->set_line_cap(Cairo::LINE_CAP_ROUND);
							cr->stroke();
//				}
				break;
			case VectorData::TYPE_STROKE:
				scolor = v[i].stroke;
				break;
			case VectorData::TYPE_MOVE:
				cr->move_to(v[i].points[0].x + origin.x, v[i].points[0].y + origin.y);
				
				p.x = v[i].points[0].x;
				p.y = v[i].points[0].y;
				break;
			case VectorData::TYPE_LINE:
				cr->line_to(v[i].points[0].x + origin.x, v[i].points[0].y + origin.y);
				
				p.x = v[i].points[0].x;
				p.y = v[i].points[0].y;
				break;
			case VectorData::TYPE_CURVE_QUADRATIC:
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
				
				cr->curve_to(
						x1 + origin.x, y1 + origin.y,
						x2 + origin.x, y2 + origin.y,
						x3 + origin.x, y3 + origin.y
					);
				
				p.x = x3;
				p.y = y3;
				break;
			case VectorData::TYPE_CURVE_CUBIC:
				cr->curve_to(
						v[i].points[0].x + origin.x, v[i].points[0].y + origin.y,
						v[i].points[1].x + origin.x, v[i].points[1].y + origin.y,
						v[i].points[2].x + origin.x, v[i].points[2].y + origin.y
				);
				break;
			case VectorData::TYPE_TEXT:
					cr->select_font_face ("Verdana", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD);
					cr->set_font_size(32);
					cr->set_source_rgb(0, 0, 1);
					cr->move_to(10, 50);
					cr->show_text("Hello, world");
					//(surface)cr->write_to_png("hello.png");
				break;
			case VectorData::TYPE_IMAGE:
				//2 '1st is for X/Y, 2nd is for width/height  -- ?!?
				break;
			case VectorData::TYPE_INIT:
				break;
			default:
				break;
		}
	}
}

void KageStage::initNodeTool() {
	selectedNode = -1;
	selectedShape = -1;
}

void KageStage::handleShapes_scaleNorth() {
	double diffOld = anchor_lowerRight.y - anchor_upperLeft.y;
	double diffNew = anchor_lowerRight.y - draw2.y;
	double diffRatio = diffNew / diffOld;
	
	vector<VectorData> v = win->getFrameData().getVectorData();
	unsigned int typeMovesIndex = -1;
	unsigned int vsize = v.size();
	for (unsigned int i = selectedShape; i < vsize; ++i) {
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
			break;
		}
	}
	
	win->setFrameData(v);
}
void KageStage::handleShapes_scaleEast() {
	double diffOld = anchor_upperLeft.x - anchor_lowerRight.x;
	double diffNew = anchor_upperLeft.x - draw2.x;
	double diffRatio = diffNew / diffOld;
	
	vector<VectorData> v = win->getFrameData().getVectorData();
	unsigned int typeMovesIndex = -1;
	unsigned int vsize = v.size();
	for (unsigned int i = selectedShape; i < vsize; ++i) {
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
			break;
		}
	}
	
	win->setFrameData(v);
}
void KageStage::handleShapes_scaleWest() {
	double diffOld = anchor_lowerRight.x - anchor_upperLeft.x;
	double diffNew = anchor_lowerRight.x - draw2.x;
	double diffRatio = diffNew / diffOld;
	
	vector<VectorData> v = win->getFrameData().getVectorData();
	unsigned int typeMovesIndex = -1;
	unsigned int vsize = v.size();
	for (unsigned int i = selectedShape; i < vsize; ++i) {
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
			break;
		}
	}
	
	win->setFrameData(v);
}
void KageStage::handleShapes_scaleSouth() {
	double diffOld = anchor_upperLeft.y - anchor_lowerRight.y;
	double diffNew = anchor_upperLeft.y - draw2.y;
	double diffRatio = diffNew / diffOld;
	
	vector<VectorData> v = win->getFrameData().getVectorData();
	unsigned int typeMovesIndex = -1;
	unsigned int vsize = v.size();
	for (unsigned int i = selectedShape; i < vsize; ++i) {
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
			break;
		}
	}
	
	win->setFrameData(v);
}
void KageStage::handleShapes_moveShape(double p_diffX, double p_diffY) {
	if (p_diffX == 0 && p_diffY == 0) {
		return;
	}
	
	vector<VectorData> v = win->getFrameData().getVectorData();
	unsigned int typeMovesIndex = -1;
	unsigned int vsize = v.size();
	for (unsigned int i = selectedShape; i < vsize; ++i) {
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
			v[typeMovesIndex].points[0].x += p_diffX;
			v[typeMovesIndex].points[0].y += p_diffY;
			break;
		}
	}
	
	win->setFrameData(v);
}
void KageStage::handleShapes_updateAnchors(double p_x, double p_y) {
	if (p_x <= anchor_upperLeft.x) {
		anchor_upperLeft.x = p_x;
	} else if (p_x >= anchor_lowerRight.x) {
		anchor_lowerRight.x = p_x;
	}
	if (p_y <= anchor_upperLeft.y) {
		anchor_upperLeft.y = p_y;
	} else if (p_y >= anchor_lowerRight.y) {
		anchor_lowerRight.y = p_y;
	}
}
void KageStage::updateShapeColor(bool p_doFill, bool p_doStroke) {
	unsigned int l_temp = -1;
	
	if (selectedShape == l_temp) {
		return;
	}
	
	vector<VectorData> v = win->getFrameData().getVectorData();
	unsigned int vsize = v.size();
	for (unsigned int i = selectedShape; i < vsize; ++i) {
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
	
	win->setFrameData(v);
	
	render();
}
void KageStage::handleShapes() {
	unsigned int l_temp = -1;
	
	if (selectedShape == l_temp) {
		return;
	}
	
	vector<VectorData> v = win->getFrameData().getVectorData();
	bool l_move = false;
	double l_x;
	double l_y;
	
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
		} else if (v[i].vectorType == VectorData::TYPE_ENDFILL) {
			i = vsize;
		}
	}
	anchor_center.x = (anchor_upperLeft.x + anchor_lowerRight.x) / 2;
	anchor_center.y = (anchor_upperLeft.y + anchor_lowerRight.y) / 2;
	
	if (mouseDown == true) {
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
//				double l_X = anchor_lowerRight.x - anchor_center.x;
//				double l_Y = anchor_lowerRight.y - anchor_center.y;
				double l_distance = 7;
//				if (l_X > l_Y) {
//					l_distance = l_Y;
//				} else {
//					l_distance = l_X;
//				}
				if (isMouseOnNode(    anchor_center.x    ,     anchor_center.y    , l_distance) == true) {
					mouseOnAnchor = AnchorData::TYPE_MOVE;
				}
			}
		}
		/// NOTE: don't use ELSE-IF to apply changes ASAP
		if (mouseOnAnchor != AnchorData::TYPE_NONE
				&& mouseOnAnchor != AnchorData::TYPE_MOVE) {
			if (mouseOnAnchor == AnchorData::TYPE_NORTH
					|| mouseOnAnchor == AnchorData::TYPE_NORTH_EAST
					|| mouseOnAnchor == AnchorData::TYPE_NORTH_WEST) {
				handleShapes_scaleNorth();
				
				anchor_upperLeft.y = draw2.y;
			} else if (mouseOnAnchor == AnchorData::TYPE_SOUTH
					|| mouseOnAnchor == AnchorData::TYPE_SOUTH_EAST
					|| mouseOnAnchor == AnchorData::TYPE_SOUTH_WEST) {
				handleShapes_scaleSouth();
				
				anchor_lowerRight.y = draw2.y;
			}
			
			if (mouseOnAnchor == AnchorData::TYPE_EAST
					|| mouseOnAnchor == AnchorData::TYPE_NORTH_EAST
					|| mouseOnAnchor == AnchorData::TYPE_SOUTH_EAST) {
				handleShapes_scaleEast();
				
				anchor_lowerRight.x = draw2.x;
			} else if (mouseOnAnchor == AnchorData::TYPE_WEST
					|| mouseOnAnchor == AnchorData::TYPE_NORTH_WEST
					|| mouseOnAnchor == AnchorData::TYPE_SOUTH_WEST) {
				handleShapes_scaleWest();
				
				anchor_upperLeft.x = draw2.x;
			}
			
			anchor_center.x = (anchor_upperLeft.x + anchor_lowerRight.x) / 2;
			anchor_center.y = (anchor_upperLeft.y + anchor_lowerRight.y) / 2;
			
			//update vector data
			v = win->getFrameData().getVectorData();
		} else if (mouseOnAnchor == AnchorData::TYPE_MOVE) {
			//move the whole shape around
			handleShapes_moveShape(
				draw2.x - anchor_center.x,
				draw2.y - anchor_center.y);
			
			anchor_center.x = draw2.x;
			anchor_center.y = draw2.y;
			//update vector data
			v = win->getFrameData().getVectorData();
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
	dashes.clear();
	cr->unset_dash();
	
	if (mouseDown == false) {
		//draw center anchor
		cr->set_line_width(1.0);
		cr->arc(anchor_center.x, anchor_center.y, 7, 0, 2 * M_PI);
			cr->set_source_rgba(1.0,1.0,1.0,1.0);
				cr->fill_preserve();
			cr->set_source_rgba(0.0, 0.0, 0.0, 1.0);
				cr->stroke();
		
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
	}
	
	if (l_move == true) {
		win->setFrameData(v);
	}
}
void KageStage::handleShapesMouseUp() {
	mouseOnAnchor = AnchorData::TYPE_NONE;
	handleNodesMouseUp();
	win->updateColors();
}

void KageStage::handleNodes() {
	unsigned int l_temp = -1;
	
	if (selectedNode == l_temp || selectedShape == l_temp) {
		return;
	}
	
	vector<VectorData> v = win->getFrameData().getVectorData();
	bool l_move = false;
	double l_x;
	double l_y;
	unsigned int typeMovesIndex = -1;
	unsigned int typeEndsPointSize;
	unsigned int vsize = v.size();
	for (unsigned int i = selectedShape; i < vsize; ++i) {
		switch (v[i].vectorType) {
			case VectorData::TYPE_FILL:
				break;
			case VectorData::TYPE_ENDFILL:
				///make sure last point before end fill is in-sync with MOVE(shape's start point)
				if (typeMovesIndex != -1 && mouseDown == true && mouseOnNode == i-1) {
					typeEndsPointSize = v[i-1].points.size()-1;
					if (v[typeMovesIndex].points[0].x != v[typeMovesIndex].points[typeEndsPointSize].x
							|| v[typeMovesIndex].points[0].y != v[typeMovesIndex].points[typeEndsPointSize].y) {
						v[typeMovesIndex].points[0].x = v[i-1].points[typeEndsPointSize].x;
						v[typeMovesIndex].points[0].y = v[i-1].points[typeEndsPointSize].y;
						l_move = true;
					}
					typeMovesIndex = -1;
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
					l_move = true;
					v[i].points[0].x = draw1.x - origin.x;
					v[i].points[0].y = draw1.y - origin.y;
				}
				l_x = v[i].points[0].x + origin.x;
				l_y = v[i].points[0].y + origin.y;
				
				if (isMouseOnNode(l_x, l_y) == true) {
					if (mouseDown == true) {
						l_move = true;
						mouseOnNode = i;
						
						v[i].points[0].x = draw1.x - origin.x;
						v[i].points[0].y = draw1.y - origin.y;
						
						l_x = v[i].points[0].x + origin.x;
						l_y = v[i].points[0].y + origin.y;
					}
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
				
				if (mouseDown == true && mouseOnNode == i && mouseOnNodeIndex == 3) {
					l_move = true;
						//move control B of current curve
						v[i].points[1].x += ((draw1.x - origin.x) - v[i].points[2].x);
						v[i].points[1].y += ((draw1.y - origin.y) - v[i].points[2].y);
						//move control A of next curve
						if (i+1 < v.size()-1 && v[i+1].vectorType == VectorData::TYPE_CURVE_CUBIC) {
							v[i+1].points[0].x += ((draw1.x - origin.x) - v[i].points[2].x);
							v[i+1].points[0].y += ((draw1.y - origin.y) - v[i].points[2].y);
						} else if (v[i+1].vectorType == VectorData::TYPE_ENDFILL) {
							v[typeMovesIndex+1].points[0].x += ((draw1.x - origin.x) - v[i].points[2].x);
							v[typeMovesIndex+1].points[0].y += ((draw1.y - origin.y) - v[i].points[2].y);
						}
					v[i].points[2].x = draw1.x - origin.x;
					v[i].points[2].y = draw1.y - origin.y;
				}
				
				l_x = v[i].points[2].x + origin.x;
				l_y = v[i].points[2].y + origin.y;
				if (isMouseOnNode(l_x, l_y) == true) {
					if (mouseDown == true) {
						l_move = true;
						mouseOnNode = i;
						mouseOnNodeIndex = 3;
						//move control B of current curve
						v[i].points[1].x += ((draw1.x - origin.x) - v[i].points[2].x);
						v[i].points[1].y += ((draw1.y - origin.y) - v[i].points[2].y);
						//move control A of next curve
						if (i+1 < v.size()-1 && v[i+1].vectorType == VectorData::TYPE_CURVE_CUBIC) {
							v[i+1].points[0].x += ((draw1.x - origin.x) - v[i].points[2].x);
							v[i+1].points[0].y += ((draw1.y - origin.y) - v[i].points[2].y);
						} else if (v[i+1].vectorType == VectorData::TYPE_ENDFILL) {
							v[typeMovesIndex+1].points[0].x += ((draw1.x - origin.x) - v[i].points[2].x);
							v[typeMovesIndex+1].points[0].y += ((draw1.y - origin.y) - v[i].points[2].y);
						}
						
						v[i].points[2].x = draw1.x - origin.x;
						v[i].points[2].y = draw1.y - origin.y;
						
						l_x = v[i].points[2].x + origin.x;
						l_y = v[i].points[2].y + origin.y;
					}
					renderNode(l_x, l_y, 1);
				} else {
					if (selectedNode == i) {
						renderNode(l_x, l_y, 2);
					} else {
						renderNode(l_x, l_y, 0);
					}
				}
				
				//draw control points
				if (mouseDown == true && mouseOnNode == i && mouseOnNodeIndex == 1) {
					l_move = true;
					v[i].points[0].x = draw1.x - origin.x;
					v[i].points[0].y = draw1.y - origin.y;
				}
				
				l_x = v[i].points[0].x + origin.x;
				l_y = v[i].points[0].y + origin.y;
				if (isMouseOnNode(l_x, l_y) == true) {
					if (mouseDown == true) {
						l_move = true;
						mouseOnNode = i;
						mouseOnNodeIndex = 1;
						
						v[i].points[0].x = draw1.x - origin.x;
						v[i].points[0].y = draw1.y - origin.y;
						
						l_x = v[i].points[0].x + origin.x;
						l_y = v[i].points[0].y + origin.y;
					}
					renderNodeControl(l_x, l_y, 1);
				} else {
					renderNodeControl(l_x, l_y, 0);
				}
				
				if (mouseDown == true && mouseOnNode == i && mouseOnNodeIndex == 2) {
					l_move = true;
					v[i].points[1].x = draw1.x - origin.x;
					v[i].points[1].y = draw1.y - origin.y;
				}
				
				l_x = v[i].points[1].x + origin.x;
				l_y = v[i].points[1].y + origin.y;
				if (isMouseOnNode(l_x, l_y) == true) {
					if (mouseDown == true) {
						l_move = true;
						mouseOnNode = i;
						mouseOnNodeIndex = 2;
						
						v[i].points[1].x = draw1.x - origin.x;
						v[i].points[1].y = draw1.y - origin.y;
						
						l_x = v[i].points[1].x + origin.x;
						l_y = v[i].points[1].y + origin.y;
					}
					renderNodeControl(l_x, l_y, 1);
				} else {
					renderNodeControl(l_x, l_y, 0);
				}
				break;
			case VectorData::TYPE_TEXT:
			case VectorData::TYPE_IMAGE:
			case VectorData::TYPE_INIT:
			default:
				break;
		}
	}
	
	if (l_move == true) {
		win->setFrameData(v);
	}
}
void KageStage::handleNodesMouseUp() {
	_nodeToMouseDistance = 1080;
	vector<VectorData> v = win->getFrameData().getVectorData();
	selectedNode = -1;
	mouseOnNode = -1;
	unsigned int vsize = v.size();
	for (unsigned int i = 0; i < vsize; ++i) {
		if (v[i].vectorType == VectorData::TYPE_CURVE_CUBIC) {
			if (handleNodes_getNearestShape(v[i].points[0].x + origin.x, v[i].points[0].y + origin.y, i, v)) {
				selectedNode = i-1; //visualization-wise, the control-point is for the previous Node -- it is its right control-point
			}
			if (handleNodes_getNearestShape(v[i].points[1].x + origin.x, v[i].points[1].y + origin.y, i, v)) {
				selectedNode = i;
			}
			if (handleNodes_getNearestShape(v[i].points[2].x + origin.x, v[i].points[2].y + origin.y, i, v)) {
				selectedNode = i;
			}
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
		return true;
	}
	
	return false;
}

double KageStage::getSelectedShapeViaNode(unsigned int p_index, vector<VectorData> p_v) {
	int l_tmp = selectedShape;
	for (unsigned int i = p_index; i > 0; --i) {
		switch (p_v[i].vectorType) {
			case VectorData::TYPE_FILL:
				fillColor = p_v[i].fillColor.clone();
				
				return i;
			case VectorData::TYPE_ENDFILL:
				break;
			case VectorData::TYPE_STROKE:
				 stroke = p_v[i].stroke.clone();
				
				break;
			case VectorData::TYPE_MOVE:
			case VectorData::TYPE_LINE:
			case VectorData::TYPE_CURVE_QUADRATIC:
			case VectorData::TYPE_CURVE_CUBIC:
			case VectorData::TYPE_TEXT:
			case VectorData::TYPE_IMAGE:
			case VectorData::TYPE_INIT:
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
	}
	
	cr->set_source_rgba(0.0, 0.0, 0.0, 1.0);
		cr->stroke();
}

bool KageStage::isMouseOnNode(double p_x, double p_y, unsigned int p_buffer) {
	if (p_x-p_buffer <= draw1.x && p_x+p_buffer >= draw1.x
			&& p_y-p_buffer <= draw1.y && p_y+p_buffer >= draw1.y) {
		return true;
	}
	return false;
}

void KageStage::handleDrawOvalMouseUp(){
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
	VectorDataManager v;
		v.addFill(KageStage::fillColor.clone());
		v.addLineStyle(KageStage::stroke.clone());
		v.addMove(p1);
		v.addCubic( p2,  p3,  p4);
		v.addCubic( p5,  p6,  p7);
		v.addCubic( p8,  p9, p10);
		v.addCubic(p11, p12, p13);
		v.addEndFill();
	win->addDataToFrame(v);
	render();
}

void KageStage::handleDrawRectMouseUp() {
	if (draw1.x == draw2.x && draw1.y == draw2.y) { return; }
	
	PointData p1(draw1);
	PointData p2(draw2.x, draw1.y);
	PointData p3(draw2);
	PointData p4(draw1.x, draw2.y);
	PointData p5(draw1);
	VectorDataManager v;
		v.addFill(KageStage::fillColor.clone());
		v.addLineStyle(KageStage::stroke.clone());
		v.addMove(p1);
		v.addLine(p2);
		v.addLine(p3);
		v.addLine(p4);
		v.addLine(p5);
		v.addEndFill();
	win->addDataToFrame(v);
	render();
}

void KageStage::handleStrokeMouseUp() {
	handleNodesMouseUp();
	
	win->updateSelectedShapeColor(false, true);
	initNodeTool();
}

bool KageStage::deleteSelectedShape() {
	unsigned int l_temp = -1;
	Kage::timestamp();
	std::cout << " KageStage::deleteSelectedShape " << selectedShape << std::endl;
	
	if (selectedShape == l_temp) {
		return false;
	}
	
	vector<VectorData> v = win->getFrameData().getVectorData();
	
	unsigned int vsize = v.size();
	for (unsigned int i = selectedShape; i < vsize; ++i) {
		if (v[i].vectorType == VectorData::TYPE_ENDFILL) {
			l_temp = i;
			break;
		}
	}
	
	v.erase (v.begin() + selectedShape,
			 v.begin() + l_temp);
	
	win->setFrameData(v);
	
	return true;
}

bool KageStage::deleteSelectedNode() {
	unsigned int l_temp = -1;
	Kage::timestamp();
	std::cout << " KageStage::deleteSelectedNode " << selectedShape << " " << selectedNode << std::endl;
	
	if (selectedShape == l_temp) {
		return false;
	}
	
	vector<VectorData> v = win->getFrameData().getVectorData();
	
	unsigned int vsize = v.size();
	
	unsigned int l_sequence = 0;
	if (vsize >= 5) {
		if (v[selectedShape + 0].vectorType == VectorData::TYPE_FILL       ) ++l_sequence;
		if (v[selectedShape + 1].vectorType == VectorData::TYPE_STROKE     ) ++l_sequence;
		if (v[selectedShape + 2].vectorType == VectorData::TYPE_MOVE       ) ++l_sequence;
		if (v[selectedShape + 3].vectorType == VectorData::TYPE_CURVE_CUBIC) ++l_sequence;
		if (v[selectedShape + 4].vectorType == VectorData::TYPE_ENDFILL    ) ++l_sequence;
	}
	std::cout << "vsize " << vsize << std::endl;
	for (unsigned int i = selectedShape; i < vsize; ++i) {
		std::cout << "[" << i << "].vectorType " << v[i].vectorType << std::endl;
		if (v[i].vectorType == VectorData::TYPE_ENDFILL) {
			l_temp = i;
			break;
		}
	}
	
	if (l_sequence == 5) {
		v.erase (v.begin() + selectedShape,
				 v.begin() + selectedShape + l_sequence);
	} else {
		v.erase (v.begin() + selectedNode);
	}
	
	win->setFrameData(v);
	
	return true;
}

//void KageStage::handleFill() {
////	handleEyedrop();
//}

void KageStage::handleFillMouseUp() {
	handleNodesMouseUp();
	
	win->updateSelectedShapeColor(true, false);
	initNodeTool();
}

void KageStage::handleEyedrop() {
	unsigned int l_temp = -1;
	Kage::timestamp();
	std::cout << " KageStage::handleEyedrop " << selectedShape << std::endl;
	
	if (selectedShape == l_temp) {
		return;
	}
	
	vector<VectorData> v = win->getFrameData().getVectorData();
	bool l_move = false;
	double l_x;
	double l_y;
	
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
		} else if (v[i].vectorType == VectorData::TYPE_ENDFILL) {
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
	}
}

void KageStage::handleEyedropMouseUp() {
	handleNodesMouseUp();
	win->updateColors();
}

void KageStage::handleDrawPolyMouseUp() {
//	if (draw1.x == draw2.x && draw1.y == draw2.y) { return false; }
	
	VectorDataManager v;
	if (drawCtr > 0) {
		PointData p2(draw2);
			v.addLinePoly(p2, polyXprev, polyYprev);
		
		polyXprev = draw2.x;
		polyYprev = draw2.y;
	} else {
		polyX = draw1.x;
		polyY = draw1.y;
		PointData p1(draw1);
			v.addFill(KageStage::fillColor.clone());
			v.addLineStyle(KageStage::stroke);
			v.addMove(p1);
	}
	win->addDataToFrame(v);
	render();
	
	++drawCtr;
}

void KageStage::handlePoly() {
	if (drawCtr == 0) return;
	
//	cr->move_to(draw1.x, draw1.y);
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
}

void KageStage::handleRect() {
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

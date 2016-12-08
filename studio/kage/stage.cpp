
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
			std::cout << "[ESC]" << std::endl;
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
		std::cout << "stage " << e->keyval << "_" << e->string << std::endl;
	}
	return true;
}
bool KageStage::on_key_release_event(GdkEventKey *e) {
	if (e->keyval == 32) { // space [hand tool]
		KageStage::toolMode = prevTool;
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
	Kage::timestamp();
	std::cout << " KageStage::on_event < " << e->type << std::endl;
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
		if (KageStage::toolMode == MODE_DRAW_OVAL
				|| KageStage::toolMode == MODE_DRAW_RECT) {
			draw1.x = e->button.x;
			draw1.y = e->button.y;
				draw2.x = e->button.x;
				draw2.y = e->button.y;
		} else if (KageStage::toolMode == MODE_SELECT
				|| KageStage::toolMode == MODE_DRAW_POLY
				|| KageStage::toolMode == MODE_NODE) {
			draw1.x = e->button.x;
			draw1.y = e->button.y;
		} else {
			//
		}
		Kage::timestamp();
		std::cout << " KageStage::on_event mouse down " << e->button.x << " " << e->button.y << std::endl;
	} else if (e->type == GDK_BUTTON_RELEASE) { ///mouse up
		mouseDown = false;
		grab_focus();
		
		draw1.x = (draw1.x - origin.x);
		draw1.y = (draw1.y - origin.y);
			draw2.x = (draw2.x - origin.x);
			draw2.y = (draw2.y - origin.y);
		
		if (KageStage::toolMode == MODE_SELECT) {
			handleShapesMouseUp();
		} else if (KageStage::toolMode == MODE_NODE) {
			handleNodesMouseUp();
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
		Kage::timestamp();
		std::cout << " KageStage::on_event mouse up " << e->button.x << " " << e->button.y << std::endl;
	} else if (e->type == GDK_MOTION_NOTIFY) { ///mouse move
		if (KageStage::toolMode == MODE_NODE) {
			draw1.x = (e->button.x);
			draw1.y = (e->button.y);
			
			render();
		} else if (KageStage::toolMode == MODE_SELECT
				|| KageStage::toolMode == MODE_DRAW_OVAL
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
			double distx = e->button.x - KageStage::moveStageXY.x + origin.x;
			double disty = e->button.y - KageStage::moveStageXY.y + origin.y;
			
			origin.x = distx;
			origin.y = disty;
			
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
//		std::cout << "stage? e " << e->type << std::endl;
	}
//	Kage::timestamp();
//	std::cout << " KageStage::on_event >" << std::endl;
	
	return true;
}

void KageStage::setStageBG(Gdk::Color p_c) {
	KageStage::stageBG.setR(p_c.get_red() / 256);
	KageStage::stageBG.setG(p_c.get_green() / 256);
	KageStage::stageBG.setB(p_c.get_blue() / 256);
	std::cout << KageStage::stageBG.getR() << " " << p_c.get_red() << " " << (p_c.get_red() / 256) << std::endl;
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
	std::cout << KageStage::fillColor.getR() << " " << p_Color.get_red() << " " << (p_Color.get_red() / 256) << std::endl;
	std::cout << KageStage::fillColor.getG() << " " << p_Color.get_green() << " " << (p_Color.get_green() / 256) << std::endl;
	std::cout << KageStage::fillColor.getB() << " " << p_Color.get_blue() << " " << (p_Color.get_blue() / 256) << std::endl;
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
	Kage::timestamp();
	std::cout << " KageStage::render <" << std::endl;
	if (!window) {
		window = get_window();
	}
	
	// force our program to redraw the entire stage
	Gdk::Rectangle r(0, 0, get_allocation().get_width(),
			get_allocation().get_height());
	window->invalidate_rect(r, false);
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
	Kage::timestamp();
	std::cout << " KageStage::renderFrame <" << std::endl;
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
	for (unsigned int i = 1; i < vsize; ++i) {
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
				if (scolor.getThickness() > 0) {
					cr->set_source_rgba((double)scolor.getR()/255,
										(double)scolor.getG()/255,
										(double)scolor.getB()/255,
										(double)scolor.getA()/255);
					cr->set_line_width(scolor.getThickness());
						cr->set_line_cap(Cairo::LINE_CAP_ROUND);
							cr->stroke();
				}
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
	Kage::timestamp();
	std::cout << " KageStage::renderFrame >" << std::endl;
}

void KageStage::initNodeTool() {
	selectedNode = -1;
}


	unsigned int vsize = v.size();
	for (unsigned int i = 1; i < vsize; ++i) {

void KageStage::handleNodes() {
	unsigned int l_temp = -1;
	Kage::timestamp();
	std::cout << " KageStage::handleNodes " << selectedNode << " " << selectedShape << " " << l_temp << std::endl;
	
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
				
				cr->move_to(
					v[i].points[2].x + origin.x,
					v[i].points[2].y + origin.y
				);
				
				//draw anchors
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
	Kage::timestamp();
	std::cout << " KageStage::handleNodesMouseUp" << std::endl;
	vector<VectorData> v = win->getFrameData().getVectorData();
	selectedNode = -1;
	mouseOnNode = -1;
	unsigned int vsize = v.size();
	for (unsigned int i = 1; i < vsize; ++i) {
		if (v[i].vectorType == VectorData::TYPE_CURVE_CUBIC) {
			if (       handleNodesMouseUpHelper(v[i].points[0].x, v[i].points[0].y, i, v)) {
				selectedNode = i-1; //control-point is from previous Node's -- it is its right control-point
			}
			if (handleNodesMouseUpHelper(v[i].points[1].x, v[i].points[1].y, i, v)) {
				selectedNode = i;
			}
			if (handleNodesMouseUpHelper(v[i].points[2].x, v[i].points[2].y, i, v)) {
				selectedNode = i;
			}
		}
	}
	
	render();
}

bool KageStage::handleNodesMouseUpHelper(double p_x, double p_y, unsigned int p_index, vector<VectorData> p_v) {
	double l_vXdiff, l_vYdiff;
	double l_distance;
	std::cout << " KageStage::handleNodesMouseUpHelper " << p_index;
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
			&& isMouseOnNode(p_x + origin.x, p_y + origin.y, 100) == true) {
		_nodeToMouseDistance = l_distance;
		for (unsigned int i = p_index; i > 1; --i) {
			switch (p_v[i].vectorType) {
				case VectorData::TYPE_FILL:
				case VectorData::TYPE_ENDFILL:
				case VectorData::TYPE_STROKE:
					break;
				case VectorData::TYPE_MOVE:
					selectedShape = i;
					std::cout << " > \t\t" << _nodeToMouseDistance << std::endl;
					return true;
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
		return true;
	}
	std::cout << " >> " << l_distance << std::endl;
	return false;
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
	cr->move_to(p_x-3, p_y);
		cr->curve_to(
				p_x-3, p_y  ,
				p_x-3, p_y-3,
				p_x  , p_y-3);
		cr->curve_to(
				p_x  , p_y-3,
				p_x+3, p_y-3,
				p_x+3, p_y
				);
		cr->curve_to(
				p_x+3, p_y,
				p_x+3, p_y+3,
				p_x  , p_y+3
				);
		cr->curve_to(
				p_x  , p_y+3,
				p_x-3, p_y+3,
				p_x-3, p_y
				);
	cr->close_path();
	if (p_selected == true) {
		cr->set_source_rgba(1.0,0.0,0.0,1.0);
		cr->fill_preserve();
	} else {
		cr->set_source_rgba(1.0,1.0,1.0,1.0);
	}
	
	cr->set_source_rgba(0.0, 0.0, 0.0, 1.0);
		cr->set_line_width(1.0);
			cr->set_line_cap(Cairo::LINE_CAP_ROUND);
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
	handlePoly();
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

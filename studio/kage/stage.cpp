
#include "stage.h"
#include "kage.h"
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
			std::cout << "[ESC]" << endl;
		} else {
			drawCtr = 0;
			VectorDataManager v;
				v.addEndFill();
			win->addDataToFrame(v);
			render();
		}
	} else if (e->keyval == 44) { //, comma
		//
	} else {
		std::cout << "stage " << e->keyval << "_" << e->string << endl;
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
		Kage::timestamp();
		std::cout << " KageStage::on_event enter" << std::endl;
		render();
	} else if (e->type == GDK_LEAVE_NOTIFY) {
		Kage::timestamp();
		std::cout << " KageStage::on_event leave" << std::endl;
//		render();
	} else if (e->type == GDK_KEY_PRESS) {
		on_key_press_event((GdkEventKey*) e);
	} else if (e->type == GDK_KEY_RELEASE) {
		on_key_release_event((GdkEventKey*) e);
	} else if (e->type == GDK_BUTTON_PRESS) {
		mouseDown = true;
		if (KageStage::toolMode == MODE_DRAW_RECT) {
			//
		} else if (KageStage::toolMode == MODE_DRAW_POLY) {
			draw1.x = e->button.x;
			draw1.y = e->button.y;
		} else {
			//
		}
		if (KageStage::toolMode == MODE_DRAW_RECT) {
			draw1.x = e->button.x;
			draw1.y = e->button.y;
				draw2.x = e->button.x;
				draw2.y = e->button.y;
		}
		Kage::timestamp();
		std::cout << " KageStage::on_event mouse down " << e->button.x << " " << e->button.y << endl;
	} else if (e->type == GDK_BUTTON_RELEASE) {
		mouseDown = false;
		grab_focus();
		if (KageStage::toolMode == MODE_DRAW_RECT) {
			draw1.x = (draw1.x - origin.x);
			draw1.y = (draw1.y - origin.y);
				draw2.x = (draw2.x - origin.x);
				draw2.y = (draw2.y - origin.y);
		} else if (KageStage::toolMode == MODE_DRAW_POLY) {
			draw1.x = (draw1.x - origin.x);
			draw1.y = (draw1.y - origin.y);
				draw2.x = (draw2.x - origin.x);
				draw2.y = (draw2.y - origin.y);
		}
		if (KageStage::toolMode == MODE_DRAW_RECT) {
			PointData p1(draw1);
			PointData p2(draw2.x, draw1.y);
			PointData p3(draw2);
			PointData p4(draw1.x, draw2.y);
			VectorDataManager v;
				v.addFill(KageStage::fillColor.clone());
				v.addLineStyle(KageStage::stroke.clone());
				v.addMove(p1);
				v.addLine(p2);
				v.addLine(p3);
				v.addLine(p4);
				v.addEndFill();
			win->addDataToFrame(v);
			render();
		} else if (KageStage::toolMode == MODE_DRAW_POLY) {
			VectorDataManager v;
			if (drawCtr > 0) {
				PointData p2(draw2);
					v.addLine(p2);
			} else {
				PointData p1(draw1);
					v.addFill(KageStage::fillColor.clone());
					v.addLineStyle(KageStage::stroke);
					v.addMove(p1);
			}
			win->addDataToFrame(v);
			render();
			
			++drawCtr;
			
			draw1.x = (e->button.x);
			draw1.y = (e->button.y);
				draw2.x = (e->button.x);
				draw2.y = (e->button.y);
		} else {
			//
		}
		Kage::timestamp();
		std::cout << " KageStage::on_event mouse up " << e->button.x << " " << e->button.y << endl;
	} else if (e->type == GDK_MOTION_NOTIFY) {
		if (mouseDown == true) {
			Kage::timestamp();
			std::cout << " KageStage::on_event mouse move " << e->button.x << " " << e->button.y << endl;
		}
		if (KageStage::toolMode == MODE_DRAW_RECT) {
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
		} else if (KageStage::toolMode == MODE_SELECT) {
			//
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
		std::cout << "stage? e " << e->type << endl;
	}
	Kage::timestamp();
	std::cout << " KageStage::on_event >" << std::endl;
	
	return true;
}

void KageStage::setStageBG(Gdk::Color p_Color) {
	KageStage::stageBG.setR(p_Color.get_red() / 256);
	KageStage::stageBG.setG(p_Color.get_green() / 256);
	KageStage::stageBG.setB(p_Color.get_blue() / 256);
	std::cout << KageStage::stageBG.getR() << " " << p_Color.get_red() << " " << (p_Color.get_red() / 256) << endl;
	render();
}
Gdk::Color KageStage::getStageBG() {
	Gdk::Color tColor;
	tColor.set_red((gushort)KageStage::stageBG.getR() * 256);
	tColor.set_green((gushort)KageStage::stageBG.getG() * 256);
	tColor.set_blue((gushort)KageStage::stageBG.getB() * 256);
	
	return tColor;
}

void KageStage::setFill(Gdk::Color p_Color) {
	KageStage::fillColor.setR(p_Color.get_red() / 256);
	KageStage::fillColor.setG(p_Color.get_green() / 256);
	KageStage::fillColor.setB(p_Color.get_blue() / 256);
	std::cout << KageStage::fillColor.getR() << " " << p_Color.get_red() << " " << (p_Color.get_red() / 256) << endl;
	std::cout << KageStage::fillColor.getG() << " " << p_Color.get_green() << " " << (p_Color.get_green() / 256) << endl;
	std::cout << KageStage::fillColor.getB() << " " << p_Color.get_blue() << " " << (p_Color.get_blue() / 256) << endl;
	render();
}
Gdk::Color KageStage::getFill() {
	Gdk::Color tColor;
	tColor.set_red((gushort)KageStage::fillColor.getR() * 256);
	tColor.set_green((gushort)KageStage::fillColor.getG() * 256);
	tColor.set_blue((gushort)KageStage::fillColor.getB() * 256);
	
	return tColor;
}

void KageStage::setStroke(Gdk::Color p_Color) {
	KageStage::stroke.setR(p_Color.get_red() / 256);
	KageStage::stroke.setG(p_Color.get_green() / 256);
	KageStage::stroke.setB(p_Color.get_blue() / 256);
	render();
}
Gdk::Color KageStage::getStroke() {
	Gdk::Color tColor;
	tColor.set_red((gushort)KageStage::stroke.getR() * 256);
	tColor.set_green((gushort)KageStage::stroke.getG() * 256);
	tColor.set_blue((gushort)KageStage::stroke.getB() * 256);
	
	return tColor;
}

void KageStage::render() {
	Kage::timestamp();
	std::cout << " KageStage::render < " << endl;
	if (!window) {
		window = get_window();
	}
	
	// force our program to redraw the entire stage
	Gdk::Rectangle r(0, 0, get_allocation().get_width(),
			get_allocation().get_height());
	Kage::timestamp();
	std::cout << " KageStage::render > " << get_allocation().get_width() << "x" << get_allocation().get_height() << endl;
	window->invalidate_rect(r, false);
}	
	
bool KageStage::on_draw(const Cairo::RefPtr<Cairo::Context>& p_cr) {
	Kage::timestamp();
	std::cout << " KageStage::on_draw <" << std::endl;
	
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
		if (KageStage::toolMode == MODE_DRAW_POLY && drawCtr > 0) {
//			cr->move_to(draw1.x, draw1.y);
			cr->line_to(draw2.x, draw2.y);
				cr->set_source_rgba((double)KageStage::fillColor.getR()/255, (double)KageStage::fillColor.getG()/255, (double)KageStage::fillColor.getB()/255, (double)KageStage::fillColor.getA()/255);
				cr->fill_preserve();
					cr->set_line_width(stroke.getThickness() / 2);
					cr->set_source_rgba((double)KageStage::stroke.getR()/255, (double)KageStage::stroke.getG()/255, (double)KageStage::stroke.getB()/255, (double)KageStage::stroke.getA()/255);
					cr->stroke();
		} else if (KageStage::toolMode == MODE_DRAW_RECT && mouseDown == true) {
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
		if (mouseDown) {
			win->renderFramesAboveCurrentLayer();
		}
	}
	Kage::timestamp();
	std::cout << " KageStage::on_draw >" << std::endl;
	return true;
}

void KageStage::clearScreen() {
	Kage::timestamp();
	std::cout << " KageStage::clearScreen <" << std::endl;
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
	Kage::timestamp();
	std::cout << " KageStage::clearScreen >" << std::endl;
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
				cr->curve_to(p.x + origin.x, p.y + origin.y,
								v[i].points[0].x + origin.x, v[i].points[0].y + origin.y,
								v[i].points[1].x + origin.x, v[i].points[1].y + origin.y);
				
				p.x = v[i].points[1].x;
				p.y = v[i].points[1].y;
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
			case VectorData::TYPE_INIT:
				///for review forgot what for.
				break;
		}
	}
	Kage::timestamp();
	std::cout << " KageStage::renderFrame >" << std::endl;
}

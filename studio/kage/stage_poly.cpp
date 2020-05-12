//part of stage.cpp

#include "stage.h"
#include "../kage.h"
#include <cairomm/context.h>

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
			vector<unsigned int> l_shape;
				l_shape.push_back(0);
		_polyVectors.recenterRotationPoint(l_shape);
		win->addDataToFrame(_polyVectors);
		drawCtr = 0;
		_polyVectors.clear();
		win->ToolSelect_onClick();
		win->stackDo();
	} else if (drawCtr > 0
			&& draw1.x-5 <= polyXtail && draw1.x+5 >= polyXtail
			&& draw1.y-5 <= polyYtail && draw1.y+5 >= polyYtail) {
		_polyVectors.addEndFill();
			vector<unsigned int> l_shape;
				l_shape.push_back(0);
		_polyVectors.recenterRotationPoint(l_shape);
		win->addDataToFrame(_polyVectors);
		_polyVectors.clear();
		drawCtr = 0;
		win->ToolSelect_onClick();
		win->stackDo();
	} else {
		if (drawCtr > 0) {
			cout << "\t\tTEST a " << draw1.x <<  " b " << draw2.x << " and " << drawConstraint.x << " ? " << origin.x << " and " << _mouseLocation.x << endl;
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
	
	if (drawCtr > 1
			&& draw2.x-5 <= polyXhead+origin.x && draw2.x+5 >= polyXhead+origin.x
			&& draw2.y-5 <= polyYhead+origin.y && draw2.y+5 >= polyYhead+origin.y) {
		cr->move_to(    polyXhead-5 + origin.x, polyYhead-5 + origin.y);
			cr->line_to(polyXhead+5 + origin.x, polyYhead-5 + origin.y);
			cr->line_to(polyXhead+5 + origin.x, polyYhead+5 + origin.y);
			cr->line_to(polyXhead-5 + origin.x, polyYhead+5 + origin.y);
		cr->close_path();
			cr->set_source_rgb(1.0, 0.0, 0.0);
			cr->fill_preserve();
				cr->set_line_width(1.0);
				cr->set_source_rgb(0.0, 0.0, 0.0);
				cr->stroke();
	} else {
		cr->move_to(    polyXhead-3 + origin.x, polyYhead-3 + origin.y);
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

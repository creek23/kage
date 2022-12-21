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

void KageStage::handleDrawPolyMouseUp() {
	if (win->isLayerLocked() == true) {
		return;
	}
	
	Kage::timestamp_IN();
	std::cout << " KageStage::handleDrawPolyMouseUp" << std::endl;
	
	if (drawCtr > 0) {
		if (       draw1.x-5 <= polyXhead && draw1.x+5 >= polyXhead
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
		} else if (draw1.x-5 <= polyXtail && draw1.x+5 >= polyXtail
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
			PointData p2(draw1);
				_polyVectors.addLinePoly(p2, polyXtail, polyYtail);
			
			polyXtail = draw1.x;
			polyYtail = draw1.y;
			++drawCtr;
		}
	} else {
		polyXhead = draw1.x;
		polyYhead = draw1.y;
		PointData p1(draw1);
			_polyVectors.addInit();
			_polyVectors.addFill(KageStage::fillColor.clone());
			_polyVectors.addLineStyle(KageStage::stroke);
			_polyVectors.addMove(p1);
		
		polyXtail = draw1.x;
		polyYtail = draw1.y;
		++drawCtr;
	}
	
	invalidateToRender();
	
	Kage::timestamp_OUT();
}

void KageStage::handlePoly() {
	if (win->isLayerLocked() == true) {
		return;
	}
	if (drawCtr == 0) return;

	cr->set_line_width(stroke.getThickness() / 2);
	cr->set_source_rgba(0.749, 1.0, 0.749, 1.0);
	cr->stroke();
	//draw guide
	cr->move_to(draw1.x, draw1.y);
	cr->line_to(draw2.x, draw2.y);
		cr->set_line_width(0.5);
		cr->set_source_rgba(1.0, 0.0, 0.0, 1.0);
		cr->stroke();
	
	//draw point-of-origin box
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
	
	invalidateToRender();
	
	return true;
}

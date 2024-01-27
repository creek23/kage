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

void KageStage::handleDrawPencilMouseDown(PointData p_point) {
	drawCtr = 0;
	
	_polyVectors.clear();
	
	polyXhead = p_point.x - origin.x;
	polyYhead = p_point.y - origin.y;
	
	drawFree1.x = polyXhead;
	drawFree1.y = polyYhead;
	
	PointData p1(drawFree1);
		_polyVectors.addInit();
		_polyVectors.addFill(KageStage::fillColor.clone());
		_polyVectors.addLineStyle(KageStage::stroke);
		_polyVectors.addMove(p1);

}

void KageStage::handleDrawPencilMouseMove(PointData p_point) {
	if (mouseDown == true) {
		++drawCtr;
		
		if (drawCtr == 1) {
			drawFree2.x = p_point.x;
			drawFree2.y = p_point.y;
		} else if (drawCtr == 2) {
			drawFree3.x = p_point.x;
			drawFree3.y = p_point.y;
		} else if (drawCtr == 3) {
			drawFree4.x = p_point.x;
			drawFree4.y = p_point.y;
			
			drawCtr = 0;
			/*
			if (_polyVectors.getVectorData().size() > 4
			        && draw1.x-5 <= polyXhead && draw1.x+5 >= polyXhead
					&& draw1.y-5 <= polyYhead && draw1.y+5 >= polyYhead) {
				_polyVectors.addCubic(PointData(drawFree2), PointData(drawFree3), PointData(drawFree4));
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
			} else {/*if (drawCtr > 0
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
				win->stackDo();*/
				drawFree2.x -= origin.x;
				drawFree2.y -= origin.y;
				drawFree3.x -= origin.x;
				drawFree3.y -= origin.y;
				drawFree4.x -= origin.x;
				drawFree4.y -= origin.y;
				_polyVectors.addCubic(PointData(drawFree2), PointData(drawFree3), PointData(drawFree4));
			//}
			
			polyXtail = drawFree4.x;
			polyYtail = drawFree4.y;
			
		}
		
		if (drawCtr > 0) {
			//TODO: copied from Draw Poly; is this needed here?
//			draw2.x = (e->button.x);
//			draw2.y = (e->button.y);
		}
		
		invalidateToRender();
	}
}
void KageStage::handleDrawPencilMouseUp() {
	if (win->isLayerLocked() == true) {
		return;
	}
	
	Kage::timestamp_IN();
	std::cout << " KageStage::handleDrawPencilMouseUp" << std::endl;
	
	if (_polyVectors.getVectorData().size() > 4) {
		if (   draw2.x-5 <= polyXhead && draw2.x+5 >= polyXhead
			&& draw2.y-5 <= polyYhead && draw2.y+5 >= polyYhead) {
			_polyVectors.addLinePoly(PointData(polyXhead,polyYhead), polyXtail, polyYtail);
			_polyVectors.addClosePath();
			_polyVectors.addEndFill();
				vector<unsigned int> l_shape;
					l_shape.push_back(0);
			_polyVectors.recenterRotationPoint(l_shape);
			win->addDataToFrame(_polyVectors);
			_polyVectors.clear();
			drawCtr = 0;
			win->stackDo();
		/*} else if (drawCtr > 0
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
		} else if (drawCtr > 0
				&& draw1.x-5 <= polyXtail && draw1.x+5 >= polyXtail
				&& draw1.y-5 <= polyYtail && draw1.y+5 >= polyYtail) {*/
		} else{
			_polyVectors.addEndFill();
				vector<unsigned int> l_shape;
					l_shape.push_back(0);
			_polyVectors.recenterRotationPoint(l_shape);
			win->addDataToFrame(_polyVectors);
			_polyVectors.clear();
			drawCtr = 0;
			win->stackDo();
		}
	}
	
	invalidateToRender();
	Kage::timestamp_OUT();
}

void KageStage::handlePencil() {
	if (win->isLayerLocked() == true) {
		return;
	}
	if (_polyVectors.getVectorData().size() < 5) return;
	
	cr->move_to(drawFree4.x+origin.x, drawFree4.y+origin.y);
	cr->line_to(    draw2.x         ,     draw2.y);
		cr->set_line_width(stroke.getThickness() / 2);
		cr->set_source_rgba(0.533, 0.541, 0.522, 1.0);
		cr->stroke();
	
	//draw point-of-origin box
	if (       draw2.x-5 <= polyXhead+origin.x && draw2.x+5 >= polyXhead+origin.x
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

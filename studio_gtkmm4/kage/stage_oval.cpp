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
	
	invalidateToRender();
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

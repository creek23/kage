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
	
	invalidateToRender();
}

void KageStage::handleRect() {
	cout << "handleRect " << draw1.x << " x " << draw2.x << endl;
	cout << "win->isLayerLocked() " << win->isLayerLocked() << endl;
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

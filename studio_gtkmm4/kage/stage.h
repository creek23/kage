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

#ifndef GTKMM_KAGE_STAGE_H
	#define GTKMM_KAGE_STAGE_H
	
	#include <gtkmm/drawingarea.h>
	#include <iostream>
	
	#include "data/anchor.h"
	#include "data/color.h"
	#include "data/strokecolor.h"
	#include "data/vectordata.h"
	#include "vectordatamanager.h"
	#include <cairomm/context.h>
	#include <gdkmm/general.h> // set_source_pixbuf()
	#include "../util/cairo/cairo_kage.h"
	
	#include <gtkmm/gesturedrag.h>
	#include <gtkmm/gesturestylus.h>
	#include <gtkmm/gestureclick.h>
	
	class Kage; //forward declaration
	
	class KageStage : public Gtk::DrawingArea {
		public:
			static Glib::RefPtr<Gdk::Pixbuf> imageSHAPE_000;
			static Glib::RefPtr<Gdk::Pixbuf> imageSHAPE_045;
			static Glib::RefPtr<Gdk::Pixbuf> imageSHAPE_090;
			static Glib::RefPtr<Gdk::Pixbuf> imageSHAPE_135;
			static Glib::RefPtr<Gdk::Pixbuf> imageSHAPE_MOVE;
			static Glib::RefPtr<Gdk::Pixbuf> imageSHAPE_NE;
			static Glib::RefPtr<Gdk::Pixbuf> imageSHAPE_NW;
			static Glib::RefPtr<Gdk::Pixbuf> imageSHAPE_SW;
			static Glib::RefPtr<Gdk::Pixbuf> imageSHAPE_SE;
			static Glib::RefPtr<Gdk::Pixbuf> imageSHAPE_ROTATE;
			
			KageStage(Kage *p_win);
			virtual ~KageStage();
		
			enum ToolMode {
				MODE_NONE,
				MODE_SELECT,
				MODE_NODE,
				MODE_STROKE,
				MODE_FILL,
				MODE_EYEDROP,
				MODE_ZOOM,
				MODE_MOVE_STAGE,
				MODE_DRAW_RECT,
				MODE_DRAW_TEXT,
				MODE_DRAW_POLY,
				MODE_DRAW_OVAL,
				MODE_DRAW_LINE,
				MODE_DRAW_PENCIL
			};
			const unsigned int _NO_SELECTION = -1;
			static ColorData stageBG; ///direct use for get only
			static ColorData fillColor; ///direct use for get only
			static StrokeColorData stroke; ///direct use for get only
			static ToolMode toolMode;
			static PointData moveStageXY;
			unsigned int fps;
			double fpsElapse;
			double propX = 0;
			int propXindex1;
			int propXindex2;
			double propY = 0;
			int propYindex1;
			int propYindex2;
			double propWidth = 0;
			double propHeight = 0;
			double nodeX = 0;
			double nodeY = 0;
			unsigned int nodeIndexX = 0;
			unsigned int nodeIndexY = 0;
			
			void printVectors();
			void cleanSlate();
			void invalidateToRender();
			bool _invalidated;
			
			void setStageBG(Gdk::RGBA p_Color);
			Gdk::RGBA getStageBG();
			void setFill(Gdk::RGBA p_Color);
			Gdk::RGBA getFill();
			void setStroke(Gdk::RGBA p_Color);
			Gdk::RGBA getStroke();
			double stageWidth; //direct use for get only
			double stageHeight; //direct use for get only
			void clearScreen(Cairo::RefPtr<Cairo::Context> p_context);
			void renderFrame(Cairo::RefPtr<Cairo::Context> p_context, bool p_force = false);
			void renderOnionFrame(Cairo::RefPtr<Cairo::Context> p_context, vector<VectorData> p_vectorData, double p_alpha);
			void renderFrame(Cairo::RefPtr<Cairo::Context> p_context, vector<VectorData> p_vectorData, double p_alpha = 1.0);
			void renderFrameOffset(Cairo::RefPtr<Cairo::Context> p_context, bool p_force = false, double p_offsetX = 0.0, double p_offsetY = 0.0);
			void renderFrameOffset(Cairo::RefPtr<Cairo::Context> p_context, vector<VectorData> p_vectorData, double p_alpha = 1.0, double p_offsetX = 0.0, double p_offsetY = 0.0);
			void initNodeTool();
			void handleShapes();
			void handleShapes_modifyingShapeRotate();
			void handleShapes_modifyingShape();
			void handleShapes_scaleNorth();
			void handleShapes_scaleEast();
			void handleShapes_scaleWest();
			void handleShapes_scaleSouth();
			void handleShapes_moveShape(double p_diffX, double p_diffY);
			void handleShapes_updateAnchors(double p_x, double p_y);
			void handleShapesMouseDown();
			void handleShapesMouseUp();
			void handleNodes();
			void handleNodes_selection();
			void handleNodes_relocation();
			void handleNodes_rendering();
			void handleNodesMouseDown();
			void handleNodesMouseUp();
			bool handleNodes_getNearestShape(double p_x, double p_y, unsigned int p_index, vector<VectorData> p_v);
			void handleDrawOvalMouseUp();
			void handleDrawRectMouseUp();
			void handleDrawPolyMouseUp();
			void handleDrawPencilMouseDown(PointData p_point);
			void handleDrawPencilMouseMove(PointData p_point);
			void handleDrawPencilMouseUp();
			void handleOval();
			void handleRect();
			void handlePoly();
			void handlePencil();
			void handleStroke(); //needed?
			void handleStrokeMouseMove();
			void handleStrokeMouseUp();
			void handleFill(); //needed?
			void handleFillMouseUp();
			void handleEyedrop();
			void handleEyedropMouseMove();
			void handleEyedropMouseUp();
			Glib::RefPtr<Gdk::Pixbuf> _bg;
			Cairo::RefPtr<Cairo::Surface> _bgcr;
			
			bool selectAllShapes();
			bool deselectSelectedShapes();
			bool deselectSelectedNodes();
			bool cancelDrawingPoly();
			bool cutSelectedShapes();
			bool copySelectedShapes();
			bool pasteSelectedShapes();
			bool deleteSelectedShapes();
			bool deleteSelectedNodes();
			bool toggleLineSelectedNodes();
			
			void updateNodeXY();
			bool isSelectedNode(unsigned int p_index);
			void addSelectedNode(unsigned int p_index);
			bool isSelectedShape(unsigned int p_index);
			void addSelectedShape(unsigned int p_index);
			void trySingleSelectShape();
			void tryMultiSelectShapes();
			void tryMultiSelectShapes_populateShapes();
			unsigned int getShape(unsigned int p_index, vector<VectorData> p_v);

			
			void updateShapeColor(bool p_doFill = true, bool p_doStroke = true);
			void updateShapeX(double p_value, bool p_stackDo = true);
			void updateShapeY(double p_value, bool p_stackDo = true);
			void updateShapeWidth(double p_value);
			void updateShapeHeight(double p_value);
			void updateNodeX(double p_value, bool p_stackDo = true);
			void updateNodeY(double p_value, bool p_stackDo = true);
			bool _stackDo = false;
			vector<VectorData> _vectorDataCopyBuffer;
			vector<VectorData> _vectorDataZOrderBufferA;
			vector<VectorData> _vectorDataZOrderBufferB;
			vector<VectorData> _vectorDataZOrderBufferC;
			
			bool renderToPNG(string p_path, bool p_transparent);
			bool renderToPNGOffset(string p_path, bool p_transparent, double p_offsetX = 0.0, double p_offsetY = 0.0);
			void renderFrameToPNG(Cairo::RefPtr<Cairo::Context> p_context);
			void renderFrameToPNGOffset(Cairo::RefPtr<Cairo::Context> p_context, double p_offsetX = 0.0, double p_offsetY = 0.0);
			
			Cairo::RefPtr<Cairo::Context> cr;
			PointData origin;
			
			bool _isModifyingShape;
			
			double _zoomRatio;
			double _zoomValue;
			double _zoomValueShapeProperty;
			PointData _zoomReference;
			PointData __origin;
			PointData __stageArea;
			
			PointData _rotateReference;
			
			PointData applyZoomRatio(PointData p_value);
			double applyZoomRatioX(double p_value);
			double applyZoomRatioY(double p_value);
			double getZoomRatioX(double p_value);
			double getZoomRatioY(double p_value);
			void applyZoom();
			vector<VectorData> applyZoom(vector<VectorData> p_v);
			
			bool _rotateMode;
			bool _rotateApply;
			
			void setSelectedShapes(vector<unsigned int> p_selectedShapes);
			vector<unsigned int> getSelectedShapes();

			void unpressKeys();
		protected:
			ToolMode prevTool; //used by Hand-tool shortcut [spacebar]
			Kage *win;
			PointData draw1;
			PointData draw2;
			PointData drawFree1;
			PointData drawFree2;
			PointData drawFree3;
			PointData drawFree4;
			short int drawFreeCtr;
			PointData drawConstraint;
			double polyXhead, polyYhead;
			double polyXtail, polyYtail;
			VectorDataManager _polyVectors;
			Glib::RefPtr<Gtk::Window> window;
			bool mouseDown;
			PointData _mouseLocation;
			unsigned int _mouseLocationShapeIndex;
			PointData _copyBufferMouse;
			bool draw;
			unsigned int drawCtr;
			//Override default signal handler:
			//virtual bool on_expose_event(GdkEventExpose *e);
			Glib::RefPtr<Gtk::GestureDrag> controllerDrag;
			Glib::RefPtr<Gtk::GestureStylus> controllerStylus;
			Glib::RefPtr<Gtk::GestureClick> controllerClick;
			void onDragBegin(double p_x, double p_y);
			void onDragUpdate(double p_x, double p_y);
			void onDragEnd(double p_x, double p_y);
			void onStylusDown(double p_x, double p_y);
			void onStylusMotion(double p_x, double p_y);
			void onStylusUp(double p_x, double p_y);
			void onClick(int n_press, double p_x, double p_y);
			void draw_brush(double p_x, double p_y, double size = 0.5);
			double start_x, start_y;
			
			void on_draw(const Cairo::RefPtr<Cairo::Context>& p_cr, int p_width, int p_height);
			
			void on_key_pressed(guint keyval, guint keycode, Gdk::ModifierType state);
			void on_key_released(guint keyval, guint keycode, Gdk::ModifierType state);
			bool on_event(GdkEvent *e);
			
			//bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
			
			bool on_timeout();
			unsigned int mouseOnNode;
			PointData mouseOnNodeOffset;
			unsigned int mouseOnNodeHover;
			unsigned int mouseOnNodeIndex;
			unsigned int selectedNode;
			unsigned int mouseOnShape;
			unsigned int selectedShape;
			vector<unsigned int> selectedNodes;
			vector<unsigned int> selectedShapes;
			
			AnchorData::type mouseOnAnchor;
			void renderNode(double p_x, double p_y, unsigned int p_state = 5);
			void renderNodeControl(double p_x, double p_y, bool p_selected);
			bool isMouseOnNode(double p_x, double p_y, unsigned int p_buffer = 5);
			
			PointData selectionBox1;
			PointData selectionBox2;
			bool isSelectionBoxNormalized();
			void normalizeSelectionBox();
			bool isNodeOnSelectionBox(double p_nodeX, double p_nodeY);
			
			void drawSelectionArea();
			
			bool deleteSelectedNode(unsigned int p_index);
			
			unsigned int getSelectedShapeViaNode(unsigned int p_index, vector<VectorData> p_v);
			double _nodeToMouseDistance;
			
			AnchorData anchor_upperLeft;
			AnchorData anchor_center;
			AnchorData anchor_lowerRight;
			
			AnchorData anchor_rotate; //for global use rotation-anchor
			
			bool keyShiftDown;
			bool keyControlDown;
			bool keyUpDown;
			bool keyDownDown;
			bool keyLeftDown;
			bool keyRightDown;
			
			int _registerWidth;
			int _registerHeight;
			
			void updateShapeXY();

			void onResize(int width, int height);
    };
#endif // GTKMM_KAGE_STAGE_H

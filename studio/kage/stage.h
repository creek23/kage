#ifndef GTKMM_KAGE_STAGE_H
	#define GTKMM_KAGE_STAGE_H
	
	#include <gtkmm/drawingarea.h>
	#include <gdkmm/general.h>
	#include <iostream>
	
	#include "data/anchor.h"
	#include "data/color.h"
	#include "data/strokecolor.h"
	#include "data/vectordata.h"
	#include <cairomm/context.h>
	
	class Kage; //forward declaration
	
	class KageStage : public Gtk::DrawingArea {
		public:
			static Glib::RefPtr<Gdk::Pixbuf> imageSHAPE_000;
			static Glib::RefPtr<Gdk::Pixbuf> imageSHAPE_045;
			static Glib::RefPtr<Gdk::Pixbuf> imageSHAPE_090;
			static Glib::RefPtr<Gdk::Pixbuf> imageSHAPE_135;
			static Glib::RefPtr<Gdk::Pixbuf> imageSHAPE_MOVE;
			
			KageStage(Kage *p_win);
			virtual ~KageStage();
		
			enum ToolMode {
				MODE_NONE,
				MODE_SELECT,
				MODE_NODE,
				MODE_STROKE,
				MODE_FILL,
				MODE_EYEDROP,
				MODE_MOVE_STAGE,
				MODE_DRAW_RECT,
				MODE_DRAW_TEXT,
				MODE_DRAW_POLY,
				MODE_DRAW_OVAL,
				MODE_DRAW_LINE
			};
			const unsigned int _NO_SELECTION = -1;
			static ColorData stageBG; //direct use for get only
			static ColorData fillColor; //direct use for get only
			static StrokeColorData stroke; //direct use for get only
			static ToolMode toolMode;
			static GdkPoint moveStageXY;
			unsigned int fps;
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
			
			void printVectors();
			void cleanSlate();
			void render();
			
			void setStageBG(Gdk::Color p_Color);
			Gdk::Color getStageBG();
			void setFill(Gdk::Color p_Color);
			Gdk::Color getFill();
			void setStroke(Gdk::Color p_Color);
			Gdk::Color getStroke();
			unsigned int stageWidth; //direct use for get only
			unsigned int stageHeight; //direct use for get only
			void clearScreen(Cairo::RefPtr<Cairo::Context> p_context);
			void renderFrame(Cairo::RefPtr<Cairo::Context> p_context);
			void initNodeTool();
			void handleShapes();
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
			void handleNodesMouseDown();
			void handleNodesMouseUp();
			bool handleNodes_getNearestShape(double p_x, double p_y, unsigned int p_index, vector<VectorData> p_v);
			void handleDrawOvalMouseUp();
			void handleDrawRectMouseUp();
			void handleDrawPolyMouseUp();
			void handlePoly();
			void handleRect();
			void handleOval();
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
			
			bool duplicateShapes();
			bool selectAllShapes();
			bool deselectSelectedShapes();
			bool deselectSelectedNodes();
			bool cancelDrawingPoly();
			bool groupSelectedShapes();
			bool ungroupSelectedShapes();
			bool cutSelectedShapes();
			bool copySelectedShapes();
			bool pasteSelectedShapes();
			bool deleteSelectedShapes();
			bool deleteSelectedNode();
			bool raiseSelectedShape();
			bool lowerSelectedShape();
			bool raiseToTopSelectedShape();
			bool lowerToBottomSelectedShape();
			
			void addSelectedNode(unsigned int p_index);
			void addSelectedShape(unsigned int p_index);
			void trySingleSelectShape();
			void tryMultiSelectShapes();
			void tryMultiSelectShapes_populateShapes();
			unsigned int getShape(unsigned int p_index, vector<VectorData> p_v);

			
			void updateShapeColor(bool p_doFill = true, bool p_doStroke = true);
			void updateShapeX(double p_value);
			void updateShapeY(double p_value);
			void updateShapeWidth(double p_value);
			void updateShapeHeight(double p_value);
			void updateNodeX(double p_value);
			void updateNodeY(double p_value);
			
			vector<VectorData> _vectorDataCopyBuffer;
			vector<VectorData> _vectorDataZOrderBufferA;
			vector<VectorData> _vectorDataZOrderBufferB;
			vector<VectorData> _vectorDataZOrderBufferC;
			
			void renderToPNG(string p_path, bool p_transparent);
			
			Cairo::RefPtr<Cairo::Context> cr;
			GdkPoint origin;
			
			bool _isModifyingShape;
			
		protected:
			ToolMode prevTool; //used by Hand-tool shortcut [spacebar]
			Kage *win;
			//GdkPoint origin;
			GdkPoint draw1;
			GdkPoint draw2;
			double polyXhead, polyYhead;
			double polyXtail, polyYtail;
			Glib::RefPtr<Gdk::Window> window;
			bool mouseDown;
			GdkPoint _mouseLocation;
			unsigned int _mouseLocationShapeIndex;
			GdkPoint _copyBufferMouse;
			bool draw;
			unsigned int drawCtr;
			//Override default signal handler:
			virtual bool on_expose_event(GdkEventExpose *e);
			virtual bool on_key_press_event(GdkEventKey *e);
			virtual bool on_key_release_event(GdkEventKey *e);
			virtual bool on_event(GdkEvent *e);
			
			bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
			
			bool on_timeout();
			unsigned int mouseOnNode;
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
			
			GdkPoint selectionBox1;
			GdkPoint selectionBox2;
			bool isSelectionBoxNormalized();
			void normalizeSelectionBox();
			bool isNodeOnSelectionBox(double p_nodeX, double p_nodeY);
			
			void drawSelectionArea();
			
			double getSelectedShapeViaNode(unsigned int p_index, vector<VectorData> p_v);
			double _nodeToMouseDistance;
			
			AnchorData anchor_upperLeft;
			AnchorData anchor_center;
			AnchorData anchor_lowerRight;
			
			bool keyShiftDown;
			bool keyControlDown;
			bool keyUpDown;
			bool keyDownDown;
			bool keyLeftDown;
			bool keyRightDown;
			
			int _registerWidth;
			int _registerHeight;
	};
#endif // GTKMM_KAGE_STAGE_H

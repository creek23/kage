#ifndef GTKMM_KAGE_STAGE_H
	#define GTKMM_KAGE_STAGE_H
	
	#include <gtkmm/drawingarea.h>
	#include <iostream>
	
	#include "data/anchor.h"
	#include "data/color.h"
	#include "data/strokecolor.h"
	#include "data/vectordata.h"
	
	class Kage; //forward declaration
	
	class KageStage : public Gtk::DrawingArea {
		public:
			static Glib::RefPtr<Gdk::Pixbuf> imageSHAPE_000;
			static Glib::RefPtr<Gdk::Pixbuf> imageSHAPE_045;
			static Glib::RefPtr<Gdk::Pixbuf> imageSHAPE_090;
			static Glib::RefPtr<Gdk::Pixbuf> imageSHAPE_135;
			
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
			
			static ColorData stageBG; //direct use for get only
			static ColorData fillColor; //direct use for get only
			static StrokeColorData stroke; //direct use for get only
			static ToolMode toolMode;
			static GdkPoint moveStageXY;
			unsigned int wid;
			unsigned int hgt;
			unsigned int fps;
			
			void render();
			
			void setStageBG(Gdk::Color p_Color);
			Gdk::Color getStageBG();
			void setFill(Gdk::Color p_Color);
			Gdk::Color getFill();
			void setStroke(Gdk::Color p_Color);
			Gdk::Color getStroke();
			unsigned int stageWidth; //direct use for get only
			unsigned int stageHeight; //direct use for get only
			void clearScreen();
			void renderFrame();
			void initNodeTool();
			void handleShapes();
			void handleShapes_scaleNorth();
			void handleShapes_scaleEast();
			void handleShapes_scaleWest();
			void handleShapes_scaleSouth();
			void handleShapes_moveShape(double p_diffX, double p_diffY);
			void handleShapes_updateAnchors(double p_x, double p_y);
			void handleShapesMouseUp();
			void handleNodes();
			void handleNodesMouseUp();
			bool handleNodes_getNearestShape(double p_x, double p_y, unsigned int p_index, vector<VectorData> p_v);
			void handleDrawOvalMouseUp();
			void handleDrawRectMouseUp();
			void handleDrawPolyMouseUp();
			void handlePoly();
			void handleRect();
			void handleOval();
			void handleStrokeMouseUp();
			void handleFillMouseUp();
			void handleEyedrop();
			void handleEyedropMouseUp();
			Glib::RefPtr<Gdk::Pixbuf> _bg;
			Cairo::RefPtr<Cairo::Surface> _bgcr;
			
			bool deleteSelectedShape();
			bool deleteSelectedNode();
			
			void updateShapeColor(bool p_doFill = true, bool p_doStroke = true);
		protected:
			ToolMode prevTool; //used by Hand-tool shortcut [spacebar]
			Kage *win;
			GdkPoint origin;
			GdkPoint draw1;
			GdkPoint draw2;
			double polyX, polyY;
			double polyXprev, polyYprev;
			Glib::RefPtr<Gdk::Window> window;
			Cairo::RefPtr<Cairo::Context> cr;
			bool gotContext;
			bool mouseDown;
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
			AnchorData::type mouseOnAnchor;
			void renderNode(double p_x, double p_y, unsigned int p_state = 5);
			void renderNodeControl(double p_x, double p_y, bool p_selected);
			bool isMouseOnNode(double p_x, double p_y, unsigned int p_buffer = 5);
			
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
	};
#endif // GTKMM_KAGE_STAGE_H

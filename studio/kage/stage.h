#ifndef GTKMM_KAGE_STAGE_H
	#define GTKMM_KAGE_STAGE_H
	
	#include <gtkmm/drawingarea.h>
	#include <iostream>
	
	#include "data/color.h"
	#include "data/strokecolor.h"
	#include "data/vectordata.h"
	
	class Kage; //forward declaration
	
	class KageStage : public Gtk::DrawingArea {
		public:
			KageStage(Kage *p_win);
			virtual ~KageStage();
		
			enum ToolMode {
				MODE_NONE,
				MODE_SELECT,
				MODE_ANCHOR,
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
			void renderAnchors();
			Glib::RefPtr<Gdk::Pixbuf> _bg;
			Cairo::RefPtr<Cairo::Surface> _bgcr;
		protected:
			ToolMode prevTool; //used by Hand-tool shortcut [spacebar]
			Kage *win;
			GdkPoint origin;
			GdkPoint draw1;
			GdkPoint draw2;
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
			unsigned int mouseOnPoint;
			void renderAnchor(double p_x, double p_y, bool p_selected);
			bool isMouseOnAnchor(double p_x, double p_y);
	};
#endif // GTKMM_KAGE_STAGE_H

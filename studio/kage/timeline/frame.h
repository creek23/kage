#ifndef GTKMM_KAGE_FRAME_H
	#define GTKMM_KAGE_FRAME_H
	
	#include <gtkmm/drawingarea.h>
	#include <time.h>
	#include "../vectordatamanager.h"
	
	class KageFrameManager; ///forward declaration
	
	class KageFrame : public Gtk::DrawingArea {
		public:
			static bool mouseIsDown;
			static Glib::RefPtr<Gdk::Pixbuf> imageNULL;
			static Glib::RefPtr<Gdk::Pixbuf> imageNULL_CUR;
			static Glib::RefPtr<Gdk::Pixbuf> imageBLANK;
			static Glib::RefPtr<Gdk::Pixbuf> imageBLANK_CUR;
			static Glib::RefPtr<Gdk::Pixbuf> imageDRAWN;
			static Glib::RefPtr<Gdk::Pixbuf> imageDRAWN_CUR;
			static Glib::RefPtr<Gdk::Pixbuf> imageEXTENSION;
			static Glib::RefPtr<Gdk::Pixbuf> imageEXTENSION_CUR;
			static Glib::RefPtr<Gdk::Pixbuf> imageSELECTED;
			static Glib::RefPtr<Gdk::Pixbuf> imageSELECTED_CUR;
			
			Glib::RefPtr<Gdk::Window> window;
			
			KageFrame(KageFrameManager *p_fm, unsigned p_layerID, unsigned int p_frameID);
			virtual ~KageFrame();
			
			bool isEmpty();
			int data; //what's this for?
			void setSelected(bool p_selected);
			bool isSelected();
			void setCurrent(bool p_current);
			bool isCurrent();
			VectorDataManager vectorsData;
			bool setCode(bool p_newCode);
			bool getCode();
			unsigned int getFrameSource();
			void setFocus();
			
			unsigned int layerID;     //which layer does it reside?
			unsigned int frameID;     //which frame among frames in a layer?
//			bool setTween(bool p_newTween);
//			bool getTween();
			
			void forceRender();
			virtual bool render();
			
			static bool _gotFocus;
			
			vector<unsigned int> raiseSelectedShape(vector<unsigned int> p_selectedShapes);
			vector<unsigned int> lowerSelectedShape(vector<unsigned int> p_selectedShapes);
			vector<unsigned int> raiseToTopSelectedShape(vector<unsigned int> p_selectedShapes);
			vector<unsigned int> lowerToBottomSelectedShape(vector<unsigned int> p_selectedShapes);
			vector<unsigned int> groupSelectedShapes(vector<unsigned int> p_selectedShapes);
			vector<unsigned int> ungroupSelectedShapes(vector<unsigned int> p_selectedShapes);
			vector<unsigned int> duplicateShapes(vector<unsigned int> p_selectedShapes);
			bool flipHorizontalSelectedShape(vector<unsigned int> p_selectedShapes);
			bool flipVerticalSelectedShape(vector<unsigned int> p_selectedShapes);
		protected:
			//Override default signal handler:
			virtual bool on_expose_event(GdkEventExpose *e);
			virtual bool on_key_press_event(GdkEventKey *e);
			virtual bool on_event(GdkEvent *e);
			
			bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
			
			Glib::ustring sCode;
			unsigned int frameSource; //drawing is an extension of a `previous frame' -- frameSource is `which frame-number'?
			bool bTween;
			bool selected;
			bool current;
			
			KageFrameManager *fm;
	};
#endif // GTKMM_KAGE_FRAME_H

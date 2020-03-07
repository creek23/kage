#ifndef GTKMM_KAGE_FRAME_PADDING_H
	#define GTKMM_KAGE_FRAME_PADDING_H
	
	#include <gtkmm/drawingarea.h>
	#include <time.h>
	
	class KageFramePadding : public Gtk::DrawingArea {
		public:
			KageFramePadding();
			virtual ~KageFramePadding();
	};
#endif // GTKMM_KAGE_FRAME_PADDING_H

#ifndef GTKMM_KAGE_MANAGER_LAYER_H
	#define GTKMM_KAGE_MANAGER_LAYER_H
	
	#include <gtkmm/box.h>
	#include <gtkmm/label.h>
	#include <gtkmm/button.h>
//	#include "framemanager.h"
	
	class KageLayerManager : public Gtk::VBox {
		public:
			KageLayerManager();
			virtual ~KageLayerManager();
			unsigned layerCount();
			void addLayer(Glib::ustring p_name = "");
		protected:
			Gtk::Label label;
			unsigned int layerCtr;
			std::vector<Gtk::Button*> labels;
//			std::vector<KageLayer*> layers;
	};
#endif // GTKMM_KAGE_MANAGER_LAYER_H

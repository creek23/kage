
#ifndef GTKMM_KAGE_LAYER_RENAME_H
	#define GTKMM_KAGE_LAYER_RENAME_H
	
	#include <gtkmm/window.h>
	#include <gtkmm/dialog.h>
	#include <gtkmm/box.h>
	#include <gtkmm/label.h>
	#include <gtkmm/entry.h>
//	#include <gtkmm/stock.h>
	#include "kage/timeline/layer.h"
	#include <iostream>
	
	using namespace std;

	class LayerRenameDialog : public Gtk::Dialog {
		public:
			LayerRenameDialog(Gtk::Window& parent, KageLayer *p_layer);
			virtual ~LayerRenameDialog();
			
			void EntryLayerLabel_onEnter();
		protected:
			//Member widgets:
			Gtk::HBox m_HBox;
			Gtk::Label _label;
			Gtk::Entry _txtLabel;
			
			KageLayer *_layer;
	};	
#endif //GTKMM_KAGE_LAYER_RENAME_H

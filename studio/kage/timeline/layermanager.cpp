
#include "layermanager.h"
//#include "framemanager.cpp"

KageLayerManager::KageLayerManager() {
	pack_start(label, Gtk::PACK_SHRINK);
		label.set_label(" ");
		label.set_size_request(100, 20);

	layerCtr = 0;
	for (int i = 0; i < 5; ++i) {
//		addLayer();
	}
	addLayer();
}

KageLayerManager::~KageLayerManager() {
	//
}

void KageLayerManager::addLayer(Glib::ustring p_name) {
	layerCtr++;
	if (p_name == "") {
		//p_name = "Layer ";// + layerCtr;
		p_name = Glib::ustring::compose("Layer %1", layerCtr) ;//"is lower than 0x%2.", ustring::format(std::hex, 16));
	}
//	layers.push_back(Gtk::manage(new KageLayer(layerCtr, "")));
	labels.push_back(Gtk::manage(new Gtk::Button(p_name)));
		pack_end(*labels.back(), Gtk::PACK_SHRINK);
//			(*labels.back()).set_label(p_name);
			(*labels.back()).set_size_request(100, 20);
			(*labels.back()).set_focus_on_click(false);
//			(*labels.back()).signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &Kage::onToolButtonsClick), toggleButtons.back()));
}

unsigned int KageLayerManager::layerCount() {
	return layerCtr;
}


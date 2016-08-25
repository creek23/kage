
#include <gtkmm/main.h>
//#include <gtkmm/window.h>

#include "kage.cpp"

Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageNULL;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageNULL_CUR;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageBLANK;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageBLANK_CUR;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageDRAWN;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageDRAWN_CUR;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageEXTENSION;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageEXTENSION_CUR;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageSELECTED;
Glib::RefPtr<Gdk::Pixbuf> KageFrame::imageSELECTED_CUR;
unsigned int KageFramesManager::currentFrame;
unsigned int KageFramesManager::currentLayer;
unsigned int VectorDataManager::idmaker;

KageStage::ToolMode KageStage::toolMode;
Gdk::Point KageStage::moveStageXY;
//PointData KageStage::moveStageXY;

int main(int argc, char *argv[]) {
	Gtk::Main kit(argc, argv);
	
	Kage kage;
	
	Gtk::Main::run(kage);
	
	return EXIT_SUCCESS;
}


#include <gtkmm/main.h>

#include "kage.h"

int main(int argc, char *argv[]) {
	auto app = Gtk::Application::create(argc, argv, "com.krixware.kage");
	
	Kage kage;
	
	return app->run(kage); //EXIT_SUCCESS;
}

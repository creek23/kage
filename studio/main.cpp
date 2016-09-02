
#include <gtkmm/main.h>

#include "kage.h"

int main(int argc, char *argv[]) {
	Gtk::Main kit(argc, argv);
	
	Kage kage;
	
	Gtk::Main::run(kage);
	
	return EXIT_SUCCESS;
}

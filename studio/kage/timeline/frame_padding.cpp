
#include <cairomm/context.h>
#include <cairomm/cairomm.h>
#include <gdkmm/general.h> // set_source_pixbuf()
#include <iostream>

#include "frame_padding.h"
#include "framesmanager.h"
#include "../../kage.h"

#include <giomm/resource.h>

KageFramePadding::KageFramePadding() {
	set_state_flags(Gtk::STATE_FLAG_NORMAL);
	set_size_request(8, 20);
}

KageFramePadding::~KageFramePadding() {
	//
}

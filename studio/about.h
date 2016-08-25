
#ifndef GTKMM_KAGE_ABOUT_H
	#define GTKMM_KAGE_ABOUT_H
	
	#include <gtkmm/window.h>
	#include <gtkmm/dialog.h>
	#include <gtkmm/box.h>
	#include <gtkmm/label.h>
	#include <gtkmm/entry.h>
	#include <gtkmm/image.h>
	#include <gtkmm/table.h>

	class AboutDialog : public Gtk::Dialog {
		public:
			AboutDialog(Gtk::Window& parent);
			virtual ~AboutDialog();
		
		protected:
			//Member widgets:
			Gtk::HBox m_HBox;
			Gtk::Table m_Table;
			Gtk::Label m_Label1, m_Label2;
			Gtk::Image m_Image;
	};	
#endif //GTKMM_KAGE_ABOUT_H
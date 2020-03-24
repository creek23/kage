
#ifndef GTKMM_KAGE_ABOUT_H
	#define GTKMM_KAGE_ABOUT_H
	
	#include <gtkmm/window.h>
	#include <gtkmm/dialog.h>
	#include <gtkmm/box.h>
	#include <gtkmm/label.h>
	#include <gtkmm/stock.h>
	#include <gtkmm/image.h>
	#include <gtkmm/table.h>
	
	using namespace std;
	
	class KageAbout : public Gtk::Dialog {
		public:
			KageAbout(Gtk::Window& parent);
			virtual ~KageAbout();
			static string app_version;
			static string app_title;
			static string app_author;
			static string app_contact;
			static string app_website;
			static string app_copyright;
			static string app_comment;
		
		protected:
			//Member widgets:
			Gtk::HBox m_HBox;
			Gtk::Table m_Table;
			Gtk::Label m_Label1, m_Label2;
			Gtk::Image m_Image;
	};	
#endif //GTKMM_KAGE_ABOUT_H

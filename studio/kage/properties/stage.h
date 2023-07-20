/* 
 * Kage Studio - a simple free and open source vector-based 2D animation software
 * Copyright (C) 2011~2023  Mj Mendoza IV
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.  Or, see <https://www.gnu.org/licenses/>.
 * 
 */

#ifndef GTKMM_KAGE_PROP_STAGE_H
	#define GTKMM_KAGE_PROP_STAGE_H
	
	#include <gtkmm/box.h>
	#include <gtkmm/label.h>
	#include <gtkmm/entry.h>
	#include <gtkmm/colorbutton.h>
	#include <gtkmm/colorselection.h>
	
	class Kage; //forward declaration

	using namespace std;
	
	class PropertyStage : public Gtk::HBox {
		public:
			PropertyStage(Kage* p_kage);
			virtual ~PropertyStage();
		
		protected:
			Kage* _kage;
			//Member widgets:
			Gtk::VBox m_PropStageV1;
			Gtk::VBox m_PropStageV2;
			Gtk::Label m_LabelStageWid;
			Gtk::Label m_LabelStageHgt;
			Gtk::Label m_LabelStageBG;
			Gtk::Label m_LabelStageFPS;
			
			Gtk::Entry m_EntryStageWid;
			Gtk::Entry m_EntryStageHgt;
			Gtk::ColorButton m_ColorButtonStage;
//				Gtk::ColorChooser m_ColorChooser;
			Gtk::ColorSelection m_ColorSelectionStage;
			Gtk::Entry m_EntryStageFPS;
		public:
				void EntryStageArea_onEnter();
				void ColorButtonStage_onClick();
				void EntryStageFPS_onEnter();
				void setWidthText(double p_width);
				void setHeightText(double p_height);
				void setFPSText(unsigned int p_fps);
				void setBackgroundColor(Gdk::Color p_color);
	};
#endif //GTKMM_KAGE_PROP_STAGE_H

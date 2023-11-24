/* 
 * Kage Studio - a simple free and open source vector-based 2D animation software
 * Copyright (C) 2023  Mj Mendoza IV
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

#ifndef GTKMM_KAGE_PROP_ASSET_H
	#define GTKMM_KAGE_PROP_ASSET_H
	
	#include <gtkmm/box.h>
	#include <gtkmm/label.h>
	#include <gtkmm/entry.h>
	#include <gtkmm/colorbutton.h>
	//#include <gtkmm/colorselection.h>
	#include <gtkmm/scale.h>
	
	class Kage; //forward declaration

	using namespace std;
	
	class PropertyAsset : public Gtk::HBox {
		public:
			PropertyAsset(Kage* p_kage);
			virtual ~PropertyAsset();
		
		protected:
			Kage* _kage;
			//Member widgets:
				Gtk::VBox m_propAssetV1;
				Gtk::VBox m_propAssetV2;
				Gtk::Label m_LabelX;
				Gtk::Entry m_EntryX;
				Gtk::Label m_LabelY;
				Gtk::Entry m_EntryY;
				Gtk::Label m_LabelWidth;
				Gtk::Entry m_EntryWidth;
				Gtk::Label m_LabelHeight;
				Gtk::Entry m_EntryHeight;
				Gtk::Label m_LabelRotation;
				Gtk::Entry m_EntryRotation;
				Gtk::Label m_LabelAlpha;
				Glib::RefPtr<Gtk::Adjustment> _adjustAlpha;
				Gtk::Scale _scaleAlpha;
		public:
				void EntryX_onEnter();
				void EntryY_onEnter();
				void EntryWidth_onEnter();
				void EntryHeight_onEnter();
				void EntryRotation_onEnter();
				void Alpha_onChange();
				
				void setXText(double p_x);
				void setYText(double p_y);
				void setWidthText(double p_width);
				void setHeightText(double p_height);
				void setRotationText(double p_rotation);
				void setScaleAlpha(double p_alpha);
				
				unsigned int getScaleAlpha();
	};
#endif //GTKMM_KAGE_PROP_ASSET_H

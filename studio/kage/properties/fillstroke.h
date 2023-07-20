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

#ifndef GTKMM_KAGE_PROP_FILL_STROKE_H
	#define GTKMM_KAGE_PROP_FILL_STROKE_H
	
	#include <gtkmm/box.h>
	#include <gtkmm/label.h>
	#include <gtkmm/entry.h>
	#include <gtkmm/colorbutton.h>
	#include <gtkmm/colorselection.h>
	#include <gtkmm/scale.h>
	#include "../data/color.h"
	
	class Kage; //forward declaration

	using namespace std;
	
	class PropertyFillStroke : public Gtk::VBox {
		public:
			PropertyFillStroke(Kage* p_kage);
			virtual ~PropertyFillStroke();
		
		protected:
			Kage* _kage;
			//Fill Property items
				Gtk::HBox m_PropFill;
				Gtk::VBox m_PropFillV1;
				Gtk::VBox m_PropFillV2;
				
				Gtk::Button m_ButtonFill;
				Gtk::ColorButton m_ColorButtonFill;
				Gtk::Label m_LabelFillR;
				Gtk::Label m_LabelFillG;
				Gtk::Label m_LabelFillB;
				Gtk::Label m_LabelFillAlpha;
				Glib::RefPtr<Gtk::Adjustment> _adjustFillR;
				Glib::RefPtr<Gtk::Adjustment> _adjustFillG;
				Glib::RefPtr<Gtk::Adjustment> _adjustFillB;
				Glib::RefPtr<Gtk::Adjustment> _adjustFillA;
				
				Gtk::Scale _scaleFillR;
				Gtk::Scale _scaleFillG;
				Gtk::Scale _scaleFillB;
				Gtk::Scale _scaleFillAlpha;
				Gtk::Label m_LabelFillRGBA;	
				Gtk::Entry m_EntryFillRGBA;
			//Stroke Property items
				Gtk::HBox m_PropStroke;
				Gtk::VBox m_PropStrokeV1;
				Gtk::VBox m_PropStrokeV2;
				
				Glib::RefPtr<Gtk::Adjustment> _adjustStrokeR;
				Glib::RefPtr<Gtk::Adjustment> _adjustStrokeG;
				Glib::RefPtr<Gtk::Adjustment> _adjustStrokeB;
				Glib::RefPtr<Gtk::Adjustment> _adjustStrokeA;
				
				Gtk::Button m_ButtonStroke;
				Gtk::Label m_LabelStrokeThickness;
				Gtk::Entry m_EntryStrokeThickness;
				Gtk::Label m_LabelStrokeR;
				Gtk::Label m_LabelStrokeG;
				Gtk::Label m_LabelStrokeB;
				Gtk::Label m_LabelStrokeAlpha;
				Gtk::Scale _scaleStrokeR;
				Gtk::Scale _scaleStrokeG;
				Gtk::Scale _scaleStrokeB;
				Gtk::Scale _scaleStrokeAlpha;
				Gtk::Label m_LabelStrokeRGBA;	
				Gtk::Entry m_EntryStrokeRGBA;
				Gtk::ColorButton m_ColorButtonStroke;
			//Toggle for Fill/Stroke Property items
				Gtk::HBox m_PropFillLabel;
				Gtk::VBox m_PropFillLabelV1;
				Gtk::VBox m_PropFillLabelV2;

				Gtk::HBox m_PropStrokeLabel;
				Gtk::VBox m_PropStrokeLabelV1;
				Gtk::VBox m_PropStrokeLabelV2;
		public:
					void ColorButtonFill_onClick();
					void FillR_onChange();
					void FillG_onChange();
					void FillB_onChange();
					void FillAlpha_onChange();
					void EntryFillRGBA_onEnter();
				void updateEntryFillRGBA();
					void EntryStrokeThickness_onEnter();
					void StrokeR_onChange();
					void StrokeG_onChange();
					void StrokeB_onChange();
					void StrokeAlpha_onChange();
					void EntryStrokeRGBA_onEnter();
				void updateEntryStrokeRGBA();
					void ColorButtonStroke_onClick();
				
				void TogglePropertiesFill_onClick();
				void TogglePropertiesStroke_onClick();

				void setFillColorData(ColorData p_colordata);
				void setStrokeColorData(ColorData p_colordata);
				void setFillButtonColor(Gdk::Color p_color);
				void setStrokeButtonColor(Gdk::Color p_color);
				void setScaleFillRGBA(ColorData p_colordata);
				void setScaleStrokeRGBA(ColorData p_colordata);
				void setStrokeThicknessText(double p_thickness);
				string getStrokeThicknessText();
				Gdk::Color getFillColor();
				Gdk::Color getStrokeColor();
				
				unsigned int getFillScaleR();
				unsigned int getFillScaleG();
				unsigned int getFillScaleB();
				unsigned int getFillScaleAlpha();
				unsigned int getStrokeScaleR();
				unsigned int getStrokeScaleG();
				unsigned int getStrokeScaleB();
				unsigned int getStrokeScaleAlpha();
	};
#endif //GTKMM_KAGE_PROP_FILL_STROKE_H

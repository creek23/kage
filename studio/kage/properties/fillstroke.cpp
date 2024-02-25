/*
 * Kage Studio - a simple free and open source vector-based 2D animation software
 * Copyright (C) 2011~2024  Mj Mendoza IV
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

#include "fillstroke.h"
#include "../../kage.h"

PropertyFillStroke::PropertyFillStroke(Kage* p_kage) :
    m_ButtonFill("Fill"),
    m_LabelFillR("Red"),
    m_LabelFillG("Green"),
    m_LabelFillB("Blue"),
    m_LabelFillAlpha("Alpha"),
    m_LabelFillRGBA("RGBA"),
    m_ButtonStroke("Stroke"),
    m_LabelStrokeR("Red"),
    m_LabelStrokeG("Green"),
    m_LabelStrokeB("Blue"),
    m_LabelStrokeAlpha("Alpha"),
    m_LabelStrokeRGBA("RGBA"),
    m_LabelStrokeThickness("Thickness"),
    _adjustFillR( Gtk::Adjustment::create(0.0, 0.0, 256.0, 0.1, 1.0, 1.0) ),
    _adjustFillG( Gtk::Adjustment::create(0.0, 0.0, 256.0, 0.1, 1.0, 1.0) ),
    _adjustFillB( Gtk::Adjustment::create(0.0, 0.0, 256.0, 0.1, 1.0, 1.0) ),
    _adjustFillA( Gtk::Adjustment::create(0.0, 0.0, 101.0, 0.1, 1.0, 1.0) ),
    _scaleFillR(_adjustFillR, Gtk::ORIENTATION_HORIZONTAL),
    _scaleFillG(_adjustFillG, Gtk::ORIENTATION_HORIZONTAL),
    _scaleFillB(_adjustFillB, Gtk::ORIENTATION_HORIZONTAL),
    _scaleFillAlpha(_adjustFillA, Gtk::ORIENTATION_HORIZONTAL),
    _adjustStrokeR( Gtk::Adjustment::create(0.0, 0.0, 256.0, 0.1, 1.0, 1.0) ),
    _adjustStrokeG( Gtk::Adjustment::create(0.0, 0.0, 256.0, 0.1, 1.0, 1.0) ),
    _adjustStrokeB( Gtk::Adjustment::create(0.0, 0.0, 256.0, 0.1, 1.0, 1.0) ),
    _adjustStrokeA( Gtk::Adjustment::create(0.0, 0.0, 101.0, 0.1, 1.0, 1.0) ),
    _scaleStrokeR(_adjustStrokeR, Gtk::ORIENTATION_HORIZONTAL),
    _scaleStrokeG(_adjustStrokeG, Gtk::ORIENTATION_HORIZONTAL),
    _scaleStrokeB(_adjustStrokeB, Gtk::ORIENTATION_HORIZONTAL),
    _scaleStrokeAlpha(_adjustStrokeA, Gtk::ORIENTATION_HORIZONTAL), Gtk::VBox() {
    _kage = p_kage;
    //Toggle for Fill Properties
    //	m_Timeline_Frame_ScrolledWindow.set_border_width(0);
    //	m_Timeline_Frame_ScrolledWindow.set_shadow_type(Gtk::SHADOW_NONE);
    //=======================================================
    pack_start(m_PropFillLabel, Gtk::PACK_SHRINK);
        m_PropFillLabel.set_border_width(0);
        m_PropFillLabel.pack_start(m_PropFillLabelV1);
            m_PropFillLabelV1.set_border_width(4);
            m_PropFillLabelV1.set_spacing(4);
            m_PropFillLabelV1.pack_start(m_ButtonFill);
                m_ButtonFill.signal_clicked().connect( sigc::mem_fun(*this, &PropertyFillStroke::TogglePropertiesFill_onClick) );
        m_PropFillLabel.pack_start(m_PropFillLabelV2);
            m_PropFillLabelV2.set_border_width(4);
            m_PropFillLabelV2.set_spacing(4);
            m_PropFillLabelV2.pack_start(m_ColorButtonFill);
                m_ColorButtonFill.set_size_request(32, 32);
                m_ColorButtonFill.set_use_alpha(true);
                m_ColorButtonFill.set_title("Choose Fill Color");
                    m_ColorButtonFill.set_color(_kage->getStageBG());
                m_ColorButtonFill.signal_color_set().connect(sigc::mem_fun(* this, &PropertyFillStroke::ColorButtonFill_onClick) );
    //=======================================================	
    pack_start(m_PropFill, Gtk::PACK_SHRINK);
        m_PropFill.set_border_width(10);
        m_PropFill.pack_start(m_PropFillV1);
            m_PropFillV1.set_border_width(4);
            m_PropFillV1.set_spacing(4);
            //m_PropFillV1.pack_start(m_LabelFill);
            m_PropFillV1.pack_start(m_LabelFillR);
            m_PropFillV1.pack_start(m_LabelFillG);
            m_PropFillV1.pack_start(m_LabelFillB);
            m_PropFillV1.pack_start(m_LabelFillAlpha);
            m_PropFillV1.pack_start(m_LabelFillRGBA);
        m_PropFill.pack_start(m_PropFillV2);
            m_PropFillV2.set_border_width(4);
            m_PropFillV2.set_spacing(4);
            //m_PropFillV2.pack_start(m_ColorButtonFill);
            m_PropFillV2.pack_start(_scaleFillR, Gtk::PACK_SHRINK);
                _scaleFillR.set_digits(0);
                _scaleFillR.set_value(255);
                _scaleFillR.set_value_pos(Gtk::POS_TOP);
                _scaleFillR.set_draw_value();
                _scaleFillR.signal_value_changed().connect(
                    sigc::mem_fun(*this, &PropertyFillStroke::FillR_onChange));
            m_PropFillV2.pack_start(_scaleFillG, Gtk::PACK_SHRINK);
                _scaleFillG.set_digits(0);
                _scaleFillG.set_value(255);
                _scaleFillG.set_value_pos(Gtk::POS_TOP);
                _scaleFillG.set_draw_value();
                _scaleFillG.signal_value_changed().connect(
                    sigc::mem_fun(*this, &PropertyFillStroke::FillG_onChange));
            m_PropFillV2.pack_start(_scaleFillB, Gtk::PACK_SHRINK);
                _scaleFillB.set_digits(0);
                _scaleFillB.set_value(255);
                _scaleFillB.set_value_pos(Gtk::POS_TOP);
                _scaleFillB.set_draw_value();
                _scaleFillB.signal_value_changed().connect(
                    sigc::mem_fun(*this, &PropertyFillStroke::FillB_onChange));
            m_PropFillV2.pack_start(_scaleFillAlpha, Gtk::PACK_SHRINK);
                _scaleFillAlpha.set_digits(0);
                _scaleFillAlpha.set_value(100);
                _scaleFillAlpha.set_value_pos(Gtk::POS_TOP);
                _scaleFillAlpha.set_draw_value();
                _scaleFillAlpha.signal_value_changed().connect(
                    sigc::mem_fun(*this, &PropertyFillStroke::FillAlpha_onChange));
            m_PropFillV2.pack_start(m_EntryFillRGBA, Gtk::PACK_SHRINK);
                m_EntryFillRGBA.set_size_request(20, 24);
                m_EntryFillRGBA.set_width_chars(8);
                m_EntryFillRGBA.set_max_length(8);
                //m_EntryFillRGBA.set_text('random hex as color'));
                m_EntryFillRGBA.signal_activate().connect(
                    sigc::mem_fun(*this, &PropertyFillStroke::EntryFillRGBA_onEnter));
    //=======================================================
    pack_start(m_PropStrokeLabel, Gtk::PACK_SHRINK);
        m_PropStrokeLabel.set_border_width(0);
        m_PropStrokeLabel.pack_start(m_PropStrokeLabelV1);
            m_PropStrokeLabelV1.set_border_width(4);
            m_PropStrokeLabelV1.set_spacing(4);
            m_PropStrokeLabelV1.pack_start(m_ButtonStroke);
                m_ButtonStroke.signal_clicked().connect( sigc::mem_fun(*this, &PropertyFillStroke::TogglePropertiesStroke_onClick) );
        m_PropStrokeLabel.pack_start(m_PropStrokeLabelV2);
            m_PropStrokeLabelV2.set_border_width(4);
            m_PropStrokeLabelV2.set_spacing(4);
            m_PropStrokeLabelV2.pack_start(m_ColorButtonStroke);
                m_ColorButtonStroke.set_size_request(32, 32);
                m_ColorButtonStroke.set_use_alpha(true);
                m_ColorButtonStroke.set_title("Choose Stroke Color");
                    m_ColorButtonStroke.set_color(_kage->getStageBG());
                m_ColorButtonStroke.signal_color_set().connect(sigc::mem_fun(* this, &PropertyFillStroke::ColorButtonStroke_onClick) );
    //=======================================================
    pack_start(m_PropStroke, Gtk::PACK_SHRINK);
        m_PropStroke.set_border_width(10);
        m_PropStroke.pack_start(m_PropStrokeV1);
            m_PropStrokeV1.pack_start(m_LabelStrokeR);
            m_PropStrokeV1.pack_start(m_LabelStrokeG);
            m_PropStrokeV1.pack_start(m_LabelStrokeB);
            m_PropStrokeV1.pack_start(m_LabelStrokeAlpha);
            m_PropStrokeV1.pack_start(m_LabelStrokeRGBA);
            m_PropStrokeV1.pack_start(m_LabelStrokeThickness);
        m_PropStroke.pack_start(m_PropStrokeV2);
            m_PropStrokeV2.set_border_width(4);
            m_PropStrokeV2.set_spacing(4);
            m_PropStrokeV2.pack_start(_scaleStrokeR, Gtk::PACK_SHRINK);
                _scaleStrokeR.set_digits(0);
                _scaleStrokeR.set_value(255);
                _scaleStrokeR.set_value_pos(Gtk::POS_TOP);
                _scaleStrokeR.set_draw_value();
                _scaleStrokeR.signal_value_changed().connect(
                    sigc::mem_fun(*this, &PropertyFillStroke::StrokeR_onChange));
            m_PropStrokeV2.pack_start(_scaleStrokeG, Gtk::PACK_SHRINK);
                _scaleStrokeG.set_digits(0);
                _scaleStrokeG.set_value(255);
                _scaleStrokeG.set_value_pos(Gtk::POS_TOP);
                _scaleStrokeG.set_draw_value();
                _scaleStrokeG.signal_value_changed().connect(
                    sigc::mem_fun(*this, &PropertyFillStroke::StrokeG_onChange));
            m_PropStrokeV2.pack_start(_scaleStrokeB, Gtk::PACK_SHRINK);
                _scaleStrokeB.set_digits(0);
                _scaleStrokeB.set_value(255);
                _scaleStrokeB.set_value_pos(Gtk::POS_TOP);
                _scaleStrokeB.set_draw_value();
                _scaleStrokeB.signal_value_changed().connect(
                    sigc::mem_fun(*this, &PropertyFillStroke::StrokeB_onChange));
            m_PropStrokeV2.pack_start(_scaleStrokeAlpha, Gtk::PACK_SHRINK);
                _scaleStrokeAlpha.set_digits(0);
                _scaleStrokeAlpha.set_value(100);
                _scaleStrokeAlpha.set_value_pos(Gtk::POS_TOP);
                _scaleStrokeAlpha.set_draw_value();
                _scaleStrokeAlpha.signal_value_changed().connect(
                    sigc::mem_fun(*this, &PropertyFillStroke::StrokeAlpha_onChange));
            m_PropStrokeV2.pack_start(m_EntryStrokeRGBA, Gtk::PACK_SHRINK);
                m_EntryStrokeRGBA.set_size_request(20, 24);
                m_EntryStrokeRGBA.set_width_chars(8);
                m_EntryStrokeRGBA.set_max_length(8);
                //m_EntryStrokeRGBA.set_text('random hex as color'));
                m_EntryStrokeRGBA.signal_activate().connect(
                    sigc::mem_fun(*this, &PropertyFillStroke::EntryStrokeRGBA_onEnter));
            m_PropStrokeV2.pack_start(m_EntryStrokeThickness, Gtk::PACK_SHRINK);
                m_EntryStrokeThickness.set_size_request(20, 24);
                m_EntryStrokeThickness.set_width_chars(6); //allows XXX.XX or XXXX.X
                m_EntryStrokeThickness.set_max_length(6);
                m_EntryStrokeThickness.set_text(StringHelper::doubleToString((int)KageStage::stroke.getThickness()));
                m_EntryStrokeThickness.signal_activate().connect(
                    sigc::mem_fun(*this, &PropertyFillStroke::EntryStrokeThickness_onEnter));
}

PropertyFillStroke::~PropertyFillStroke() {
	//
}


void PropertyFillStroke::TogglePropertiesFill_onClick() {
	m_PropFill.set_visible(!m_PropFill.get_visible());
	_kage->_propFillVisible = m_PropFill.get_visible();
}
void PropertyFillStroke::TogglePropertiesStroke_onClick() {
	m_PropStroke.set_visible(!m_PropStroke.get_visible());
	_kage->_propStrokeVisible = m_PropStroke.get_visible();
}

void PropertyFillStroke::ColorButtonFill_onClick() {
	_kage->m_Color = m_ColorButtonFill.get_color(); //<-- deprecate this
	Gdk::RGBA l_rgba = m_ColorButtonFill.get_rgba();
	double l_red = l_rgba.get_red();
	double l_green = l_rgba.get_green();
	double l_blue = l_rgba.get_blue();
	double l_alpha = l_rgba.get_alpha();
	
	_scaleFillR.set_value(l_red * 255.0f);
	_scaleFillG.set_value(l_green * 255.0f);
	_scaleFillB.set_value(l_blue * 255.0f);
	_scaleFillAlpha.set_value(l_alpha * 255.0f);
	if (KageStage::toolMode == KageStage::ToolMode::MODE_DRAW_PENCIL) {
		KageStage::pencilFillColor.setR((int) (l_red * 255.0f));
		KageStage::pencilFillColor.setG((int) (l_green * 255.0f));
		KageStage::pencilFillColor.setB((int) (l_blue * 255.0f));
		KageStage::pencilFillColor.setA((int) (l_alpha * 255.0f));
	} else {
		KageStage::fillColor.setR((int) (l_red * 255.0f));
		KageStage::fillColor.setG((int) (l_green * 255.0f));
		KageStage::fillColor.setB((int) (l_blue * 255.0f));
		KageStage::fillColor.setA((int) (l_alpha * 255.0f));
	}
	_kage->_stage.setFill(_kage->m_Color);
	_kage->_stage.updateShapeColor(_kage->_UPDATE_SHAPE_COLORS, _kage->_UPDATE_SHAPE_COLORS);
}

void PropertyFillStroke::ColorButtonStroke_onClick() {
	_kage->m_Color = m_ColorButtonStroke.get_color(); //<-- deprecate this
	Gdk::RGBA l_rgba = m_ColorButtonStroke.get_rgba();
	double l_alpha = l_rgba.get_alpha();
	
	_scaleStrokeAlpha.set_value(l_alpha * 100.0f);
	if (KageStage::toolMode == KageStage::ToolMode::MODE_DRAW_PENCIL) {
		KageStage::pencilStroke.setA((int) (l_alpha * 255.0f));
	} else {
		KageStage::stroke.setA((int) (l_alpha * 255.0f));
	}
	_kage->_stage.setStroke(_kage->m_Color);
	_kage->_stage.updateShapeColor(_kage->_UPDATE_SHAPE_COLORS, _kage->_UPDATE_SHAPE_COLORS);
}


void PropertyFillStroke::updateEntryFillRGBA() {
	unsigned int l_R = 0;
	unsigned int l_G = 0;
	unsigned int l_B = 0;
	unsigned int l_A = 0;
	if (KageStage::toolMode == KageStage::ToolMode::MODE_DRAW_PENCIL) {
		l_R = KageStage::pencilFillColor.getR();
		l_G = KageStage::pencilFillColor.getG();
		l_B = KageStage::pencilFillColor.getB();
		l_A = KageStage::pencilFillColor.getA();
	} else {
		l_R = KageStage::fillColor.getR();
		l_G = KageStage::fillColor.getG();
		l_B = KageStage::fillColor.getB();
		l_A = KageStage::fillColor.getA();
	}
	m_EntryFillRGBA.set_text(_kage->int255ToHex(l_R) + _kage->int255ToHex(l_G) + _kage->int255ToHex(l_B) + _kage->int255ToHex(l_A));
}
void PropertyFillStroke::FillR_onChange() {
	double l_red = _scaleFillR.get_value();
	Gdk::RGBA l_rgba = m_ColorButtonFill.get_rgba();
		l_rgba.set_red( l_red / 255.0f );
	m_ColorButtonFill.set_rgba(l_rgba);
	
	if (KageStage::toolMode == KageStage::ToolMode::MODE_DRAW_PENCIL) {
		KageStage::pencilFillColor.setR(l_red);
	} else {
		KageStage::fillColor.setR(l_red);
	}
		updateEntryFillRGBA();
	_kage->_stage.updateShapeColor(_kage->_UPDATE_SHAPE_COLORS, _kage->_UPDATE_SHAPE_COLORS);
}
void PropertyFillStroke::FillG_onChange() {
	double l_green = _scaleFillG.get_value();
	Gdk::RGBA l_rgba = m_ColorButtonFill.get_rgba();
		l_rgba.set_green( l_green / 255.0f );
	m_ColorButtonFill.set_rgba(l_rgba);
	
	if (KageStage::toolMode == KageStage::ToolMode::MODE_DRAW_PENCIL) {
		KageStage::pencilFillColor.setG(l_green);
	} else {
		KageStage::fillColor.setG(l_green);
	}
		updateEntryFillRGBA();
	_kage->_stage.updateShapeColor(_kage->_UPDATE_SHAPE_COLORS, _kage->_UPDATE_SHAPE_COLORS);
}
void PropertyFillStroke::FillB_onChange() {
	double l_blue = _scaleFillB.get_value();
	Gdk::RGBA l_rgba = m_ColorButtonFill.get_rgba();
		l_rgba.set_blue( l_blue / 255.0f );
	m_ColorButtonFill.set_rgba(l_rgba);
	
	if (KageStage::toolMode == KageStage::ToolMode::MODE_DRAW_PENCIL) {
		KageStage::pencilFillColor.setB(l_blue);
	} else {
		KageStage::fillColor.setB(l_blue);
	}
		updateEntryFillRGBA();
	_kage->_stage.updateShapeColor(_kage->_UPDATE_SHAPE_COLORS, _kage->_UPDATE_SHAPE_COLORS);
}
void PropertyFillStroke::FillAlpha_onChange() {
	double l_alpha = _scaleFillAlpha.get_value();
	Gdk::RGBA l_rgba = m_ColorButtonFill.get_rgba();
		l_rgba.set_alpha( l_alpha / 100.f );
	m_ColorButtonFill.set_rgba(l_rgba);
	
	if (KageStage::toolMode == KageStage::ToolMode::MODE_DRAW_PENCIL) {
		KageStage::pencilFillColor.setA((int) (l_alpha * 256.0f/ 100.0f));
	} else {
		KageStage::fillColor.setA((int) (l_alpha * 256.0f/ 100.0f));
	}
		updateEntryFillRGBA();
	_kage->_stage.updateShapeColor(_kage->_UPDATE_SHAPE_COLORS, _kage->_UPDATE_SHAPE_COLORS);
}

void PropertyFillStroke::EntryFillRGBA_onEnter() {
	//see: https://sourceforge.net/p/kage/tickets/66/?limit=25#7488
	std::string l_input = m_EntryFillRGBA.get_text();
	unsigned int l_inputLength = l_input.length();
	unsigned int l_R = 0;
	unsigned int l_G = 0;
	unsigned int l_B = 0;
	unsigned int l_A = 0;
	if (KageStage::toolMode == KageStage::ToolMode::MODE_DRAW_PENCIL) {
		l_A = KageStage::pencilFillColor.getA();
	} else {
		l_A = KageStage::fillColor.getA();
	}
	std::string l_sR = "";
	std::string l_sG = "";
	std::string l_sB = "";
	std::string l_sA = "";
	
	if (l_inputLength == 1) {
		l_sR = l_input + l_input;
		l_sG = l_input + l_input;
		l_sB = l_input + l_input;
	} else if (l_inputLength == 2) {
		l_sR = l_input;
		l_sG = l_input;
		l_sB = l_input;
	} else if (l_inputLength == 3) {
		l_sR = l_input.substr(0,1) + l_input.substr(0,1); //get R
		l_sG = l_input.substr(1,1) + l_input.substr(1,1); //get G
		l_sB = l_input.substr(2,1) + l_input.substr(2,1); //get B
	} else if (l_inputLength == 4) {
		l_sR = l_input.substr(0,1) + l_input.substr(0,1); //get R
		l_sG = l_input.substr(1,1) + l_input.substr(1,1); //get G
		l_sB = l_input.substr(2,1) + l_input.substr(2,1); //get B
		l_sA = l_input.substr(3,1) + l_input.substr(3,1); //get A
		l_A = _kage->hexToInt255(l_sA);
	} else if (l_inputLength == 5) {
		l_sR = l_input.substr(0,2); //get R
		l_sG = l_input.substr(2,2); //get G
		l_sB = l_input.substr(4,1) + l_input.substr(0,1); //get B
	} else if (l_inputLength == 6) {
		l_sR = l_input.substr(0,2); //get R
		l_sG = l_input.substr(2,2); //get G
		l_sB = l_input.substr(4,2); //get B
	} else if (l_inputLength == 7) {
		//apply as is
		l_sR = l_input.substr(0,2); //get R
		l_sG = l_input.substr(2,2); //get G
		l_sB = l_input.substr(4,2); //get B
		if (_kage->int255ToHex(l_A).length() > 1) {
			l_sA = l_input.substr(6,1) + _kage->int255ToHex(l_A).substr(1,1); //get A
		} else {
			l_sA = l_input.substr(6,1) + _kage->int255ToHex(l_A); //get A
		}
		l_A = _kage->hexToInt255(l_sA);
	} else if (l_inputLength == 8) {
		//apply as is
		l_sR = l_input.substr(0,2); //get R
		l_sG = l_input.substr(2,2); //get G
		l_sB = l_input.substr(4,2); //get B
		l_sA = l_input.substr(6,2); //get A
		l_A = _kage->hexToInt255(l_sA);
	}
	l_R = _kage->hexToInt255(l_sR);
	l_G = _kage->hexToInt255(l_sG);
	l_B = _kage->hexToInt255(l_sB);

	std::cout << "\n\nRGBA A " << l_sR << "\t" << l_sG << "\t" << l_sB << "\t" << l_sA << std::endl;
	std::cout << "\n\nRGBA B " << l_R  << "\t" << l_G  << "\t" << l_B  << "\t" << l_A << std::endl << std::endl << std::endl;
	_scaleFillR.set_value(l_R);
	_scaleFillG.set_value(l_G);
	_scaleFillB.set_value(l_B);
	_scaleFillAlpha.set_value((double) l_A / 255.0f * 100.0f);
	
	if (KageStage::toolMode == KageStage::ToolMode::MODE_DRAW_PENCIL) {
		KageStage::pencilFillColor.setR(l_R);
		KageStage::pencilFillColor.setG(l_G);
		KageStage::pencilFillColor.setB(l_B);
		KageStage::pencilFillColor.setA(l_A);
	} else {
		KageStage::fillColor.setR(l_R);
		KageStage::fillColor.setG(l_G);
		KageStage::fillColor.setB(l_B);
		KageStage::fillColor.setA(l_A);
	}

	Gdk::RGBA l_rgba = m_ColorButtonFill.get_rgba();
		l_rgba.set_red  ( l_R / 255.f );
		l_rgba.set_blue ( l_G / 255.f );
		l_rgba.set_green( l_B / 255.f );
		l_rgba.set_alpha( l_A / 255.f );
			m_ColorButtonFill.set_rgba(l_rgba);
	
	_kage->_stage.updateShapeColor(_kage->_UPDATE_SHAPE_COLORS, _kage->_UPDATE_SHAPE_COLORS);
}


void PropertyFillStroke::updateEntryStrokeRGBA() {
	unsigned int l_R = 0;
	unsigned int l_G = 0;
	unsigned int l_B = 0;
	unsigned int l_A = 0;
	if (KageStage::toolMode == KageStage::ToolMode::MODE_DRAW_PENCIL) {
		l_R = KageStage::pencilStroke.getR();
		l_G = KageStage::pencilStroke.getG();
		l_B = KageStage::pencilStroke.getB();
		l_A = KageStage::pencilStroke.getA();
	} else {
		l_R = KageStage::stroke.getR();
		l_G = KageStage::stroke.getG();
		l_B = KageStage::stroke.getB();
		l_A = KageStage::stroke.getA();
	}
	m_EntryStrokeRGBA.set_text(_kage->int255ToHex(l_R) + _kage->int255ToHex(l_G) + _kage->int255ToHex(l_B) + _kage->int255ToHex(l_A));
}
void PropertyFillStroke::StrokeR_onChange() {
	double l_red = _scaleStrokeR.get_value();
	Gdk::RGBA l_rgba = m_ColorButtonStroke.get_rgba();
		l_rgba.set_red( l_red / 255.0f );
	m_ColorButtonStroke.set_rgba(l_rgba);
	
	if (KageStage::toolMode == KageStage::ToolMode::MODE_DRAW_PENCIL) {
		KageStage::pencilStroke.setR(l_red);
	} else {
		KageStage::stroke.setR(l_red);
	}
		updateEntryStrokeRGBA();
	_kage->_stage.updateShapeColor(_kage->_UPDATE_SHAPE_COLORS, _kage->_UPDATE_SHAPE_COLORS);
}
void PropertyFillStroke::StrokeG_onChange() {
	double l_green = _scaleStrokeG.get_value();
	Gdk::RGBA l_rgba = m_ColorButtonStroke.get_rgba();
		l_rgba.set_green( l_green / 255.0f );
	m_ColorButtonStroke.set_rgba(l_rgba);
	
	if (KageStage::toolMode == KageStage::ToolMode::MODE_DRAW_PENCIL) {
		KageStage::pencilStroke.setG(l_green);
	} else {
		KageStage::stroke.setG(l_green);
	}
		updateEntryStrokeRGBA();
	_kage->_stage.updateShapeColor(_kage->_UPDATE_SHAPE_COLORS, _kage->_UPDATE_SHAPE_COLORS);
}
void PropertyFillStroke::StrokeB_onChange() {
	double l_blue = _scaleStrokeB.get_value();
	Gdk::RGBA l_rgba = m_ColorButtonStroke.get_rgba();
		l_rgba.set_blue( l_blue / 255.0f );
	m_ColorButtonStroke.set_rgba(l_rgba);
	
	if (KageStage::toolMode == KageStage::ToolMode::MODE_DRAW_PENCIL) {
		KageStage::pencilStroke.setB(l_blue);
	} else {
		KageStage::stroke.setB(l_blue);
	}
		updateEntryStrokeRGBA();
	_kage->_stage.updateShapeColor(_kage->_UPDATE_SHAPE_COLORS, _kage->_UPDATE_SHAPE_COLORS);
}
void PropertyFillStroke::StrokeAlpha_onChange() {
	double l_alpha = _scaleStrokeAlpha.get_value();
	Gdk::RGBA l_rgba = m_ColorButtonStroke.get_rgba();
		l_rgba.set_alpha( l_alpha / 100.f );
	m_ColorButtonStroke.set_rgba(l_rgba);
	if (KageStage::toolMode == KageStage::ToolMode::MODE_DRAW_PENCIL) {
		KageStage::pencilStroke.setA((int) (l_alpha * 256.0f/ 100.0f));
	} else {
		KageStage::stroke.setA((int) (l_alpha * 256.0f/ 100.0f));
	}
		updateEntryStrokeRGBA();
	_kage->_stage.updateShapeColor(_kage->_UPDATE_SHAPE_COLORS, _kage->_UPDATE_SHAPE_COLORS);
}
void PropertyFillStroke::EntryStrokeRGBA_onEnter() {
	//see: https://sourceforge.net/p/kage/tickets/66/?limit=25#7488
	std::string l_input = m_EntryStrokeRGBA.get_text();
	unsigned int l_inputLength = l_input.length();
	unsigned int l_R = 0;
	unsigned int l_G = 0;
	unsigned int l_B = 0;
	unsigned int l_A = 0;
	if (KageStage::toolMode == KageStage::ToolMode::MODE_DRAW_PENCIL) {
		l_A = KageStage::pencilStroke.getA();
	} else {
		l_A = KageStage::stroke.getA();
	}
	std::string l_sR = "";
	std::string l_sG = "";
	std::string l_sB = "";
	std::string l_sA = "";
	
	if (l_inputLength == 1) {
		l_sR = l_input + l_input;
		l_sG = l_input + l_input;
		l_sB = l_input + l_input;
	} else if (l_inputLength == 2) {
		l_sR = l_input;
		l_sG = l_input;
		l_sB = l_input;
	} else if (l_inputLength == 3) {
		l_sR = l_input.substr(0,1) + l_input.substr(0,1); //get R
		l_sG = l_input.substr(1,1) + l_input.substr(1,1); //get G
		l_sB = l_input.substr(2,1) + l_input.substr(2,1); //get B
	} else if (l_inputLength == 4) {
		l_sR = l_input.substr(0,1) + l_input.substr(0,1); //get R
		l_sG = l_input.substr(1,1) + l_input.substr(1,1); //get G
		l_sB = l_input.substr(2,1) + l_input.substr(2,1); //get B
		l_sA = l_input.substr(3,1) + l_input.substr(3,1); //get A
		l_A = _kage->hexToInt255(l_sA);
	} else if (l_inputLength == 5) {
		l_sR = l_input.substr(0,2); //get R
		l_sG = l_input.substr(2,2); //get G
		l_sB = l_input.substr(4,1) + l_input.substr(0,1); //get B
	} else if (l_inputLength == 6) {
		l_sR = l_input.substr(0,2); //get R
		l_sG = l_input.substr(2,2); //get G
		l_sB = l_input.substr(4,2); //get B
	} else if (l_inputLength == 7) {
		//apply as is
		l_sR = l_input.substr(0,2); //get R
		l_sG = l_input.substr(2,2); //get G
		l_sB = l_input.substr(4,2); //get B
		if (_kage->int255ToHex(l_A).length() > 1) {
			l_sA = l_input.substr(6,1) + _kage->int255ToHex(l_A).substr(1,1); //get A
		} else {
			l_sA = l_input.substr(6,1) + _kage->int255ToHex(l_A); //get A
		}
		l_A = _kage->hexToInt255(l_sA);
	} else if (l_inputLength == 8) {
		//apply as is
		l_sR = l_input.substr(0,2); //get R
		l_sG = l_input.substr(2,2); //get G
		l_sB = l_input.substr(4,2); //get B
		l_sA = l_input.substr(6,2); //get A
		l_A = _kage->hexToInt255(l_sA);
	}
	l_R = _kage->hexToInt255(l_sR);
	l_G = _kage->hexToInt255(l_sG);
	l_B = _kage->hexToInt255(l_sB);

	std::cout << "\n\nRGBA A " << l_sR << "\t" << l_sG << "\t" << l_sB << "\t" << l_sA << std::endl;
	std::cout << "\n\nRGBA B " << l_R  << "\t" << l_G  << "\t" << l_B  << "\t" << l_A << std::endl << std::endl << std::endl;
	_scaleStrokeR.set_value(l_R);
	_scaleStrokeG.set_value(l_G);
	_scaleStrokeB.set_value(l_B);
	_scaleStrokeAlpha.set_value((double) l_A / 255.0f * 100.0f);
	
	if (KageStage::toolMode == KageStage::ToolMode::MODE_DRAW_PENCIL) {
		KageStage::pencilStroke.setR(l_R);
		KageStage::pencilStroke.setG(l_G);
		KageStage::pencilStroke.setB(l_B);
		KageStage::pencilStroke.setA(l_A);
	} else {
		KageStage::stroke.setR(l_R);
		KageStage::stroke.setG(l_G);
		KageStage::stroke.setB(l_B);
		KageStage::stroke.setA(l_A);
	}

	Gdk::RGBA l_rgba = m_ColorButtonStroke.get_rgba();
		l_rgba.set_red  ( l_R / 255.f );
		l_rgba.set_blue ( l_G / 255.f );
		l_rgba.set_green( l_B / 255.f );
		l_rgba.set_alpha( l_A / 255.f );
			m_ColorButtonStroke.set_rgba(l_rgba);
	
	_kage->_stage.updateShapeColor(_kage->_UPDATE_SHAPE_COLORS, _kage->_UPDATE_SHAPE_COLORS);
}
void PropertyFillStroke::EntryStrokeThickness_onEnter() {
	double l_dbl = StringHelper::toDouble(m_EntryStrokeThickness.get_text());
    if (l_dbl < 0) { l_dbl = 0; }
    if (l_dbl > 999) { l_dbl = 999; }
	if (KageStage::toolMode == KageStage::ToolMode::MODE_DRAW_PENCIL) {
		KageStage::pencilStroke.setThickness(l_dbl);
		m_EntryStrokeThickness.set_text(StringHelper::doubleToString(KageStage::pencilStroke.getThickness()));
	} else {
		KageStage::stroke.setThickness(l_dbl);
		m_EntryStrokeThickness.set_text(StringHelper::doubleToString(KageStage::stroke.getThickness()));
	}
	_kage->_stage.updateShapeColor(_kage->_UPDATE_SHAPE_COLORS, _kage->_UPDATE_SHAPE_COLORS);
}

void PropertyFillStroke::setFillColorData(ColorData p_colordata) {
    unsigned int l_R = p_colordata.getR();
    unsigned int l_G = p_colordata.getG();
    unsigned int l_B = p_colordata.getB();
    unsigned int l_A = p_colordata.getA();
    m_EntryFillRGBA.set_text(_kage->int255ToHex(l_R) + _kage->int255ToHex(l_G) + _kage->int255ToHex(l_B) + _kage->int255ToHex(l_A));
    setScaleFillRGBA(p_colordata);
}

void PropertyFillStroke::setStrokeColorData(ColorData p_colordata) {
    unsigned int l_R = p_colordata.getR();
    unsigned int l_G = p_colordata.getG();
    unsigned int l_B = p_colordata.getB();
    unsigned int l_A = p_colordata.getA();
    m_EntryStrokeRGBA.set_text(_kage->int255ToHex(l_R) + _kage->int255ToHex(l_G) + _kage->int255ToHex(l_B) + _kage->int255ToHex(l_A));
    _scaleStrokeR.set_value(l_R);
    _scaleStrokeG.set_value(l_G);
    _scaleStrokeB.set_value(l_B);
    _scaleStrokeAlpha.set_value((double) l_A / 255.0f * 100.0f);
}

void PropertyFillStroke::setFillButtonColor(Gdk::Color p_color) {
	m_ColorButtonFill.set_color(p_color);
}
void PropertyFillStroke::setStrokeButtonColor(Gdk::Color p_color) {
	m_ColorButtonStroke.set_color(p_color);
}
void PropertyFillStroke::setScaleFillRGBA(ColorData p_colordata) {
    _scaleFillR.set_value(p_colordata.getR());
    _scaleFillG.set_value(p_colordata.getG());
    _scaleFillB.set_value(p_colordata.getB());
    _scaleFillAlpha.set_value((double) p_colordata.getA()*100.0f/255.0f);
}
void PropertyFillStroke::setScaleStrokeRGBA(ColorData p_colordata) {
    _scaleStrokeR.set_value(p_colordata.getR());
    _scaleStrokeG.set_value(p_colordata.getG());
    _scaleStrokeB.set_value(p_colordata.getB());
    _scaleStrokeAlpha.set_value((double) p_colordata.getA()*100.0f/255.0f);
}
void PropertyFillStroke::setStrokeThicknessText(double p_thickness) {
	m_EntryStrokeThickness.set_text(StringHelper::doubleToString(p_thickness));
}
std::string PropertyFillStroke::getStrokeThicknessText() {
	return m_EntryStrokeThickness.get_text();
}

Gdk::Color PropertyFillStroke::getFillColor() {
    return m_ColorButtonFill.get_color();
}

Gdk::Color PropertyFillStroke::getStrokeColor() {
    return m_ColorButtonStroke.get_color();
}

unsigned int PropertyFillStroke::getFillScaleR() {
    return _scaleFillR.get_value();
}
unsigned int PropertyFillStroke::getFillScaleG() {
    return _scaleFillG.get_value();
}
unsigned int PropertyFillStroke::getFillScaleB() {
    return _scaleFillB.get_value();
}
unsigned int PropertyFillStroke::getFillScaleAlpha() {
    return _scaleFillAlpha.get_value();
}
unsigned int PropertyFillStroke::getStrokeScaleR() {
    return _scaleStrokeR.get_value();
}
unsigned int PropertyFillStroke::getStrokeScaleG() {
    return _scaleStrokeG.get_value();
}
unsigned int PropertyFillStroke::getStrokeScaleB() {
    return _scaleStrokeB.get_value();
}
unsigned int PropertyFillStroke::getStrokeScaleAlpha() {
    return _scaleStrokeAlpha.get_value();
}
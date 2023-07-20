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

#include "stage.h"
#include "../../kage.h"

PropertyStage::PropertyStage(Kage* p_kage) :
        m_LabelStageWid("Width"),
        m_LabelStageHgt("Height"),
        m_LabelStageBG("Background"),
        m_LabelStageFPS("FPS"), Gtk::HBox() {
    _kage = p_kage;
    set_border_width(10);
    pack_start(m_PropStageV1);
        m_PropStageV1.set_border_width(4);
        m_PropStageV1.set_spacing(4);
        m_PropStageV1.pack_start(m_LabelStageWid);
        m_PropStageV1.pack_start(m_LabelStageHgt);
        m_PropStageV1.pack_start(m_LabelStageBG);
        m_PropStageV1.pack_start(m_LabelStageFPS);
    pack_start(m_PropStageV2);
        m_PropStageV2.set_border_width(4);
        m_PropStageV2.set_spacing(4);
        //m_PropStageV2.set_size_request(40, 80);
        m_PropStageV2.pack_start(m_EntryStageWid, Gtk::PACK_SHRINK);
        m_PropStageV2.pack_start(m_EntryStageHgt, Gtk::PACK_SHRINK);
        m_PropStageV2.pack_start(m_ColorButtonStage, Gtk::PACK_SHRINK);
        m_PropStageV2.pack_start(m_EntryStageFPS, Gtk::PACK_SHRINK);
            m_EntryStageWid.set_size_request(20, 24);
            m_EntryStageWid.set_width_chars(4);
            m_EntryStageWid.set_max_length(4);
            setWidthText(_kage->_document.Project._width);
                m_EntryStageWid.signal_activate().connect(
                    sigc::mem_fun(*this, &PropertyStage::EntryStageArea_onEnter) );
            m_EntryStageHgt.set_size_request(20, 24);
            m_EntryStageHgt.set_width_chars(4);
            m_EntryStageHgt.set_max_length(4);
            setHeightText(_kage->_document.Project._height);
                m_EntryStageHgt.signal_activate().connect(
                    sigc::mem_fun(*this, &PropertyStage::EntryStageArea_onEnter) );
            m_ColorButtonStage.set_size_request(32, 32);
            //m_ColorButtonStage.set_use_alpha(true);
            m_ColorButtonStage.set_title("Choose Stage Background Color");
            setBackgroundColor(_kage->getStageBG());
                m_ColorButtonStage.signal_color_set().connect(sigc::mem_fun(* this, &PropertyStage::ColorButtonStage_onClick) );
            m_EntryStageFPS.set_size_request(20, 24);
            m_EntryStageFPS.set_width_chars(3);
            m_EntryStageFPS.set_max_length(3);
            setFPSText(_kage->_document.Project._fps);
                m_EntryStageFPS.signal_activate().connect(
                    sigc::mem_fun(*this, &PropertyStage::EntryStageFPS_onEnter) );
}

PropertyStage::~PropertyStage() {
	//
}

void PropertyStage::EntryStageArea_onEnter() {
	string t = m_EntryStageWid.get_text();
		_kage->_document.Project._width = StringHelper::toDouble(t);
        setWidthText(_kage->_document.Project._width);
	t = m_EntryStageHgt.get_text();
		_kage->_document.Project._height = StringHelper::toDouble(t);
        setHeightText(_kage->_document.Project._height);
	_kage->_stage.invalidateToRender();
}
void PropertyStage::EntryStageFPS_onEnter() {
	string t = m_EntryStageFPS.get_text();
	cout << "_kage->_document.Project._fps " << _kage->_document.Project._fps << " t " << t << endl;
	_kage->_document.Project._fps = StringHelper::toUnsignedInteger(t);
	cout << "_kage->_document.Project._fps ..." << _kage->_document.Project._fps << " t " << t << endl;
	setFPSText(_kage->_document.Project._fps);
}

void PropertyStage::ColorButtonStage_onClick() {
	_kage->m_Color = m_ColorButtonStage.get_color();
//	_stage.modify_bg(Gtk::STATE_NORMAL, m_Color);
	_kage->setStageBG(_kage->m_Color);
}

void PropertyStage::setWidthText(double p_width) {
    m_EntryStageWid.set_text(StringHelper::doubleToString(p_width));
}

void PropertyStage::setHeightText(double p_height) {
    m_EntryStageHgt.set_text(StringHelper::doubleToString(p_height));
}

void PropertyStage::setFPSText(unsigned int p_fps) {
    m_EntryStageFPS.set_text(StringHelper::unsignedIntegerToString(p_fps));
    _kage->_timeline.forceRender();
}

void PropertyStage::setBackgroundColor(Gdk::Color p_color) {
    m_ColorButtonStage.set_color(p_color);
}
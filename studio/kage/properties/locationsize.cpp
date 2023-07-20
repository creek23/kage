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

#include "locationsize.h"
#include "../../kage.h"

PropertyLocationSize::PropertyLocationSize(Kage* p_kage) :
        m_LabelX("X"),
        m_LabelY("Y"),
        m_LabelWidth("Width"),
        m_LabelHeight("Height"), Gtk::HBox() {
    _kage = p_kage;
    set_border_width(10);
    pack_start(m_propLocationSizeV1);
        m_propLocationSizeV1.set_border_width(4);
        m_propLocationSizeV1.set_spacing(4);
        m_propLocationSizeV1.pack_start(m_LabelX);
        m_propLocationSizeV1.pack_start(m_LabelY);
        m_propLocationSizeV1.pack_start(m_LabelWidth);
        m_propLocationSizeV1.pack_start(m_LabelHeight);
    pack_start(m_propLocationSizeV2);
        m_propLocationSizeV2.set_border_width(4);
        m_propLocationSizeV2.set_spacing(4);
        m_propLocationSizeV2.pack_start(m_EntryX, Gtk::PACK_SHRINK);
            m_EntryX.set_size_request(20, 24);
            m_EntryX.set_width_chars(9);
            m_EntryX.set_max_length(9);
            setXText(_kage->_stage.propX);
            m_EntryX.signal_activate().connect(
                sigc::mem_fun(*this, &PropertyLocationSize::EntryX_onEnter));
        m_propLocationSizeV2.pack_start(m_EntryY, Gtk::PACK_SHRINK);
            m_EntryY.set_size_request(20, 24);
            m_EntryY.set_width_chars(9);
            m_EntryY.set_max_length(9);
            setYText(_kage->_stage.propY);
            m_EntryY.signal_activate().connect(
                sigc::mem_fun(*this, &PropertyLocationSize::EntryY_onEnter));
        m_propLocationSizeV2.pack_start(m_EntryWidth, Gtk::PACK_SHRINK);
            m_EntryWidth.set_size_request(20, 24);
            m_EntryWidth.set_width_chars(9);
            m_EntryWidth.set_max_length(9);
            setWidthText(_kage->_stage.propWidth);
            m_EntryWidth.signal_activate().connect(
                sigc::mem_fun(*this, &PropertyLocationSize::EntryWidth_onEnter));
        m_propLocationSizeV2.pack_start(m_EntryHeight, Gtk::PACK_SHRINK);
            m_EntryHeight.set_size_request(20, 24);
            m_EntryHeight.set_width_chars(9);
            m_EntryHeight.set_max_length(9);
            setHeightText(_kage->_stage.propHeight);
            m_EntryHeight.signal_activate().connect(
                sigc::mem_fun(*this, &PropertyLocationSize::EntryHeight_onEnter));
}

PropertyLocationSize::~PropertyLocationSize() {
	//
}

void PropertyLocationSize::EntryX_onEnter() {
	double l_dbl = StringHelper::toDouble(m_EntryX.get_text());
	    setXText(l_dbl);
	_kage->_stage.updateShapeX(l_dbl);
}

void PropertyLocationSize::EntryY_onEnter() {
	double l_dbl = StringHelper::toDouble(m_EntryY.get_text());
	    setYText(l_dbl);
	_kage->_stage.updateShapeY(l_dbl);
}

void PropertyLocationSize::EntryWidth_onEnter() {
	double l_dbl = StringHelper::toDouble(m_EntryWidth.get_text());
	
	setWidthText(l_dbl);
	_kage->_stage.updateShapeWidth(l_dbl);
}

void PropertyLocationSize::EntryHeight_onEnter() {
	double l_dbl = StringHelper::toDouble(m_EntryHeight.get_text());
	
	setHeightText(l_dbl);
	_kage->_stage.updateShapeHeight(l_dbl);
}

void PropertyLocationSize::setXText(double p_x) {
    m_EntryX.set_text(StringHelper::doubleToString(p_x));
}

void PropertyLocationSize::setYText(double p_y) {
    m_EntryY.set_text(StringHelper::doubleToString(p_y));
}

void PropertyLocationSize::setWidthText(double p_width) {
    m_EntryWidth.set_text(StringHelper::doubleToString(p_width));
}

void PropertyLocationSize::setHeightText(double p_height) {
    m_EntryHeight.set_text(StringHelper::doubleToString(p_height));
}
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

#include "locationsize.h"
#include "../../kage.h"

PropertyAsset::PropertyAsset(Kage* p_kage) :
        m_LabelX("X"),
        m_LabelY("Y"),
        m_LabelWidth("Width"),
        m_LabelHeight("Height"), 
        m_LabelRotation("Rotation"),
        m_LabelAlpha("Alpha"),
        _adjustAlpha( Gtk::Adjustment::create(0.0, 0.0, 101.0, 0.1, 1.0, 1.0) ),
        _scaleAlpha(_adjustAlpha, Gtk::ORIENTATION_HORIZONTAL), Gtk::HBox() {
    _kage = p_kage;
    set_border_width(10);
    pack_start(m_propAssetV1);
        m_propAssetV1.set_border_width(4);
        m_propAssetV1.set_spacing(4);
        m_propAssetV1.pack_start(m_LabelX);
        m_propAssetV1.pack_start(m_LabelY);
        m_propAssetV1.pack_start(m_LabelWidth);
        m_propAssetV1.pack_start(m_LabelHeight);
        m_propAssetV1.pack_start(m_LabelRotation);
        m_propAssetV1.pack_start(m_LabelAlpha);
    pack_start(m_propAssetV2);
        m_propAssetV2.set_border_width(4);
        m_propAssetV2.set_spacing(4);
        m_propAssetV2.pack_start(m_EntryX, Gtk::PACK_SHRINK);
            m_EntryX.set_size_request(20, 24);
            m_EntryX.set_width_chars(9);
            m_EntryX.set_max_length(9);
            setXText(_kage->_stage.propX);
            m_EntryX.signal_activate().connect(
                sigc::mem_fun(*this, &PropertyAsset::EntryX_onEnter));
        m_propAssetV2.pack_start(m_EntryY, Gtk::PACK_SHRINK);
            m_EntryY.set_size_request(20, 24);
            m_EntryY.set_width_chars(9);
            m_EntryY.set_max_length(9);
            setYText(_kage->_stage.propY);
            m_EntryY.signal_activate().connect(
                sigc::mem_fun(*this, &PropertyAsset::EntryY_onEnter));
        m_propAssetV2.pack_start(m_EntryWidth, Gtk::PACK_SHRINK);
            m_EntryWidth.set_size_request(20, 24);
            m_EntryWidth.set_width_chars(9);
            m_EntryWidth.set_max_length(9);
            setWidthText(_kage->_stage.propWidth);
            m_EntryWidth.signal_activate().connect(
                sigc::mem_fun(*this, &PropertyAsset::EntryWidth_onEnter));
        m_propAssetV2.pack_start(m_EntryHeight, Gtk::PACK_SHRINK);
            m_EntryHeight.set_size_request(20, 24);
            m_EntryHeight.set_width_chars(9);
            m_EntryHeight.set_max_length(9);
            setHeightText(_kage->_stage.propHeight);
            m_EntryHeight.signal_activate().connect(
                sigc::mem_fun(*this, &PropertyAsset::EntryHeight_onEnter));
        m_propAssetV2.pack_start(m_EntryRotation, Gtk::PACK_SHRINK);
            m_EntryRotation.set_size_request(20, 24);
            m_EntryRotation.set_width_chars(9);
            m_EntryRotation.set_max_length(9);
            setHeightText(_kage->_stage.propHeight);
            m_EntryRotation.signal_activate().connect(
                sigc::mem_fun(*this, &PropertyAsset::EntryRotation_onEnter));
        m_propAssetV2.pack_start(_scaleAlpha, Gtk::PACK_SHRINK);
                _scaleAlpha.set_digits(0);
                _scaleAlpha.set_value(100);
                _scaleAlpha.set_value_pos(Gtk::POS_TOP);
                _scaleAlpha.set_draw_value();
                _scaleAlpha.signal_value_changed().connect(
                    sigc::mem_fun(*this, &PropertyAsset::Alpha_onChange));

}

PropertyAsset::~PropertyAsset() {
	//
}

void PropertyAsset::EntryX_onEnter() {
	double l_dbl = StringHelper::toDouble(m_EntryX.get_text());
	    setXText(l_dbl);
	_kage->_stage.updateShapeX(l_dbl);
}

void PropertyAsset::EntryY_onEnter() {
	double l_dbl = StringHelper::toDouble(m_EntryY.get_text());
	    setYText(l_dbl);
	_kage->_stage.updateShapeY(l_dbl);
}

void PropertyAsset::EntryWidth_onEnter() {
	double l_dbl = StringHelper::toDouble(m_EntryWidth.get_text());
	
	setWidthText(l_dbl);
	_kage->_stage.updateShapeWidth(l_dbl);
}

void PropertyAsset::EntryHeight_onEnter() {
	double l_dbl = StringHelper::toDouble(m_EntryHeight.get_text());
	
	setHeightText(l_dbl);
	_kage->_stage.updateShapeHeight(l_dbl);
}

void PropertyAsset::EntryRotation_onEnter() {
	double l_dbl = StringHelper::toDouble(m_EntryRotation.get_text());
	
	setRotationText(l_dbl);
	//TODO: apply rotation to selected image
	//_kage->_stage.updateShapeHeight(l_dbl);
}

void PropertyAsset::Alpha_onChange() {
	double l_alpha = _scaleAlpha.get_value();
    //TODO: set alpha to selected image
	//_kage->_stage.updateImageAlpha(l_alpha / 100.f);
}

void PropertyAsset::setXText(double p_x) {
    m_EntryX.set_text(StringHelper::doubleToString(p_x));
}

void PropertyAsset::setYText(double p_y) {
    m_EntryY.set_text(StringHelper::doubleToString(p_y));
}

void PropertyAsset::setWidthText(double p_width) {
    m_EntryWidth.set_text(StringHelper::doubleToString(p_width));
}

void PropertyAsset::setHeightText(double p_height) {
    m_EntryHeight.set_text(StringHelper::doubleToString(p_height));
}

void PropertyAsset::setRotationText(double p_rotation) {
    m_EntryRotation.set_text(StringHelper::doubleToString(p_rotation));
}

unsigned int PropertyAsset::getScaleAlpha() {
    return _scaleAlpha.get_value();
}
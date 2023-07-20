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

#include "nodexy.h"
#include "../../kage.h"

PropertyNodeXY::PropertyNodeXY(Kage* p_kage) :
        m_LabelNodeX("X"),
        m_LabelNodeY("Y"),
        m_LabelToggleLine("Straighten"), Gtk::HBox() {
    _kage = p_kage;
    set_border_width(10);
    pack_start(m_propNodeXYV1);
        m_propNodeXYV1.set_border_width(4);
        m_propNodeXYV1.set_spacing(4);
        m_propNodeXYV1.pack_start(m_LabelNodeX);
        m_propNodeXYV1.pack_start(m_LabelNodeY);
        m_propNodeXYV1.pack_start(m_LabelToggleLine);
    pack_start(m_propNodeXYV2);
        m_propNodeXYV2.set_border_width(4);
        m_propNodeXYV2.set_spacing(4);
        m_propNodeXYV2.pack_start(m_EntryNodeX, Gtk::PACK_SHRINK);
            m_EntryNodeX.set_size_request(20, 24);
            m_EntryNodeX.set_width_chars(9);
            m_EntryNodeX.set_max_length(9);
            setNodeXText(_kage->m_KageStage.nodeX);
            m_EntryNodeX.signal_activate().connect(
                sigc::mem_fun(*this, &PropertyNodeXY::EntryNodeX_onEnter));
        m_propNodeXYV2.pack_start(m_EntryNodeY, Gtk::PACK_SHRINK);
            m_EntryNodeY.set_size_request(20, 24);
            m_EntryNodeY.set_width_chars(9);
            m_EntryNodeY.set_max_length(9);
            setNodeYText(_kage->m_KageStage.nodeY);
            m_EntryNodeY.signal_activate().connect(
                sigc::mem_fun(*this, &PropertyNodeXY::EntryNodeY_onEnter));
        m_propNodeXYV2.pack_start(_btnToggleLine, Gtk::PACK_SHRINK);
            _btnToggleLine_pixbuf = Gdk::Pixbuf::create_from_resource("/kage/share/icons/straighten.png");
                _btnToggleLine_img = Gtk::Image(_btnToggleLine_pixbuf);
                    _btnToggleLine.set_image(_btnToggleLine_img);
                        _btnToggleLine.property_always_show_image();
                        _btnToggleLine.show();
                        _btnToggleLine.set_size_request(20, 20);
            _btnToggleLine.set_focus_on_click(false);
            _btnToggleLine.signal_clicked().connect(sigc::mem_fun(*this, &PropertyNodeXY::ToggleLine_onClick));
}

PropertyNodeXY::~PropertyNodeXY() {
	//
}

void PropertyNodeXY::EntryNodeX_onEnter() {
	double l_dbl = StringHelper::toDouble(m_EntryNodeX.get_text());
	
	m_EntryNodeX.set_text(StringHelper::doubleToString(l_dbl));
	_kage->m_KageStage.updateNodeX(l_dbl);
}

void PropertyNodeXY::EntryNodeY_onEnter() {
	double l_dbl = StringHelper::toDouble(m_EntryNodeY.get_text());
	
	m_EntryNodeY.set_text(StringHelper::doubleToString(l_dbl));
	_kage->m_KageStage.updateNodeY(l_dbl);
}

void PropertyNodeXY::ToggleLine_onClick() {
	Kage::timestamp_IN(); std::cout << " Kage::ToggleLine_onClick" << std::endl;
	if (_kage->m_KageStage.toggleLineSelectedNodes() == true) {
		_kage->forceRenderFrames();
	}
	Kage::timestamp_OUT();
}

void PropertyNodeXY::setNodeXText(double p_nodeX) {
    m_EntryNodeX.set_text(StringHelper::doubleToString(p_nodeX));
}
void PropertyNodeXY::setNodeYText(double p_nodeY) {
    m_EntryNodeY.set_text(StringHelper::doubleToString(p_nodeY));
}
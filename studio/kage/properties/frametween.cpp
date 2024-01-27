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

#include "frametween.h"
#include "../../kage.h"

PropertyFrameTween::PropertyFrameTween(Kage* p_kage) :
        m_LabelTweenX("Tween X"),
        m_LabelTweenY("Tween Y"), Gtk::HBox() {
    _kage = p_kage;
    set_border_width(10);
    pack_start(m_propFrameTweenV1);
        m_propFrameTweenV1.set_border_width(4);
        m_propFrameTweenV1.set_spacing(4);
        m_propFrameTweenV1.pack_start(m_LabelTweenX);
        m_propFrameTweenV1.pack_start(m_LabelTweenY);
    pack_start(m_propFrameTweenV2);
        m_propFrameTweenV2.set_border_width(4);
        m_propFrameTweenV2.set_spacing(4);
        m_propFrameTweenV2.pack_start(m_ComboX);
            m_ComboX.append("Linear");
            m_ComboX.append("Ease In");
            m_ComboX.append("Ease Out");
            setTweenXText("Linear");
            m_ComboX.signal_changed().connect( sigc::mem_fun(*this, &PropertyFrameTween::FrameTween_onChange) );
        m_propFrameTweenV2.pack_start(m_ComboY);
            m_ComboY.append("Linear");
            m_ComboY.append("Ease In");
            m_ComboY.append("Ease Out");
            setTweenYText("Linear");
            m_ComboY.signal_changed().connect( sigc::mem_fun(*this, &PropertyFrameTween::FrameTween_onChange) );
}

PropertyFrameTween::~PropertyFrameTween() {
	//
}

void PropertyFrameTween::FrameTween_onChange() {
	Glib::ustring nameX = m_ComboX.get_active_text();
	Glib::ustring nameY = m_ComboY.get_active_text();
	unsigned int l_tweenX = 0;
	unsigned int l_tweenY = 0;
	if (nameX == "Linear") {
		l_tweenX = 10;
	} else if (nameX == "Ease In") {
		l_tweenX = 20;
	} else if (nameX == "Ease Out") {
		l_tweenX = 30;
	}
	if (nameY == "Linear") {
		l_tweenY = 1;
	} else if (nameY == "Ease In") {
		l_tweenY = 2;
	} else if (nameY == "Ease Out") {
		l_tweenY = 3;
	}
	
	try {
		_kage->_document.getScene()->setTween(l_tweenX + l_tweenY);
	} catch (std::exception& e) {
		std::cout << "PropertyFrameTween::FrameTween_onChange Exception : " << e.what() << std::endl;
	}
    std::cout << "FrameTween_onChange X = " << nameX << " Y = " << nameY << " l_tweenX " << (l_tweenX+l_tweenY) << std::endl;
}

void PropertyFrameTween::setTweenXText(string p_tween) {
    m_ComboX.set_active_text(p_tween);
}
void PropertyFrameTween::setTweenYText(string p_tween) {
    m_ComboY.set_active_text(p_tween);
}
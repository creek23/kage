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

#include "form_new.h"

FormNewDialog::FormNewDialog(Gtk::Window& parent)
		: Gtk::Dialog("New From Template", parent, true),
		_label("Label:", true),
		_lblOrientation("Orientation:", true),
		_lblTemplate("Template:", true),
		_lblFPS("FPS:", true),
		_lblWidth("Width:", true),
		_lblHeight("Height:", true) {
//	add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
	set_resizable(false);
	//m_VBox.set_border_width(8);
	//get_vbox()->pack_start(m_VBox, Gtk::PACK_SHRINK);
	add_button(Gtk::Stock::OK, Gtk::ResponseType::RESPONSE_OK);
	add_button(Gtk::Stock::CANCEL, Gtk::ResponseType::RESPONSE_CANCEL);

	m_HBox.set_border_width(8);
	get_vbox()->pack_start(m_HBox, Gtk::PACK_SHRINK);
		m_HBox.pack_start(m_VBox1);
			m_VBox1.set_border_width(4);
			m_VBox1.set_spacing(4);
			m_VBox1.pack_start(_lblOrientation);
			m_VBox1.pack_start(_lblTemplate);
			m_VBox1.pack_start(_lblFPS);
			m_VBox1.pack_start(_lblWidth);
			m_VBox1.pack_start(_lblHeight);
		m_HBox.pack_start(m_VBox2);
			m_VBox2.set_border_width(4);
			m_VBox2.set_spacing(4);
			m_VBox2.pack_start(_cmbOrientation);
				_cmbOrientation.append("All");
				_cmbOrientation.append("Landscape");
				_cmbOrientation.append("Portrait");
			m_VBox2.pack_start(_cmbTemplate);
			m_VBox2.pack_start(_txtFPS);
				_txtFPS.set_size_request(20, 24);
				_txtFPS.set_width_chars(3);
				_txtFPS.set_max_length(3);
				//setFPSText(_kage->_document._fps);
				_txtFPS.signal_activate().connect( sigc::mem_fun(*this, &FormNewDialog::txtFPS_onEnter) );
			m_VBox2.pack_start(_txtWidth);
				_txtWidth.set_size_request(20, 24);
				_txtWidth.set_width_chars(4);
				_txtWidth.set_max_length(4);
				//setWidthText(_kage->_document._width);
				_txtWidth.signal_activate().connect( sigc::mem_fun(*this, &FormNewDialog::txtWidth_onEnter) );
			m_VBox2.pack_start(_txtHeight);
				_txtHeight.set_size_request(20, 24);
				_txtHeight.set_width_chars(4);
				_txtHeight.set_max_length(4);
				//setHeightText(_kage->_document._height);
				_txtHeight.signal_activate().connect( sigc::mem_fun(*this, &FormNewDialog::txtHeight_onEnter) );
	
	_templateProfiles.clear();
	
	TemplateProfile l_templateProfile8fps;
		l_templateProfile8fps._name = "Standard 8fps"; 
		l_templateProfile8fps._fps = 8;
		l_templateProfile8fps._width = 800.0f;
		l_templateProfile8fps._height = 600.0f;
		l_templateProfile8fps._square = false;
		l_templateProfile8fps._landscape = true;
			_templateProfiles.push_back(l_templateProfile8fps);
	TemplateProfile l_templateProfile12fps;
		l_templateProfile12fps._name = "Standard 12fps";
		l_templateProfile12fps._fps = 12;
		l_templateProfile12fps._width = 800.0f;
		l_templateProfile12fps._height = 600.0f;
		l_templateProfile12fps._square = false;
		l_templateProfile12fps._landscape = true;
			_templateProfiles.push_back(l_templateProfile12fps);
	
	TemplateProfile l_templateProfileHD8fps;
		l_templateProfileHD8fps._name = "Standard HD 8fps"; 
		l_templateProfileHD8fps._fps = 8;
		l_templateProfileHD8fps._width = 960.0f;
		l_templateProfileHD8fps._height = 720.0f;
		l_templateProfileHD8fps._square = false;
		l_templateProfileHD8fps._landscape = true;
			_templateProfiles.push_back(l_templateProfileHD8fps);
	TemplateProfile l_templateProfileHD12fps;
		l_templateProfileHD12fps._name = "Standard HD 12fps";
		l_templateProfileHD12fps._fps = 12;
		l_templateProfileHD12fps._width = 960.0f;
		l_templateProfileHD12fps._height = 720.0f;
		l_templateProfileHD12fps._square = false;
		l_templateProfileHD12fps._landscape = true;
			_templateProfiles.push_back(l_templateProfileHD12fps);
	
	TemplateProfile l_templateProfileHD8fpswide;
		l_templateProfileHD8fpswide._name = "Standard Wide HD 8fps"; 
		l_templateProfileHD8fpswide._fps = 8;
		l_templateProfileHD8fpswide._width = 1280.0f;
		l_templateProfileHD8fpswide._height = 720.0f;
		l_templateProfileHD8fpswide._square = false;
		l_templateProfileHD8fpswide._landscape = true;
			_templateProfiles.push_back(l_templateProfileHD8fpswide);
	TemplateProfile l_templateProfileHD12fpswide;
		l_templateProfileHD12fpswide._name = "Standard Wide HD 12fps";
		l_templateProfileHD12fpswide._fps = 12;
		l_templateProfileHD12fpswide._width = 1280.0f;
		l_templateProfileHD12fpswide._height = 720.0f;
		l_templateProfileHD12fpswide._square = false;
		l_templateProfileHD12fpswide._landscape = true;
			_templateProfiles.push_back(l_templateProfileHD12fpswide);
	
	doOpenTemplateProfile();
	
	_cmbOrientation.signal_changed().connect( sigc::mem_fun(*this, &FormNewDialog::Orientation_onChange) );
	
	//populate _cmbTemplate
	for (TemplateProfile l_templateProfile: _templateProfiles) {
		_cmbTemplate.append(l_templateProfile._name);
	}
	
	_cmbTemplate.signal_changed().connect( sigc::mem_fun(*this, &FormNewDialog::Template_onChange) );
	
	show_all();
	_cmbOrientation.set_active_text("All");
	_cmbTemplate.set_active_text("Standard HD 8fps");
}

FormNewDialog::~FormNewDialog() {
	//
}

void FormNewDialog::txtFPS_onEnter() {
	unsigned int l_uint = StringHelper::toUnsignedInteger(_txtFPS.get_text());
		setFPSText(l_uint);
	//_kage->_stage.updateShapeY(l_dbl);
}
void FormNewDialog::setFPSText(unsigned int p_fps) {
    _txtFPS.set_text(StringHelper::unsignedIntegerToString(p_fps));
}
void FormNewDialog::txtWidth_onEnter() {
	unsigned int l_uint = StringHelper::toUnsignedInteger(_txtWidth.get_text());
		setWidthText(l_uint);
	//_kage->_stage.updateShapeY(l_dbl);
}
void FormNewDialog::setWidthText(double p_width) {
    _txtWidth.set_text(StringHelper::unsignedIntegerToString(p_width));
}
void FormNewDialog::txtHeight_onEnter() {
	unsigned int l_uint = StringHelper::toUnsignedInteger(_txtHeight.get_text());
		setHeightText(l_uint);
	//_kage->_stage.updateShapeY(l_dbl);
}
void FormNewDialog::setHeightText(double p_height) {
    _txtHeight.set_text(StringHelper::unsignedIntegerToString(p_height));
}

void FormNewDialog::Orientation_onChange() {
	Glib::ustring l_orientation = _cmbOrientation.get_active_text();
	_cmbTemplate.remove_all();
	if (l_orientation == "All") {
		for (TemplateProfile l_templateProfile: _templateProfiles) {
			_cmbTemplate.append(l_templateProfile._name);
		}
	} else if (l_orientation == "Landscape") {
		for (TemplateProfile l_templateProfile: _templateProfiles) {
			if (l_templateProfile._landscape || l_templateProfile._square) {
				_cmbTemplate.append(l_templateProfile._name);
			}
		}
	} else if (l_orientation == "Portrait") {
		for (TemplateProfile l_templateProfile: _templateProfiles) {
			if (l_templateProfile._landscape == false || l_templateProfile._square) {
				_cmbTemplate.append(l_templateProfile._name);
			}
		}
	}
	_cmbTemplate.set_active(0);
}

void FormNewDialog::Template_onChange() {
	Glib::ustring l_template = _cmbTemplate.get_active_text();
	
	for (TemplateProfile l_templateProfile: _templateProfiles) {
		if (l_templateProfile._name == l_template) {
			_txtFPS.set_text(StringHelper::unsignedIntegerToString(l_templateProfile._fps));
			_txtWidth.set_text(StringHelper::doubleToString(l_templateProfile._width));
			_txtHeight.set_text(StringHelper::doubleToString(l_templateProfile._height));
			_templateProfile._fps = l_templateProfile._fps;
			_templateProfile._width = l_templateProfile._width;
			_templateProfile._height = l_templateProfile._height;
			_templateProfile._square = l_templateProfile._square;
			_templateProfile._landscape = l_templateProfile._landscape;
			
			break;
		}
	}
}

unsigned int FormNewDialog::getFPS() {
	return StringHelper::toUnsignedInteger(_txtFPS.get_text());
}
double FormNewDialog::getWidth() {
	return StringHelper::toDouble(_txtWidth.get_text());
}
double FormNewDialog::getHeight() {
	return StringHelper::toDouble(_txtHeight.get_text());
}

bool FormNewDialog::doOpenTemplateProfile() {
	std::string l_templateProfiles = "template.profile.xml";
	
	l_templateProfiles = BasicXml::openXMLFile(l_templateProfiles);
	
	BasicXml _xml;
	if (_xml.parse(l_templateProfiles)) {
		if (_xml.tokenize()) {
			XmlTag l_root = _xml.getRoot();
			//std::cout << l_root.getName() << std::endl;
			if (l_root.getName() == "template") {//} && version="0.8 (Emcee)") {
				std::vector<XmlTagProperty> l_xmlTagProperties = l_root.getProperties();
				if (l_xmlTagProperties.size() > 0
						&& l_xmlTagProperties[0].getName() == "profile"
						&& l_xmlTagProperties[0].getValue() == "Kage Studio"
						&& l_xmlTagProperties[1].getName() == "version"
						&& l_xmlTagProperties[1].getValue() == "0.8 (Emcee)"
					) {
					//get profiles
					for (XmlTag l_child: l_root._children) {
						//std::cout << "\t" << l_child.getName() << std::endl;
						if (l_child.getName() == "profile") {//} && version="0.8 (Emcee)") {
							TemplateProfile l_templateProfile;
							l_templateProfile._name = "Profile";
							l_templateProfile._fps = 12;
							l_templateProfile._width = 960.0f;
							l_templateProfile._height = 720.0f;
							l_templateProfile._square = false;
							l_templateProfile._landscape = true;
							
							for (XmlTagProperty l_childProperty: l_child.getProperties()) {
								if (l_childProperty.getName() == "name") {
									l_templateProfile._name = l_childProperty.getValue();
								} else if (l_childProperty.getName() == "fps") {
									l_templateProfile._fps = StringHelper::toUnsignedInteger(l_childProperty.getValue());
								} else if (l_childProperty.getName() == "width") {
									l_templateProfile._width = StringHelper::toDouble(l_childProperty.getValue());
								} else if (l_childProperty.getName() == "height") {
									l_templateProfile._height = StringHelper::toDouble(l_childProperty.getValue());
								}
								//std::cout << "\t\t" << l_childProperty.getName() << "=" << l_childProperty.getValue() << std::endl;
							}
							l_templateProfile._square = (l_templateProfile._width == l_templateProfile._height);
							l_templateProfile._landscape = (l_templateProfile._width > l_templateProfile._height);
							_templateProfiles.push_back(l_templateProfile);
						}
					}
					
					return true;
				}
			}
		} else {
			//unable to tokenize Templates
			std::cout << "Loading Templates Error 1: Unable to load template.profiles.xml" << std::endl;
		}
	} else {
		//unable to parse Templates
		std::cout << "Loading Error 2: Unable to load template.profiles.xml" << std::endl;
	}
	
	return false;
}
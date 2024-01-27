/*
 * xmltagproperty.cpp
 * 
 * Copyright 2019 Mj Mendoza IV <mj.mendoza.iv@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
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
 * MA 02110-1301, USA.
 * 
 * 
 */

#include "xmltagproperty.h"

XmlTagProperty::XmlTagProperty() {
	setName("");
}

XmlTagProperty::XmlTagProperty(string p_name, string p_value) {
	setName(p_name);
	setValue(p_value);
}

XmlTagProperty::~XmlTagProperty() {
	//
}

XmlTagProperty XmlTagProperty::clone() {
	XmlTagProperty l_xmlTagProperty(getName(), getValue());
	return l_xmlTagProperty;
}

bool XmlTagProperty::equalTo(XmlTagProperty p_xmlTagProperty) {
	return (getName() == p_xmlTagProperty.getName()
				&& getValue() == p_xmlTagProperty.getValue());
}

void XmlTagProperty::copy(XmlTagProperty p_xmlTagProperty) {
	setName(p_xmlTagProperty.getName());
	setValue(p_xmlTagProperty.getValue());
}

std::string XmlTagProperty::toString() {
	std::ostringstream l_ostringstream;
		l_ostringstream << getName() << "=\"" << getValue() << "\"";
	return l_ostringstream.str();
}

void XmlTagProperty::setName(string p_name) {
	_name = p_name;
}

void XmlTagProperty::setValue(string p_value) {
	_value = p_value;
}

string XmlTagProperty::getName() {
	return _name;
}

string XmlTagProperty::getValue() {
	return _value;
}

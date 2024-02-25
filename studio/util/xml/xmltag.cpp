/*
 * xmltag.cpp
 * 
 * Copyright 2019-2024 Mj Mendoza IV <mj.mendoza.iv@gmail.com>
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

#include "xmltag.h"

XmlTag::XmlTag() {
	setName("");
	_xmlTagProperties.clear();
}

XmlTag::XmlTag(std::string p_name) {
	setName(p_name);
	_xmlTagProperties.clear();
}

XmlTag::XmlTag(std::string p_name, std::vector<XmlTagProperty> p_xmlTagProperties) {
	setName(p_name);
	setProperties(p_xmlTagProperties);
}

XmlTag::~XmlTag() {
	//
}

XmlTag XmlTag::clone() {
	XmlTag l_xmlTag(getName(), getProperties());
	return l_xmlTag;
}

bool XmlTag::equalTo(XmlTag p_xmlTag) {
	if (getName() == p_xmlTag.getName() && getProperties().size() == p_xmlTag.getProperties().size()) {
		std::vector<XmlTagProperty> l_xmlTagPropertiesA = getProperties();
		std::vector<XmlTagProperty> l_xmlTagPropertiesB = p_xmlTag.getProperties();
		unsigned int l_count = 0;
		for (unsigned int i = 0; i < l_xmlTagPropertiesA.size(); ++i) {
			if (l_xmlTagPropertiesA[i].equalTo(l_xmlTagPropertiesB[i])) {
				++l_count;
			}
		}
		if (l_count == l_xmlTagPropertiesA.size()) {
			return true;
		}
	}
	return false;
}

void XmlTag::copy(XmlTag p_xmlTag) {
	setName(p_xmlTag.getName());
	setProperties(p_xmlTag.getProperties());
}

std::string XmlTag::toString() {
	std::ostringstream l_ostringstream;
		l_ostringstream << getName() << " ";
//	std::vector<XmlTagProperty> l_xmlTagProperties = getProperties();
//	for (unsigned int i = 0; i < l_xmlTagProperties.size(); ++i) {
//		l_ostringstream << l_xmlTagProperties[i].toString() << " ";
	for (unsigned int i = 0; i < _xmlTagProperties.size(); ++i) {
		l_ostringstream << _xmlTagProperties[i].toString() << " ";
	}
	l_ostringstream << std::endl;
	for (unsigned int i = 0; i < _children.size(); ++i) {
		l_ostringstream << "\t" << _children[i].toString() << std::endl;
	}
	return l_ostringstream.str();
}

void XmlTag::setName(std::string p_name) {
	_name = p_name;
}

void XmlTag::setProperties(std::vector<XmlTagProperty> p_xmlTagProperties) {
	_xmlTagProperties.clear();
	for (unsigned int i = 0; i < p_xmlTagProperties.size(); ++i) {
		_xmlTagProperties.push_back(p_xmlTagProperties[i]);
	}
}

std::string XmlTag::getName() {
	return _name;
}

std::vector<XmlTagProperty> XmlTag::getProperties() {
	std::vector<XmlTagProperty> l_xmlTagProperties;
	l_xmlTagProperties.clear();
	for (unsigned int i = 0; i < _xmlTagProperties.size(); ++i) {
		XmlTagProperty l_XmlTagProperty;
		l_XmlTagProperty = _xmlTagProperties[i];
		l_xmlTagProperties.push_back(l_XmlTagProperty);
	}
	return l_xmlTagProperties;
}

unsigned int XmlTag::addChild(XmlTag p_child) {
	_children.push_back(p_child);
	return _children.size();
}

XmlTag XmlTag::getChild(unsigned int p_index) {
	if (p_index >= 0 && p_index < _children.size()) {
		return _children[p_index];
	}
	XmlTag l_xmlTag;
	return l_xmlTag;
}

/*
 * xmltoken.cpp
 * 
 * Copyright 2019~2024 Mj Mendoza IV <mj.mendoza.iv@gmail.com>
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

#include "xmltoken.h"

XmlToken::XmlToken() {
	setValue("");
}

XmlToken::XmlToken(std::string p_value) {
	setValue(p_value);
}

XmlToken::~XmlToken() {
	//
}

void XmlToken::setValue(std::string p_value) {
	_value = p_value;
}

std::string XmlToken::getValue() {
	return _value;
}

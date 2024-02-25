/*
 * xmltoken.h
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

#ifndef KAGE_UTIL_XML_TOKEN_H
	#define KAGE_UTIL_XML_TOKEN_H
	
	#include <string>
	#include <sstream>
	
	class XmlToken {
		public:
			XmlToken();
			XmlToken(std::string p_value);
			virtual ~XmlToken();
			
			void setValue(std::string p_value);
			std::string getValue();
			
		protected:
			std::string _value;
	};
#endif //KAGE_UTIL_XML_TOKEN_H

/*
 * xmltag.h
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

#ifndef KAGE_UTIL_XMLTAG_H
	#define KAGE_UTIL_XMLTAG_H
	
	#include <vector>
	#include <string>
	#include <sstream>
	#include "xmltagproperty.h"
	
	class XmlTag {
		public:
			XmlTag();
			XmlTag(std::string p_name);
			XmlTag(std::string p_name, std::vector<XmlTagProperty> p_xmlTagProperties);
			virtual ~XmlTag();
			
			void setName(std::string p_name);
			std::string getName();
			void setProperties(std::vector<XmlTagProperty> p_xmlTagProperties);
			std::vector<XmlTagProperty> getProperties();
			unsigned int addChild(XmlTag p_xmlTag);
			XmlTag getChild(unsigned int p_index);
			
			XmlTag clone();
			std::string toString();
			void copy(XmlTag p_xmlTag);
			bool equalTo(XmlTag p_xmlTag);
			std::vector<XmlTag> _children;
			std::string _value; // <tag>values</tag>
		protected:
			std::string _name;
			std::vector<XmlTagProperty> _xmlTagProperties;
	};
#endif //KAGE_UTIL_XMLTAG_H

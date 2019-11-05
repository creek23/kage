/*
 * xmltag.h
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

#ifndef KAGE_UTIL_XMLTAG_H
	#define KAGE_UTIL_XMLTAG_H
	
	#include <vector>
	#include <string>
	#include <sstream>
	#include "xmltagproperty.h"
	using namespace std;
	
	class XmlTag {
		public:
			XmlTag();
			XmlTag(string p_name);
			XmlTag(string p_name, vector<XmlTagProperty> p_xmlTagProperties);
			virtual ~XmlTag();
			
			void setName(string p_name);
			string getName();
			void setProperties(vector<XmlTagProperty> p_xmlTagProperties);
			vector<XmlTagProperty> getProperties();
			unsigned int addChild(XmlTag p_xmlTag);
			XmlTag getChild(unsigned int p_index);
			
			XmlTag clone();
			std::string toString();
			void copy(XmlTag p_xmlTag);
			bool equalTo(XmlTag p_xmlTag);
			vector<XmlTag> _children;
			string _value; // <tag>values</tag>
		protected:
			string _name;
			vector<XmlTagProperty> _xmlTagProperties;
	};
#endif //KAGE_UTIL_XMLTAG_H

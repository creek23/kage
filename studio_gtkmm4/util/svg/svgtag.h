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

#ifndef KAGE_UTIL_SVGTAG_H
	#define KAGE_UTIL_SVGTAG_H
	
	#include <vector>
	#include <string>
	#include <sstream>
	#include "../xml/xmltag.h"
	#include "svgtagproperty.h"
	using namespace std;
	
	class SvgTag : public XmlTag {
		public:
			SvgTag();
			SvgTag(string p_name);
			SvgTag(string p_name, vector<SvgTagProperty> p_svgTagProperties);
			virtual ~SvgTag();
			
			void setProperties(vector<SvgTagProperty> p_svgTagProperties);
			vector<SvgTagProperty> getProperties();
			unsigned int addChild(SvgTag p_svgTag);
			SvgTag getChild(unsigned int p_index);
			
			SvgTag clone();
			std::string toString();
			void copy(SvgTag p_svgTag);
			bool equalTo(SvgTag p_svgTag);
			vector<SvgTag> _children;
			string _value; // <tag>values</tag>
		protected:
			vector<SvgTagProperty> _svgTagProperties;
	};
#endif //KAGE_UTIL_SVGTAG_H

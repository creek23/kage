/*
 * svg.h
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

#ifndef KAGE_UTIL_SVG_H
	#define KAGE_UTIL_SVG_H
	
	#include <vector>
	#include <string>
	#include <sstream>
	#include <iostream>
	#include <string.h>
	#include <fstream>
	#include "../xml/xml.h"
	#include "svgtagproperty.h"
	#include "svgtoken.h"
	#include "svgtag.h"
	using namespace std;
	
	class BasicSvg : public BasicXml {
		public:
			BasicSvg(string p_svgContent);
			BasicSvg();
			virtual ~BasicSvg();
			
			void setContent(string p_svgContent);
			string getContent();
			bool parse(string p_content);
			bool parse();
			
			void setProperties(vector<SvgTagProperty> p_svgTagProperties);
			string getProperties();
			BasicSvg clone();
			std::string toString();
			void copy(BasicSvg p_svgTag);
			bool equalTo(BasicSvg p_svgTag);
			
			vector<SvgToken> _tokens;
			bool tokenize();
			SvgTag getRoot();
			
			void debugToken(unsigned int p_index);
			void printSVG();
			string getSVG();
			bool setSVG(string p_svgdata);
			
			static string openXMLFile(string p_path);
			static bool isNumeric(string p_value);
		protected:
			unsigned int createTag(unsigned int p_index, SvgTag &p_svgTagParent);
			void listChildren(vector<SvgTag> p_tags, string p_tab);
			string getSvgChildren(vector<SvgTag> p_children, string p_tab);
			
			string _svgContent;
			vector<SvgTagProperty> _svgTagProperties;
			SvgTag _root;
//			vector<SvgTag> _children;
	};
#endif //KAGE_UTIL_SVG_H

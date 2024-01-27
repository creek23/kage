/*
 * xml.h
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

#ifndef KAGE_UTIL_XML_H
	#define KAGE_UTIL_XML_H
	
	#include <vector>
	#include <string>
	#include <sstream>
	#include <iostream>
	#include <string.h>
	#include <fstream>
	#include "xmltagproperty.h"
	#include "xmltoken.h"
	#include "xmltag.h"
	using namespace std;
	
	class BasicXml {
		public:
			BasicXml(string p_xmlContent);
			BasicXml();
			virtual ~BasicXml();
			
			void setContent(string p_xmlContent);
			string getContent();
			bool parse(string p_content);
			bool parse();
			
			void setProperties(vector<XmlTagProperty> p_xmlTagProperties);
			string getProperties();
			BasicXml clone();
			std::string toString();
			void copy(BasicXml p_xmlTag);
			bool equalTo(BasicXml p_xmlTag);
			
			std::vector<XmlToken> _tokens;
			bool tokenize();
			XmlTag getRoot();
			
			void debugToken(unsigned int p_index);
			void printXML();
			string getXML();
			bool setXML(string p_xmldata);
			
			static string openXMLFile(string p_path);
			static bool isNumeric(string p_value);
		protected:
			unsigned int createTag(unsigned int p_index, XmlTag &p_xmlTagParent);
			unsigned int skipWhitespace(unsigned int p_index);
			unsigned int expectFor(unsigned int p_index, string p_token);
			unsigned int lookFor(unsigned int p_index, string p_token);
			unsigned int getPreviousIndex(unsigned int p_index);
			void listChildren(vector<XmlTag> p_tags, string p_tab);
			string getXMLChildren(vector<XmlTag> p_children, string p_tab);
			
			string _xmlContent;
			std::vector<XmlTagProperty> _xmlTagProperties;
			XmlTag _root;
//			std::vector<XmlTag> _children;
	};
#endif //KAGE_UTIL_XML_H

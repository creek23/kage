/*
 * xml.cpp
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

#include "xml.h"

BasicXml::BasicXml() {
	setContent("");
}

BasicXml::BasicXml(std::string p_content) {
	setContent(p_content);
}

BasicXml::~BasicXml() {
	//
}

void BasicXml::setContent(std::string p_content) {
	_xmlContent = p_content;
}
std::string BasicXml::getContent() {
	return _xmlContent;
}

bool BasicXml::parse(std::string p_content) {
	setContent(p_content);
	return parse();
}

XmlTag BasicXml::getRoot() {
	return _root;
}
std::string BasicXml::toString() {
	std::ostringstream l_ostringstream;
	for (unsigned int i = 0; i < _xmlTagProperties.size(); ++i) {
		l_ostringstream << _xmlTagProperties[i].toString() << " ";
	}
	l_ostringstream << "\t" << _root.toString() << std::endl;
	
	return l_ostringstream.str();
}

unsigned int BasicXml::skipWhitespace(unsigned int p_index) {
	unsigned int l_len = _tokens.size();
	for (unsigned int i = p_index; i < l_len; ++i) {
		if (_tokens[i].getValue() != " " && _tokens[i].getValue() != "\t") {
			return i;
		}
	}
	return -1;
}

unsigned int BasicXml::expectFor(unsigned int p_index, std::string p_token) {
	unsigned int l_len = _tokens.size();
	for (unsigned int i = p_index; i < l_len; ++i) {
		i = skipWhitespace(i);
		if (_tokens[i].getValue() == p_token) {
			return i;
		} else {
			return -1;
		}
	}
	return -1;
}

unsigned int BasicXml::lookFor(unsigned int p_index, std::string p_token) {
	unsigned int l_len = _tokens.size();
	for (unsigned int i = p_index; i < l_len; ++i) {
		i = skipWhitespace(i);
		if (_tokens[i].getValue() == p_token) {
			return i;
		}
	}
	return -1;
}

unsigned int BasicXml::getPreviousIndex(unsigned int p_index) {
	unsigned int l_len = _tokens.size();
	for (unsigned int i = p_index; i >= 0; --i) {
		if (_tokens[i].getValue() != " " && _tokens[i].getValue() != "\t") {
			return i;
		} else {
			return -1;
		}
	}
	return -1;
}

bool BasicXml::isNumeric(std::string p_value) {
	bool l_gotDOT = false;
	
	for (unsigned int i = 0; i < p_value.length(); ++i) {
		switch (p_value[i]) {//[0]) {
			case '0': case '1': case '2':
			case '3': case '4': case '5':
			case '6': case '7': case '8':
			case '9':
				//okay
				break;
			case '-':
				if (i > 0 || p_value.length() == 1) { return false; }
				break;
			case '.':
				if (l_gotDOT == false) {
					l_gotDOT = true;
				} else {
					//can't have a value of "1.2.3"
					return false;
				}
				break;
			default:
				return false;
		}
	}
	
	return true;
}

bool BasicXml::parse() {
	if (_xmlContent == "") {
		std::cout << "Nothing to parse" << std::endl;
		return false;
	}
	unsigned int l_len = strlen(_xmlContent.c_str());
	
	std::string l_token = "";
	for (unsigned int l_index = 0; l_index < l_len; ++l_index) {
		if (BasicXml::isNumeric(l_token)
				&& (_xmlContent[l_index] == '0' || _xmlContent[l_index] == '.' || _xmlContent[l_index] == '-'
				|| _xmlContent[l_index] >= '1' && _xmlContent[l_index] <= '9')) {
			l_token += _xmlContent[l_index];
		} else if ((_xmlContent[l_index] >= 'a' && _xmlContent[l_index] <= 'z')
				|| (_xmlContent[l_index] >= 'A' && _xmlContent[l_index] <= 'Z')
				|| _xmlContent[l_index] == '0' || _xmlContent[l_index] == '_'
				|| (_xmlContent[l_index] >= '1' && _xmlContent[l_index] <= '9')
				|| _xmlContent[l_index] == ':' || _xmlContent[l_index] == '-' ) {//added support for `tagproperty:sub-property' for SVG
			//alpha-numeric
			l_token += _xmlContent[l_index];
		} else if (_xmlContent[l_index] == ' ' || _xmlContent[l_index] == '\t') {
			if (l_token != "") {
				_tokens.push_back(XmlToken(l_token));
				l_token = "";
			}
			
			l_token = _xmlContent[l_index];
			_tokens.push_back(XmlToken(l_token));
			l_token = "";
		} else if (_xmlContent[l_index] == '"') {
			if (l_token != "") {
				_tokens.push_back(XmlToken(l_token));
				l_token = "";
			}
			
			for (unsigned int j = l_index+1; j < l_len; ++j) {
				if (_xmlContent[j] == '"') {
					l_index = j;
					break;
				} else if (_xmlContent[j] == '\r' || _xmlContent[j] == '\n') {
					l_index = j;
					break;
				}
				l_token += _xmlContent[j];
			}
			_tokens.push_back(XmlToken(l_token));
			l_token = "";
		} else if (_xmlContent[l_index] == '\'') {
			if (l_token != "") {
				_tokens.push_back(XmlToken(l_token));
				l_token = "";
			}
			
			for (unsigned int j = l_index+1; j < l_len; ++j) {
				if (_xmlContent[l_index] == '\'') {
					l_index = j;
					break;
				} else if (_xmlContent[l_index] == '\r' || _xmlContent[l_index] == '\n') {
					l_index = j;
					break;
				}
				l_token += _xmlContent[j];
			}
			_tokens.push_back(XmlToken(l_token));
			l_token = "";
		} else if (_xmlContent[l_index] == '\r' || _xmlContent[l_index] == '\n') {
			//disregard CR/LF
		} else if (_xmlContent[l_index] == '<'
				&& l_index+1 < l_len && _xmlContent[l_index+1] == '!'
				&& l_index+2 < l_len && _xmlContent[l_index+2] == '-'
				&& l_index+3 < l_len && _xmlContent[l_index+3] == '-') {
			//possibly XML-comment; <!-- comment -->
			bool l_commentDone = false;
			for (unsigned int j = l_index+4; j < l_len; ++j) {
				if (_xmlContent[j] == '-'
						&& j+1 < l_len && _xmlContent[j+1] == '-'
						&& j+2 < l_len && _xmlContent[j+2] == '>') {
					l_index = j+2;
					l_commentDone = true;
					break;
				}
			}
			if (l_commentDone == false) {
				std::cout << "Unable to parse content; incomplete comment-tag" << std::endl;
				return "";
			}
		} else {
			if (l_token != "") {
				_tokens.push_back(XmlToken(l_token));
				l_token = "";
			}
			
			l_token = _xmlContent[l_index];
			_tokens.push_back(XmlToken(l_token));
			l_token = "";
		}
	}
	
	return true;
}
bool BasicXml::tokenize() {
	unsigned int l_len = _tokens.size();
	if (l_len == 0) {
		std::cout << "Nothing to tokenize" << std::endl;
		return false;
	}
	
	for (unsigned int i = 0; i < l_len; ++i) {
		i = createTag(i, _root);
		if (i == -1) {
			return false;
		}
	}
	
	return true;
}


void BasicXml::listChildren(std::vector<XmlTag> p_tags, std::string p_tab) {
	std::cout << std::endl;
	for (unsigned int i = 0; i < p_tags.size(); ++i) {
		std::cout << p_tab << "[" << i << "]\t" << p_tags[i].getName() << " -> ";
		std::vector<XmlTagProperty> l_properties = p_tags[i].getProperties();
		for (unsigned int j = 0; j < l_properties.size(); ++j) {
			std::cout << l_properties[j].getName() << "='"<< l_properties[j].getValue() << "' ";
		}
		listChildren(p_tags[i]._children, "\t" + p_tab);
	}
}

void BasicXml::printXML() {
	std::cout << "[0]\t" << _root.getName() << " -> ";
	listChildren(_root._children, "\t");
}

bool BasicXml::setXML(std::string p_xmldata) {
	if (parse(p_xmldata)) {
		if (tokenize()) {
			return true;
		} else {
			std::cout << "BasicXml: unable to tokenize";
		}
	} else {
		std::cout << "BasicXml: unable to parse";
	}
	return false;
}

std::string BasicXml::getXMLChildren(std::vector<XmlTag> p_children, std::string p_tab) {
	std::string l_xml = "";
	for (unsigned int i = 0; i < p_children.size(); ++i) {
		l_xml += p_tab + "<" + p_children[i].getName();
		std::vector<XmlTagProperty> l_properties = p_children[i].getProperties();
		if (l_properties.size() > 0) {
			l_xml += " ";
		}
		for (unsigned int j = 0; j < l_properties.size(); ++j) {
			l_xml += l_properties[j].getName() + "=\""+ l_properties[j].getValue() + "\"";
			if (j < l_properties.size()-1) {
				l_xml += " ";
			}
		}
		if (p_children[i]._children.size() > 0) {
			l_xml += ">\n";
			l_xml += getXMLChildren(p_children[i]._children, "\t" + p_tab);
			l_xml += p_tab + "</" + p_children[i].getName() + ">";
		} else if (p_children[i]._value != "") {
			l_xml += ">";
			l_xml += p_children[i]._value;
			l_xml += "</" + p_children[i].getName() + ">";
		} else {
			l_xml += "/>";
		}
		l_xml += "\n";
	}
	l_xml += "";
	return l_xml;
}

std::string BasicXml::getXML() {
	std::vector<XmlTag> l_root;
	l_root.push_back(_root);
	return getXMLChildren(l_root, "");
	#ifdef zxcasd
		std::string l_xml = "<" + _root.getName();
		std::vector<XmlTagProperty> l_properties = _root.getProperties();
		if (l_properties.size() > 0) {
			l_xml += " ";
		}
		for (unsigned int j = 0; j < l_properties.size(); ++j) {
			l_xml += l_properties[j].getName() + "=\"" + l_properties[j].getValue() + "\"";
			if (j < l_properties.size()-1) {
				l_xml += " ";
			}
		}
		l_xml += ">";
		l_xml += getXMLChildren(_root._children, "\t");
		l_xml += "</" + _root.getName() + ">";
		
		return l_xml;
	#endif
}

void BasicXml::debugToken(unsigned int p_index) {
	for (unsigned int i = 0; i < p_index; ++i) {
		if (_tokens[i].getValue() != " " && _tokens[i].getValue() != "\t") {
			std::cout << i << "\t? " << _tokens[i].getValue() << std::endl;
		}
	}
}

unsigned int BasicXml::createTag(unsigned int p_index, XmlTag &p_xmlTagParent) {
	unsigned int l_len = _tokens.size();
	unsigned int i = p_index;
		i = expectFor(i, "<");
		if (i == -1) {
			std::cout << "unable to parse; expecting XML." << std::endl;
			return i;
		}
		
		if (_tokens[i].getValue() == "<") {
			i = skipWhitespace(i+1);
			if (i == -1) {
				std::cout << "unable to parse; XML is incomplete." << std::endl;
				return i;
			}
			
			//parse: <?xml version="1.0" encoding="UTF-8" standalone="no"?>
			if (_tokens[i].getValue() == "?") {
				i = lookFor(i+1, "?");
				i = lookFor(i+1, ">");

				i = lookFor(i+1, "<");
				i = skipWhitespace(i+1);
			}
			
			p_xmlTagParent.setName(_tokens[i].getValue());
			
			i = skipWhitespace(i+1);
			if (i == -1) {
				std::cout << "unable to parse; XML TAG is incomplete." << std::endl;
				return i;
			}
			
			bool l_tagPairNeeded = true;
			//get properties until '>'
			std::vector<XmlTagProperty> l_xmlTagProperties;
			l_xmlTagProperties.clear();
				while (_tokens[i].getValue() != ">") {
					if (_tokens[i].getValue() == "/") {
						//single XML-tag; <tag/>
						l_tagPairNeeded = false;
					} else if (_tokens[i].getValue() == p_xmlTagParent.getName()) {
						break;
					} else {
						//possibly XML TAG-property
						XmlTagProperty l_xmlTagProperty;
						l_xmlTagProperty.setName(_tokens[i].getValue());
						
						unsigned int l_debug = i+1;
						i = expectFor(i+1, "=");
						if (i == -1) {
							debugToken(l_debug);
							std::cout << "unable to parse; XML TAG property is incomplete." << std::endl;
							return i;
						}
						
						i = skipWhitespace(i+1);
						if (i == -1) {
							std::cout << "unable to parse; XML TAG property is incomplete.." << std::endl;
							return i;
						}
						
						l_xmlTagProperty.setValue(_tokens[i].getValue());
							l_xmlTagProperties.push_back(l_xmlTagProperty);
					}
				
					i = skipWhitespace(i+1);
					if (i == -1) {
						std::cout << "unable to parse; XML TAG property is incomplete..." << std::endl;
						return i;
					}
				}
				
			if (_tokens[i].getValue() == ">") {
				//register XML TAG and try to populate children
				p_xmlTagParent.setProperties(l_xmlTagProperties);
			} else {
				std::cout << "unable to parse; XML TAG is incomplete.." << std::endl;
				return i;
			}
			
			while (l_tagPairNeeded == true) {
				//try to get CHILD/closing-TAG; <tag property='x'><CHILD>
				// OR
				//try to get VALUEs; <tag property='x'>VALUEs
				
				i = skipWhitespace(i+1);
				if (i == -1) {
					std::cout << "unable to parse; XML is incomplete.." << std::endl;
					return i;
				}
				
				if (_tokens[i].getValue() == "<") {
					//try to get closing-TAG; <tag property='x'></TAG>
					// OR
					//try to get CHILD TAG; <tag property='x'><CHILD>
					i = skipWhitespace(i+1);
					if (i == -1) {
						std::cout << "unable to parse; XML is incomplete..." << std::endl;
						return i;
					}
					
					if (_tokens[i].getValue() == "/") {
						//possibly a closing-TAG?
						i = expectFor(i+1, p_xmlTagParent.getName());
						if (i == -1) {
							std::cout << "unable to parse possibly-malformed XML." << std::endl;
							return i;
						}
						
						i = expectFor(i+1, ">");
						if (i == -1) {
							std::cout << "unable to parse; expecting XML .." << std::endl;
							return i;
						}
						l_tagPairNeeded = false;
					} else {
						//possibly a child TAG?
						i = getPreviousIndex(i-1);
						if (i == -1) {
							std::cout << "unable to parse; XML is incomplete..." << std::endl;
							return i;
						}
						
						XmlTag l_xmlTagChild;
						p_xmlTagParent._children.push_back(l_xmlTagChild);
						i = createTag(i, p_xmlTagParent._children[p_xmlTagParent._children.size()-1]);
						if (i == -1) {
							std::cout << "unable to create Child Tag for " << p_xmlTagParent.getName() << std::endl;
							return i;
						}
					}
				} else {
					p_xmlTagParent._value = "";
					while (_tokens[i].getValue() != "<") {
						p_xmlTagParent._value += _tokens[i].getValue();
						++i;
					}
					
					if (_tokens[i].getValue() == "<") {
						//try to get closing-TAG; <tag property='x'></TAG>
						// OR
						//try to get CHILD TAG; <tag property='x'><CHILD>
						i = skipWhitespace(i+1);
						if (i == -1) {
							std::cout << "unable to parse; XML is incomplete..." << std::endl;
							return i;
						}
						
						if (_tokens[i].getValue() == "/") {
							//possibly a closing-TAG?
							i = expectFor(i+1, p_xmlTagParent.getName());
							if (i == -1) {
								std::cout << "unable to parse; expecting XML  ." << std::endl;
								return i;
							}
							
							i = expectFor(i+1, ">");
							if (i == -1) {
								std::cout << "unable to parse; expecting XML  .." << std::endl;
								return i;
							}
							return i; //DONE?
						} else {
							//possibly a child TAG?
							i = getPreviousIndex(i);
							if (i == -1) {
								std::cout << "unable to parse; XML is incomplete  ..." << std::endl;
								return i;
							}
							
							XmlTag l_xmlTagChild;
							p_xmlTagParent._children.push_back(l_xmlTagChild);
							i = createTag(i, p_xmlTagParent._children[p_xmlTagParent._children.size()-1]);
							if (i == -1) {
								std::cout << "unable to create Child Tag for " << p_xmlTagParent.getName();
								return i;
							}
						}
					} else {
						return i; //DONE?
					}
				}
			}
			return i;
		}
	return i;
}

std::string BasicXml::openXMLFile(std::string p_path) {
	char ch;
	std::string l_string = "";
	std::fstream fin(p_path, std::fstream::in);
	while (fin >> std::noskipws >> ch) {
		l_string += ch;
	}
	
	return l_string;
}

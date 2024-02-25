/*
 * stringhelper.cpp
 * 
 * Copyright 2011-2024 Mj Mendoza IV <mj.mendoza.iv@gmail.com>
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

#include "stringhelper.h"
	
std::string StringHelper::toUpper(std::string p_str) {
	unsigned int i;
	
	for(i = 0; i < p_str.length(); ++i) {
		p_str[i] = toupper(p_str[i]);
	}
	
	return p_str;
}

std::string StringHelper::toLower(std::string p_str) {
	unsigned int i;
	for(i = 0; i < p_str.length(); ++i) {
		p_str[i] = tolower(p_str[i]);
	}
	return p_str;
}

std::string StringHelper::integerToString(int p_src) {
	if (p_src == 0) {
		return "0";
	}
	
	std::ostringstream l_ostringstream;
		l_ostringstream << p_src;
	return l_ostringstream.str();
}

std::string StringHelper::unsignedIntegerToString(unsigned int p_src) {
	if (p_src == 0) {
		return "0";
	}
	
	std::ostringstream l_ostringstream;
		l_ostringstream << p_src;
	return l_ostringstream.str();
}

std::string StringHelper::doubleToString(double p_src) {
	if (p_src == 0.0) {
		return "0.0";
	}
	
	std::ostringstream l_ostringstream;
		l_ostringstream << p_src;
	return l_ostringstream.str();
}

int StringHelper::toInteger(std::string p_src) {
	//TODO: validate string to be numerics
	return atoi(p_src.c_str());
}

unsigned int StringHelper::toUnsignedInteger(std::string p_src) {
	//TODO: validate string to be numerics
	return (unsigned)atoi(p_src.c_str());
}

long StringHelper::toLong(std::string p_src) {
	//TODO: validate string to be numerics
	return atol(p_src.c_str());
}

double StringHelper::toDouble(std::string p_src) {
	//TODO: validate string to be numerics
	return (double)atof(p_src.c_str());
}

bool StringHelper::toBoolean(std::string p_src) {
	if (p_src == "1" || StringHelper::toLower(p_src) == "true") {
		return true;
	} else {
		return false;
	}
}

std::vector<std::string> StringHelper::split(const std::string &p_source, const std::string &p_delimiter) {
	std::vector<std::string> l_return;
	if (p_delimiter.empty()) {
		l_return.push_back(p_source);
		return l_return;
	}
	std::string::const_iterator substart = p_source.begin(), subend;
	while (true) {
		subend = search(substart, p_source.end(), p_delimiter.begin(), p_delimiter.end());
		std::string l_temp(substart, subend);
		if (!l_temp.empty()) {
			l_return.push_back(StringHelper::trim(l_temp));
		}
		if (subend == p_source.end()) {
			break;
		}
		substart = subend + p_delimiter.size();
	}
	return l_return;
}

std::string StringHelper::trim(std::string p_source) {
	std::string l_source = "";
	int i;
	int l_len;
	//remove "space" before
	l_len = strlen(p_source.c_str());
	for (i = 0; i < l_len; ++i) {
		if (p_source[i] == ' ') {
			//skip to next...
		}
		else {
			l_source = p_source.substr(i, l_len - i);
			break;
		}
	}

	//remove "space" after
	l_len = strlen(l_source.c_str());
	for (i = l_len - 1; i > 0; --i) {
		if (l_source[i] == ' ') {
			//skip to next...
		}
		else {
			l_source = l_source.substr(0, i + 1);
			break;
		}
	}
	return l_source;
}

#include <iostream>
std::string StringHelper::kHash(std::string par, int p_len) {
	unsigned int i = par.length();
	char hArr[i - 1];
	int hNum[16] = { 75, 114, 105, 120, 119, 97, 114, 101, 32, 83, 116, 117, 100, 105, 111, 115 };
	int hVal = 10;
	std::string sHash;

	//delimit hash length
	if (p_len < 8) { p_len = 8; }
	if (p_len > 24) { p_len = 24; }
	
	unsigned int ii;
	int ii2, temp, prem = ((int)par.substr( 0, 1)[0]) % hVal;
	
	for (ii = 0; ii < par.length() - 1; ++ii) {
		hArr[ii] = par[ii+1];
		for (ii2 = 0; ii2 < 16; ++ii2) {
			temp = (hNum[ii2] + int(hArr[ii]) + prem) % hVal;
			if (temp < 25) {
				temp = 97 + temp % 26;
			} else {
				temp = 65 + temp;
			}
			prem = temp % 52;
			hNum[ii2] = temp;
		}
		prem = int(hArr[ii]) % hVal;
	}
	
	std::stringstream ss;
	ss  << char(hNum[ 0]) << char(hNum[ 1])
		<< char(hNum[ 2]) << char(hNum[ 3])
		<< char(hNum[ 4]) << char(hNum[ 5])
		<< char(hNum[ 6]) << char(hNum[ 7])
		<< char(hNum[ 8]) << char(hNum[ 9])
		<< char(hNum[10]) << char(hNum[11])
		<< char(hNum[12]) << char(hNum[13])
		<< char(hNum[14]) << char(hNum[15]);
	ss >> sHash;
	
	return sHash.substr(0, p_len);
}

std::string StringHelper::replace(std::string s, std::string const& p_find, std::string const& p_replace) {
    std::size_t pos = s.find(p_find);
    if (pos == std::string::npos) return s;
    s.replace(pos, p_find.length(), p_replace);
	return s;
}

std::string StringHelper::replaceAll(std::string s, std::string const& p_find, std::string const& p_replace) {
	std::string temp;
	do {
		temp = s;
		s = StringHelper::replace(s, p_find, p_replace);
	} while (temp != s);
	return temp;
}
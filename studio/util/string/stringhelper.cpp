/*
 * stringhelper.cpp
 * 
 * Copyright 2011-2020 Mj Mendoza IV <mj.mendoza.iv@gmail.com>
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
	
string StringHelper::toUpper(string p_str) {
	unsigned int i;
	
	for(i = 0; i < p_str.length(); ++i) {
		p_str[i] = toupper(p_str[i]);
	}
	
	return p_str;
}

string StringHelper::toLower(string p_str) {
	unsigned int i;
	for(i = 0; i < p_str.length(); ++i) {
		p_str[i] = tolower(p_str[i]);
	}
	return p_str;
}

string StringHelper::integerToString(int p_src) {
	if (p_src == 0) {
		return "0";
	}
	
	ostringstream l_ostringstream;
		l_ostringstream << p_src;
	return l_ostringstream.str();
}

string StringHelper::unsignedIntegerToString(unsigned int p_src) {
	if (p_src == 0) {
		return "0";
	}
	
	ostringstream l_ostringstream;
		l_ostringstream << p_src;
	return l_ostringstream.str();
}

string StringHelper::doubleToString(double p_src) {
	if (p_src == 0.0) {
		return "0.0";
	}
	
	ostringstream l_ostringstream;
		l_ostringstream << p_src;
	return l_ostringstream.str();
}

int StringHelper::toInteger(string p_src) {
	//TODO: validate string to be numerics
	return atoi(p_src.c_str());
}

unsigned int StringHelper::toUnsignedInteger(string p_src) {
	//TODO: validate string to be numerics
	return (unsigned)atoi(p_src.c_str());
}

long StringHelper::toLong(string p_src) {
	//TODO: validate string to be numerics
	return atol(p_src.c_str());
}

double StringHelper::toDouble(string p_src) {
	//TODO: validate string to be numerics
	return (double)atof(p_src.c_str());
}

bool StringHelper::toBoolean(string p_src) {
	if (p_src == "1" || StringHelper::toLower(p_src) == "true") {
		return true;
	} else {
		return false;
	}
}

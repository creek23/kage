/*
 * stringhelper.h
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

#ifndef UTIL_STRING_HELPER_H
	#define UTIL_STRING_HELPER_H
	
	#include <string>
	#include <sstream>
	#include <vector>
	#include <algorithm>
	#include <cstring>
	#include <iostream>
	
	class StringHelper {
		public:
			static std::string toUpper(std::string p_str);
			static std::string toLower(std::string p_str);
			static std::string integerToString(int p_src);
			static std::string unsignedIntegerToString(unsigned int p_src);
			static std::string doubleToString(double p_src);
			static int toInteger(std::string p_str);
			static unsigned int toUnsignedInteger(std::string p_str);
			static long toLong(std::string p_str);
			static double toDouble(std::string p_str);
			static bool toBoolean(std::string p_str);
			static std::vector<std::string> split(const std::string &p_source, const std::string &p_delimiter);
			static std::string trim(std::string p_source);
			static std::string kHash(std::string par, int p_len = 8);

			static std::string replace(std::string s, std::string const& p_find, std::string const& p_replace);
			static std::string replaceAll(std::string s, std::string const& p_find, std::string const& p_replace);
			
			static bool isScientificNotation(std::string p_value);
			static bool isNumeric(std::string p_value);
	};
	
#endif //UTIL_STRING_HELPER_H

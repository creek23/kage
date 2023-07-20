/*
 * stringhelper.h
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

#ifndef UTIL_STRING_HELPER_H
	#define UTIL_STRING_HELPER_H
	
	#include <string>
	#include <sstream>
	#include <vector>
	#include <algorithm>
	#include <cstring>
	using namespace std;
	
	class StringHelper {
		public:
			static string toUpper(string p_str);
			static string toLower(string p_str);
			static string integerToString(int p_src);
			static string unsignedIntegerToString(unsigned int p_src);
			static string doubleToString(double p_src);
			static int toInteger(string p_str);
			static unsigned int toUnsignedInteger(string p_str);
			static long toLong(string p_str);
			static double toDouble(string p_str);
			static bool toBoolean(string p_str);
			static vector<string> split(const string &p_source, const string &p_delimiter);
			static string trim(string p_source);
			static string kHash(string par, int p_len = 8);
	};
	
#endif //UTIL_STRING_HELPER_H

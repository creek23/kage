/*
 * cairo_kage.h
 * 
 * Copyright 2020-2024 Mj Mendoza IV <mj.mendoza.iv@gmail.com>
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

#ifndef UTIL_CAIRO_KAGE_H
	#define UTIL_CAIRO_KAGE_H
	
	#include <string>
	#include <cairomm/surface.h>
	#include <fstream> //ofstream
	
	class CairoKage {
		static std::ofstream pngFile;
		
		public:
			static Cairo::ErrorStatus writeToPNGStream(const unsigned char* data, unsigned int length);
			static bool writeToPNG(std::string p_path, Cairo::RefPtr<Cairo::ImageSurface> p_surface);
	};
	
#endif //UTIL_CAIRO_KAGE_H

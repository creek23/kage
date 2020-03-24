/*
 * cairo_kage.cpp
 * 
 * Copyright 2020 Mj Mendoza IV <mj.mendoza.iv@gmail.com>
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

#include "cairo_kage.h"

std::ofstream CairoKage::pngFile;

Cairo::ErrorStatus CairoKage::writeToPNGStream(const unsigned char* data, unsigned int length) {
	if (CairoKage::pngFile.is_open() == false) {
		return CAIRO_STATUS_WRITE_ERROR;
	}
	
	CairoKage::pngFile.write((char*)data, length);
	
	return CAIRO_STATUS_SUCCESS;
}

bool CairoKage::writeToPNG(string p_path, Cairo::RefPtr<Cairo::ImageSurface> p_surface) {
	CairoKage::pngFile.close();
	
	CairoKage::pngFile.open(p_path.c_str(), std::ofstream::binary);
	if (CairoKage::pngFile.is_open() == true) {
			p_surface->write_to_png_stream(&CairoKage::writeToPNGStream);
			CairoKage::pngFile.close();
		return true;
	}
	return false;
}

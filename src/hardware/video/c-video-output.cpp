/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Vinace
 * Copyright (C) P.Y. Rollo 2009 <dev@pyrollo.com>
 *
 * Vinace is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Vinace is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "c-video-output.hpp"

CVideoOutput::CVideoOutput() : pixels(QByteArray(255,0x00)) { }

const QByteArray &CVideoOutput::get_pixels() const {
    return pixels;
}

void CVideoOutput::set_pixel(int x, int y, apple_color color)
{
    pixels[y*VIDEO_OUTPUT_WIDTH+x] = color;
}

void CVideoOutput::set_dpixel(int x, int y, apple_color color)
{
    pixels[y*VIDEO_OUTPUT_WIDTH+x] = color;
    pixels[y*VIDEO_OUTPUT_WIDTH+x+1] = color;
}



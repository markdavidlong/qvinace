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

// Good tips about Apple graphics can be found there:
// http://www.lazilong.com/apple_II/a2pix/dgr2.htm

#ifndef _C_VIDEO_RENDERER_HPP_
#define _C_VIDEO_RENDERER_HPP_
#include "c-video-output.hpp"
#include "core/c-memory.hpp"
#include <QBitmap>

// Weird base address calculation for Apple graphics
#define GBASCALC(line) (((line&0x07)<<7)|((line&0x18)<<2)|(line&0x18))

class CVideoRenderer
{
public:
    CVideoRenderer(CMemory* memory, CVideoOutput* vo);
    virtual ~CVideoRenderer() { }
    virtual void render(int startline, int endline) = 0;
    virtual QImage &renderToBitmap(QImage &bitmap, int startline, int endline) = 0;

    void render_full();
    void render_top();
    void render_bottom();

    QImage &renderFullToBitmap(QImage &bitmap);
    QImage &renderTopToBitmap(QImage &bitmap);
    QImage &renderBottomToBitmap(QImage &bitmap);

protected:

    CVideoOutput *vo;
    CMemory      *mem;


};

#endif // _C_VIDEO_RENDERER_HPP_

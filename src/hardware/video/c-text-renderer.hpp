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

#ifndef _C_TEXT_RENDERER_HPP_
#define _C_TEXT_RENDERER_HPP_

#include "c-video-renderer.hpp"
#include "c-charset.hpp"
#include "font.h"

class CTextRenderer : public CVideoRenderer
{
public:

    CTextRenderer(CMemory* memory, CVideoOutput* vo):CVideoRenderer(memory, vo) {};
    virtual ~CTextRenderer() { }
    void set_charset(CCharset* charset);
    void set_font(Font *font);
    void renderChar(CVideoOutput *vo, uint8_t byte, int basex, int basey, bool flashon, bool dblwidth);
    virtual QImage& renderToBitmap(QImage &bitmap, int startline, int endline) = 0;
    virtual void renderCharToBitmap(QPainter &p, uint8_t byte, int offset_x, int offset_y, bool flashon, bool dblwidth);

protected:
    CCharset *charset;
    Font *font;

};

class CTxt40ColRenderer: public CTextRenderer
{
public:
    CTxt40ColRenderer(CMemory* memory, CVideoOutput* vo):CTextRenderer(memory, vo) {};
    void render(int startline, int endline);
    virtual QImage &renderToBitmap(QImage& bitmap, int startline, int endline);

protected:

};

class CTxt80ColRenderer: public CTextRenderer
{
public:
    CTxt80ColRenderer(CMemory* mainMemory, CMemory* auxMemory, CVideoOutput* vo);
    void render(int startline, int endline);
    virtual QImage &renderToBitmap(QImage &bitmap, int startline, int endline);

protected:
    CMemory  *aux;
};

#endif // _C_TEXT_RENDERER_HPP_

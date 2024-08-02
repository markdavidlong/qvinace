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

#pragma once

#include "c-video-renderer.hpp"
//#include "c-gr-renderer.hpp"
#include "core/units/c-graphic-mode-unit.hpp"

class CGraphicModeRenderer: public CVideoRenderer
{
public:
    CGraphicModeRenderer(CMemory* memory, CGraphicModeUnit* graphicModeUnit, CVideoOutput *vo, bool color);
    virtual ~CGraphicModeRenderer();
    void render(int startline, int endline);
    virtual QImage &renderToBitmap(QImage &bitmap, int startline, int endline);

protected:
    CGraphicModeUnit* graphicModeUnit;

    // Memory pages used for display
    CMemory *memgr[2];
    CMemory *memhgr[2];

    // Renderers for display modes
    CVideoRenderer  *gr[2];
    CVideoRenderer *hgr[2];

};




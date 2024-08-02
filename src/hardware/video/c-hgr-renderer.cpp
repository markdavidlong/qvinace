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

#include "c-hgr-renderer.hpp"
#include <QPainter>
#include <QDebug>

// B&W mode color correspondance
// Index is the bit of scanline
apple_color hgrbw[2] = {col_black, col_white};

// Color mode color correspondance
// Fist index is the 3 bits of scanline, second index is shifted, even
apple_color hgrcolor[8][4] = {
    {col_black, col_black, col_black, col_black},
    {col_black, col_black, col_black, col_black},
    {col_purple, col_light_green, col_medium_blue, col_orange},
    {col_white, col_white, col_white, col_white},
    {col_black, col_black, col_black, col_black},
    {col_light_green, col_purple, col_orange, col_medium_blue},
    {col_white, col_white, col_white, col_white},
    {col_white, col_white, col_white, col_white}
};

CHgrRenderer::CHgrRenderer(CMemory* memory, CVideoOutput* vo, bool color):CVideoRenderer(memory, vo) {
    this->color = color;
};

void CHgrRenderer::render(int startline, int endline) {
    int column, x, y, scanx, bascalc;
    BYTE byte;
    apple_color pixel;
    char scanbits,shifted;

    bool scanline[282]; // 280 pixels + 2 out of screen pixels for color computation
    scanline[0]   = false;
    scanline[281] = false;

    bool shifteds[40]; // Shifted bit (B7 of each column)

    for (y=startline*8; y<endline*8+8; y++)
    {
        // First pass : decode bits
        bascalc = GBASCALC(y>>3)|((y&0x7)<<10);
        scanx = 1;
        for (column=0; column<40; column++) {
            byte = mem->read(bascalc++);
            shifteds[column] = byte & 0x80;
            scanline[scanx++] = byte & 0x01;
            scanline[scanx++] = byte & 0x02;
            scanline[scanx++] = byte & 0x04;
            scanline[scanx++] = byte & 0x08;
            scanline[scanx++] = byte & 0x10;
            scanline[scanx++] = byte & 0x20;
            scanline[scanx++] = byte & 0x40;
        }

        // Second pass : render pixels
        x=0; scanx=2;
        scanbits = (scanline[0]?1<<1:0)|(scanline[1]?1:0);
        pixel = col_black; shifted = false; // Previous pixel state (out of screen = black, not shifted)

        for (column=0; column<40; column++) {
            shifted = shifteds[column]<<1;
            // If shifted segment, first half pixel repeats the previous pixel color
            if (shifted) vo->set_pixel(x++,y, pixel);  // Pixels are now shifted (x++)

            if (color) {
                scanbits = ((scanbits<<1)|(scanline[scanx]?1:0))&0x7;
                pixel = hgrcolor[int(scanbits)][int(shifted|(scanx&0x01))];
                scanx++;
                vo->set_pixel(x++,y, pixel);
                vo->set_pixel(x++,y, pixel);
                scanbits = ((scanbits<<1)|(scanline[scanx]?1:0))&0x7;
                pixel = hgrcolor[(int) scanbits][shifted|(scanx&0x01)];
                scanx++;

                vo->set_pixel(x++,y, pixel);
                vo->set_pixel(x++,y, pixel);
                scanbits = ((scanbits<<1)|(scanline[scanx]?1:0))&0x7;
                pixel = hgrcolor[(int)scanbits][shifted|(scanx&0x01)];
                scanx++;
                vo->set_pixel(x++,y, pixel);
                vo->set_pixel(x++,y, pixel);
                scanbits = ((scanbits<<1)|(scanline[scanx]?1:0))&0x7;
                pixel = hgrcolor[int(scanbits)][shifted|(scanx&0x01)];
                scanx++;
                vo->set_pixel(x++,y, pixel);
                vo->set_pixel(x++,y, pixel);
                scanbits = ((scanbits<<1)|(scanline[scanx]?1:0))&0x7;
                pixel = hgrcolor[int(scanbits)][shifted|(scanx&0x01)];
                scanx++;
                vo->set_pixel(x++,y, pixel);
                vo->set_pixel(x++,y, pixel);
                scanbits = ((scanbits<<1)|(scanline[scanx]?1:0))&0x7;
                pixel = hgrcolor[int(scanbits)][shifted|(scanx&0x01)];
                scanx++;
                vo->set_pixel(x++,y, pixel);
                vo->set_pixel(x++,y, pixel);
                scanbits = ((scanbits<<1)|(scanline[scanx]?1:0))&0x7;
                pixel = hgrcolor[int(scanbits)][shifted|(scanx&0x01)];
                scanx++;
                vo->set_pixel(x++,y, pixel);
            } else {
                pixel = hgrbw[scanline[scanx++]];
                vo->set_pixel(x++,y, pixel);
                vo->set_pixel(x++,y, pixel);
                pixel = hgrbw[scanline[scanx++]];
                vo->set_pixel(x++,y, pixel);
                vo->set_pixel(x++,y, pixel);
                pixel = hgrbw[scanline[scanx++]];
                vo->set_pixel(x++,y, pixel);
                vo->set_pixel(x++,y, pixel);
                pixel = hgrbw[scanline[scanx++]];
                vo->set_pixel(x++,y, pixel);
                vo->set_pixel(x++,y, pixel);
                pixel = hgrbw[scanline[scanx++]];
                vo->set_pixel(x++,y, pixel);
                vo->set_pixel(x++,y, pixel);
                pixel = hgrbw[scanline[scanx++]];
                vo->set_pixel(x++,y, pixel);
                vo->set_pixel(x++,y, pixel);
                pixel = hgrbw[scanline[scanx++]];
                vo->set_pixel(x++,y, pixel);
            }
            // If not shifted, we have to set the 14th half pixel (we are on 13th)
            if (!shifted) vo->set_pixel(x++,y, pixel);
        }
    }
}

QImage &CHgrRenderer::renderToBitmap(QImage &bitmap, int startline, int endline)
{
    //    qDebug() << "Hgr: Start/End: " << startline << endline;
    QPainter p(&bitmap);
    p.setBrush(Qt::black);
    p.setPen(Qt::black);
    p.drawRect (0,startline*8,bitmap.width(),((endline-startline)*8)+7);
    p.setBrush(Qt::white);
    p.setPen(Qt::white);

    int column, x, y, scanx, bascalc;
    BYTE byte;
    bool pixel;
    bool shifted;

    bool scanline[280];
    scanline[0]   = false;

    QPolygon poly;

    bool shifteds[40]; // Shifted bit (B7 of each column)

    for (y=startline*8; y<endline*8+8; y++)
    {
        // First pass : decode bits
        bascalc = GBASCALC(y>>3)|((y&0x7)<<10);
        scanx = 0;

        for (column=0; column<40; column++) {
            byte = mem->read(bascalc++);

            shifteds[column] = byte & 0x80;
            scanline[scanx++] = byte & 0x01;
            scanline[scanx++] = byte & 0x02;
            scanline[scanx++] = byte & 0x04;
            scanline[scanx++] = byte & 0x08;
            scanline[scanx++] = byte & 0x10;
            scanline[scanx++] = byte & 0x20;
            scanline[scanx++] = byte & 0x40;
        }

        // Second pass : render pixels
        x=0; scanx=0;

        pixel = false; shifted = false; // Previous pixel state (out of screen = black, not shifted)

        for (column=0; column<40; column++) {
            shifted = shifteds[column];//<<1;
            // If shifted segment, first half pixel repeats the previous pixel color
            if (shifted) // Pixels are now shifted (x++)
            {
                if (pixel)
                {
                    poly.append(QPoint(x,y));
                }
                x++;
            }

            for (int tmp = 0; tmp < 6; tmp++)
            {
                pixel = scanline[scanx++];
                if (pixel)
                {
                    poly.append(QPoint(x,y));
                    poly.append(QPoint(x+1,y));
                }
                x+=2;
            }

            pixel = scanline[scanx++];
            if (pixel)
            {
                poly.append(QPoint(x++,y));
                if (!shifted) poly.append(QPoint(x++,y));
            }
            else
            {
                x++;
                if (!shifted) x++;
            }

        }
    }
    p.drawPoints(poly);
    bitmap.setText("gr","hires");

    return bitmap;
}

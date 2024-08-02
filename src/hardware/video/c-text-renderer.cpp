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


#include "c-text-renderer.hpp"
// Needed for text flashing
#include <QDateTime>
#include <QDebug>
#include <QPainter>

#include <iostream>

void CTextRenderer::set_charset(CCharset* charset) {
    this->charset = charset;
}

void CTextRenderer::set_font(Font* font) {
    this->font = font;
}

void  CTextRenderer::renderChar(CVideoOutput *vo, BYTE byte, int basex, int basey, bool flashon, bool dblwidth) {
    //int x;
  //  qDebug() << "RenderChar " << byte;
    int y;
    BYTE *chr;
    BYTE mask;
    char_mode mode;

    chr = charset->get_char(byte);
    mode = charset->get_mode(byte);
    if (mode == mode_inverse || (mode == mode_flash && flashon))
        mask = 0x7f;
    else
        mask = 0x00;

    for (y=0; y<8; y++) {
        byte = chr[y]^mask;
        if (dblwidth) {
            vo->set_dpixel(basex+12, basey+y, (byte&0x01)?col_white:col_black);
            vo->set_dpixel(basex+10, basey+y, (byte&0x02)?col_white:col_black);
            vo->set_dpixel(basex+ 8, basey+y, (byte&0x04)?col_white:col_black);
            vo->set_dpixel(basex+ 6, basey+y, (byte&0x08)?col_white:col_black);
            vo->set_dpixel(basex+ 4, basey+y, (byte&0x10)?col_white:col_black);
            vo->set_dpixel(basex+ 2, basey+y, (byte&0x20)?col_white:col_black);
            vo->set_dpixel(basex   , basey+y, (byte&0x40)?col_white:col_black);
        } else {
            vo->set_pixel(basex+6, basey+y, (byte&0x01)?col_white:col_black);
            vo->set_pixel(basex+5, basey+y, (byte&0x02)?col_white:col_black);
            vo->set_pixel(basex+4, basey+y, (byte&0x04)?col_white:col_black);
            vo->set_pixel(basex+3, basey+y, (byte&0x08)?col_white:col_black);
            vo->set_pixel(basex+2, basey+y, (byte&0x10)?col_white:col_black);
            vo->set_pixel(basex+1, basey+y, (byte&0x20)?col_white:col_black);
            vo->set_pixel(basex  , basey+y, (byte&0x40)?col_white:col_black);
        }
    }
}

void CTextRenderer::renderCharToBitmap(QPainter &p,BYTE byte, int offset_x, int offset_y, bool flashon, bool dblwidth)
{
    if (byte & 0b10000000)  // Inverse
    {
        byte = byte + 0;
    }
    else if  (byte & 0b01000000)  // Flash
    {
        if (flashon)  // Flash in INVERSE state
        {
            byte += 0x40;
        }
        else // Flash in NORMAL state
        {
            byte -= 0x40;
        }
    }
    else // Normal
    {
        byte = byte + 0;
    }


    QRect cell = QRect(offset_x,offset_y,dblwidth?14:7,8);
    font->paintGlyph(&p,byte,cell,flashon);
}

/******************************************************************************/


void CTxt40ColRenderer::render(int startline, int endline) {
 //   int x, y;
 //   BYTE* chr;
 int line, column, basex, basey;
    BYTE byte;
    //BYTE mask;
    //char_mode mode;
    bool flashon;

    flashon = (QDateTime::currentMSecsSinceEpoch() % 1000) < 500;
    if (charset) {
        basey = startline * 8;
        for (line=startline; line<=endline; line++) {
            basex = 0;
            for (column=0; column<40; column++) {
                byte = mem->read(GBASCALC(line) + column);
                renderChar(vo, byte, basex, basey, flashon, true);
                basex+=14;
            }
            basey+=8;
        }
    }
    else
        printf("No charset!!\n");
}

QImage &CTxt40ColRenderer::renderToBitmap(QImage &bitmap, int startline, int endline)
{
    QPainter p(&bitmap);
    p.setBrush(Qt::red);
    p.setPen(Qt::red);
    p.drawRect (0,startline*8,bitmap.width(),((endline-startline)*8)+7);
    p.setBrush(Qt::white);
    p.setPen(Qt::white);

    int line, column, basex, basey;
    BYTE byte;

    bool flashon;


   flashon = (QDateTime::currentMSecsSinceEpoch() % 458) < 229;

    if (charset) {
        basey = startline * 8;
        for (line=startline; line<=endline; line++) {
            basex = 0;
            for (column=0; column<40; column++) {
                byte = mem->read(GBASCALC(line) + column);
//                qDebug() <<
//                    "Preparing to render char " <<  byte << " at " << basex << ","
//                         << basey << "With active painter" << p.isActive();
                renderCharToBitmap(p,byte,basex,basey,flashon,true);
                basex+=14;
            }
            basey+=8;
        }
    }
    else
        printf("No charset!!\n");
    bitmap.setText("text","40");

    return bitmap;
}

/******************************************************************************/


CTxt80ColRenderer::CTxt80ColRenderer(CMemory* mainMemory, CMemory* auxMemory, CVideoOutput* vo):CTextRenderer(mainMemory, vo) {
    this->aux = auxMemory;
}

void CTxt80ColRenderer::render(int startline, int endline) {
    int line, column, basex, basey;

    BYTE byte;

    // FIXME: This is hokey.
    bool flashon;
    flashon = (QDateTime::currentMSecsSinceEpoch() % 1000) < 500;


    if (charset) {
        basey = 0;
        for (line=startline; line<=endline; line++) {
            basex = 0;
            for (column=0; column<40; column++) {
                byte = aux->read(GBASCALC(line) + column - 1);
                renderChar(vo, byte, basex, basey, flashon, false);
                basex+=7;
                byte = mem->read(GBASCALC(line) + column);
                renderChar(vo, byte, basex, basey, flashon, false);
                basex+=7;
            }
            basey+=8;
        }
    }
    else
        printf("No charset!!\n");
}

QImage &CTxt80ColRenderer::renderToBitmap(QImage &bitmap, int startline, int endline)
{
    QPainter p(&bitmap);
    p.setBrush(Qt::magenta);
    p.setPen(Qt::magenta);
    p.drawRect (0,startline*8,bitmap.width(),((endline-startline)*8)+7);
    p.setBrush(Qt::black);
    p.setPen(Qt::black);

    int line, column, basex, basey;
    BYTE byte;

    bool flashon;


    flashon = (QDateTime::currentMSecsSinceEpoch() % 458) < 224;
    if (charset) {
        basey = startline * 8;
        for (line=startline; line<=endline; line++) {
            basex = 0;
            for (column=0; column<40; column++) {
                byte = aux->read(GBASCALC(line) + column - 1); // A verifier en 128K
                renderCharToBitmap(p,byte,basex,basey,flashon,false);
                basex+=7;

                byte = mem->read(GBASCALC(line) + column);
                renderCharToBitmap(p,byte,basex,basey,flashon,false);
                basex+=7;
            }
            basey+=8;
        }
    }
    else
        printf("No charset!!\n");

    bitmap.setText("text","80");
    return bitmap;
}


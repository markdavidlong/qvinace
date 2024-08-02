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

#include "c-main-video-output.hpp"
#include <QDebug>

CMainVideoOutput::CMainVideoOutput(CMemory *main, CMemory *aux, CTextModeUnit* textModeUnit, CGraphicModeUnit* graphicModeUnit, bool color)
{
    // Main members
    this->m_renderbitmap =  QImage(560,192,QImage::Format_Grayscale8);
    this->m_renderbitmap.fill(Qt::black);
    this->graphicModeUnit = graphicModeUnit;

    // Sub-renderers
    this->textModeRenderer    = new CTextModeRenderer(main, aux, textModeUnit, graphicModeUnit, this);
    this->graphicModeRenderer = new CGraphicModeRenderer(main, graphicModeUnit, this, color);
}

CMainVideoOutput::~CMainVideoOutput() {
    delete textModeRenderer;
    delete graphicModeRenderer;
}

void CMainVideoOutput::render()
{
 //   qDebug("CMainVideoOutput::render()");
    if (graphicModeUnit->get_text()) {
        textModeRenderer->render_full();
    } else {
        if (graphicModeUnit->get_mixed()) {
            textModeRenderer->render_bottom();
            graphicModeRenderer->render_top();
        } else {
            graphicModeRenderer->render_full();
        }
    }
}

QPixmap &CMainVideoOutput::renderToPixmap()
{
    qFatal("renderToPixmap not implemented");
    return m_rendermap;
}

QImage &CMainVideoOutput::renderToBitmap()
{
    if (graphicModeUnit->get_text()) {
   //     qDebug() << " *** Rendering Full Text";

        m_renderbitmap.setText("mode","text");
        m_renderbitmap.setText("gr","");
        textModeRenderer->renderFullToBitmap(m_renderbitmap);
    } else {
        if (graphicModeUnit->get_mixed()) {
//            qDebug() <<
   //             " *** Rendering Mixed Graphics/Text";
            m_renderbitmap.setText("mode","mixed");
            textModeRenderer->renderBottomToBitmap(m_renderbitmap);
            graphicModeRenderer->renderTopToBitmap(m_renderbitmap);
        } else {
    //        qDebug() <<" *** Rendering Full Graphics";
            m_renderbitmap.setText("mode","full");
            m_renderbitmap.setText("text","");
            graphicModeRenderer->renderFullToBitmap(m_renderbitmap);
        }
    }

    return m_renderbitmap;
}



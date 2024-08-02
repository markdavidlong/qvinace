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


#ifndef _C_GUI_MONITOR_HPP_
#define _C_GUI_MONITOR_HPP_

//#include <QWidget>
#include <QImage>
#include <QQuickWidget>
#include <QElapsedTimer>

#include <QQuickImageProvider>
#include "hardware/video/c-video-output.hpp"

class CGuiMonitor:public QQuickWidget, public QQuickImageProvider
{
public:
    CGuiMonitor(CVideoOutput *vo, QWidget *parent = 0);
    virtual ~CGuiMonitor();
    QImage &renderToBitmap();
    bool on_timer_event();

    virtual QImage requestImage(const QString &id, QSize *size,
                                const QSize &requestedSize);

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

public slots:
    void saveBuffer();
protected:
  //  void paintEvent(QPaintEvent *event);

protected:
    QImage background;
    QImage rendered;

    CVideoOutput *vo;

private:
    int xmargin, ymargin;
    QElapsedTimer tmr;
};


#endif // _C_GUI_MONITOR_HPP_

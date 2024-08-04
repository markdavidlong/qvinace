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


#include <QPainter>

#include "c-gui-monitor.hpp"
#include <QImage>
#include <QLatin1String>
#include <QDebug>
#include <QQuickItem>
#include <QQmlProperty>


QImage MyProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize) {
    return m_mon->requestImage(id,size,requestedSize);
}

CGuiMonitor::CGuiMonitor(CVideoOutput *vo,  QWidget *parent):
    QQuickWidget(parent)//,QQuickImageProvider(QQmlImageProviderBase::Image)
{
    this->vo = vo;
    m_provider = new MyProvider(this);
    engine()->addImageProvider("crt",m_provider);
    setSource(QUrl::fromLocalFile(":GuiMonitor.qml"));

    setResizeMode(ResizeMode::SizeRootObjectToView);
    auto width = VIDEO_OUTPUT_WIDTH+16;
    auto height =VIDEO_OUTPUT_HEIGHT*2;
    //   this->background = QImage(width,height,QImage::Format::Format_ARGB32);
    //   this->background.fill(Qt::black);
    //   this->rendered = QImage(this->background); // Copy background

    //  xmargin = (background.width() - VIDEO_OUTPUT_WIDTH)/2;
    //  ymargin = background.height()/2 - VIDEO_OUTPUT_HEIGHT; // Pixels are twice as hight than they are wide
    // resize(rendered.width(), rendered.height());
    resize(width,height);

    QSizePolicy sp(
        QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    setSizePolicy(sp);
    updateGeometry();
}

CGuiMonitor::~CGuiMonitor()
{
    delete m_provider;
}

QSize CGuiMonitor::sizeHint() const
{
    return QSize(576,384);
}

QSize CGuiMonitor::minimumSizeHint() const
{
    return QSize(576,384);
}

bool CGuiMonitor::on_timer_event() {

    //   rendered = QImage(renderToBitmap());

    return true;
}

QImage CGuiMonitor::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(id);
    Q_UNUSED(requestedSize);

//    qDebug("Requesting image %s",qPrintable(id));
#if 0
    QImage testimg(576,384,QImage::Format_ARGB32_Premultiplied);
    testimg.fill(Qt::black);
    QPainter p(&testimg);
    p.setPen(Qt::white);

    int x = qrand() % 576;
    int y = qrand() % 384;
    for (int idx = 0; idx < 576; idx+=8) {
       p.drawLine(x,y,idx,383);
         p.drawLine(x,y,idx,0);
    }
    for (int idx = 0; idx < 384; idx+=8)
    {
        p.drawLine(x,y,575,idx);
           p.drawLine(x,y,0,idx);
    }
    if (size) *size = testimg.size();
 //   qDebug() << (testimg.isNull()?"Null Image":"Non-Null Image");
    return testimg;
#else
    QImage new_img(576,384,QImage::Format_RGBA64_Premultiplied);
    if (status() ==  QQuickWidget::Status::Ready)
    {
        QImage old = renderToBitmap().scaled(560,384);
        //Todo: Grab Keys and set QML status based on graphics mode.

        new_img.fill(Qt::black);
        QPainter p(&new_img);
        p.drawImage(8,0,old);
        if (size) *size = new_img.size();

        QString screenMode = old.text("mode");  // text, mixed, or full
        QString graphicsRes = old.text("gr");   // hires or lores
        QString textWidth = old.text("text");   // 40 or 80

        auto ro = rootObject();
        if (ro)
        {
            if (screenMode == "text")
            {
                QQmlProperty(ro,"colorKiller").write(true);
            }
            else
            {
                QQmlProperty(ro,"colorKiller").write(false);
            }

        } else qDebug() << "No root object!";


    } else {qDebug("QML Not ready yet"); }
    return new_img;


#endif
}

void CGuiMonitor::saveBuffer()
{
    rendered = renderToBitmap().scaled(560,24*16);
    qDebug() << "Saving buffer.";
    bool result = rendered.save("buffer.png");
    qDebug() << (result?"Saved ok.":"Could not save.");

}



//void CGuiMonitor::paintEvent(QPaintEvent * /*event*/)
//{
//    qDebug("Elapsed: %ld",tmr.restart());

//    QPainter painter(this);
//    painter.setBrush(Qt::black);
//    painter.setPen(Qt::black);
//    painter.drawRect(this->rect());

//    painter.drawImage(xmargin,ymargin,renderToBitmap().scaled(560,24*16));
//}


QImage & CGuiMonitor::renderToBitmap()
{
    return vo->renderToBitmap();
}

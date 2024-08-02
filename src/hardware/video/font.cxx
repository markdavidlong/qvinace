#include "font.h"

#include <QFile>
#include <QDebug>
#include <QPainter>


Font::Font(QString filename)
{
    m_char_data_width = 8;
    if (filename != "") { load(filename); }
}

Font::~Font()
{

}

bool Font::load(QString filename)
{
    bool retval = true;


    // (Mousetext = pos 64-96)
    QFile infile(filename); //":/roms/char2.rom");

    if (infile.open(QIODevice::ReadOnly))
    {
        //m_raw_glyphs_40.clear();

        m_raw_glyphs.clear();

        QByteArray tmp = infile.readAll();

        //        qDebug() << "Read" << tmp.size() << "bytes";
        //        qDebug() << "Resizing to " << 8*256;
        tmp.resize(8*256);


        //        for (int idx = 0; idx < tmp.size(); idx++)
        //        {
        //            tmp[idx] = tmp[idx] ^ 0xff;
        //        }

        while (tmp.size())
        {
            QByteArray ch = tmp.left(8);
            tmp.remove(0,8);
            //QImage bm((const uchar*) ch.data(), m_char_data_width-1,8, QImage::Format_MonoLSB);

            QImage bm(7,8,QImage::Format_Mono);
            bm.fill(Qt::black);
            QPainter p(&bm);
            p.setPen(Qt::red);
            p.setBrush(Qt::white);

            for (int idx = 0; idx < 8; idx++)
            {
                if (ch[idx] & 0b01000000) { p.drawPoint(6,idx); }
                if (ch[idx] & 0b00100000) { p.drawPoint(5,idx); }
                if (ch[idx] & 0b00010000) { p.drawPoint(4,idx); }
                if (ch[idx] & 0b00001000) { p.drawPoint(3,idx); }
                if (ch[idx] & 0b00000100) { p.drawPoint(2,idx); }
                if (ch[idx] & 0b00000010) { p.drawPoint(1,idx); }
                if (ch[idx] & 0b00000001) { p.drawPoint(0,idx); }
            }

            m_raw_glyphs.append(bm);

            QImage inverted = QImage(bm);
            inverted.invertPixels();

            m_inverted_glyphs.append(inverted);

        }
        infile.close();
        qDebug() << "Loaded font " << filename;
        qDebug() << "  with " << m_raw_glyphs.count() << "glyphs and ";
        qDebug() << "  with " << m_inverted_glyphs.count() << "inverted glyphs";

    }
    else
    {
        qWarning() << "Could not load font " << filename;
        retval = false;
    }


    QImage testimg(7,8,QImage::Format_ARGB32_Premultiplied);
    testimg.fill(Qt::black);
    QPainter tp(&testimg);
    tp.setPen(Qt::white);
    tp.setBrush(Qt::white);
    tp.drawLine(0,0,6,6);
    tp.drawLine(6,1,0,7);
    tp.end();
    testimg.invertPixels();
    tmpbm = testimg;
    return retval;
}

bool Font::paintGlyph(QPainter *painter, quint8 value,
                      QRect loc, bool invert)
{

        //   qDebug() << "paintGlyph: " << value << loc << invert;
    if (!painter || !painter->isActive())
    {
        qWarning() << "Painter inactive!";
        return false;
    }

    if (value >= m_raw_glyphs.count())
    {
        qWarning()<< "Glyph Index " << value <<" out of range.";
        return false;

    }


    //    if (invert)
    //    {
    //        qDebug() << "Inverted";
    //        painter->drawImage(loc,*m_inverted_glyphs[value]);
    //    }
    //    else
    {
        //        qDebug() << "Not inverted";
        //        qDebug() << "A";
        //        Q_ASSERT(value < m_raw_glyphs.size());
        //        qDebug() << "B";
        //        qDebug() << "C";
        //        qDebug() << "Loc = " << loc;
        //painter->drawRect(loc);
        painter->drawImage(loc,m_raw_glyphs[value]);
        //        painter->drawImage(loc,tmpbm);
        //        qDebug() << "D";
    }

    //    qDebug() << "Done painting!";
    return true;
}

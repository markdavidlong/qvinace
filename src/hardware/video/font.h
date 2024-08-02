#ifndef FONT_H
#define FONT_H

#include <QString>
#include <QImage>
#include <QSharedPointer>
#include <QList>
#include <QPainter>


class Font
{
public:
    Font(QString filename ="");
    ~Font();
    bool load(QString filename = "");
    bool paintGlyph(QPainter *painter, quint8 value,
                    QRect loc, bool invert);
protected:
    quint8 m_char_data_width;
    QList<QImage> m_raw_glyphs;
    QList<QImage> m_inverted_glyphs;

    QImage tmpbm;
};


#endif // FONT_H

#include "awesomeicon.h"
#include <QIcon>
#include <QLabel>
#include <QPainter>
QIcon AwesomeIcon::setIcon(const char *icon_unicode)
{
    QPixmap *pixmap = new QPixmap(50, 50);
    pixmap->fill(Qt::transparent);
    QLabel *label = new QLabel;
    label->resize(50, 50);
    QFont font;
    font.setFamily("FontAwesome");
    font.setPixelSize(35);
    label->setFont(font);
    label->setText(icon_unicode);

    QFontMetrics fm(font);
    int pixelsWide = fm.width(label->text());
    int pixelsHigh = fm.height();

    QPainter *painter = new QPainter(pixmap);
    painter->setPen("#333333");
    painter->setFont(font);
    painter->drawText(0 + (50 - pixelsWide) / 2, 50 - (50-pixelsHigh) /2, label->text());
    QIcon *icon = new QIcon(*pixmap);
    return *icon;
}


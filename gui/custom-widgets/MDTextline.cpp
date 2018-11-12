#include "MDTextline.hpp"

#include <QStylePainter>


int MDTextline::getLabelX() const { return labelX; }

void MDTextline::setLabelX(int value) { labelX = value; }

int MDTextline::getLabelY() const { return labelY; }

void MDTextline::setLabelY(int value) { labelY = value; }

int MDTextline::getLabelTextWidth() const { return labelTextWidth; }

void MDTextline::setLabelTextWidth(int value) { labelTextWidth = value; }

QColor MDTextline::getLabelColor() const { return labelColor; }

void MDTextline::setLabelColor(const QColor& value) { labelColor = value; }

QColor MDTextline::getLabelFocusColor() const { return labelFocusColor; }

void MDTextline::setLabelFocusColor(const QColor& value) { labelFocusColor = value; }

MDTextline::MDTextline(QWidget* parent) :
    QLineEdit(parent) {

}


void MDTextline::paintEvent(QPaintEvent* event) {

    QLineEdit::paintEvent(event);

    QStylePainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QPalette pal = palette();
    QPen pen;
    QColor labelColor;
    if (this->hasFocus())
        labelColor = this->labelFocusColor;
    else
        labelColor = pal.text().color();

    pen.setColor(labelColor);
    p.setPen(pen);
    QFont font = this->font();
    font.setPixelSize(getLabelTextWidth());
    p.setFont(font);

    p.drawText(QPoint(this->getLabelX(), getLabelY()), tr("Qt by\nThe Qt Company"));

//    int l, t, r, b;
//    this->getContentsMargins(&l, &t, &r, &b);
//    int x, y;
//    x = this->width();
//    y = this->height();
//    QRect rect = this->rect();
//    int offset = this->getLabelVerticalDistance();
//    rect.asdjust(0, offset, 0, 0);

}

#include "MDLineEdit.hpp"

#include <QStylePainter>


int MDLineEdit::getLabelX() const { return labelX; }

void MDLineEdit::setLabelX(int value) { labelX = value; }

int MDLineEdit::getLabelY() const { return labelY; }

void MDLineEdit::setLabelY(int value) { labelY = value; }

int MDLineEdit::getLabelTextWidth() const { return labelTextWidth; }

void MDLineEdit::setLabelTextWidth(int value) { labelTextWidth = value; }

QColor MDLineEdit::getLabelColor() const { return labelColor; }

void MDLineEdit::setLabelColor(const QColor& value) { labelColor = value; }

QColor MDLineEdit::getLabelFocusColor() const { return labelFocusColor; }

void MDLineEdit::setLabelFocusColor(const QColor& value) { labelFocusColor = value; }

QString MDLineEdit::getLabel() const { return label; }

void MDLineEdit::setLabel(const QString& value) { label = value; }

MDLineEdit::MDLineEdit(QWidget* parent) :
    QLineEdit(parent), label("") {

}


void MDLineEdit::paintEvent(QPaintEvent* event) {

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

    p.drawText(QPoint(this->getLabelX(), getLabelY()), label);

//    int l, t, r, b;
//    this->getContentsMargins(&l, &t, &r, &b);
//    int x, y;
//    x = this->width();
//    y = this->height();
//    QRect rect = this->rect();
//    int offset = this->getLabelVerticalDistance();
//    rect.asdjust(0, offset, 0, 0);

}

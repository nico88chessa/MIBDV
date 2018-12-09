#include "MDSpinBox.hpp"

#include <QStylePainter>


int MDSpinBox::getLabelX() const { return labelX; }

void MDSpinBox::setLabelX(int value) { labelX = value; }

int MDSpinBox::getLabelY() const { return labelY; }

void MDSpinBox::setLabelY(int value) { labelY = value; }

int MDSpinBox::getLabelTextWidth() const { return labelTextWidth; }

void MDSpinBox::setLabelTextWidth(int value) { labelTextWidth = value; }

QColor MDSpinBox::getLabelColor() const { return labelColor; }

void MDSpinBox::setLabelColor(const QColor& value) { labelColor = value; }

QColor MDSpinBox::getLabelFocusColor() const { return labelFocusColor; }

void MDSpinBox::setLabelFocusColor(const QColor& value) { labelFocusColor = value; }

QString MDSpinBox::getLabel() const { return label; }

void MDSpinBox::setLabel(const QString& value) { label = value; }

MDSpinBox::MDSpinBox(QWidget* parent) :
    QSpinBox(parent), label("") {

    this->setButtonSymbols(ButtonSymbols::NoButtons);

}


void MDSpinBox::paintEvent(QPaintEvent* event) {

    QSpinBox::paintEvent(event);

    QStylePainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QPalette pal = palette();
    QPen pen;
    QColor labelColor;
    if (this->hasFocus() && !this->isReadOnly())
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

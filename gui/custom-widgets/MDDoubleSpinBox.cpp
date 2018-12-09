#include "MDDoubleSpinBox.hpp"

#include <QStylePainter>


int MDDoubleSpinBox::getLabelX() const { return labelX; }

void MDDoubleSpinBox::setLabelX(int value) { labelX = value; }

int MDDoubleSpinBox::getLabelY() const { return labelY; }

void MDDoubleSpinBox::setLabelY(int value) { labelY = value; }

int MDDoubleSpinBox::getLabelTextWidth() const { return labelTextWidth; }

void MDDoubleSpinBox::setLabelTextWidth(int value) { labelTextWidth = value; }

QColor MDDoubleSpinBox::getLabelColor() const { return labelColor; }

void MDDoubleSpinBox::setLabelColor(const QColor& value) { labelColor = value; }

QColor MDDoubleSpinBox::getLabelFocusColor() const { return labelFocusColor; }

void MDDoubleSpinBox::setLabelFocusColor(const QColor& value) { labelFocusColor = value; }

QString MDDoubleSpinBox::getLabel() const { return label; }

void MDDoubleSpinBox::setLabel(const QString& value) { label = value; }

MDDoubleSpinBox::MDDoubleSpinBox(QWidget* parent) :
    QDoubleSpinBox(parent), label("") {

    this->setButtonSymbols(ButtonSymbols::NoButtons);

}


void MDDoubleSpinBox::paintEvent(QPaintEvent* event) {

    QDoubleSpinBox::paintEvent(event);

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

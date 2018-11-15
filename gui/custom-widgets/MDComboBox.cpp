#include "MDComboBox.hpp"

#include <QListView>
#include <QStylePainter>

QString MDComboBox::getLabelText() const { return labelText; }

void MDComboBox::setLabelText(const QString& value) { labelText = value; }

int MDComboBox::getLabelLeft() const { return labelLeft; }

void MDComboBox::setLabelLeft(int value) { labelLeft = value; }

int MDComboBox::getLabelTop() const { return labelTop; }

void MDComboBox::setLabelTop(int value) { labelTop = value; }

int MDComboBox::getLabelTextWidth() const { return labelTextWidth; }

void MDComboBox::setLabelTextWidth(int value) { labelTextWidth = value; }

QColor MDComboBox::getLabelHoverColor() const { return labelHoverColor; }

void MDComboBox::setLabelHoverColor(const QColor& value) { labelHoverColor = value; }

MDComboBox::MDComboBox(QWidget* parent) : QComboBox(parent) {
    this->setView(new QListView());
}

void MDComboBox::paintEvent(QPaintEvent* event) {

    QComboBox::paintEvent(event);

    QStylePainter p(this);
    QPalette pal(this->palette());
    QRect rect = this->rect();

    QPen pen;
    QColor labelColor;
    if (this->underMouse())
        labelColor = labelHoverColor;
    else
        labelColor = pal.text().color();

    pen.setColor(labelColor);
    p.setPen(pen);
    QFont font = this->font();
    font.setPixelSize(getLabelTextWidth());
    p.setFont(font);
    QFontMetrics fontMetrics = this->fontMetrics();


    QRect labelRect;
    labelRect.setTopLeft(QPoint(labelLeft, labelTop));
    labelRect.setWidth(fontMetrics.width(labelText));
    labelRect.setHeight(fontMetrics.height());

    p.drawItemText(labelRect, Qt::AlignLeft, pal, true, labelText);

}

#include "MDTwoLineItem.hpp"
#include <QStylePainter>


MDTwoLineItem::MDTwoLineItem(QWidget* parent, Qt::WindowFlags f) :
    MDTwoLineItem("", "", parent, f) { }

MDTwoLineItem::MDTwoLineItem(const QString& primaryLabel, QWidget* parent, Qt::WindowFlags f) :
    MDTwoLineItem(primaryLabel, "", parent, f) { }

MDTwoLineItem::MDTwoLineItem(const QString& primaryLabel, const QString& secondaryLabel, QWidget* parent, Qt::WindowFlags f) :
    QLabel(primaryLabel, parent, f), secondaryLabel(secondaryLabel) {

}

int MDTwoLineItem::getSecondaryLabelFontSize() const { return secondaryLabelFontSize; }

void MDTwoLineItem::setSecondaryLabelFontSize(int value) { secondaryLabelFontSize = value; }

int MDTwoLineItem::getLabelX() const { return labelX; }

void MDTwoLineItem::setLabelX(int value) { labelX = value; }

int MDTwoLineItem::getLabelY() const { return labelY; }

void MDTwoLineItem::setLabelY(int value) { labelY = value; }

QString MDTwoLineItem::getIconPath() const { return iconPath; }

void MDTwoLineItem::setIconPath(const QString& value) { iconPath = value; }

int MDTwoLineItem::getIconWidth() const { return iconWidth; }

void MDTwoLineItem::setIconWidth(int value) { iconWidth = value; }

int MDTwoLineItem::getIconHeight() const { return iconHeight; }

void MDTwoLineItem::setIconHeight(int value) { iconHeight = value; }

int MDTwoLineItem::getLabelDistance() const { return labelDistance; }

void MDTwoLineItem::setLabelDistance(int value) { labelDistance = value; }

QString MDTwoLineItem::getSecondaryLabel() const { return secondaryLabel; }

void MDTwoLineItem::setSecondaryLabel(const QString& value) { secondaryLabel = value; }

QString MDTwoLineItem::getPrimaryLabel() const { return this->text(); }

void MDTwoLineItem::setPrimaryLabel(const QString& value) { this->setText(value); }

QColor MDTwoLineItem::getSecondaryLabelColor() const { return secondaryLabelColor; }

void MDTwoLineItem::setSecondaryLabelColor(const QColor& value) { secondaryLabelColor = value; }

void MDTwoLineItem::paintEvent(QPaintEvent* event) {

    QLabel::paintEvent(event);

    QStylePainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QPalette pal = palette();
    QPen pen;

    QColor secLabelColor = this->getSecondaryLabelColor();
    pen.setColor(secLabelColor);
    p.setPen(pen);

    QFont font = this->font();
    font.setPixelSize(this->getSecondaryLabelFontSize());
    p.setFont(font);

    QRect contentsRect = this->contentsRect();
    int x = contentsRect.x();
    x += this->getLabelX();
    int y = contentsRect.y() + this->getSecondaryLabelFontSize();
    y += this->getLabelY() + this->getLabelDistance();

    p.drawText(QPoint(x, y), secondaryLabel);

    // per test
//    QRect fontBoundingRect = this->fontMetrics().boundingRect(this->text());

}

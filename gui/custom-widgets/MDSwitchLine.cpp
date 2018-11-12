#include "MDSwitchLine.hpp"

#include <QStyleOption>
#include <QStylePainter>

QColor MDSwitchLine::getCircleColor() const { return circleColor; }

void MDSwitchLine::setCircleColor(const QColor& value) { circleColor = value; }

int MDSwitchLine::getCircleLeftPadding() const { return circleLeftPadding; }

void MDSwitchLine::setCircleLeftPadding(int value) { circleLeftPadding = value; }

int MDSwitchLine::getCircleRadiuos() const { return circleRadiuos; }

void MDSwitchLine::setCircleRadiuos(int value) { circleRadiuos = value; }

QColor MDSwitchLine::getCircleBorderColor() const { return circleBorderColor; }

void MDSwitchLine::setCircleBorderColor(const QColor& value) { circleBorderColor = value; }

int MDSwitchLine::getCircleBorderWidth() const { return circleBorderWidth; }

void MDSwitchLine::setCircleBorderWidth(int value) { circleBorderWidth = value; }

int MDSwitchLine::getCircleRightPadding() const { return circleRightPadding; }

void MDSwitchLine::setCircleRightPadding(int value) { circleRightPadding = value; }

bool MDSwitchLine::getIsOn() const { return isOn; }

void MDSwitchLine::setIsOn(bool value) {
    isOn = value;
    update();
}

QColor MDSwitchLine::getCircleOnColor() const { return circleOnColor; }

void MDSwitchLine::setCircleOnColor(const QColor& value) { circleOnColor = value; }

MDSwitchLine::MDSwitchLine(QWidget* parent) :
    QCheckBox(parent), isOn(false) {

}

void MDSwitchLine::paintEvent(QPaintEvent* event) {

    Q_UNUSED(event)

    QStylePainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QStyleOptionButton opt;
    initStyleOption(&opt);
    opt.text.clear();
    p.drawControl(QStyle::CE_CheckBox, opt);

    QPalette pal = this->palette();

    QRect rect = this->rect();
    int x = rect.x();
    int y = rect.y();
    int height = rect.height();

    int yRect = y + (height - circleRadiuos*2)/2;
    int xRect = x + circleLeftPadding;

    // disegno il cerchio
    QPainterPath path;
    QRect rect2Draw(xRect, yRect, circleRadiuos*2, circleRadiuos*2);
    path.addEllipse(rect2Draw);
    QPen pen(circleBorderColor, circleBorderWidth);
    p.setPen(pen);
    if (this->isOn)
        p.fillPath(path, circleOnColor);
    else
        p.fillPath(path, circleColor);
    p.drawPath(path);

    // scrivo il testo della checkbox
    QFontMetrics fm = this->fontMetrics();
    QRect rText = fm.boundingRect(this->text());
    int textWidth = fm.width(this->text());
    int textHeight = rText.height();
    QPoint pTopLeft;
    pTopLeft.setX(x + circleLeftPadding + circleRadiuos*2 + circleRightPadding);
    pTopLeft.setY(y + (height - rText.height())/2);
    rText.setTopLeft(pTopLeft);
    rText.setSize(QSize(textWidth, textHeight));

    pen.setBrush(pal.text());
    p.setPen(pen);
    p.drawItemText(rText, Qt::AlignLeft, pal, true, this->text());

}

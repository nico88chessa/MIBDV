#include "MDSwitchLine.hpp"

#include <QStyleOption>
#include <QStylePainter>

int MDSwitchLine::getImageLeftPadding() const { return imageLeftPadding; }

void MDSwitchLine::setImageLeftPadding(int value) { imageLeftPadding = value; }

int MDSwitchLine::getImageRightPadding() const { return imageRightPadding; }

void MDSwitchLine::setImageRightPadding(int value) { imageRightPadding = value; }

QString MDSwitchLine::getImageOnEnable() const { return imageOnEnable; }

void MDSwitchLine::setImageOnEnable(const QString& value) { imageOnEnable = value; }

QString MDSwitchLine::getImageOnDisable() const { return imageOnDisable; }

void MDSwitchLine::setImageOnDisable(const QString& value) { imageOnDisable = value; }

QString MDSwitchLine::getImageOffEnable() const { return imageOffEnable; }

void MDSwitchLine::setImageOffEnable(const QString& value) { imageOffEnable = value; }

QString MDSwitchLine::getImageOffDisable() const { return imageOffDisable; }

void MDSwitchLine::setImageOffDisable(const QString& value) { imageOffDisable = value; }

bool MDSwitchLine::getIsOn() const { return isOn; }

int MDSwitchLine::getImageWidth() const { return imageWidth; }

void MDSwitchLine::setImageWidth(int value) { imageWidth = value; }

int MDSwitchLine::getImageHeight() const { return imageHeight; }

void MDSwitchLine::setImageHeight(int value) { imageHeight = value; }

void MDSwitchLine::setIsOn(bool value) {
    isOn = value;
    update();
}

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

    QRect rect = this->rect();
    int x = rect.x();
    int y = rect.y();
    int height = rect.height();

    QPixmap image;
    if (this->isEnabled())
        if (this->getIsOn())
            image.load(this->imageOnEnable);
        else
            image.load(this->imageOffEnable);
    else
        if (this->getIsOn())
            image.load(this->imageOnDisable);
        else
            image.load(this->imageOffDisable);

    int yRect = y + (height - imageHeight)/2;
    int xRect = x + imageLeftPadding;

    image = image.scaled(imageWidth, imageHeight, Qt::AspectRatioMode::KeepAspectRatio, Qt::SmoothTransformation);
    p.drawPixmap(xRect, yRect, image);

    // disegno il cerchio
//    QPainterPath path;
//    QRect rect2Draw(xRect, yRect, circleRadiuos*2, circleRadiuos*2);
//    path.addEllipse(rect2Draw);
//    QPen pen(circleBorderColor, circleBorderWidth);
//    p.setPen(pen);
//    if (this->isOn)
//        p.fillPath(path, circleOnColor);
//    else
//        p.fillPath(path, circleColor);
//    p.drawPath(path);

    // scrivo il testo della checkbox
    QFontMetrics fm = this->fontMetrics();
    QRect rText = fm.boundingRect(this->text());
    int textWidth = fm.width(this->text());
    int textHeight = rText.height();
    QPoint pTopLeft;
    pTopLeft.setX(x + imageLeftPadding + imageWidth + imageRightPadding);
    pTopLeft.setY(y + (height - rText.height())/2);
    rText.setTopLeft(pTopLeft);
    rText.setSize(QSize(textWidth, textHeight));

    QPalette pal = this->palette();
//    pen.setBrush(pal.text());
//    p.setPen(pen);
    p.drawItemText(rText, Qt::AlignLeft, pal, true, this->text());

}

#include "MDListItemWidget.hpp"

#include <QStylePainter>
#include <QStyleOption>

#include <Logger.hpp>

using namespace PROGRAM_NAMESPACE;


MDListItemWidget::MDListItemWidget(QWidget* parent) :
    MDListItemWidget("", parent) { }

MDListItemWidget::MDListItemWidget(const QString& text, QWidget* parent) :
    QFrame(parent), text(text), isActive(false), isHover(false) { }

QString MDListItemWidget::getIconPath() const { return iconPath; }

void MDListItemWidget::setIconPath(const QString& value) { iconPath = value; }

QSize MDListItemWidget::sizeHint() const {
    return QSize(100, 48);
}

void MDListItemWidget::paintEvent(QPaintEvent* event) {

    QFrame::paintEvent(event);

    QStylePainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QRect rect = this->rect();
    QRect contentRect = this->contentsRect();

    QRect iconRect = contentRect;
    QPixmap pixmap;

    pixmap.load(this->iconPath);
    p.drawPixmap(iconRect.x(), iconRect.y(), this->iconWidth, this->iconHeight, pixmap);


    QFontMetrics fontMetrics = p.fontMetrics();
    int height = fontMetrics.height();
    int width = fontMetrics.width(this->getText());
    QRect textRectCorrect;
    int x = contentRect.x() + this->iconWidth + this->iconPaddingRight;
    int y = rect.y() + (this->rect().height() - height)/2;
    textRectCorrect.setX(x);
    textRectCorrect.setY(y);
    textRectCorrect.setWidth(width);
    textRectCorrect.setHeight(height);

    QPalette pal = this->palette();
    p.drawItemText(textRectCorrect, Qt::AlignLeft, pal, false, this->getText());

}

void MDListItemWidget::enterEvent(QEvent* event)
{
    QWidget::enterEvent(event);
    this->setIsHover(true);
    this->style()->polish(this);
}

void MDListItemWidget::leaveEvent(QEvent* event)
{
    QWidget::leaveEvent(event);
    this->setIsHover(false);
    this->style()->polish(this);
}

QString MDListItemWidget::getText() const { return text; }

void MDListItemWidget::setText(const QString& value) { text = value; }

bool MDListItemWidget::getIsHover() const { return isHover; }

void MDListItemWidget::setIsHover(bool value) { isHover = value; }

bool MDListItemWidget::getIsActive() const { return isActive; }

void MDListItemWidget::setIsActive(bool value) { isActive = value; }

int MDListItemWidget::getIconPaddingRight() const { return iconPaddingRight; }

void MDListItemWidget::setIconPaddingRight(int value) { iconPaddingRight = value; }

int MDListItemWidget::getIconHeight() const { return iconHeight; }

void MDListItemWidget::setIconHeight(int value) { iconHeight = value; }

int MDListItemWidget::getIconWidth() const { return iconWidth; }

void MDListItemWidget::setIconWidth(int value) { iconWidth = value; }

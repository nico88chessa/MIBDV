#include "MDListWidget.hpp"

#include <QStylePainter>
/*
MDListWidgetItem::MDListWidgetItem(QListWidget* view, int type) :
    QListWidgetItem(view, type) {

}

MDListItemDelegate::MDListItemDelegate(QObject* parent) :
    QStyledItemDelegate(parent) {

}

void MDListItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {

    QStyledItemDelegate::paint(painter, option, index);

}
*/
MDListWidget::MDListWidget(QWidget* parent) : QListWidget(parent) {

//    this->setItemDelegate(new MDListItemDelegate());

}


MDCustomItem::MDCustomItem(QWidget* parent) :
    QLabel(parent) {

}

MDCustomItem::MDCustomItem(const QString& text, const QString& iconPath) :
    QLabel(text), iconPath(iconPath) { }

QString MDCustomItem::getIconPath() const { return iconPath; }

void MDCustomItem::setIconPath(const QString& value) { iconPath = value; }

QSize MDCustomItem::sizeHint() const {

    return QSize(100, 200);

}

void MDCustomItem::paintEvent(QPaintEvent* event) {

    Q_UNUSED(event)

    QStylePainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QRect iconRect = this->rect();
    iconRect.setWidth(this->iconWidth);
    iconRect.setHeight(this->iconHeight);
    iconRect.setX(0);
    iconRect.setY((rect().height() - iconHeight) / 2);

    QPixmap pixmap(this->getIconPath());
    p.drawPixmap(iconRect.x(), iconRect.y(), this->iconWidth, this->iconHeight, pixmap);

    QFontMetrics fontMetrics = p.fontMetrics();
    int height = fontMetrics.height();
    int width = fontMetrics.width(this->text());
    QRect textRectCorrect;
    int x = rect().x();
    int y = rect().y();
    textRectCorrect.setX(x + this->iconWidth + this->iconPaddingRight);
    textRectCorrect.setY(y + (this->rect().height() - height)/2);
    textRectCorrect.setWidth(width);
    textRectCorrect.setHeight(height);

    QPen pen = p.pen();
    pen.setColor(QColor(100,100,100));

    p.drawItemText(textRectCorrect, Qt::AlignLeft, this->palette(), false, this->text());

}

int MDCustomItem::getIconPaddingRight() const
{
    return iconPaddingRight;
}

void MDCustomItem::setIconPaddingRight(int value)
{
    iconPaddingRight = value;
}

int MDCustomItem::getIconHeight() const
{
    return iconHeight;
}

void MDCustomItem::setIconHeight(int value)
{
    iconHeight = value;
}

int MDCustomItem::getIconWidth() const
{
    return iconWidth;
}

void MDCustomItem::setIconWidth(int value)
{
    iconWidth = value;
}

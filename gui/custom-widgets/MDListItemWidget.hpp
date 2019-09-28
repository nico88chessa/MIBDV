#ifndef MDLISTITEMWIDGET_HPP
#define MDLISTITEMWIDGET_HPP

#include <QFrame>


class MDListItemWidget : public QFrame {

    Q_OBJECT
    Q_PROPERTY(QString iconPath READ getIconPath WRITE setIconPath)
    Q_PROPERTY(int iconWidth READ getIconWidth WRITE setIconWidth)
    Q_PROPERTY(int iconHeight READ getIconHeight WRITE setIconHeight)
    Q_PROPERTY(int iconPaddingRight READ getIconPaddingRight WRITE setIconPaddingRight)
    Q_PROPERTY(bool isActive READ getIsActive WRITE setIsActive)
    Q_PROPERTY(bool isHover READ getIsHover WRITE setIsHover)


public:
    using Ptr = MDListItemWidget*;
    using ConstPtr = const MDListItemWidget*;

    static constexpr const char* FOCUS_PROPERTY_NAME = "focusCustom";

public:
    explicit MDListItemWidget(QWidget* parent = nullptr);
    explicit MDListItemWidget(const QString& text, QWidget* parent = nullptr);

    QString getIconPath() const;
    void setIconPath(const QString& value);
    QSize sizeHint() const;

    int getIconWidth() const;
    void setIconWidth(int value);

    int getIconHeight() const;
    void setIconHeight(int value);

    int getIconPaddingRight() const;
    void setIconPaddingRight(int value);

    QString getDisabledIconPath() const;
    void setDisabledIconPath(const QString& value);

    QString getSelectedIconPath() const;
    void setSelectedIconPath(const QString& value);

    bool getIsActive() const;
    void setIsActive(bool value);

    bool getIsHover() const;
    void setIsHover(bool value);

    QColor getBackgroundColorActive() const;
    void setBackgroundColorActive(const QColor& value);

    QColor getBackgroundColorHover() const;
    void setBackgroundColorHover(const QColor& value);

    QColor getTextColorActive() const;
    void setTextColorActive(const QColor& value);

    QColor getTextColorHover() const;
    void setTextColorHover(const QColor& value);

    QColor getTextColorDisabled() const;
    void setTextColorDisabled(const QColor& value);

    QString getText() const;
    void setText(const QString& value);

protected:
    void paintEvent(QPaintEvent* event);

private:
    QString text;
    QString iconPath;
    int iconWidth;
    int iconHeight;
    int iconPaddingRight;
    bool isActive;
    bool isHover;

protected:
    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);
};



#endif // MDLISTITEMWIDGET_HPP

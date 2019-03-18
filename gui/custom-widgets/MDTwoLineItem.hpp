#ifndef MDTWOLINEITEM_H
#define MDTWOLINEITEM_H

#include <QLabel>
#include <QColor>


class MDTwoLineItem : public QLabel {
    Q_OBJECT

    Q_PROPERTY(int labelX READ getLabelX WRITE setLabelX)
    Q_PROPERTY(int labelY READ getLabelY WRITE setLabelY)
    Q_PROPERTY(QString iconPath READ getIconPath WRITE setIconPath)
    Q_PROPERTY(int iconWidth READ getIconWidth WRITE setIconWidth)
    Q_PROPERTY(int iconHeight READ getIconHeight WRITE setIconHeight)
    Q_PROPERTY(int labelDistance READ getLabelDistance WRITE setLabelDistance)
    Q_PROPERTY(int secondaryLabelFontSize READ getSecondaryLabelFontSize WRITE setSecondaryLabelFontSize)
    Q_PROPERTY(QColor secondaryLabelColor READ getSecondaryLabelColor WRITE setSecondaryLabelColor)
    Q_PROPERTY(QString secondaryLabel READ getSecondaryLabel WRITE setSecondaryLabel)

public:
    using Ptr = MDTwoLineItem*;
    using ConstPtr = const MDTwoLineItem*;

private:
    int labelX;
    int labelY;
    QString iconPath;
    int iconWidth;
    int iconHeight;
    int labelDistance;
    int secondaryLabelFontSize;
    QColor secondaryLabelColor;
    QString secondaryLabel;

public:
    explicit MDTwoLineItem(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    explicit MDTwoLineItem(const QString &primaryLabel, QWidget *parent=Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    explicit MDTwoLineItem(const QString &primaryLabel, const QString& secondaryLabel, QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    int getLabelX() const;
    void setLabelX(int value);

    int getLabelY() const;
    void setLabelY(int value);

    QString getSecondaryLabel() const;
    void setSecondaryLabel(const QString& value);

    QString getPrimaryLabel() const;
    void setPrimaryLabel(const QString& value);

    QString getIconPath() const;
    void setIconPath(const QString& value);

    int getIconWidth() const;
    void setIconWidth(int value);

    int getIconHeight() const;
    void setIconHeight(int value);

    int getLabelDistance() const;
    void setLabelDistance(int value);

    QColor getSecondaryLabelColor() const;
    void setSecondaryLabelColor(const QColor& value);

    int getSecondaryLabelFontSize() const;
    void setSecondaryLabelFontSize(int value);

protected:
    void paintEvent(QPaintEvent* event);

};

#endif // MDTWOLINEITEM_H

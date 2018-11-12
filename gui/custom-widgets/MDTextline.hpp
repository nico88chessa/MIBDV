#ifndef MDTEXTLINE_HPP
#define MDTEXTLINE_HPP

#include <QLineEdit>

class MDTextline : public QLineEdit {
    Q_OBJECT

    Q_PROPERTY(int labelX READ getLabelX WRITE setLabelX)
    Q_PROPERTY(int labelY READ getLabelY WRITE setLabelY)
    Q_PROPERTY(int labelTextWidth READ getLabelTextWidth WRITE setLabelTextWidth)
    Q_PROPERTY(QColor labelColor READ getLabelColor WRITE setLabelColor)
    Q_PROPERTY(QColor labelFocusColor READ getLabelFocusColor WRITE setLabelFocusColor)

public:
    using Ptr = MDTextline*;
    using ConstPtr = const MDTextline*;

private:
    int labelY;
    int labelX;
    int labelTextWidth;
    QColor labelColor;
    QColor labelFocusColor;

public:
    explicit MDTextline(QWidget* parent = nullptr);


    int getLabelX() const;
    void setLabelX(int value);

    int getLabelY() const;
    void setLabelY(int value);

    int getLabelTextWidth() const;
    void setLabelTextWidth(int value);

    QColor getLabelColor() const;
    void setLabelColor(const QColor& value);

    QColor getLabelFocusColor() const;
    void setLabelFocusColor(const QColor& value);

protected:
    void paintEvent(QPaintEvent* event);
};

#endif // MDTEXTLINE_HPP

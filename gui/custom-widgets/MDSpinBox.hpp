#ifndef MDSPINBOX_HPP
#define MDSPINBOX_HPP

#include <QSpinBox>

class MDSpinBox : public QSpinBox {
    Q_OBJECT

    Q_PROPERTY(int labelX READ getLabelX WRITE setLabelX)
    Q_PROPERTY(int labelY READ getLabelY WRITE setLabelY)
    Q_PROPERTY(int labelTextWidth READ getLabelTextWidth WRITE setLabelTextWidth)
    Q_PROPERTY(QColor labelColor READ getLabelColor WRITE setLabelColor)
    Q_PROPERTY(QColor labelFocusColor READ getLabelFocusColor WRITE setLabelFocusColor)
    Q_PROPERTY(QString label READ getLabel WRITE setLabel)


public:
    using Ptr = MDSpinBox*;
    using ConstPtr = const MDSpinBox*;

private:
    int labelY;
    int labelX;
    int labelTextWidth;
    QColor labelColor;
    QColor labelFocusColor;
    QString label;

public:
    explicit MDSpinBox(QWidget* parent = nullptr);


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

    QString getLabel() const;
    void setLabel(const QString& value);

protected:
    void paintEvent(QPaintEvent* event);

};

#endif // MDSPINBOX_HPP

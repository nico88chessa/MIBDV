#ifndef MDLINEEDIT_HPP
#define MDLINEEDIT_HPP

#include <QLineEdit>

class MDLineEdit : public QLineEdit {
    Q_OBJECT

    Q_PROPERTY(int labelX READ getLabelX WRITE setLabelX)
    Q_PROPERTY(int labelY READ getLabelY WRITE setLabelY)
    Q_PROPERTY(int labelTextWidth READ getLabelTextWidth WRITE setLabelTextWidth)
    Q_PROPERTY(QColor labelColor READ getLabelColor WRITE setLabelColor)
    Q_PROPERTY(QColor labelFocusColor READ getLabelFocusColor WRITE setLabelFocusColor)
    Q_PROPERTY(QString label READ getLabel WRITE setLabel)


public:
    using Ptr = MDLineEdit*;
    using ConstPtr = const MDLineEdit*;

private:
    int labelY;
    int labelX;
    int labelTextWidth;
    QColor labelColor;
    QColor labelFocusColor;
    QString label;

public:
    explicit MDLineEdit(QWidget* parent = nullptr);


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

#endif // MDLINEEDIT_HPP

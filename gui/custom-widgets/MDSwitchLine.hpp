#ifndef MDSWITCHLINE_HPP
#define MDSWITCHLINE_HPP

#include <QCheckBox>

class MDSwitchLine : public QCheckBox {
    Q_OBJECT

    Q_PROPERTY(int circleRadiuos READ getCircleRadiuos WRITE setCircleRadiuos)
    Q_PROPERTY(int circleLeftPadding READ getCircleLeftPadding WRITE setCircleLeftPadding)
    Q_PROPERTY(int circleRightPadding READ getCircleRightPadding WRITE setCircleRightPadding)
    Q_PROPERTY(int circleBorderWidth READ getCircleBorderWidth WRITE setCircleBorderWidth)
    Q_PROPERTY(QColor circleColor READ getCircleColor WRITE setCircleColor)
    Q_PROPERTY(QColor circleOnColor READ getCircleOnColor WRITE setCircleOnColor)
    Q_PROPERTY(QColor circleBorderColor READ getCircleBorderColor WRITE setCircleBorderColor)

private:
    int circleRadiuos;
    int circleLeftPadding;
    int circleRightPadding;
    int circleBorderWidth;
    QColor circleColor;
    QColor circleOnColor;
    QColor circleBorderColor;

    bool isOn;

public:
    explicit MDSwitchLine(QWidget* prent = nullptr);

    int getCircleRadiuos() const;
    void setCircleRadiuos(int value);
    int getCircleLeftPadding() const;
    void setCircleLeftPadding(int value);
    int getCircleRightPadding() const;
    void setCircleRightPadding(int value);
    int getCircleBorderWidth() const;
    void setCircleBorderWidth(int value);
    QColor getCircleColor() const;
    void setCircleColor(const QColor& value);
    QColor getCircleBorderColor() const;
    void setCircleBorderColor(const QColor& value);
    bool getIsOn() const;
    void setIsOn(bool value);
    QColor getCircleOnColor() const;
    void setCircleOnColor(const QColor& value);

protected:
    void paintEvent(QPaintEvent* event);
};

#endif // MDSWITCHLINE_HPP

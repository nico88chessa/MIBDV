#ifndef MDSWITCHLINE_HPP
#define MDSWITCHLINE_HPP

#include <QCheckBox>

class MDSwitchLine : public QCheckBox {
    Q_OBJECT

    Q_PROPERTY(int imageLeftPadding READ getImageLeftPadding WRITE setImageLeftPadding)
    Q_PROPERTY(int imageRightPadding READ getImageRightPadding WRITE setImageRightPadding)
    Q_PROPERTY(int imageWidth READ getImageWidth WRITE setImageWidth)
    Q_PROPERTY(int imageHeight READ getImageHeight WRITE setImageHeight)
    Q_PROPERTY(QString imageOnEnable READ getImageOnEnable WRITE setImageOnEnable)
    Q_PROPERTY(QString imageOnDisable READ getImageOnDisable WRITE setImageOnDisable)
    Q_PROPERTY(QString imageOffEnable READ getImageOffEnable WRITE setImageOffEnable)
    Q_PROPERTY(QString imageOffDisable READ getImageOffDisable WRITE setImageOffDisable)

private:
    int imageLeftPadding;
    int imageRightPadding;
    int imageWidth;
    int imageHeight;
    QString imageOnEnable;
    QString imageOnDisable;
    QString imageOffEnable;
    QString imageOffDisable;

    bool isOn;

public:
    explicit MDSwitchLine(QWidget* prent = nullptr);

    int getImageLeftPadding() const;
    void setImageLeftPadding(int value);
    int getImageRightPadding() const;
    void setImageRightPadding(int value);
    QString getImageOnEnable() const;
    void setImageOnEnable(const QString& value);
    QString getImageOnDisable() const;
    void setImageOnDisable(const QString& value);
    QString getImageOffEnable() const;
    void setImageOffEnable(const QString& value);
    QString getImageOffDisable() const;
    void setImageOffDisable(const QString& value);
    int getImageWidth() const;
    void setImageWidth(int value);
    int getImageHeight() const;
    void setImageHeight(int value);
    bool getIsOn() const;
    void setIsOn(bool value);

protected:
    void paintEvent(QPaintEvent* event);
};

#endif // MDSWITCHLINE_HPP

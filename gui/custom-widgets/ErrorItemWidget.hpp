#ifndef ERRORITEMWIDGET_HPP
#define ERRORITEMWIDGET_HPP


#include <QStyleOption>
#include <QWidget>

#include <configure.h>
#include <data/Error.hpp>

namespace PN = PROGRAM_NAMESPACE;

class ErrorItemWidget : public QWidget {
    Q_OBJECT

    Q_PROPERTY(int iconWidth READ getIconWidth WRITE setIconWidth)
    Q_PROPERTY(int iconHeight READ getIconHeight WRITE setIconHeight)
    Q_PROPERTY(int iconLeft READ getIconLeft WRITE setIconLeft)
    Q_PROPERTY(int iconTop  READ getIconTop WRITE setIconTop)
    Q_PROPERTY(int deviceKeyLeft READ getDeviceKeyLeft WRITE setDeviceKeyLeft)
    Q_PROPERTY(int deviceKeyTop READ getDeviceKeyTop WRITE setDeviceKeyTop)
    Q_PROPERTY(int errorDescriptionLeft READ getErrorDescriptionLeft WRITE setErrorDescriptionLeft)
    Q_PROPERTY(int errorDescriptionTop READ getErrorDescriptionTop WRITE setErrorDescriptionTop)
    Q_PROPERTY(int errorIdLeft READ getErrorIdLeft WRITE setErrorIdLeft)
    Q_PROPERTY(int errorIdTop READ getErrorIdTop WRITE setErrorIdTop)
    Q_PROPERTY(int errorDescriptionFontSize READ getErrorDescriptionFontSize WRITE setErrorDescriptionFontSize)
    Q_PROPERTY(int errorDescriptionMaxRowCount READ getErrorDescriptionMaxRowCount WRITE setErrorDescriptionMaxRowCount)
    Q_PROPERTY(QString infoIcon READ getInfoIcon WRITE setInfoIcon)
    Q_PROPERTY(QString warningIcon READ getWarningIcon WRITE setWarningIcon)
    Q_PROPERTY(QString errorIcon READ getErrorIcon WRITE setErrorIcon)
    Q_PROPERTY(QString fatalIcon READ getFatalIcon WRITE setFatalIcon)


public:
    using Ptr = ErrorItemWidget*;
    using ConstPtr = const ErrorItemWidget*;

    /* NOTE NIC 05/07/2019 - focus string
     * non usare "focus" perche' va in conflitto con la property usata da Qt,
     * tant'e' che "focus" come property non funziona
     */
    static constexpr char* FOCUS_PROPERTY_NAME = "focusCustom";
    static constexpr char* IS_INFO_PROPERTY_NAME = "isInfo";
    static constexpr char* IS_WARNING_PROPERTY_NAME = "isWarning";
    static constexpr char* IS_ERROR_PROPERTY_NAME = "isError";
    static constexpr char* IS_FATAL_PROPERTY_NAME = "isFatal";

private:
    int deviceKey;
    int deviceInstance;
    PN::ErrorID errorId;
    QString errorDescription;
    PN::ErrorType errorType;
    QString infoIcon, warningIcon, errorIcon, fatalIcon;

    int iconWidth;
    int iconHeight;
    int iconLeft, iconTop;
    int deviceKeyLeft, deviceKeyTop;
    int errorDescriptionLeft, errorDescriptionTop;
    int errorIdLeft, errorIdTop;
    int errorDescriptionFontSize;
    int errorDescriptionMaxRowCount;

public:
    explicit ErrorItemWidget(QWidget* parent = Q_NULLPTR);
    ~ErrorItemWidget();

    int getDeviceKey() const;
    void setDeviceKey(int value);

    int getDeviceInstance() const;
    void setDeviceInstance(int value);

    PN::ErrorID getErrorId() const;
    void setErrorId(const PN::ErrorID& value);

    QString getErrorDescription() const;
    void setErrorDescription(const QString& value);

    PN::ErrorType getErrorType() const;
    void setErrorType(const PN::ErrorType& value);

    void setError(const PN::Error& error);

    int getIconWidth() const;
    void setIconWidth(int value);

    int getIconHeight() const;
    void setIconHeight(int value);

    int getIconLeft() const;
    void setIconLeft(int value);

    int getIconTop() const;
    void setIconTop(int value);

    int getDeviceKeyLeft() const;
    void setDeviceKeyLeft(int value);

    int getDeviceKeyTop() const;
    void setDeviceKeyTop(int value);

    int getErrorDescriptionLeft() const;
    void setErrorDescriptionLeft(int value);

    int getErrorDescriptionTop() const;
    void setErrorDescriptionTop(int value);

    int getErrorIdLeft() const;
    void setErrorIdLeft(int value);

    int getErrorIdTop() const;
    void setErrorIdTop(int value);

    QString getInfoIcon() const;
    void setInfoIcon(const QString& value);

    QString getWarningIcon() const;
    void setWarningIcon(const QString& value);

    QString getErrorIcon() const;
    void setErrorIcon(const QString& value);

    QString getFatalIcon() const;
    void setFatalIcon(const QString& value);

    int getErrorDescriptionFontSize() const;
    void setErrorDescriptionFontSize(int value);

    int getErrorDescriptionMaxRowCount() const;
    void setErrorDescriptionMaxRowCount(int value);

protected:
    void paintEvent(QPaintEvent* event);

    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);
};

#endif // ERRORITEMWIDGET_HPP

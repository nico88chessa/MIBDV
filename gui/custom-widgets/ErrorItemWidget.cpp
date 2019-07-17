#include "ErrorItemWidget.hpp"

#include <QStyleOption>
#include <QStylePainter>
#include <QPaintEvent>
#include <QTextLayout>

#include <Utils.hpp>


using namespace PROGRAM_NAMESPACE;

ErrorItemWidget::ErrorItemWidget(QWidget* parent) : QWidget(parent) {  }

ErrorItemWidget::~ErrorItemWidget() {  }

void ErrorItemWidget::setError(const Error& error) {

    this->setDeviceKey(static_cast<int>(error.getDeviceKey()));
    this->setDeviceInstance(error.getDeviceInstance());
    this->setErrorId(error.getErrorId());
    this->setErrorDescription(error.getErrorDescription());
    this->setErrorType(error.getErrorType());

    switch (error.getErrorType()) {
    case ErrorType::INFO : this->setProperty(IS_INFO_PROPERTY_NAME, true); break;
    case ErrorType::WARNING : this->setProperty(IS_WARNING_PROPERTY_NAME, true); break;
    case ErrorType::ERROR : this->setProperty(IS_ERROR_PROPERTY_NAME, true); break;
    case ErrorType::FATAL : this->setProperty(IS_FATAL_PROPERTY_NAME, true); break;
    }

}

int ErrorItemWidget::getIconWidth() const { return iconWidth; }

void ErrorItemWidget::setIconWidth(int value) { iconWidth = value; }

int ErrorItemWidget::getIconHeight() const { return iconHeight; }

void ErrorItemWidget::setIconHeight(int value) { iconHeight = value; }

int ErrorItemWidget::getDeviceKey() const { return deviceKey; }

void ErrorItemWidget::setDeviceKey(int value) { deviceKey = value; }

int ErrorItemWidget::getDeviceInstance() const { return deviceInstance; }

void ErrorItemWidget::setDeviceInstance(int value) { deviceInstance = value; }

ErrorID ErrorItemWidget::getErrorId() const { return errorId; }

void ErrorItemWidget::setErrorId(const ErrorID& value) { errorId = value; }

QString ErrorItemWidget::getErrorDescription() const { return errorDescription; }

void ErrorItemWidget::setErrorDescription(const QString& value) { errorDescription = value; }

ErrorType ErrorItemWidget::getErrorType() const { return errorType; }

void ErrorItemWidget::setErrorType(const ErrorType& value) { errorType = value; }

int ErrorItemWidget::getIconLeft() const { return iconLeft; }

void ErrorItemWidget::setIconLeft(int value) { iconLeft = value; }

int ErrorItemWidget::getIconTop() const { return iconTop; }

void ErrorItemWidget::setIconTop(int value) { iconTop = value; }

int ErrorItemWidget::getDeviceKeyLeft() const { return deviceKeyLeft; }

void ErrorItemWidget::setDeviceKeyLeft(int value) { deviceKeyLeft = value; }

int ErrorItemWidget::getDeviceKeyTop() const { return deviceKeyTop; }

void ErrorItemWidget::setDeviceKeyTop(int value) { deviceKeyTop = value; }

int ErrorItemWidget::getErrorDescriptionLeft() const { return errorDescriptionLeft; }

void ErrorItemWidget::setErrorDescriptionLeft(int value) { errorDescriptionLeft = value; }

int ErrorItemWidget::getErrorDescriptionTop() const { return errorDescriptionTop; }

void ErrorItemWidget::setErrorDescriptionTop(int value) { errorDescriptionTop = value; }

int ErrorItemWidget::getErrorIdLeft() const { return errorIdLeft; }

void ErrorItemWidget::setErrorIdLeft(int value) { errorIdLeft = value; }

int ErrorItemWidget::getErrorIdTop() const { return errorIdTop; }

void ErrorItemWidget::setErrorIdTop(int value) { errorIdTop = value; }

QString ErrorItemWidget::getInfoIcon() const { return infoIcon; }

void ErrorItemWidget::setInfoIcon(const QString& value) { infoIcon = value; }

QString ErrorItemWidget::getWarningIcon() const { return warningIcon; }

void ErrorItemWidget::setWarningIcon(const QString& value) { warningIcon = value; }

QString ErrorItemWidget::getErrorIcon() const { return errorIcon; }

void ErrorItemWidget::setErrorIcon(const QString& value) { errorIcon = value; }

QString ErrorItemWidget::getFatalIcon() const { return fatalIcon; }

void ErrorItemWidget::setFatalIcon(const QString& value) { fatalIcon = value; }

int ErrorItemWidget::getErrorDescriptionFontSize() const { return errorDescriptionFontSize; }

void ErrorItemWidget::setErrorDescriptionFontSize(int value) { errorDescriptionFontSize = value; }

int ErrorItemWidget::getErrorDescriptionMaxRowCount() const { return errorDescriptionMaxRowCount; }

void ErrorItemWidget::setErrorDescriptionMaxRowCount(int value) { errorDescriptionMaxRowCount = value; }


/*QSize ErrorItemWidget::sizeHint() const {
    // TODO NIC 05/07/2019 - sarebbe interessante leggerlo da stylesheet .qss
    QSize sizeHint = QWidget::sizeHint();
    QSize test = this->minimumSizeHint();
    int test2 = this->minimumHeight();
    int iconWidth = this->getIconWidth();
    sizeHint.setHeight(48);
    return sizeHint;
}*/

void ErrorItemWidget::paintEvent(QPaintEvent* event) {

    QWidget::paintEvent(event);
    QStyleOption opt;
    opt.initFrom(this);
    QStylePainter p(this);

    QRect widgetRect = opt.rect; // rect della widget
    QRect frameRect = style()->subElementRect(QStyle::SE_FrameContents, &opt, this); // rect del frame (tiene conto di padding, border, margin);

    p.save();
    p.drawPrimitive(QStyle::PE_Widget, opt); // questo mi serve per disegnare la widget (padding, border, margin)

    // solo test
    //int value = opt.state;
    //if (opt.state & QStyle::State_MouseOver) {
    //    this->setStyleSheet("color: red");
    //} else
    //    this->setStyleSheet("color: white");
//    QRect r = widgetRect;

    p.setRenderHint(QPainter::Antialiasing);

    // qui disegno l'icona
    QPixmap icon;
    switch (this->errorType) {
        case ErrorType::INFO : icon.load(infoIcon); break;
        case ErrorType::WARNING : icon.load(warningIcon); break;
        case ErrorType::ERROR : icon.load(errorIcon); break;
        case ErrorType::FATAL : icon.load(fatalIcon); break;
    }

    int iconTopPosition = (frameRect.height() - iconHeight)/2;
    icon = icon.scaled(iconWidth, iconHeight, Qt::AspectRatioMode::KeepAspectRatio, Qt::SmoothTransformation);
    p.drawPixmap(iconLeft, iconTopPosition, iconWidth, iconHeight, icon);

    // qui configuro il testo per il device e per il codice errore
    QFont font = this->font();
    QPen pen;
    pen.setColor(palette().color(QPalette::Active, QPalette::Text));
    p.setFont(font);
    p.setPen(pen);

    QFontMetrics fMetrics(font);
    int capHeight = fMetrics.ascent();

    int yText = (frameRect.height() - capHeight)/2 + capHeight;
    QString deviceKeyStr = QString(Utils::getStringFromDeviceKey(static_cast<DeviceKey>(deviceKey)) + " (%1)").arg(deviceInstance);

    // qui scrivo il testo
    p.drawText(deviceKeyLeft, yText, deviceKeyStr);
    p.drawText(errorIdLeft, yText, QString::number(errorId));

    // qui la descirione
    font.setPixelSize(errorDescriptionFontSize);
    p.setFont(font);
    QRect descrRect = frameRect;
    descrRect.setX(errorDescriptionLeft);
    // p.drawText(descrRect, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, errorDescription);

    QFontMetrics fm(font);
    int lineSpacing = fm.lineSpacing();

    QTextLayout textLayout(errorDescription, font);
    int y = 0;
    int rowToUse = 0;

    // questo lo uso per calcolare il numero di righe necessarie per visualizzare la scritta
    // e mi salvo il numero di righe necessarie
    textLayout.beginLayout();
    forever {
        QTextLine line = textLayout.createLine();

        if (!line.isValid())
            break;

        line.setLineWidth(descrRect.width());
        int nextLineY = y + lineSpacing;
        ++rowToUse;

        // verifico se ci sta la riga successiva a quella che sto creando;
        // se si, ok, se no, devo fare "elide" sulla riga corrente
        if (lineSpacing * errorDescriptionMaxRowCount >= nextLineY + lineSpacing) {
//            line.draw(&p, QPoint(descrRect.x(), y));
            y = nextLineY;
        } else {
            QString lastLine = errorDescription.mid(line.textStart());
            QString elidedLastLine = fm.elidedText(lastLine, Qt::ElideRight, descrRect.width());
//            p.drawText(QPoint(descrRect.x(), y + fm.ascent()), elidedLastLine);
            line = textLayout.createLine();
            break;
        }
    }
    textLayout.endLayout();


    // qui renderizzo le scritte vere e proprie
    QTextLayout textLayout2(errorDescription, font);

    int textDescriptionAreaHeight = fm.lineSpacing() * rowToUse;
    int yTextDescription = qMax(0, (frameRect.height() - textDescriptionAreaHeight)/2);
    y = 0;

    textLayout2.beginLayout();
    forever {
        QTextLine line = textLayout2.createLine();

        if (!line.isValid())
            break;

        line.setLineWidth(descrRect.width());
        int nextLineY = y + lineSpacing;

        // verifico se ci sta la riga successiva a quella che sto creando;
        // se si, ok, se no, devo fare "elide" sulla riga corrente
        if (lineSpacing * errorDescriptionMaxRowCount >= nextLineY + lineSpacing) {
            line.draw(&p, QPoint(descrRect.x(), yTextDescription + y));
            y = nextLineY;
        } else {
            QString lastLine = errorDescription.mid(line.textStart());
            QString elidedLastLine = fm.elidedText(lastLine, Qt::ElideRight, descrRect.width());
            p.drawText(QPoint(descrRect.x(), yTextDescription + y + fm.ascent()), elidedLastLine);
            line = textLayout2.createLine();
            break;
        }
    }
    textLayout2.endLayout();

    p.restore();

}

void ErrorItemWidget::enterEvent(QEvent* event) {
    QWidget::enterEvent(event);
    this->setProperty(FOCUS_PROPERTY_NAME, true);
    this->style()->polish(this);
}

void ErrorItemWidget::leaveEvent(QEvent* event) {
    QWidget::leaveEvent(event);
    this->setProperty(FOCUS_PROPERTY_NAME, QVariant::Invalid);
    this->style()->polish(this);
}

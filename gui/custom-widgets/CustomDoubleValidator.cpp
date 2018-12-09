#include "CustomDoubleValidator.hpp"

CustomDoubleValidator::CustomDoubleValidator(QObject* parent) :
    QDoubleValidator(parent) { }

CustomDoubleValidator::CustomDoubleValidator(double bottom, double top, int decimals, QObject* parent) :
    QDoubleValidator(bottom, top, decimals, parent) { }

CustomDoubleValidator::~CustomDoubleValidator() { }

QValidator::State CustomDoubleValidator::validate(QString& s, int& pos) const {

    Q_UNUSED(pos)

    if (s.isEmpty() || s == "-")
        return QValidator::Intermediate;

    QChar decimalPoint = locale().decimalPoint();
    if(s.indexOf(decimalPoint) != -1) {
        int charsAfterPoint = s.length() - s.indexOf(decimalPoint) - 1;

        if (charsAfterPoint > decimals()) {
            return QValidator::Invalid;
        }
    }

    return QDoubleValidator::validate(s, pos);

}

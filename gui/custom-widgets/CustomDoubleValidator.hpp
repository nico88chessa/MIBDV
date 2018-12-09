#ifndef CUSTOMDOUBLEVALIDATOR_HPP
#define CUSTOMDOUBLEVALIDATOR_HPP

#include <QDoubleValidator>

class CustomDoubleValidator : public QDoubleValidator {
    Q_OBJECT

public:
    using Ptr = CustomDoubleValidator*;
    using ConstPtr = const CustomDoubleValidator*;

public:
    explicit CustomDoubleValidator(QObject* parent = nullptr);
    CustomDoubleValidator(double bottom, double top, int decimals, QObject *parent = Q_NULLPTR);
    ~CustomDoubleValidator() override;

    QValidator::State validate(QString& s, int& pos) const override;

};

#endif // CUSTOMDOUBLEVALIDATOR_HPP

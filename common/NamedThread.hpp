#ifndef NAMEDTHREAD_HPP
#define NAMEDTHREAD_HPP

#include <QThread>

class NamedThread : public QThread {
    Q_OBJECT

public:
    using Ptr = NamedThread*;
    using ConstPtr = const NamedThread*;

public:
    explicit NamedThread(const QString& threadName, QObject *parent = Q_NULLPTR) :
        QThread(parent) {
        this->setObjectName(threadName);
    }

};

#endif // NAMEDTHREAD_HPP
